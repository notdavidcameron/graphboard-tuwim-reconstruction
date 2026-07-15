// gbgame: a windowed front-end for the reconstructed GraphBoard runtime.
//
// Wraps the headless runtime (Project/Page + renderPage) in a Win32 window:
// the live scene is composited each tick and blitted with GDI, raw mouse and
// keyboard input feed the page's recovered input entry points, a real-time
// clock drives advanceTime (so recovered clip durations pace cutscenes), and
// script LoadPage navigation is followed page to page — the same loop the
// original GraphBoard view ran. Text rendering and the active group's indexed
// cursor artwork are composited in the final Win32 paint pass.

#include "graphboard/binary_reader.hpp"
#include "graphboard/format.hpp"
#include "graphboard/image.hpp"
#include "graphboard/render.hpp"
#include "graphboard/runtime/page.hpp"
#include "graphboard/runtime/project.hpp"
#include "graphboard/text.hpp"
#include "graphboard/video.hpp"

#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <mmsystem.h>
#include <shellapi.h>

#include <algorithm>
#include <cctype>
#include <cwctype>
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

constexpr UINT_PTR kTickTimerId = 1;
constexpr int kTickMs = 33;
// LoadPage chains are script-driven; bound follows per pump so a page cycle
// (A loads B loads A ...) cannot wedge the UI thread.
constexpr int kMaxFollowsPerPump = 8;

std::wstring utf8ToWide(const std::string& s) {
    if (s.empty()) return L"";
    const int n = MultiByteToWideChar(CP_UTF8, 0, s.data(), static_cast<int>(s.size()), nullptr, 0);
    std::wstring out(n, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.data(), static_cast<int>(s.size()), out.data(), n);
    return out;
}

// Page names travel through the runtime as cp1250; window text wants UTF-16.
std::wstring pageNameToWide(const std::string& cp1250) {
    return utf8ToWide(graphboard::cp1250ToUtf8(cp1250));
}

// ---------------------------------------------------------------------------
// Audio: each playing clip gets its own waveOut device handle so background
// music (menu PlayDSound 25) and voice-overs overlap, matching DirectSound's
// mixing in the original. Clips are keyed by "component/id" so a script Stop
// can silence exactly the clip it names.
class AudioEngine {
public:
    ~AudioEngine() { stopAll(); }

    // `pcm` is raw sample data (already stripped of any WAV framing).
    void play(const std::string& key, std::vector<std::uint8_t> pcm,
              std::uint32_t sampleRate, std::uint32_t bitsPerSample, std::uint32_t channels) {
        stop(key);
        if (pcm.empty() || sampleRate == 0 || channels == 0 || bitsPerSample == 0) {
            return;
        }
        auto clip = std::make_unique<Clip>();
        clip->pcm = std::move(pcm);

        WAVEFORMATEX fmt{};
        fmt.wFormatTag = WAVE_FORMAT_PCM;
        fmt.nChannels = static_cast<WORD>(channels);
        fmt.nSamplesPerSec = sampleRate;
        fmt.wBitsPerSample = static_cast<WORD>(bitsPerSample);
        fmt.nBlockAlign = static_cast<WORD>(channels * bitsPerSample / 8);
        fmt.nAvgBytesPerSec = sampleRate * fmt.nBlockAlign;
        if (waveOutOpen(&clip->device, WAVE_MAPPER, &fmt, 0, 0, CALLBACK_NULL) !=
            MMSYSERR_NOERROR) {
            return;
        }
        clip->header.lpData = reinterpret_cast<LPSTR>(clip->pcm.data());
        clip->header.dwBufferLength = static_cast<DWORD>(clip->pcm.size());
        if (waveOutPrepareHeader(clip->device, &clip->header, sizeof(clip->header)) !=
            MMSYSERR_NOERROR) {
            waveOutClose(clip->device);
            return;
        }
        if (waveOutWrite(clip->device, &clip->header, sizeof(clip->header)) != MMSYSERR_NOERROR) {
            waveOutUnprepareHeader(clip->device, &clip->header, sizeof(clip->header));
            waveOutClose(clip->device);
            return;
        }
        playing_[key] = std::move(clip);
    }

