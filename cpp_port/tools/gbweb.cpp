// gbweb: the browser front-end shim for the reconstructed GraphBoard runtime.
//
// Compiled with Emscripten to gbweb.wasm and driven from JavaScript. It is
// gbgame.cpp with the Win32 layer removed: the same tick/advanceTime loop, the
// same callLog-observer audio replay, the same VideoPlayer frame source — but
// instead of GDI/waveOut the shim exposes a small extern "C" API. JS owns the
// clock (setInterval), the canvas (putImageData of the RGBA frame buffer),
// WebAudio playback (fed by drained audio events), text drawing (fillText of
// the visible TextHolder items), and page navigation (fetching a pending
// page's .BDF into MEMFS before asking the shim to follow it).
//
// Everything stateful lives here so the JS side stays a thin device layer,
// mirroring how gbgame's WndProc drives the identical GameState.

#include "graphboard/binary_reader.hpp"
#include "graphboard/format.hpp"
#include "graphboard/image.hpp"
#include "graphboard/render.hpp"
#include "graphboard/runtime/page.hpp"
#include "graphboard/runtime/project.hpp"
#include "graphboard/text.hpp"
#include "graphboard/video.hpp"

#include <emscripten/emscripten.h>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace {

namespace fs = std::filesystem;
using graphboard::BdfHeader;
using graphboard::CursorBitmap;
using graphboard::Image;
using graphboard::runtime::Page;
using graphboard::runtime::Project;

// LoadPage chains are script-driven; the JS pump bounds follows per tick the
// same way gbgame's pumpNavigation does (kMaxFollowsPerPump = 8 over there).

// ---------------------------------------------------------------------------
// Audio events: instead of opening waveOut devices, the shim queues commands
// for JS to replay through WebAudio. Each play event owns a copy of its PCM so
// the pointer stays valid until the next drain, regardless of page changes.
struct AudioEvent {
    enum class Type { Play, Stop, StopComponent, StopAll };
    Type type;
    std::string key;               // "component/id" (Stop) or prefix (StopComponent)
    std::vector<std::uint8_t> pcm; // raw samples, WAV framing already stripped
    std::uint32_t sampleRate = 0;
    std::uint32_t bitsPerSample = 0;
    std::uint32_t channels = 0;
};

// Same RIFF/WAVE walk gbgame's AudioEngine does.
bool parseWav(const std::uint8_t* wav, std::size_t n, std::uint32_t& sampleRate,
              std::uint32_t& channels, std::uint32_t& bits, std::size_t& dataOff,
              std::size_t& dataBytes) {
    auto u32 = [&](std::size_t off) -> std::uint32_t {
        return off + 4 <= n ? static_cast<std::uint32_t>(wav[off]) | (wav[off + 1] << 8) |
                                  (wav[off + 2] << 16) | (wav[off + 3] << 24)
                            : 0;
    };
    auto u16 = [&](std::size_t off) -> std::uint32_t {
        return off + 2 <= n ? static_cast<std::uint32_t>(wav[off]) | (wav[off + 1] << 8) : 0;
    };
    if (n < 12 || std::memcmp(wav, "RIFF", 4) != 0 || std::memcmp(wav + 8, "WAVE", 4) != 0) {
        return false;
    }
    std::size_t pos = 12;
    bool haveFmt = false;
    dataBytes = 0;
    while (pos + 8 <= n) {
        const std::uint32_t chunkSize = u32(pos + 4);
        if (chunkSize > n - pos - 8) break;
        if (std::memcmp(wav + pos, "fmt ", 4) == 0 && pos + 8 + 16 <= n) {
            const std::uint32_t formatTag = u16(pos + 8);
            channels = u16(pos + 8 + 2);
            sampleRate = u32(pos + 8 + 4);
            bits = u16(pos + 8 + 14);
            haveFmt = formatTag == 1 && channels > 0 && sampleRate > 0 && bits > 0 &&
                      bits % 8 == 0;
        } else if (std::memcmp(wav + pos, "data", 4) == 0) {
            dataOff = pos + 8;
            dataBytes = std::min<std::size_t>(chunkSize, n - dataOff);
        }
        pos += 8 + chunkSize + (chunkSize & 1);
    }
    return haveFmt && dataBytes > 0;
}

