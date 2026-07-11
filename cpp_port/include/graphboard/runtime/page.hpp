#pragma once

#include "graphboard/binary_reader.hpp"
#include "graphboard/holders.hpp"
#include "graphboard/format.hpp"
#include "graphboard/runtime/interpreter.hpp"
#include "graphboard/runtime/value.hpp"

#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace graphboard::runtime {

// The live state of one component instance on a page. GraphBoard components are
// index-addressed (a Sprite_Holder holds many sprites, a HotSpot_Holder many
// hotspots), so mutating methods like MoveTo(id, x, y) update per-index item
// state; holder-wide effects (which sound is playing) go in `props`.
// Static geometry of one placed sprite instance. Its live position, phase and
// visibility are script-mutable and live in ComponentState::items; only the
// layer and the per-phase frame sizes are fixed by the file.
struct SpriteGeometry {
    std::int32_t layer = 0;
    bool draggable = false;            // instance+0x1c == 1
    std::vector<SpriteFrame> frames;   // indexed by phase
};

// Static geometry of one Bitmap_Holder bitmap. Its top-left position is
// script-mutable (MoveTo) and lives in items; the size, layer and transparency
// mask are fixed.
struct BitmapGeometry {
    std::int32_t layer = 0;
    std::int32_t width = 0;
    std::int32_t height = 0;
    std::vector<std::uint8_t> opaque;  // width*height, or empty for rect-only
    std::vector<std::uint8_t> pixels;  // indexed, top-down, for rendering
    std::uint8_t transparentIndex = 0;
};

// Static geometry of one Transparent_Video_Holder entry. The live position and
// layer ("deep") are script-mutable (MoveTo/SetDeep) and live in items; the
// stream/still-frame locations inside the .BDF are fixed, so a renderer or
// audio player can slice the media straight out of the page bytes.
struct VideoGeometry {
    std::int32_t width = 0;
    std::int32_t height = 0;
    std::int32_t frameDurationMs = 0;
    std::int32_t frameCount = 0;
    std::size_t streamOffset = 0;       // "Board Video File" header start
    std::uint32_t streamByteCount = 0;  // header + chunk records
    std::size_t paletteOffset = 0;      // streamOffset + 0xe8: 256 RGBQUADs
    std::size_t stillFrameOffset = 0;   // 8-bpp bottom-up DIB bits, stride-padded
    std::uint32_t stillFrameByteCount = 0;
    bool showStillAtRest = false;       // paint the still frame when not playing
};

// One Sound_Holder clip: a complete RIFF/WAVE file embedded in the page bytes.
struct SoundClip {
    std::size_t offset = 0;
    std::uint32_t byteCount = 0;
};

struct ComponentState {
    std::string displayName;               // e.g. "Sprite_Holder"
    HolderKind kind = HolderKind::Unknown;
    std::map<int, std::map<std::string, Value>> items;  // per-index item state
    std::map<std::string, Value> props;                 // holder-wide state
    std::vector<HotSpot> hotspots;         // HotSpot_Holder geometry (for hit tests)
    std::vector<SpriteGeometry> sprites;   // Sprite_Holder geometry, by spriteID
    std::vector<BitmapGeometry> bitmaps;   // Bitmap_Holder geometry, by bitmapID
    std::vector<VideoGeometry> videos;     // Transparent_Video_Holder entries
    std::vector<SoundClip> soundClips;     // Sound_Holder embedded WAVs
    // Playback length per clip id, in ms: video = frameDurationMs*frameCount,
    // sound = WAV data length. Used by the clock to schedule TheEnd/EndPlaySound.
    std::vector<int> clipDurationMs;
};

// One host call made while executing script (builtin or component method).
struct HostCallRecord {
    bool builtin = true;
    std::string component;   // component method only
    std::string name;
    std::vector<Value> args;
};