    // Play a complete RIFF/WAVE file image.
    void playWav(const std::string& key, const std::uint8_t* wav, std::size_t byteCount) {
        std::uint32_t sampleRate = 0, channels = 0, bits = 0;
        std::size_t dataOff = 0, dataBytes = 0;
        if (!parseWav(wav, byteCount, sampleRate, channels, bits, dataOff, dataBytes)) {
            return;
        }
        play(key, std::vector<std::uint8_t>(wav + dataOff, wav + dataOff + dataBytes),
             sampleRate, bits, channels);
    }

    void stop(const std::string& key) {
        const auto it = playing_.find(key);
        if (it != playing_.end()) {
            release(*it->second);
            playing_.erase(it);
        }
    }

    // Stop every clip of one component ("Sound_Holder/..." prefix).
    void stopComponent(const std::string& componentPrefix) {
        for (auto it = playing_.begin(); it != playing_.end();) {
            if (it->first.rfind(componentPrefix, 0) == 0) {
                release(*it->second);
                it = playing_.erase(it);
            } else {
                ++it;
            }
        }
    }

    void stopAll() {
        for (auto& [key, clip] : playing_) {
            release(*clip);
        }
        playing_.clear();
    }

    // Reap devices whose buffer finished so handles do not accumulate.
    void reapFinished() {
        for (auto it = playing_.begin(); it != playing_.end();) {
            if (it->second->header.dwFlags & WHDR_DONE) {
                release(*it->second);
                it = playing_.erase(it);
            } else {
                ++it;
            }
        }
    }

private:
    struct Clip {
        HWAVEOUT device = nullptr;
        WAVEHDR header{};
        std::vector<std::uint8_t> pcm;
    };

    static void release(Clip& clip) {
        if (clip.device) {
            waveOutReset(clip.device);
            if (clip.header.dwFlags & WHDR_PREPARED) {
                waveOutUnprepareHeader(clip.device, &clip.header, sizeof(clip.header));
            }
            waveOutClose(clip.device);
            clip.device = nullptr;
        }
    }

    static bool parseWav(const std::uint8_t* wav, std::size_t n, std::uint32_t& sampleRate,
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
                haveFmt = formatTag == WAVE_FORMAT_PCM && channels > 0 && sampleRate > 0 &&
                          bits > 0 && bits % 8 == 0;
            } else if (std::memcmp(wav + pos, "data", 4) == 0) {
                dataOff = pos + 8;
                dataBytes = std::min<std::size_t>(chunkSize, n - dataOff);
            }
            pos += 8 + chunkSize + (chunkSize & 1);
        }
        return haveFmt && dataBytes > 0;
    }

    std::map<std::string, std::unique_ptr<Clip>> playing_;
};

// ---------------------------------------------------------------------------
// Video frames: one BoardVideoDecoder per Transparent_Video_Holder entry of
// the current page, queried by renderPage for whatever entry is playing.
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
        for (const auto& c : page_->components()) {
            if (c.kind != graphboard::HolderKind::TransparentVideoHolder) continue;
            for (const auto& [id, item] : c.items) {
                const auto it = item.find("playing");
                if (it != item.end() && it->second.toInt() != 0) return true;
            }
        }
        for (const auto& c : page_->groupComponents()) {
            if (c.kind != graphboard::HolderKind::TransparentVideoHolder) continue;
            for (const auto& [id, item] : c.items) {
                const auto it = item.find("playing");
                if (it != item.end() && it->second.toInt() != 0) return true;
            }
        }
        return false;
    }

private:
    const Page* page_ = nullptr;
    std::map<std::string, std::vector<std::unique_ptr<graphboard::BoardVideoDecoder>>> decoders_;
};

struct GameState {
    std::unique_ptr<Project> project;
    fs::path dataDir;