// ---------------------------------------------------------------------------
// Video frames: identical to gbgame's VideoPlayer — one BoardVideoDecoder per
// Transparent_Video_Holder entry, queried by renderPage.
class VideoPlayer : public graphboard::VideoFrameSource {
public:
    void loadPage(const Page& page, const std::vector<std::uint8_t>& bytes) {
        decoders_.clear();
        page_ = &page;
        auto add = [&](const std::vector<graphboard::runtime::ComponentState>& components,
                       const std::vector<std::uint8_t>& source) {
            for (const auto& c : components) {
                if (c.kind != graphboard::HolderKind::TransparentVideoHolder) continue;
                auto& list = decoders_[c.displayName];
                for (const auto& geom : c.videos) {
                    list.push_back(
                        std::make_unique<graphboard::BoardVideoDecoder>(source, geom));
                }
            }
        };
        add(page.components(), bytes);
        add(page.groupComponents(), page.groupBytes());
    }

    const std::uint8_t* currentFrame(const std::string& component, int entry,
                                     const std::uint8_t*& palette) override {
        const auto it = decoders_.find(component);
        if (it == decoders_.end() || entry < 0 ||
            static_cast<std::size_t>(entry) >= it->second.size() || !page_) {
            return nullptr;
        }
        auto& decoder = *it->second[static_cast<std::size_t>(entry)];
        const auto* c = page_->component(component);
        if (!c) return nullptr;
        const auto ci = c->items.find(entry);
        if (ci == c->items.end()) return nullptr;
        auto itemInt = [&](const char* key) -> int {
            const auto vi = ci->second.find(key);
            return vi != ci->second.end() ? static_cast<int>(vi->second.toInt()) : 0;
        };
        if (itemInt("playing") == 0 && itemInt("hasPlayed") == 0) {
            return nullptr;
        }
        int index;
        if (itemInt("playing") != 0) {
            const auto& geom = c->videos[static_cast<std::size_t>(entry)];
            const int duration = geom.frameDurationMs > 0 ? geom.frameDurationMs : 100;
            index = (page_->clockMs() - itemInt("playStartMs")) / duration;
        } else {
            // Not playing: the last played frame persists on screen.
            index = (std::max)(decoder.lastDecodedFrame(), 0);
        }
        const auto& frame = decoder.frameAt(index);
        if (frame.empty()) return nullptr;
        palette = decoder.palette().size() >= 1024 ? decoder.palette().data() : nullptr;
        return frame.data();
    }

    graphboard::BoardVideoDecoder* decoder(const std::string& component, int entry) {
        const auto it = decoders_.find(component);
        if (it == decoders_.end() || entry < 0 ||
            static_cast<std::size_t>(entry) >= it->second.size()) {
            return nullptr;
        }
        return it->second[static_cast<std::size_t>(entry)].get();
    }

    // True while any entry on the page is mid-playback (forces re-render).
    bool anyPlaying() const {
        if (!page_) return false;
        auto playing = [](const std::vector<graphboard::runtime::ComponentState>& components) {
            for (const auto& c : components) {
                if (c.kind != graphboard::HolderKind::TransparentVideoHolder) continue;
                for (const auto& [id, item] : c.items) {
                    const auto it = item.find("playing");
                    if (it != item.end() && it->second.toInt() != 0) return true;
                }
            }
            return false;
        };
        return playing(page_->components()) || playing(page_->groupComponents());
    }

private:
    const Page* page_ = nullptr;
    std::map<std::string, std::vector<std::unique_ptr<graphboard::BoardVideoDecoder>>> decoders_;
};

struct GameState {
    std::unique_ptr<Project> project;
    fs::path dataDir;

    std::vector<std::uint8_t> pageBytes;
    BdfHeader pageHeader;

    Image frame;                       // last composited scene
    std::vector<std::uint8_t> rgba;    // frame + cursor converted for putImageData
    bool dirty = true;

    int timerAccumMs = 0;              // toward the page's SetTimer interval

    VideoPlayer video;
    std::size_t seenCalls = 0;         // callLog() entries already reacted to
    std::vector<AudioEvent> audioEvents;
    std::string eventsJson;            // storage backing gb_drain_events()
    std::string textJson;              // storage backing gb_text_items_json()
    std::string pendingPage;           // storage backing gb_pending_page()
    std::string status;                // last script/load error, for the JS HUD