// An executable GraphBoard page: parsed components wired to the script
// interpreter as a live Host. Running an event handler mutates component and
// page state exactly as the script directs, so a scene can be driven headless
// and its script-visible state asserted.
//
// This models both directions. Script->component: dispatch to stateful
// components plus page/host builtins. Component->script: raw input (click/hover/
// drag) hit-tests the holders and fires the matching callback, and playback
// completion (video/sound/animation ending) is delivered through the *End
// entry points below. Both only fire a callback the page actually defines, so a
// scene can be driven headless and its script-visible state asserted.
class Page : public Host {
public:
    static std::unique_ptr<Page> loadFromFile(const std::filesystem::path& path);
    static std::unique_ptr<Page> loadFromReader(BinaryReader& reader, std::string sourceLabel);

    // Event dispatch (only fires if the handler is defined in the page).
    bool hasHandler(const std::string& name) const;
    void runEvent(const std::string& name, const std::vector<Value>& args);
    void open() { runEvent("OnOpenPage", {}); }
    void close() { runEvent("OnClosePage", {}); }
    void timer() { runEvent("OnTimer", {Value::integer(0)}); }

    // Raw input dispatch. Mirrors the original view's two-step handling: the
    // page-level script handler always runs, then (as the board would deliver
    // IObject::LButtonDown/MouseMove to components) page and active-group
    // hotspots, sprites, bitmaps, and transparent videos fire their recovered
    // callbacks. Button hits use the merged layer order; hover is tracked per
    // holder because the cursor toolbar intentionally overlaps its sprites and
    // controlling hotspot.
    void lButtonDown(int x, int y);
    void lButtonUp(int x, int y);
    void rButtonDown(int x, int y);
    void mouseMove(int x, int y);
    void keyDown(int key) { runEvent("OnKeyDown", {Value::integer(key)}); }

    // Playback-completion callbacks (component->script). The board fires these
    // when a clip finishes; headless, a driver delivers them explicitly. Each
    // fires the recovered event on the first component of that kind, only if the
    // page defines the handler. Video-end chains cutscenes
    // (Transparent_Video_Holder.TheEnd(id) -> Play(id+1)); the others complete
    // sounds and animations.
    void videoEnd(int id);       // Transparent_Video_Holder.TheEnd(id)
    void soundEnd(int id);       // Sound_Holder.EndPlaySound(id)
    void animationEnd(int id);   // Sprite_Holder.EndAnimation(id)

    // Advance the simulated clock by `ms`, firing each clip's completion callback
    // when its recovered duration elapses. A video that finishes may start the
    // next one (TheEnd -> Play), so cutscene chains play through on their own:
    // advanceTime(60000) walks INTRO's ten clips and LoadPages to the menu.
    void advanceTime(int ms);
    int clockMs() const { return clockMs_; }

    // Convenience: a full press -> move -> release drag of whatever is under the
    // start point. Fires MouseClickOnDown, drags a draggable sprite to (x1,y1),
    // then MouseClickOnUp + MouseDrop.
    void drag(int x0, int y0, int x1, int y1) {
        lButtonDown(x0, y0);
        mouseMove(x1, y1);
        lButtonUp(x1, y1);
    }

    // Host interface.
    Value callBuiltin(const std::string& name, const std::vector<Value>& args) override;
    ComponentResult callComponent(const std::string& component, const std::string& method,
                                  const std::vector<Value>& args) override;

    // Queries.
    const ComponentState* component(const std::string& name) const;
    std::vector<std::string> componentNames() const;
    const std::vector<ComponentState>& components() const { return components_; }
    const std::vector<ComponentState>& groupComponents() const { return groupComponents_; }
    const std::vector<std::uint8_t>& groupBytes() const { return groupBytes_; }
    const std::vector<CursorBitmap>& groupCursors() const { return groupDocument_.cursors; }
    const std::string& scriptText() const { return script_; }
    const std::vector<HostCallRecord>& callLog() const { return callLog_; }