    // Raw bytes + parsed header of the current page's .BDF, kept alongside the
    // live Page because renderPage composites over the file's own background
    // and palette.
    std::vector<std::uint8_t> pageBytes;
    BdfHeader pageHeader;

    Image frame;                       // last composited scene
    std::vector<std::uint8_t> blit;    // frame converted to BGRA for GDI
    bool dirty = true;

    ULONGLONG lastTickMs = 0;
    int timerAccumMs = 0;              // toward the page's SetTimer interval

    AudioEngine audio;
    VideoPlayer video;
    std::size_t seenCalls = 0;         // callLog() entries already reacted to

    std::wstring projectTitle;
    std::wstring statusText;           // script error surfaced in the title bar
    POINT cursorPage{0, 0};
    bool cursorInside = false;
    bool trackingMouse = false;
};

GameState g;
HWND gWnd = nullptr;

// The same case-insensitive lookup Project::resolvePagePath uses; the shipped
// DATA folder is upper case while scripts say "intro.bdf".
fs::path resolvePagePath(const fs::path& dir, const std::string& pageName) {
    const fs::path direct = dir / pageName;
    if (fs::exists(direct)) return direct;
    std::string wanted = pageName;
    std::transform(wanted.begin(), wanted.end(), wanted.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    std::error_code ec;
    for (const auto& entry : fs::directory_iterator(dir, ec)) {
        if (!entry.is_regular_file()) continue;
        std::string name = entry.path().filename().string();
        std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });
        if (name == wanted) return entry.path();
    }
    throw graphboard::ParseError("page not found: " + pageName);
}

void setTitle() {
    std::wstring title = g.projectTitle.empty() ? L"GraphBoard" : g.projectTitle;
    title += L" — ";
    title += g.project ? pageNameToWide(g.project->currentPageName()) : L"(no project)";
    if (!g.statusText.empty()) {
        title += L"  [";
        title += g.statusText;
        title += L"]";
    }
    SetWindowTextW(gWnd, title.c_str());
}

std::wstring projectTitle(const fs::path& startPrj, const Project& project) {
    if (!project.manifest().decodedSignature.empty()) {
        return pageNameToWide(project.manifest().decodedSignature);
    }
    fs::path directory = startPrj.parent_path();
    std::wstring leaf = directory.filename().wstring();
    std::wstring lower = leaf;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](wchar_t ch) { return static_cast<wchar_t>(std::towlower(ch)); });
    if (lower == L"data" && directory.has_parent_path() &&
        !directory.parent_path().filename().empty()) {
        leaf = directory.parent_path().filename().wstring();
    }
    return leaf.empty() ? L"GraphBoard" : leaf;
}

// Size the window's client area to the rendered page and lock resizing to it.
void fitWindowToFrame() {
    if (g.frame.width <= 0 || g.frame.height <= 0) return;
    RECT rc{0, 0, g.frame.width, g.frame.height};
    const DWORD style = static_cast<DWORD>(GetWindowLongPtrW(gWnd, GWL_STYLE));
    AdjustWindowRect(&rc, style, FALSE);
    SetWindowPos(gWnd, nullptr, 0, 0, rc.right - rc.left, rc.bottom - rc.top,
                 SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

// Load the current page's file bytes + header for rendering. Must run after
// every page change; the Page object alone cannot re-render its background.
void loadPageVisuals() {
    const auto path = resolvePagePath(g.dataDir, g.project->currentPageName());
    auto reader = graphboard::BinaryReader::fromFile(path);
    g.pageBytes = reader.bytes();
    g.pageHeader = graphboard::parseBdfHeader(reader);
    g.timerAccumMs = 0;
    g.dirty = true;
    g.audio.stopAll();
    g.seenCalls = 0;
    if (Page* page = g.project->currentPage()) {
        g.video.loadPage(*page, g.pageBytes);
    }
}

// React to host calls the scripts made since the last pump: start/stop the
// real audio the headless runtime only schedules. The call log is the same
// record gbinspect prints, so playback stays a pure observer of the runtime.
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
                g.audio.stopComponent("Group.");
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
                        g.audio.playWav(key, source.data() + clip.offset, clip.byteCount);
                    }
                }
            } else if (call.name == "Stop" || call.name == "StopDSound") {
                g.audio.stop(key);
            } else if (call.name == "StopAll") {
                g.audio.stopComponent(state->displayName + "/");
            }
        } else if (state->kind == graphboard::HolderKind::TransparentVideoHolder) {
            if (call.name == "Play") {
                // One clip at a time per holder, exactly like the runtime state.
                g.audio.stopComponent(state->displayName + "/");
                if (auto* decoder = g.video.decoder(state->displayName, id)) {
                    if (!decoder->audioPcm().empty()) {
                        g.audio.play(key, decoder->audioPcm(), decoder->audioSampleRate(),
                                     decoder->audioBitsPerSample(), decoder->audioChannels());
                    }
                }
            } else if (call.name == "Stop" || call.name == "StopAll") {
                g.audio.stopComponent(state->displayName + "/");
            }
        }
    }
    g.seenCalls = calls.size();
}