    int cursorX = 0, cursorY = 0;
    bool cursorInside = false;
};

GameState g;

void queueStopAll() {
    AudioEvent ev;
    ev.type = AudioEvent::Type::StopAll;
    g.audioEvents.push_back(std::move(ev));
}

// Load the current page's file bytes + header for rendering. Must run after
// every page change; the Page object alone cannot re-render its background.
void loadPageVisuals() {
    const auto path = g.dataDir / g.project->currentPageName();
    // MEMFS is provisioned by JS with the exact canonical name before
    // followPendingPage runs, but scripts may use any casing — reuse the
    // project's resolution by going through BinaryReader on the resolved path.
    fs::path resolved = path;
    if (!fs::exists(resolved)) {
        std::string wanted = g.project->currentPageName();
        std::transform(wanted.begin(), wanted.end(), wanted.begin(), [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });
        std::error_code ec;
        for (const auto& entry : fs::directory_iterator(g.dataDir, ec)) {
            if (!entry.is_regular_file()) continue;
            std::string name = entry.path().filename().string();
            std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {
                return static_cast<char>(std::tolower(c));
            });
            if (name == wanted) {
                resolved = entry.path();
                break;
            }
        }
    }
    auto reader = graphboard::BinaryReader::fromFile(resolved);
    g.pageBytes = reader.bytes();
    g.pageHeader = graphboard::parseBdfHeader(reader);
    g.timerAccumMs = 0;
    g.dirty = true;
    queueStopAll();
    g.seenCalls = 0;
    if (Page* page = g.project->currentPage()) {
        g.video.loadPage(*page, g.pageBytes);
    }
}

// React to host calls the scripts made since the last pump: queue the real
// audio the headless runtime only schedules. Mirrors gbgame::processHostCalls.
void processHostCalls() {
    Page* page = g.project ? g.project->currentPage() : nullptr;
    if (!page) return;
    const auto& calls = page->callLog();
    if (g.seenCalls > calls.size()) {
        g.seenCalls = 0;   // page changed under us
    }
    for (std::size_t i = g.seenCalls; i < calls.size(); ++i) {
        const auto& call = calls[i];
        if (call.builtin) {
            if (call.name == "LoadGroup" || call.name == "CloseGroup") {
                AudioEvent ev;
                ev.type = AudioEvent::Type::StopComponent;
                ev.key = "Group.";
                g.audioEvents.push_back(std::move(ev));
                g.video.loadPage(*page, g.pageBytes);
                g.dirty = true;
            }
            continue;
        }
        const auto* state = page->component(call.component);
        if (!state) continue;
        const int id = call.args.empty() ? 0 : static_cast<int>(call.args[0].toInt());
        const std::string key = state->displayName + "/" + std::to_string(id);

        if (state->kind == graphboard::HolderKind::SoundHolder) {
            if (call.name == "PlayDSound" || call.name == "PlayDSoundEx") {
                if (id >= 0 && static_cast<std::size_t>(id) < state->soundClips.size()) {
                    const auto& clip = state->soundClips[static_cast<std::size_t>(id)];
                    const auto& source = state->displayName.rfind("Group.", 0) == 0
                                             ? page->groupBytes() : g.pageBytes;
                    if (clip.offset + clip.byteCount <= source.size()) {
                        std::uint32_t rate = 0, channels = 0, bits = 0;
                        std::size_t dataOff = 0, dataBytes = 0;
                        const std::uint8_t* wav = source.data() + clip.offset;
                        if (parseWav(wav, clip.byteCount, rate, channels, bits, dataOff,
                                     dataBytes)) {
                            AudioEvent ev;
                            ev.type = AudioEvent::Type::Play;
                            ev.key = key;
                            ev.pcm.assign(wav + dataOff, wav + dataOff + dataBytes);
                            ev.sampleRate = rate;
                            ev.bitsPerSample = bits;
                            ev.channels = channels;
                            g.audioEvents.push_back(std::move(ev));
                        }
                    }
                }
            } else if (call.name == "Stop" || call.name == "StopDSound") {
                AudioEvent ev;
                ev.type = AudioEvent::Type::Stop;
                ev.key = key;
                g.audioEvents.push_back(std::move(ev));
            } else if (call.name == "StopAll") {
                AudioEvent ev;
                ev.type = AudioEvent::Type::StopComponent;
                ev.key = state->displayName + "/";
                g.audioEvents.push_back(std::move(ev));
            }
        } else if (state->kind == graphboard::HolderKind::TransparentVideoHolder) {
            if (call.name == "Play") {
                // One clip at a time per holder, exactly like the runtime state.
                AudioEvent stop;
                stop.type = AudioEvent::Type::StopComponent;
                stop.key = state->displayName + "/";
                g.audioEvents.push_back(std::move(stop));
                if (auto* decoder = g.video.decoder(state->displayName, id)) {
                    if (!decoder->audioPcm().empty()) {
                        AudioEvent ev;
                        ev.type = AudioEvent::Type::Play;
                        ev.key = key;
                        ev.pcm = decoder->audioPcm();
                        ev.sampleRate = decoder->audioSampleRate();
                        ev.bitsPerSample = decoder->audioBitsPerSample();
                        ev.channels = decoder->audioChannels();
                        g.audioEvents.push_back(std::move(ev));
                    }
                }
            } else if (call.name == "Stop" || call.name == "StopAll") {
                AudioEvent ev;
                ev.type = AudioEvent::Type::StopComponent;
                ev.key = state->displayName + "/";
                g.audioEvents.push_back(std::move(ev));
            }
        }
    }
    g.seenCalls = calls.size();
}

