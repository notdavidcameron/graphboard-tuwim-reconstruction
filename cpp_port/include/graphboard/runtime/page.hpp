#pragma once

#include "graphboard/binary_reader.hpp"
#include "graphboard/holders.hpp"
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
};

struct ComponentState {
    std::string displayName;               // e.g. "Sprite_Holder"
    HolderKind kind = HolderKind::Unknown;
    std::map<int, std::map<std::string, Value>> items;  // per-index item state
    std::map<std::string, Value> props;                 // holder-wide state
    std::vector<HotSpot> hotspots;         // HotSpot_Holder geometry (for hit tests)
    std::vector<SpriteGeometry> sprites;   // Sprite_Holder geometry, by spriteID
    std::vector<BitmapGeometry> bitmaps;   // Bitmap_Holder geometry, by bitmapID
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
// This models the script->component direction (dispatch to stateful components)
// and page/host builtins. The component->script callback direction (a hotspot
// click or video-end firing Sprite_Holder.MouseClickOnDown, etc.) is exposed as
// a hit-test query for now; wiring those callbacks is the next step.
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
    // IObject::LButtonDown/MouseMove to each component) the hit HotSpot_Holder
    // item's own callback fires if the page script defines it. Only
    // HotSpot_Holder is wired here: its hit-test rule and event names are
    // verified (see docs/component_interfaces.md); other holder kinds need
    // per-item click geometry and a verified cross-kind z-order rule before
    // they can be dispatched the same way (see runtime_recovery_notes.md).
    void lButtonDown(int x, int y);
    void rButtonDown(int x, int y);
    void mouseMove(int x, int y);
    void keyDown(int key) { runEvent("OnKeyDown", {Value::integer(key)}); }

    // Host interface.
    Value callBuiltin(const std::string& name, const std::vector<Value>& args) override;
    Value callComponent(const std::string& component, const std::string& method,
                        const std::vector<Value>& args) override;

    // Queries.
    const ComponentState* component(const std::string& name) const;
    std::vector<std::string> componentNames() const;
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
    std::unique_ptr<Interpreter> interpreter_;

    std::vector<HostCallRecord> callLog_;
    std::string pendingPage_;
    std::string currentGroup_;
    int cursor_ = -1;
    int timerInterval_ = 0;
    bool exited_ = false;
    std::uint32_t randomState_ = 0x12345678u;

    // Last hovered item, for MouseMoveIn/MouseMoveOut edge detection across
    // successive mouseMove() calls.
    Hit hover_;
};

} // namespace graphboard::runtime