// Composite the live scene and convert to the BGRA layout StretchDIBits wants.
void renderFrame() {
    Page* page = g.project ? g.project->currentPage() : nullptr;
    if (!page) return;
    const int oldW = g.frame.width, oldH = g.frame.height;
    g.frame = graphboard::renderPage(*page, g.pageBytes, g.pageHeader, &g.video);
    g.blit.resize(static_cast<std::size_t>(g.frame.width) * g.frame.height * 4);
    const std::uint8_t* src = g.frame.rgb.data();
    std::uint8_t* dst = g.blit.data();
    for (std::size_t i = 0, n = static_cast<std::size_t>(g.frame.width) * g.frame.height;
         i < n; ++i, src += 3, dst += 4) {
        dst[0] = src[2];
        dst[1] = src[1];
        dst[2] = src[0];
        dst[3] = 255;
    }
    if (g.frame.width != oldW || g.frame.height != oldH) {
        fitWindowToFrame();
    }
    g.dirty = false;
}

// After any script ran: follow LoadPage requests (bounded), honor Exit(), and
// refresh the visuals if the page changed.
void pumpNavigation() {
    if (!g.project) return;
    Page* page = g.project->currentPage();
    if (page && page->exited()) {
        DestroyWindow(gWnd);
        return;
    }
    try {
        for (int i = 0; i < kMaxFollowsPerPump && g.project->followPendingPage(); ++i) {
            loadPageVisuals();
            setTitle();
            processHostCalls();   // audio the new page's OnOpenPage started
            page = g.project->currentPage();
            if (page && page->exited()) {
                DestroyWindow(gWnd);
                return;
            }
        }
    } catch (const std::exception& ex) {
        g.statusText = utf8ToWide(ex.what());
        setTitle();
    }
}

// Run one runtime entry point, surfacing script errors in the title bar
// instead of tearing the window down.
template <typename Fn>
void runScriptEvent(Fn&& fn) {
    if (!g.project || !g.project->currentPage()) return;
    Page* before = g.project->currentPage();
    const std::size_t callsBefore = before->callLog().size();
    try {
        fn(*before);
    } catch (const std::exception& ex) {
        g.statusText = utf8ToWide(ex.what());
        setTitle();
    }
    // Only a script that actually called into the host can have changed the
    // scene; pure mouse-move polling stays cheap.
    if (before->callLog().size() != callsBefore) {
        g.dirty = true;
    }
    processHostCalls();
    pumpNavigation();
}

// Map a client-area point to page coordinates (the window client is kept at
// the frame's size, so this is normally identity; guard against a mismatch
// while a resize is in flight).
POINT clientToPagePoint(POINT p) {
    RECT rc;
    GetClientRect(gWnd, &rc);
    const int cw = rc.right - rc.left, ch = rc.bottom - rc.top;
    if (cw > 0 && ch > 0 && g.frame.width > 0 && g.frame.height > 0 &&
        (cw != g.frame.width || ch != g.frame.height)) {
        p.x = MulDiv(p.x, g.frame.width, cw);
        p.y = MulDiv(p.y, g.frame.height, ch);
    }
    return p;
}