const CursorBitmap* activeCursor() {
    const Page* page = g.project ? g.project->currentPage() : nullptr;
    if (!page || page->cursor() < 0) return nullptr;
    const auto& cursors = page->groupCursors();
    const auto index = static_cast<std::size_t>(page->cursor());
    return index < cursors.size() ? &cursors[index] : nullptr;
}

// Composite the group's indexed cursor artwork straight into the RGBA buffer
// (gbgame paints it in the GDI pass; here the frame buffer is the only canvas).
void drawCursorInto(std::vector<std::uint8_t>& rgba) {
    const CursorBitmap* cursor = activeCursor();
    if (!cursor || !g.cursorInside || cursor->width == 0 || cursor->height == 0 ||
        cursor->width > 1024 || cursor->height > 1024 || g.frame.width <= 0 ||
        g.frame.height <= 0) {
        return;
    }
    const std::size_t packedStride = cursor->width;
    const std::size_t dibStride = (static_cast<std::size_t>(cursor->width) + 3u) & ~3u;
    const bool legacyBottomUp = cursor->height <= cursor->pixels.size() / dibStride;
    const std::size_t stride = legacyBottomUp ? dibStride : packedStride;
    if (cursor->height > cursor->pixels.size() / stride) return;

    const bool hasPalette = g.pageHeader.paletteByteCount >= 1024 &&
                            g.pageHeader.paletteOffset + 1024 <= g.pageBytes.size();
    const auto* palette = hasPalette ? g.pageBytes.data() + g.pageHeader.paletteOffset : nullptr;
    const int originX = g.cursorX - cursor->hotX;
    const int originY = g.cursorY - cursor->hotY;

    for (std::uint32_t row = 0; row < cursor->height; ++row) {
        const std::uint32_t sourceRow = legacyBottomUp ? cursor->height - 1u - row : row;
        const auto* source = cursor->pixels.data() + static_cast<std::size_t>(sourceRow) * stride;
        const int y = originY + static_cast<int>(row);
        if (y < 0 || y >= g.frame.height) continue;
        for (std::uint32_t col = 0; col < cursor->width; ++col) {
            const std::uint8_t index = source[col];
            if (index == cursor->transparentIndex) continue;
            const int x = originX + static_cast<int>(col);
            if (x < 0 || x >= g.frame.width) continue;
            const auto* color = palette ? palette + static_cast<std::size_t>(index) * 4u : nullptr;
            auto* dst = rgba.data() +
                        (static_cast<std::size_t>(y) * g.frame.width + x) * 4;
            if (color) {
                dst[0] = color[0];
                dst[1] = color[1];
                dst[2] = color[2];
            } else {
                dst[0] = dst[1] = dst[2] = index;
            }
            dst[3] = 255;
        }
    }
}