    // Script globals. A Project seeds the project-wide globals (declared by the
    // START.PRJ setup block) before running OnOpenPage, and harvests them back
    // afterwards so writes carry to the next page.
    bool hasGlobal(const std::string& name) const;
    Value getGlobal(const std::string& name) const;
    void setGlobal(const std::string& name, Value value);

    // Page/host state set by builtins.
    const std::string& pendingPage() const { return pendingPage_; }
    const std::string& currentGroup() const { return currentGroup_; }
    int cursor() const { return cursor_; }
    int timerInterval() const { return timerInterval_; }
    bool exited() const { return exited_; }

    // Stored id ("rectID") of the top-most enabled HotSpot_Holder hotspot
    // containing (x, y), or -1. This is the value the engine hands the script,
    // which is the record's +0x18 field and NOT its array index. Uses the
    // recovered click-path rule: enabled, layer == the board's current layer
    // (so the highest layer wins), rect inclusive on every edge, last record on
    // that layer wins an overlap. See findHotSpotHit in page.cpp.
    int hitTestHotSpot(int x, int y) const;

private:
    Page() = default;
    void parse(BinaryReader& reader);
    ComponentState* resolve(const std::string& componentPath);
    const ComponentState* resolve(const std::string& componentPath) const;
    void loadGroup(const std::string& name);
    void clearGroup();
    std::filesystem::path resolveResourcePath(const std::string& name) const;

    // A pending playback completion: fire `event` on `component` with `id` once
    // the clock reaches `fireMs`.
    struct Pending {
        std::string component;
        std::string event;
        int id = 0;
        int fireMs = 0;
    };
    // Schedule/cancel a clip's completion when Play/Stop is dispatched.
    void scheduleCompletion(const ComponentState& state, const std::string& event, int id);
    void cancelCompletions(const std::string& component);

    // Advance gliding sprites (GotoXY) toward their targets, firing InPlace.
    void stepGlides(int elapsed);

    // The item a point lands on, across every clickable holder kind. `index` is
    // whatever the engine hands the script for that kind: a HotSpot_Holder's
    // stored id (record+0x18), or a Sprite_Holder's instance index.
    struct Hit {
        std::string component;
        HolderKind kind = HolderKind::Unknown;
        int index = -1;   // -1 for no hit
    };
    Hit findHit(int x, int y) const;

    // Fire `event` on the hit component with the item's id, if the page defines
    // the handler. No-op when nothing was hit.
    void fireHitEvent(const Hit& hit, const char* event);

    std::string script_;
    std::vector<ComponentState> components_;
    std::map<std::string, std::size_t> byName_;
    std::vector<ComponentState> groupComponents_;
    std::map<std::string, std::size_t> groupByName_;
    std::vector<std::uint8_t> groupBytes_;
    GroupDocument groupDocument_;
    std::filesystem::path resourceDirectory_;
    std::unique_ptr<Interpreter> interpreter_;

    std::vector<HostCallRecord> callLog_;
    std::string pendingPage_;
    std::string currentGroup_;
    int cursor_ = -1;
    int timerInterval_ = 0;
    bool exited_ = false;
    std::uint32_t randomState_ = 0x12345678u;

    // Each component holder receives mouse movement independently in the
    // original board. Track one hovered item per holder so a group hotspot can
    // slide the toolbar while a sprite drawn above it also receives hover.
    std::map<std::string, Hit> hoverByComponent_;

    // Simulated playback clock and the completions waiting on it.
    int clockMs_ = 0;
    std::vector<Pending> pending_;

    // Mouse-button / drag state spanning down -> move -> up.
    Hit pressed_;               // item that received MouseClickOnDown, for the Up
    bool dragging_ = false;     // a draggable sprite is being dragged
    int dragId_ = -1;           // sprite instance index being dragged
    int grabOffsetX_ = 0;       // cursor-to-topleft offset captured on press
    int grabOffsetY_ = 0;
};

} // namespace graphboard::runtime