POINT toPagePoint(LPARAM lParam) {
    return clientToPagePoint({GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)});
}

// A page with a running video or animating sprite changes appearance every
// tick even when no script call lands.
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

const CursorBitmap* activeCursor() {
    const Page* page = g.project ? g.project->currentPage() : nullptr;
    if (!page || page->cursor() < 0) return nullptr;
    const auto& cursors = page->groupCursors();
    const auto index = static_cast<std::size_t>(page->cursor());
    return index < cursors.size() ? &cursors[index] : nullptr;
}

void drawGraphBoardCursor(HDC dc, const RECT& client) {
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
    const int clientWidth = client.right - client.left;
    const int clientHeight = client.bottom - client.top;
    const int originX = g.cursorPage.x - cursor->hotX;
    const int originY = g.cursorPage.y - cursor->hotY;
    HBRUSH brush = reinterpret_cast<HBRUSH>(GetStockObject(DC_BRUSH));

    for (std::uint32_t row = 0; row < cursor->height; ++row) {
        const std::uint32_t sourceRow = legacyBottomUp ? cursor->height - 1u - row : row;
        const auto* source = cursor->pixels.data() + static_cast<std::size_t>(sourceRow) * stride;
        for (std::uint32_t col = 0; col < cursor->width; ++col) {
            const std::uint8_t index = source[col];
            if (index == cursor->transparentIndex) continue;
            const auto* color = palette ? palette + static_cast<std::size_t>(index) * 4u : nullptr;
            const COLORREF rgb = color ? RGB(color[0], color[1], color[2])
                                       : RGB(index, index, index);
            const int pageX = originX + static_cast<int>(col);
            const int pageY = originY + static_cast<int>(row);
            RECT pixel{
                MulDiv(pageX, clientWidth, g.frame.width),
                MulDiv(pageY, clientHeight, g.frame.height),
                MulDiv(pageX + 1, clientWidth, g.frame.width),
                MulDiv(pageY + 1, clientHeight, g.frame.height)};
            if (pixel.right <= 0 || pixel.bottom <= 0 || pixel.left >= clientWidth ||
                pixel.top >= clientHeight) {
                continue;
            }
            if (pixel.right - pixel.left == 1 && pixel.bottom - pixel.top == 1) {
                SetPixelV(dc, pixel.left, pixel.top, rgb);
            } else {
                SetDCBrushColor(dc, rgb);
                FillRect(dc, &pixel, brush);
            }
        }
    }
}

void onTick() {
    if (!g.project || !g.project->currentPage()) return;
    const ULONGLONG now = GetTickCount64();
    int elapsed = static_cast<int>(now - g.lastTickMs);
    g.lastTickMs = now;
    elapsed = std::clamp(elapsed, 0, 250);

    g.audio.reapFinished();

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
            while (g.timerAccumMs >= interval && g.project->currentPage() == page) {
                g.timerAccumMs -= interval;
                runScriptEvent([](Page& p) { p.timer(); });
            }
        } else {
            g.timerAccumMs = 0;
        }
    }

    if (g.dirty) {
        renderFrame();
        InvalidateRect(gWnd, nullptr, FALSE);
    }
}