// A page with a running video or animating/gliding sprite changes appearance
// every tick even when no script call lands.
bool sceneIsLive() {
    const Page* page = g.project ? g.project->currentPage() : nullptr;
    if (!page) return false;
    if (g.video.anyPlaying()) return true;
    auto liveSprites = [](const std::vector<graphboard::runtime::ComponentState>& components) {
        for (const auto& c : components) {
            if (c.kind != graphboard::HolderKind::SpriteHolder) continue;
            for (const auto& [id, item] : c.items) {
                const auto anim = item.find("animating");
                if (anim != item.end() && anim->second.toInt() != 0) return true;
                const auto glide = item.find("gliding");
                if (glide != item.end() && glide->second.toInt() != 0) return true;
            }
        }
        return false;
    };
    return liveSprites(page->components()) || liveSprites(page->groupComponents());
}

// Run one runtime entry point, surfacing script errors via gb_status instead
// of aborting.
template <typename Fn>
void runScriptEvent(Fn&& fn) {
    if (!g.project || !g.project->currentPage()) return;
    Page* before = g.project->currentPage();
    const std::size_t callsBefore = before->callLog().size();
    try {
        fn(*before);
    } catch (const std::exception& ex) {
        g.status = ex.what();
    }
    if (before->callLog().size() != callsBefore) {
        g.dirty = true;
    }
    processHostCalls();
}

void appendJsonString(std::string& out, const std::string& s) {
    out.push_back('"');
    for (const char raw : s) {
        const unsigned char c = static_cast<unsigned char>(raw);
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                if (c < 0x20) {
                    char buf[8];
                    std::snprintf(buf, sizeof(buf), "\\u%04x", c);
                    out += buf;
                } else {
                    out.push_back(raw);
                }
        }
    }
    out.push_back('"');
}

} // namespace

extern "C" {

// ---- project lifecycle ----

EMSCRIPTEN_KEEPALIVE
int gb_load_project(const char* startPrjUtf8) {
    try {
        const fs::path startPrj(startPrjUtf8);
        g = GameState{};
        g.project = Project::loadFromFile(startPrj);
        g.dataDir = startPrj.parent_path();
        return 1;
    } catch (const std::exception& ex) {
        g.status = ex.what();
        return 0;
    }
}

// Manifest's startup page name (raw, as scripts spell it) so JS can fetch its
// .BDF into MEMFS before gb_open_startup runs.
EMSCRIPTEN_KEEPALIVE
const char* gb_startup_page() {
    static std::string out;
    out = g.project ? g.project->manifest().startupPage : "";
    return out.c_str();
}

EMSCRIPTEN_KEEPALIVE
int gb_open_startup() {
    if (!g.project) return 0;
    try {
        g.project->openStartupPage();
        loadPageVisuals();
        processHostCalls();   // audio the startup page's OnOpenPage started
        return 1;
    } catch (const std::exception& ex) {
        g.status = ex.what();
        return 0;
    }
}

EMSCRIPTEN_KEEPALIVE
int gb_exited() {
    const Page* page = g.project ? g.project->currentPage() : nullptr;
    return page && page->exited() ? 1 : 0;
}

// Last script/load error (empty string when clean). Cleared on read.
EMSCRIPTEN_KEEPALIVE
const char* gb_status() {
    static std::string out;
    out = g.status;
    g.status.clear();
    return out.c_str();
}

// UTF-8 name of the current page (cp1250 converted), for the JS HUD.
EMSCRIPTEN_KEEPALIVE
const char* gb_current_page() {
    static std::string out;
    out = g.project ? graphboard::cp1250ToUtf8(g.project->currentPageName()) : "";
    return out.c_str();
}

// ---- navigation protocol ----
// JS: after every tick/input call, read gb_pending_page(); if non-empty, fetch
// that .BDF into MEMFS under the data dir, then call gb_follow_pending().
// Repeat (bounded) because the new page's OnOpenPage may chain another load.

EMSCRIPTEN_KEEPALIVE
const char* gb_pending_page() {
    const Page* page = g.project ? g.project->currentPage() : nullptr;
    g.pendingPage = page ? page->pendingPage() : "";
    return g.pendingPage.c_str();
}

EMSCRIPTEN_KEEPALIVE
int gb_follow_pending() {
    if (!g.project) return 0;
    try {
        if (!g.project->followPendingPage()) return 0;
        loadPageVisuals();
        processHostCalls();
        return 1;
    } catch (const std::exception& ex) {
        g.status = ex.what();
        return 0;
    }
}

// ---- clock + input (mirrors gbgame's onTick / WndProc handlers) ----

EMSCRIPTEN_KEEPALIVE
void gb_tick(int elapsedMs) {
    if (!g.project || !g.project->currentPage()) return;
    const int elapsed = std::clamp(elapsedMs, 0, 250);

    // Clip playback clock: fires TheEnd/EndPlaySound/EndAnimation on schedule,
    // which is what paces the intro cutscene chain.
    runScriptEvent([&](Page& page) { page.advanceTime(elapsed); });
    if (sceneIsLive()) {
        g.dirty = true;
    }

    // Script timer: OnTimer every SetTimer(interval) ms.
    Page* page = g.project->currentPage();
    if (page) {
        const int interval = page->timerInterval();
        if (interval > 0) {
            g.timerAccumMs += elapsed;
            while (g.timerAccumMs >= interval && g.project->currentPage() == page &&
                   page->pendingPage().empty()) {
                g.timerAccumMs -= interval;
                runScriptEvent([](Page& p) { p.timer(); });
            }
        } else {
            g.timerAccumMs = 0;
        }
    }
}

EMSCRIPTEN_KEEPALIVE
void gb_mouse_move(int x, int y) {
    g.cursorX = x;
    g.cursorY = y;
    g.cursorInside = true;
    runScriptEvent([&](Page& page) { page.mouseMove(x, y); });
    // The software cursor is composited over a clean scene buffer, so a
    // mouse-only move needs a re-blit even when no script call fired.
    if (activeCursor()) {
        g.dirty = true;
    }
}

EMSCRIPTEN_KEEPALIVE
void gb_mouse_leave() {
    g.cursorInside = false;
    if (activeCursor()) {
        g.dirty = true;
    }
}

EMSCRIPTEN_KEEPALIVE
void gb_mouse_down(int x, int y) {
    runScriptEvent([&](Page& page) { page.lButtonDown(x, y); });
}

EMSCRIPTEN_KEEPALIVE
void gb_mouse_up(int x, int y) {
    runScriptEvent([&](Page& page) { page.lButtonUp(x, y); });
}

EMSCRIPTEN_KEEPALIVE
void gb_mouse_rdown(int x, int y) {
    runScriptEvent([&](Page& page) { page.rButtonDown(x, y); });
}

EMSCRIPTEN_KEEPALIVE
void gb_key_down(int vk) {
    runScriptEvent([&](Page& page) { page.keyDown(vk); });
}

// ---- frame buffer ----

// Composite if dirty. Returns 1 when the RGBA buffer changed.
EMSCRIPTEN_KEEPALIVE
int gb_render() {
    Page* page = g.project ? g.project->currentPage() : nullptr;
    if (!page || !g.dirty) return 0;
    try {
        g.frame = graphboard::renderPage(*page, g.pageBytes, g.pageHeader, &g.video);
    } catch (const std::exception& ex) {
        g.status = ex.what();
        return 0;
    }
    const std::size_t n = static_cast<std::size_t>(g.frame.width) * g.frame.height;
    g.rgba.resize(n * 4);
    const std::uint8_t* src = g.frame.rgb.data();
    std::uint8_t* dst = g.rgba.data();
    for (std::size_t i = 0; i < n; ++i, src += 3, dst += 4) {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = 255;
    }
    drawCursorInto(g.rgba);
    g.dirty = false;
    return 1;
}

EMSCRIPTEN_KEEPALIVE const std::uint8_t* gb_frame_ptr() { return g.rgba.data(); }
EMSCRIPTEN_KEEPALIVE int gb_frame_w() { return g.frame.width; }
EMSCRIPTEN_KEEPALIVE int gb_frame_h() { return g.frame.height; }

// 1 when JS should hide the CSS cursor: the page hides the pointer (index < 0)
// or the group cursor is being composited. Mirrors gbgame's WM_SETCURSOR.
EMSCRIPTEN_KEEPALIVE
int gb_cursor_hidden() {
    const Page* page = g.project ? g.project->currentPage() : nullptr;
    if (!page) return 0;
    return page->cursor() < 0 || activeCursor() != nullptr ? 1 : 0;
}

// ---- audio + text for the JS device layer ----

// Drain queued audio commands as JSON:
//   [{"type":"play","key":"Sound_Holder/3","ptr":123,"len":456,
//     "rate":22050,"bits":8,"channels":1},
//    {"type":"stop","key":...}, {"type":"stopComponent","key":prefix},
//    {"type":"stopAll"}]
// PCM pointers stay valid until the NEXT drain call — JS must copy the sample
// bytes out of HEAPU8 synchronously.
EMSCRIPTEN_KEEPALIVE
const char* gb_drain_events() {
    static std::vector<AudioEvent> held;  // keeps last drain's PCM alive
    held = std::move(g.audioEvents);
    g.audioEvents.clear();
    g.eventsJson = "[";
    bool first = true;
    for (const auto& ev : held) {
        if (!first) g.eventsJson.push_back(',');
        first = false;
        switch (ev.type) {
            case AudioEvent::Type::Play:
                g.eventsJson += "{\"type\":\"play\",\"key\":";
                appendJsonString(g.eventsJson, ev.key);
                g.eventsJson += ",\"ptr\":" +
                    std::to_string(reinterpret_cast<std::uintptr_t>(ev.pcm.data())) +
                    ",\"len\":" + std::to_string(ev.pcm.size()) +
                    ",\"rate\":" + std::to_string(ev.sampleRate) +
                    ",\"bits\":" + std::to_string(ev.bitsPerSample) +
                    ",\"channels\":" + std::to_string(ev.channels) + "}";
                break;
            case AudioEvent::Type::Stop:
                g.eventsJson += "{\"type\":\"stop\",\"key\":";
                appendJsonString(g.eventsJson, ev.key);
                g.eventsJson += "}";
                break;
            case AudioEvent::Type::StopComponent:
                g.eventsJson += "{\"type\":\"stopComponent\",\"key\":";
                appendJsonString(g.eventsJson, ev.key);
                g.eventsJson += "}";
                break;
            case AudioEvent::Type::StopAll:
                g.eventsJson += "{\"type\":\"stopAll\"}";
                break;
        }
    }
    g.eventsJson.push_back(']');
    return g.eventsJson.c_str();
}

// Visible TextHolder items as JSON [{"l":..,"t":..,"r":..,"b":..,"text":".."}],
// text already cp1250→UTF-8 with the per-line leading '/' markers stripped —
// the same cleanup gbgame's paint pass applies before DrawTextW.
EMSCRIPTEN_KEEPALIVE
const char* gb_text_items_json() {
    g.textJson = "[";
    const Page* page = g.project ? g.project->currentPage() : nullptr;
    bool first = true;
    if (page) {
        for (const auto& component : page->components()) {
            if (component.kind != graphboard::HolderKind::TextHolder) continue;
            for (const auto& [id, item] : component.items) {
                const auto visible = item.find("visible");
                if (visible == item.end() || visible->second.toInt() == 0 || id < 0 ||
                    static_cast<std::size_t>(id) >= component.texts.size()) continue;
                const auto& text = component.texts[static_cast<std::size_t>(id)];
                std::string cleaned;
                cleaned.reserve(text.text.size());
                bool lineStart = true;
                for (char ch : text.text) {
                    if (lineStart && ch == '/') continue;
                    cleaned.push_back(ch);
                    lineStart = ch == '\n' || ch == '\r';
                }
                if (!first) g.textJson.push_back(',');
                first = false;
                g.textJson += "{\"l\":" + std::to_string(text.left) +
                              ",\"t\":" + std::to_string(text.top) +
                              ",\"r\":" + std::to_string(text.right) +
                              ",\"b\":" + std::to_string(text.bottom) + ",\"text\":";
                appendJsonString(g.textJson, graphboard::cp1250ToUtf8(cleaned));
                g.textJson += "}";
            }
        }
    }
    g.textJson.push_back(']');
    return g.textJson.c_str();
}

} // extern "C"