void onPaint() {
    PAINTSTRUCT ps;
    HDC dc = BeginPaint(gWnd, &ps);
    if (!g.blit.empty()) {
        RECT rc;
        GetClientRect(gWnd, &rc);
        BITMAPINFO bmi{};
        bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
        bmi.bmiHeader.biWidth = g.frame.width;
        bmi.bmiHeader.biHeight = -g.frame.height;  // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;
        SetStretchBltMode(dc, HALFTONE);
        StretchDIBits(dc, 0, 0, rc.right, rc.bottom, 0, 0, g.frame.width, g.frame.height,
                      g.blit.data(), &bmi, DIB_RGB_COLORS, SRCCOPY);

        // TextHolder uses the platform font here while the indexed scene stays
        // in the portable renderer. This restores poem/synchronised-text pages
        // such as TRALALA, which intentionally hide all picture layers on open.
        const Page* page = g.project ? g.project->currentPage() : nullptr;
        if (page) {
            SetBkMode(dc, TRANSPARENT);
            SetTextColor(dc, RGB(24, 24, 24));
            HFONT font = CreateFontW(-18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                     EASTEUROPE_CHARSET, OUT_DEFAULT_PRECIS,
                                     CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                                     DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
            HGDIOBJ oldFont = SelectObject(dc, font);
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
                    std::wstring wide = utf8ToWide(graphboard::cp1250ToUtf8(cleaned));
                    RECT viewport{
                        MulDiv(text.left, rc.right, g.frame.width),
                        MulDiv(text.top, rc.bottom, g.frame.height),
                        MulDiv(text.right, rc.right, g.frame.width),
                        MulDiv(text.bottom, rc.bottom, g.frame.height)};
                    const int offsetX = static_cast<int>(item.count("offsetX")
                        ? item.at("offsetX").toInt() : 0);
                    const int offsetY = static_cast<int>(item.count("offsetY")
                        ? item.at("offsetY").toInt() : 0);
                    RECT tr = viewport;
                    OffsetRect(&tr, MulDiv(offsetX, rc.right, g.frame.width),
                               MulDiv(offsetY, rc.bottom, g.frame.height));
                    const int saved = SaveDC(dc);
                    IntersectClipRect(dc, viewport.left, viewport.top, viewport.right, viewport.bottom);
                    DrawTextW(dc, wide.c_str(), static_cast<int>(wide.size()), &tr,
                              DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX);
                    RestoreDC(dc, saved);
                }
            }
            SelectObject(dc, oldFont);
            DeleteObject(font);
        }
        drawGraphBoardCursor(dc, rc);
    } else {
        RECT rc;
        GetClientRect(gWnd, &rc);
        FillRect(dc, &rc, reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
    }
    EndPaint(gWnd, &ps);
}

LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT:
            onPaint();
            return 0;
        case WM_TIMER:
            if (wParam == kTickTimerId) onTick();
            return 0;
        case WM_LBUTTONDOWN: {
            SetCapture(hwnd);
            const POINT p = toPagePoint(lParam);
            runScriptEvent([&](Page& page) { page.lButtonDown(p.x, p.y); });
            return 0;
        }
        case WM_LBUTTONUP: {
            ReleaseCapture();
            const POINT p = toPagePoint(lParam);
            runScriptEvent([&](Page& page) { page.lButtonUp(p.x, p.y); });
            return 0;
        }
        case WM_RBUTTONDOWN: {
            const POINT p = toPagePoint(lParam);
            runScriptEvent([&](Page& page) { page.rButtonDown(p.x, p.y); });
            return 0;
        }
        case WM_MOUSEMOVE: {
            const POINT p = toPagePoint(lParam);
            g.cursorPage = p;
            g.cursorInside = true;
            if (!g.trackingMouse) {
                TRACKMOUSEEVENT tracking{sizeof(tracking), TME_LEAVE, hwnd, 0};
                TrackMouseEvent(&tracking);
                g.trackingMouse = true;
            }
            runScriptEvent([&](Page& page) { page.mouseMove(p.x, p.y); });
            // The software cursor is painted over a clean scene buffer, so a
            // mouse-only move needs a repaint even when no script call fired.
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        }
        case WM_MOUSEWHEEL: {
            POINT p{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            ScreenToClient(hwnd, &p);
            p = clientToPagePoint(p);
            const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            bool consumed = false;
            runScriptEvent([&](Page& page) { consumed = page.mouseWheel(p.x, p.y, delta); });
            if (consumed) {
                g.dirty = true;
                renderFrame();
                InvalidateRect(hwnd, nullptr, FALSE);
                return 0;
            }
            break;
        }
        case WM_MOUSELEAVE:
            g.cursorInside = false;
            g.trackingMouse = false;
            InvalidateRect(hwnd, nullptr, FALSE);
            return 0;
        case WM_KEYDOWN:
            runScriptEvent([&](Page& page) { page.keyDown(static_cast<int>(wParam)); });
            return 0;
        case WM_SETCURSOR:
            if (LOWORD(lParam) == HTCLIENT && g.project && g.project->currentPage()) {
                const int index = g.project->currentPage()->cursor();
                // Negative values explicitly hide the pointer. Valid positive
                // indices are drawn from the active GRP; only an invalid index
                // falls back to a visible system arrow so the window stays usable.
                ::SetCursor(index < 0 || activeCursor() != nullptr
                                ? nullptr
                                : LoadCursorW(nullptr, IDC_ARROW));
                return TRUE;
            }
            break;
        case WM_ERASEBKGND:
            return 1;
        case WM_DESTROY:
            KillTimer(hwnd, kTickTimerId);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

// START.PRJ discovery: explicit argument, then conventional spots next to the
// exe and the working directory, then a file-open dialog.
fs::path findStartPrj(const std::wstring& arg) {
    if (!arg.empty()) return fs::path(arg);

    wchar_t exePathBuf[MAX_PATH]{};
    GetModuleFileNameW(nullptr, exePathBuf, MAX_PATH);
    const fs::path exeDir = fs::path(exePathBuf).parent_path();
    const fs::path candidates[] = {
        exeDir / L"DATA" / L"START.PRJ",
        exeDir / L"START.PRJ",
        fs::current_path() / L"DATA" / L"START.PRJ",
        fs::current_path() / L"START.PRJ",
    };
    for (const auto& c : candidates) {
        std::error_code ec;
        if (fs::exists(c, ec)) return c;
    }

    wchar_t fileBuf[MAX_PATH]{};
    OPENFILENAMEW ofn{};
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter = L"GraphBoard project (START.PRJ)\0*.PRJ\0All files\0*.*\0";
    ofn.lpstrFile = fileBuf;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = L"Locate the game's START.PRJ (in its DATA folder)";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    if (!GetOpenFileNameW(&ofn)) return {};
    return fs::path(fileBuf);
}

} // namespace

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int showCmd) {
    // First non-flag argument, if any, is the START.PRJ path.
    std::wstring prjArg;
    int argc = 0;
    if (LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc)) {
        if (argc > 1) prjArg = argv[1];
        LocalFree(argv);
    }

    const fs::path startPrj = findStartPrj(prjArg);
    if (startPrj.empty()) {
        return 0;  // user cancelled the open dialog
    }

    try {
        g.project = Project::loadFromFile(startPrj);
        g.dataDir = startPrj.parent_path();
        g.projectTitle = projectTitle(startPrj, *g.project);
        g.project->openStartupPage();
    } catch (const std::exception& ex) {
        MessageBoxW(nullptr, utf8ToWide(ex.what()).c_str(), L"gbgame: failed to load project",
                    MB_ICONERROR);
        return 1;
    }

    WNDCLASSW wc{};
    wc.lpfnWndProc = wndProc;
    wc.hInstance = instance;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.lpszClassName = L"GraphBoardGame";
    RegisterClassW(&wc);

    gWnd = CreateWindowW(wc.lpszClassName, L"", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                         CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, nullptr, nullptr, instance, nullptr);
    if (!gWnd) return 1;

    try {
        loadPageVisuals();
        processHostCalls();   // audio the startup page's OnOpenPage started
        renderFrame();
    } catch (const std::exception& ex) {
        MessageBoxW(gWnd, utf8ToWide(ex.what()).c_str(), L"gbgame: failed to render page",
                    MB_ICONERROR);
        return 1;
    }
    setTitle();
    fitWindowToFrame();
    ShowWindow(gWnd, showCmd);

    g.lastTickMs = GetTickCount64();
    SetTimer(gWnd, kTickTimerId, kTickMs, nullptr);
    pumpNavigation();  // OnOpenPage may already have queued a LoadPage

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}
