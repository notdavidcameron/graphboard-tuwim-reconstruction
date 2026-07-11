#include "graphboard/component.hpp"
#include "graphboard/format.hpp"
#include "graphboard/holders.hpp"
#include "graphboard/runtime/interpreter.hpp"
#include "graphboard/runtime/page.hpp"
#include "graphboard/runtime/project.hpp"
#include "graphboard/runtime/script.hpp"
#include "graphboard/text.hpp"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <array>
#include <cctype>
#include <filesystem>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#endif

namespace {

using graphboard::BdfHeader;
using graphboard::ComponentWrapper;
using graphboard::HotSpotHolderState;
using graphboard::ProjectManifest;
using nlohmann::json;

// All GraphBoard strings are Windows-1250; JSON requires UTF-8.
std::string t(const std::string& cp1250) {
    return graphboard::cp1250ToUtf8(cp1250);
}

std::string lowerExtension(const std::filesystem::path& path) {
    auto ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return ext;
}

json projectToJson(const ProjectManifest& project) {
    return {
        {"kind", "START.PRJ"},
        {"version", project.version},
        {"startupPage", t(project.startupPage)},
        {"currentPageOrGroupState", t(project.currentPageOrGroupState)},
        {"audioPresetIndex", project.audioPresetIndex},
        {"pageCount", project.pageNames.size()},
        {"pages", [&] {
             json names = json::array();
             for (const auto& name : project.pageNames) names.push_back(t(name));
             return names;
         }()},
        {"groupCount", project.groupNames.size()},
        {"groups", [&] {
             json names = json::array();
             for (const auto& name : project.groupNames) names.push_back(t(name));
             return names;
         }()},
        {"decodedSignature", t(project.decodedSignature)},
        {"globalScriptVersion", project.globalScriptVersion},
        {"globalScriptByteCount", project.globalScript.size()},
        {"globalScript", t(project.globalScript)},
    };
}

json headerToJson(const BdfHeader& header) {
    return {
        {"banner", t(header.banner)},
        {"version", header.version},
        {"pageRect", {header.pageRect.left, header.pageRect.top, header.pageRect.right, header.pageRect.bottom}},
        {"layerRange", {header.minLayer, header.maxLayer}},
        {"backgroundFlag", header.backgroundFlag},
        {"backgroundColorIndex", header.backgroundColorIndex},
        {"paletteByteCount", header.paletteByteCount},
        {"paletteOffset", header.paletteOffset},
        {"dibByteCount", header.dibByteCount},
        {"dibOffset", header.dibOffset},
        {"componentListOffset", header.componentListOffset},
    };
}

json wrapperToJson(const ComponentWrapper& wrapper) {
    const auto* info = graphboard::lookupHolder(wrapper.clsid);
    // In the reflected typeinfo record the real member name is `displayName`
    // and `descriptionOrHelpName` is the parameter signature; `rawName` holds
    // an internal mangled token (not human-readable).
    json functions = json::array();
    for (const auto& fn : wrapper.functions) {
        functions.push_back({
            {"name", t(fn.displayName)},
            {"returnType", t(fn.typeOrReturnName)},
            {"signature", t(fn.descriptionOrHelpName)},
            {"dispatchId", fn.dispatchIdOrOffset},
            {"flags", fn.flagsOrInvokeKind},
        });
    }
    json properties = json::array();
    for (const auto& pr : wrapper.properties) {
        properties.push_back({
            {"name", t(pr.displayName)},
            {"variantType", pr.variantTypeOrDispatchMetadata},
            {"flags", pr.flagsOrInvokeKind},
        });
    }
    json out = {
        {"displayName", t(wrapper.displayName)},
        {"clsid", wrapper.clsid.toString()},
        {"holderKind", info ? info->displayName : std::string("Unknown")},
        {"wrapperVersion", wrapper.wrapperVersion},
        {"functionCount", wrapper.functions.size()},
        {"propertyCount", wrapper.properties.size()},
        {"functions", functions},
        {"properties", properties},
        {"wrapperOffset", wrapper.wrapperOffset},
        {"privateStateOffset", wrapper.privateStateOffset},
    };
    if (info && !info->serializerAnchor.empty()) {
        out["serializerAnchor"] = info->serializerAnchor;
    }
    return out;
}

json spriteStateToJson(const graphboard::SpriteHolderState& state) {
    json definitions = json::array();
    for (const auto& definition : state.definitions) {
        json frames = json::array();
        for (const auto& frame : definition.frames) {
            json f = {{"width", frame.width}, {"height", frame.height}};
            if (!frame.opaque.empty()) {
                std::size_t opaque = 0;
                for (const auto v : frame.opaque) opaque += v;
                f["opaquePixels"] = opaque;   // present only when a mask exists
            }
            frames.push_back(f);
        }
        definitions.push_back({
            {"name", t(definition.name)},
            {"phaseCount", definition.phaseCount},
            {"width", definition.width},
            {"height", definition.height},
            {"frames", frames},
            {"blobByteCount", definition.blobByteCount},
            {"blobOffset", definition.blobOffset},
        });
    }
    json instances = json::array();
    for (const auto& instance : state.instances) {
        instances.push_back({
            {"definitionIndex", instance.definitionIndex},
            {"field04", instance.field04},
            {"posX", instance.posX},
            {"posY", instance.posY},
            {"layer", instance.layer},
            {"phase", instance.phase},
            {"visible", instance.visible},
        });
    }
    return {
        {"version", state.version},
        {"definitionCount", state.definitions.size()},
        {"definitions", definitions},
        {"instanceCount", state.instances.size()},
        {"instances", instances},
    };
}

json multiBitmapStateToJson(const graphboard::MultiBitmapState& state) {
    json records = json::array();
    for (const auto& record : state.records) {
        records.push_back({
            {"name", t(record.name)},
            {"width", record.width},
            {"height", record.height},
            {"pixelByteCount", record.pixelByteCount},
            {"pixelOffset", record.pixelOffset},
            {"pixelsAreRawIndexed", record.pixelsAreRawIndexed},
        });
    }
    return {
        {"version", state.version},
        {"recordCount", state.records.size()},
        {"records", records},
    };
}

json transparentVideoStateToJson(const graphboard::TransparentVideoHolderState& state) {
    json entries = json::array();
    for (const auto& entry : state.entries) {
        entries.push_back({
            {"streamOffset", entry.stream.streamOffset},
            {"streamByteCount", entry.stream.streamByteCount},
            {"magicOk", entry.stream.magic == 0xada77777u},
            {"frameDurationMs", entry.stream.frameDurationMs},
            {"videoSize", {entry.stream.width, entry.stream.height}},
            {"chunkRecordCount", entry.stream.chunkRecordCount},
            {"frameCount", entry.stream.frameCount},
            {"audioSampleRate", entry.stream.audioSampleRate},
            {"original", {entry.originalX, entry.originalY, entry.originalZ}},
            {"stage", {entry.stageX, entry.stageY, entry.stageZ}},
            {"stillFrameOffset", entry.stillFrameOffset},
            {"stillFrameByteCount", entry.stillFrameByteCount},
        });
    }
    return {
        {"version", state.version},
        {"entryCount", state.entries.size()},
        {"entries", entries},
    };
}

json soundStateToJson(const graphboard::SoundHolderState& state) {
    json sounds = json::array();
    for (const auto& sound : state.sounds) {
        sounds.push_back({
            {"name", t(sound.name)},
            {"soundByteCount", sound.soundByteCount},
            {"soundOffset", sound.soundOffset},
            {"looksLikeRiff", sound.looksLikeRiff},
            {"waveFormatByteCount", sound.waveFormatByteCount},
        });
    }
    return {
        {"version", state.version},
        {"soundCount", state.sounds.size()},
        {"sounds", sounds},
    };
}

json textStateToJson(const graphboard::TextHolderState& state) {
    json entries = json::array();
    for (const auto& entry : state.entries) {
        json item = {
            {"hasRenderCache", entry.hasRenderCache},
            {"lineCount", entry.lineCount},
            {"primaryText", t(entry.primaryText)},
        };
        if (entry.hasSecondaryText) {
            item["secondaryText"] = t(entry.secondaryText);
        }
        if (entry.streamByteCount != 0) {
            item["streamByteCount"] = entry.streamByteCount;
            item["streamOffset"] = entry.streamOffset;
        }
        entries.push_back(std::move(item));
    }
    json fonts = json::array();
    for (const auto& slot : state.fontSlots) {
        fonts.push_back({{"height", slot.height}, {"glyphCount", slot.glyphCount}});
    }
    return {
        {"version", state.version},
        {"colors", {state.colors[0], state.colors[1], state.colors[2], state.colors[3]}},
        {"entryCount", state.entries.size()},
        {"entries", entries},
        {"fontSlots", fonts},
    };
}

json bitmapHolderStateToJson(const graphboard::BitmapHolderState& state) {
    json bitmaps = json::array();
    for (const auto& bitmap : state.bitmaps) {
        json j = {
            {"name", t(bitmap.name)},
            {"rect", {bitmap.left, bitmap.top, bitmap.right, bitmap.bottom}},
            {"layer", bitmap.layer},
            {"blobByteCount", bitmap.blobByteCount},
            {"pixelOffset", bitmap.pixelOffset},
            {"pixelSizeConsistent", bitmap.pixelSizeConsistent},
        };
        if (!bitmap.opaque.empty()) {
            std::size_t opaque = 0;
            for (const auto v : bitmap.opaque) opaque += v;
            j["opaquePixels"] = opaque;
        }
        bitmaps.push_back(j);
    }
    return {
        {"version", state.version},
        {"bitmapCount", state.bitmaps.size()},
        {"bitmaps", bitmaps},
    };
}

json puzzleStateToJson(const graphboard::PuzzleState& state) {
    json boards = json::array();
    for (const auto& board : state.boards) {
        json chips = json::array();
        for (const auto& chip : board.chips) {
            chips.push_back({
                {"rect", {chip.left, chip.top, chip.right, chip.bottom}},
                {"pixelByteCount", chip.pixelByteCount},
                {"subRecordCount", chip.subRecordCount},
            });
        }
        boards.push_back({{"chipCount", board.chips.size()}, {"chips", chips}});
    }
    return {
        {"version", state.version},
        {"boardCount", state.boards.size()},
        {"boards", boards},
    };
}

json videoHolderStateToJson(const graphboard::VideoHolderState& state) {
    json entries = json::array();
    for (const auto& entry : state.entries) {
        entries.push_back({
            {"name", t(entry.name)},
            {"pos", {entry.posX, entry.posY}},
            {"recordOffset", entry.recordOffset},
        });
    }
    return {
        {"version", state.version},
        {"entryCount", state.entries.size()},
        {"entries", entries},
    };
}

json panoramaHolderStateToJson(const graphboard::PanoramaHolderState& state) {
    json scenes = json::array();
    for (const auto& scene : state.scenes) {
        scenes.push_back({
            {"recordOffset", scene.recordOffset},
            {"dibByteCount", scene.dibByteCount},
            {"subImageCount", scene.subImageCount},
            {"regionCount", scene.regionCount},
        });
    }
    return {{"version", state.version}, {"sceneCount", state.scenes.size()}, {"scenes", scenes}};
}

json panoramaStateToJson(const graphboard::PanoramaState& state) {
    json scenes = json::array();
    for (const auto& scene : state.scenes) {
        scenes.push_back({
            {"size", {scene.width, scene.height}},
            {"subImageCount", scene.subImageCount},
            {"regionCount", scene.regionCount},
            {"recordOffset", scene.recordOffset},
        });
    }
    return {{"version", state.version}, {"sceneCount", state.scenes.size()}, {"scenes", scenes}};
}

json hotspotStateToJson(const HotSpotHolderState& state) {
    json hotspots = json::array();
    for (const auto& hotspot : state.hotspots) {
        hotspots.push_back({
            {"rect", {hotspot.left, hotspot.top, hotspot.right, hotspot.bottom}},
            {"layer", hotspot.layer},
            {"enabled", hotspot.enabled},
            {"name", t(hotspot.name)},
        });
    }
    return {
        {"version", state.version},
        {"flags", {state.flag0, state.flag1, state.flag2}},
        {"hotspotCount", state.hotspots.size()},
        {"hotspots", hotspots},
    };
}

// Walk a page/group component list. Each item is a reflected wrapper followed by
// a holder-specific private payload. We can only advance past a component whose
// private-state parser is implemented, so the walk stops at the first component
// we cannot skip and reports how far it got.
json componentListToJson(graphboard::BinaryReader& reader) {
    const auto header = graphboard::parseComponentListHeader(reader);
    json components = json::array();
    std::string stopReason;

    for (std::uint32_t i = 0; i < header.count; ++i) {
        auto item = graphboard::parseComponentListItem(reader);
        const auto& wrapper = item.wrapper;
        json component = wrapperToJson(wrapper);
        component["index"] = i;

        const auto* info = graphboard::lookupHolder(item.clsid);
        const auto kind = info ? info->kind : graphboard::HolderKind::Unknown;
        if (kind == graphboard::HolderKind::HotSpotHolder) {
            component["privateStateParsed"] = true;
            component["privateState"] = hotspotStateToJson(graphboard::parseHotSpotHolderState(reader));
            components.push_back(std::move(component));
        } else if (kind == graphboard::HolderKind::SpriteHolder) {
            component["privateStateParsed"] = true;
            component["privateState"] = spriteStateToJson(graphboard::parseSpriteHolderState(reader));
            components.push_back(std::move(component));
        } else if (kind == graphboard::HolderKind::MultiBitmap) {
            component["privateStateParsed"] = true;
            component["privateState"] = multiBitmapStateToJson(graphboard::parseMultiBitmapState(reader));
            components.push_back(std::move(component));
        } else if (kind == graphboard::HolderKind::TransparentVideoHolder) {
            component["privateStateParsed"] = true;
            component["privateState"] =
                transparentVideoStateToJson(graphboard::parseTransparentVideoHolderState(reader));
            components.push_back(std::move(component));
        } else if (kind == graphboard::HolderKind::SoundHolder) {
            component["privateStateParsed"] = true;
            component["privateState"] = soundStateToJson(graphboard::parseSoundHolderState(reader));
            components.push_back(std::move(component));
        } else if (kind == graphboard::HolderKind::BitmapHolder) {
            component["privateStateParsed"] = true;
            component["privateState"] = bitmapHolderStateToJson(graphboard::parseBitmapHolderState(reader));
            components.push_back(std::move(component));
        } else if (kind == graphboard::HolderKind::Puzzle) {
            component["privateStateParsed"] = true;
            component["privateState"] = puzzleStateToJson(graphboard::parsePuzzleState(reader));
            components.push_back(std::move(component));
        } else if (kind == graphboard::HolderKind::PanoramaHolder) {
            component["privateStateParsed"] = true;
            component["privateState"] = panoramaHolderStateToJson(graphboard::parsePanoramaHolderState(reader));
            components.push_back(std::move(component));
        } else if (kind == graphboard::HolderKind::Panorama) {
            component["privateStateParsed"] = true;
            component["privateState"] = panoramaStateToJson(graphboard::parsePanoramaState(reader));
            components.push_back(std::move(component));
        } else if (kind == graphboard::HolderKind::VideoHolder) {
            component["privateStateParsed"] = true;
            component["privateState"] = videoHolderStateToJson(graphboard::parseVideoHolderState(reader));
            components.push_back(std::move(component));
        } else if (kind == graphboard::HolderKind::Recorder) {
            const auto recorder = graphboard::parseRecorderState(reader);
            component["privateStateParsed"] = true;
            component["privateState"] = {
                {"version", recorder.version},
                {"recordOffset", recorder.recordOffset},
                {"note", "fixed 0x68-byte runtime record; layout empirical"},
            };
            components.push_back(std::move(component));
        } else if (kind == graphboard::HolderKind::TextHolder) {
            component["privateStateParsed"] = true;
            component["privateState"] = textStateToJson(graphboard::parseTextHolderState(reader));
            components.push_back(std::move(component));
        } else {
            component["privateStateParsed"] = false;
            const std::string who = info ? info->displayName : wrapper.clsid.toString();
            component["note"] =
                "private-state parser not implemented for " + who +
                "; cannot advance to the next component";
            components.push_back(std::move(component));
            stopReason = "stopped after component " + std::to_string(i) +
                         " (" + who + "): private-state layout not implemented in this port";
            break;
        }
    }

    json out = {
        {"version", header.version},
        {"count", header.count},
        {"componentsParsed", components.size()},
        {"components", components},
    };
    if (!stopReason.empty()) {
        out["note"] = stopReason;
    }
    return out;
}

json bdfToJson(graphboard::BinaryReader& reader) {
    // The original loader never validates the banner, but as a tool guard we
    // do: the DATA folder contains misnamed non-BDF files (e.g. KOTEK2.BDF is
    // a 16-bit NE executable) that would otherwise mis-walk with absurd sizes.
    // Peek before the full header parse — the palette/dib skips inside
    // parseBdfHeader would throw a confusing end-of-input error first.
    {
        const auto banner = reader.readBytesAsString(20);
        reader.seek(0);
        if (banner != "YDP Board data file.") {
            throw graphboard::ParseError("not a YDP Board data file (banner mismatch)");
        }
    }
    const auto header = graphboard::parseBdfHeader(reader);
    json out = {
        {"kind", "BDF"},
        {"header", headerToJson(header)},
        {"componentList", componentListToJson(reader)},
    };

    // When every component parsed, the reader sits at the page script text
    // block (GraphBrdScriptEditor_SerializeText), followed by the script
    // engine parse cache and the COleDocument trailing state.
    if (!out["componentList"].contains("note")) {
        const auto scriptOffset = reader.position();
        const auto script = graphboard::parseScriptText(reader);
        const auto engineOffset = reader.position();
        const auto engine = graphboard::parseScriptEngineState(reader);

        json switchBlocks = json::array();
        for (const auto& block : engine.switchBlocks) {
            json cases = json::array();
            for (const auto& entry : block.cases) {
                cases.push_back({{"value", entry.caseValue}, {"bodyOffset", entry.caseBodyOffset}});
            }
            switchBlocks.push_back({
                {"start", block.blockStart},
                {"end", block.blockEnd},
                {"defaultBodyOffset", block.defaultBodyOffset},
                {"cases", cases},
            });
        }
        json handlers = json::array();
        for (const auto& handler : graphboard::runtime::discoverHandlers(script.text)) {
            json params = json::array();
            for (const auto& param : handler.parameters) {
                params.push_back({{"type", param.type}, {"name", param.name}});
            }
            const char* kindName =
                handler.kind == graphboard::runtime::HandlerKind::PageEvent ? "pageEvent"
                : handler.kind == graphboard::runtime::HandlerKind::ComponentCallback
                    ? "componentCallback"
                    : "userFunction";
            json entry = {
                {"kind", kindName},
                {"name", handler.name},
                {"returnType", handler.returnType},
                {"parameters", params},
                {"nameOffset", handler.nameOffset},
                {"bodyOffset", handler.bodyOffset},
                {"bodyEndOffset", handler.bodyEndOffset},
            };
            if (handler.kind == graphboard::runtime::HandlerKind::ComponentCallback) {
                entry["component"] = handler.component;
                entry["method"] = handler.method;
            }
            handlers.push_back(std::move(entry));
        }
        // Unique API surface used by this page's script.
        std::set<std::string> builtinsUsed;
        std::set<std::string> componentMethodsUsed;
        for (const auto& call : graphboard::runtime::collectCalls(script.text)) {
            if (call.isBuiltin) {
                builtinsUsed.insert(call.name);
            } else {
                componentMethodsUsed.insert(call.component + "." + call.name);
            }
        }
        out["pageScript"] = {
            {"offset", scriptOffset},
            {"version", script.version},
            {"textByteCount", script.text.size()},
            {"handlers", handlers},
            {"builtinsUsed", builtinsUsed},
            {"componentMethodsUsed", componentMethodsUsed},
            {"text", t(script.text)},
        };
        out["scriptEngineState"] = {
            {"offset", engineOffset},
            {"schemaVersion", engine.schemaVersion},
            {"switchBlockCount", engine.switchBlocks.size()},
            {"switchBlocks", switchBlocks},
            {"builtinCallCount", engine.builtinTokenOffsets.size()},
            {"builtinTokenOffsets", engine.builtinTokenOffsets},
            {"builtinCallKinds", engine.builtinCallKinds},
        };
        out["trailingByteCount"] = reader.remaining();
    }
    return out;
}

} // namespace

namespace {

// Collect argv as filesystem paths. On Windows we take the wide command line so
// non-ASCII paths (the game data lives under a "...świat..." folder) survive
// without a lossy narrow-locale conversion.
std::vector<std::filesystem::path> collectArgs(int argc, char** argv) {
    std::vector<std::filesystem::path> args;
#ifdef _WIN32
    (void)argc;
    (void)argv;
    int wargc = 0;
    LPWSTR* wargv = ::CommandLineToArgvW(::GetCommandLineW(), &wargc);
    if (wargv != nullptr) {
        for (int i = 0; i < wargc; ++i) {
            args.emplace_back(wargv[i]);
        }
        ::LocalFree(wargv);
    }
#else
    for (int i = 0; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }
#endif
    return args;
}

// Records builtin/component calls so a page can be driven headless and its
// externally-visible behavior inspected or asserted.
class RecordingHost : public graphboard::runtime::Host {
public:
    json calls = json::array();

    graphboard::runtime::Value callBuiltin(
        const std::string& name,
        const std::vector<graphboard::runtime::Value>& args) override {
        calls.push_back(callToJson(false, "", name, args));
        if (name == "IsProject") return graphboard::runtime::Value::integer(1);
        return graphboard::runtime::Value();
    }
    graphboard::runtime::Value callComponent(
        const std::string& component, const std::string& method,
        const std::vector<graphboard::runtime::Value>& args) override {
        calls.push_back(callToJson(true, component, method, args));
        return graphboard::runtime::Value();
    }

private:
    static json callToJson(bool component, const std::string& comp, const std::string& name,
                           const std::vector<graphboard::runtime::Value>& args) {
        json a = json::array();
        for (const auto& value : args) {
            if (value.isInt()) {
                a.push_back(value.toInt());
            } else {
                a.push_back(t(value.toString()));
            }
        }
        json call = {{"name", name}, {"args", a}};
        if (component) {
            call["component"] = comp;
        }
        return call;
    }
};

// Drive one handler of a .BDF page and print the resulting call trace as JSON.
int runHandler(const std::filesystem::path& path, const std::string& handler) {
    auto reader = graphboard::BinaryReader::fromFile(path);
    const auto info = bdfToJson(reader);
    if (!info.contains("pageScript")) {
        std::cerr << "gbinspect: page has no reachable script (walk stopped early)\n";
        return 1;
    }
    const std::string script = info["pageScript"]["text"].get<std::string>();

    RecordingHost host;
    graphboard::runtime::Interpreter interp(script, host);
    if (!interp.hasHandler(handler)) {
        std::cerr << "gbinspect: handler '" << handler << "' is not defined in this page\n";
        return 1;
    }
    interp.runHandler(handler, {});

    json out = {{"handler", handler}, {"calls", host.calls}};
    std::cout << out.dump(2) << "\n";
    return 0;
}

json valueToJson(const graphboard::runtime::Value& v) {
    return v.isInt() ? json(v.toInt()) : json(t(v.toString()));
}

// Serialize a page's live component + page state (the shared body of --drive
// and --interact output).
json pageStateJson(graphboard::runtime::Page& page) {
    json components = json::array();
    for (const auto& name : page.componentNames()) {
        const auto* c = page.component(name);
        json items = json::object();
        for (const auto& item : c->items) {
            json fields = json::object();
            for (const auto& field : item.second) {
                fields[field.first] = valueToJson(field.second);
            }
            items[std::to_string(item.first)] = fields;
        }
        json props = json::object();
        for (const auto& prop : c->props) {
            props[prop.first] = valueToJson(prop.second);
        }
        components.push_back({{"name", t(name)}, {"items", items}, {"props", props}});
    }
    return {
        {"pendingPage", t(page.pendingPage())},
        {"currentGroup", t(page.currentGroup())},
        {"cursor", page.cursor()},
        {"timerInterval", page.timerInterval()},
        {"components", components},
    };
}

// Drive a handler through the executable Page model (holders wired as stateful
// components) and print the resulting page + component state as JSON.
int drivePage(const std::filesystem::path& path, const std::string& handler) {
    auto page = graphboard::runtime::Page::loadFromFile(path);
    if (!page->hasHandler(handler)) {
        std::cerr << "gbinspect: handler '" << handler << "' is not defined in this page\n";
        return 1;
    }
    page->runEvent(handler, {});

    json out = pageStateJson(*page);
    out["handler"] = handler;
    std::cout << out.dump(2) << "\n";
    return 0;
}

// One synthetic input event applied through the Page's raw-input entry points.
struct InputEvent {
    enum class Kind {
        Click, Down, Up, RClick, Move, Drag, Timer, Key,
        VideoEnd, SoundEnd, AnimEnd, Advance,
    } kind;
    int a = 0;  // x, key code for Key, item id for *End, or drag start-x
    int b = 0;  // y, or drag start-y
    int c = 0;  // drag end-x
    int d = 0;  // drag end-y
};

// Apply a synthetic input sequence to a live page, in order.
void applyEvents(graphboard::runtime::Page& page, const std::vector<InputEvent>& events) {
    for (const auto& ev : events) {
        switch (ev.kind) {
            case InputEvent::Kind::Click:  page.lButtonDown(ev.a, ev.b); break;
            case InputEvent::Kind::Down:   page.lButtonDown(ev.a, ev.b); break;
            case InputEvent::Kind::Up:     page.lButtonUp(ev.a, ev.b); break;
            case InputEvent::Kind::RClick: page.rButtonDown(ev.a, ev.b); break;
            case InputEvent::Kind::Move:   page.mouseMove(ev.a, ev.b); break;
            case InputEvent::Kind::Drag:   page.drag(ev.a, ev.b, ev.c, ev.d); break;
            case InputEvent::Kind::Timer:  page.timer(); break;
            case InputEvent::Kind::Key:    page.keyDown(ev.a); break;
            case InputEvent::Kind::VideoEnd: page.videoEnd(ev.a); break;
            case InputEvent::Kind::SoundEnd: page.soundEnd(ev.a); break;
            case InputEvent::Kind::AnimEnd:  page.animationEnd(ev.a); break;
            case InputEvent::Kind::Advance:  page.advanceTime(ev.a); break;
        }
    }
}

json callLogJson(const graphboard::runtime::Page& page) {
    json calls = json::array();
    for (const auto& rec : page.callLog()) {
        json a = json::array();
        for (const auto& v : rec.args) a.push_back(valueToJson(v));
        json entry = {{"name", t(rec.name)}, {"args", a}};
        if (!rec.builtin) entry["component"] = t(rec.component);
        calls.push_back(entry);
    }
    return calls;
}

// Open a real page (OnOpenPage) and replay a synthetic input sequence through
// the runtime, exercising the component->script callback direction against real
// game data. Prints the final page state plus the full host call log so fired
// callbacks (e.g. HotSpot_Holder.LeftButtonClickOn) are visible.
int interactPage(const std::filesystem::path& path, bool runOpen,
                 const std::vector<InputEvent>& events) {
    auto page = graphboard::runtime::Page::loadFromFile(path);
    if (runOpen) {
        page->open();
    }
    // Calls up to here are OnOpenPage's; anything after is attributable to the
    // synthetic input events, so a consumer can slice calls[openCallCount:].
    const std::size_t openCallCount = page->callLog().size();
    applyEvents(*page, events);

    json out = pageStateJson(*page);
    out["opened"] = runOpen;
    out["openCallCount"] = openCallCount;
    out["clockMs"] = page->clockMs();
    out["calls"] = callLogJson(*page);
    std::cout << out.dump(2) << "\n";
    return 0;
}

// Load a project: run the START.PRJ global-setup block, open the startup page
// with those globals seeded, then replay the input sequence. With --follow, keep
// opening whatever page the script requests via LoadPage (bounded).
int interactProject(const std::filesystem::path& path, const std::vector<InputEvent>& events,
                    int followLimit, const std::string& pageName) {
    auto project = graphboard::runtime::Project::loadFromFile(path);
    auto& page = pageName.empty() ? project->openStartupPage() : project->openPage(pageName);
    applyEvents(page, events);

    json visited = json::array();
    visited.push_back(t(project->currentPageName()));
    for (int i = 0; i < followLimit && project->followPendingPage(); ++i) {
        visited.push_back(t(project->currentPageName()));
    }

    json globals = json::object();
    for (const auto& [name, value] : project->globals()) {
        globals[name] = valueToJson(value);
    }

    auto* cur = project->currentPage();
    json out = pageStateJson(*cur);
    out["startupPage"] = t(project->manifest().startupPage);
    out["currentPage"] = t(project->currentPageName());
    out["visitedPages"] = visited;
    out["projectGlobals"] = globals;
    out["calls"] = callLogJson(*cur);
    std::cout << out.dump(2) << "\n";
    return 0;
}

bool isFlag(const std::filesystem::path& arg, const char* flag) {
    const auto s = arg.u8string();
    return std::string(s.begin(), s.end()) == flag;
}

std::string argToUtf8(const std::filesystem::path& arg) {
    const auto s = arg.u8string();
    return std::string(s.begin(), s.end());
}

// Parse "X,Y" (or "X Y") into a pair; throws on malformed input.
std::pair<int, int> parseXY(const std::filesystem::path& arg) {
    std::string s = argToUtf8(arg);
    for (char& c : s) {
        if (c == ',') c = ' ';
    }
    std::istringstream in(s);
    int x = 0, y = 0;
    if (!(in >> x >> y)) {
        throw std::runtime_error("expected X,Y but got '" + s + "'");
    }
    return {x, y};
}

// Parse "X1,Y1,X2,Y2" into four ints.
std::array<int, 4> parseXYXY(const std::filesystem::path& arg) {
    std::string s = argToUtf8(arg);
    for (char& c : s) {
        if (c == ',') c = ' ';
    }
    std::istringstream in(s);
    std::array<int, 4> v{};
    if (!(in >> v[0] >> v[1] >> v[2] >> v[3])) {
        throw std::runtime_error("expected X1,Y1,X2,Y2 but got '" + s + "'");
    }
    return v;
}

} // namespace

int main(int argc, char** argv) {
    const auto args = collectArgs(argc, argv);

    // gbinspect <file>                         -> inspect
    // gbinspect <file.bdf> --run <handler>     -> drive one handler headless
    std::filesystem::path file;
    std::string runTarget;
    std::string driveTarget;
    std::vector<InputEvent> events;
    bool interact = false;
    bool runOpen = true;
    int followLimit = 0;
    std::string pageName;
    try {
        for (std::size_t i = 1; i < args.size(); ++i) {
            if (isFlag(args[i], "--run") && i + 1 < args.size()) {
                runTarget = argToUtf8(args[++i]);
            } else if (isFlag(args[i], "--drive") && i + 1 < args.size()) {
                driveTarget = argToUtf8(args[++i]);
            } else if (isFlag(args[i], "--click") && i + 1 < args.size()) {
                const auto [x, y] = parseXY(args[++i]);
                events.push_back({InputEvent::Kind::Click, x, y});
                interact = true;
            } else if (isFlag(args[i], "--rclick") && i + 1 < args.size()) {
                const auto [x, y] = parseXY(args[++i]);
                events.push_back({InputEvent::Kind::RClick, x, y});
                interact = true;
            } else if (isFlag(args[i], "--move") && i + 1 < args.size()) {
                const auto [x, y] = parseXY(args[++i]);
                events.push_back({InputEvent::Kind::Move, x, y});
                interact = true;
            } else if (isFlag(args[i], "--down") && i + 1 < args.size()) {
                const auto [x, y] = parseXY(args[++i]);
                events.push_back({InputEvent::Kind::Down, x, y});
                interact = true;
            } else if (isFlag(args[i], "--up") && i + 1 < args.size()) {
                const auto [x, y] = parseXY(args[++i]);
                events.push_back({InputEvent::Kind::Up, x, y});
                interact = true;
            } else if (isFlag(args[i], "--drag") && i + 1 < args.size()) {
                const auto v = parseXYXY(args[++i]);
                events.push_back({InputEvent::Kind::Drag, v[0], v[1], v[2], v[3]});
                interact = true;
            } else if (isFlag(args[i], "--timer")) {
                events.push_back({InputEvent::Kind::Timer, 0, 0});
                interact = true;
            } else if (isFlag(args[i], "--video-end") && i + 1 < args.size()) {
                events.push_back({InputEvent::Kind::VideoEnd, std::stoi(argToUtf8(args[++i]))});
                interact = true;
            } else if (isFlag(args[i], "--sound-end") && i + 1 < args.size()) {
                events.push_back({InputEvent::Kind::SoundEnd, std::stoi(argToUtf8(args[++i]))});
                interact = true;
            } else if (isFlag(args[i], "--anim-end") && i + 1 < args.size()) {
                events.push_back({InputEvent::Kind::AnimEnd, std::stoi(argToUtf8(args[++i]))});
                interact = true;
            } else if (isFlag(args[i], "--advance") && i + 1 < args.size()) {
                events.push_back({InputEvent::Kind::Advance, std::stoi(argToUtf8(args[++i]))});
                interact = true;
            } else if (isFlag(args[i], "--key") && i + 1 < args.size()) {
                events.push_back({InputEvent::Kind::Key, std::stoi(argToUtf8(args[++i])), 0});
                interact = true;
            } else if (isFlag(args[i], "--no-open")) {
                runOpen = false;
            } else if (isFlag(args[i], "--follow") && i + 1 < args.size()) {
                followLimit = std::stoi(argToUtf8(args[++i]));
                interact = true;
            } else if (isFlag(args[i], "--page") && i + 1 < args.size()) {
                pageName = argToUtf8(args[++i]);
                interact = true;
            } else if (file.empty()) {
                file = args[i];
            }
        }
    } catch (const std::exception& ex) {
        std::cerr << "gbinspect: bad argument: " << ex.what() << "\n";
        return 2;
    }
    if (file.empty()) {
        std::cerr << "usage: gbinspect <START.PRJ|PAGE.BDF> [--run <handler>] [--drive <handler>]\n"
                     "       gbinspect <PAGE.BDF> [--no-open] (--click X,Y | --rclick X,Y |\n"
                     "                 --move X,Y | --down X,Y | --up X,Y |\n"
                     "                 --drag X1,Y1,X2,Y2 | --timer | --key N)...\n"
                     "       gbinspect <START.PRJ> [--follow N] (--click X,Y | ...)...\n"
                     "                 runs the global setup block, opens the startup page,\n"
                     "                 applies input, and follows up to N LoadPage jumps\n";
        return 2;
    }

    try {
        const auto ext = lowerExtension(file);
        if (!runTarget.empty()) {
            if (ext != ".bdf") {
                std::cerr << "gbinspect: --run requires a .BDF page\n";
                return 2;
            }
            return runHandler(file, runTarget);
        }
        if (!driveTarget.empty()) {
            if (ext != ".bdf") {
                std::cerr << "gbinspect: --drive requires a .BDF page\n";
                return 2;
            }
            return drivePage(file, driveTarget);
        }
        if (interact) {
            // START.PRJ drives the whole project: global setup -> startup page
            // -> input (-> LoadPage navigation). A .BDF drives that page alone.
            if (ext == ".prj") {
                return interactProject(file, events, followLimit, pageName);
            }
            if (ext != ".bdf") {
                std::cerr << "gbinspect: input events require a .BDF page or START.PRJ\n";
                return 2;
            }
            return interactPage(file, runOpen, events);
        }

        auto reader = graphboard::BinaryReader::fromFile(file);
        if (ext == ".prj") {
            std::cout << projectToJson(graphboard::parseProjectManifest(reader)).dump(2) << "\n";
        } else if (ext == ".bdf") {
            std::cout << bdfToJson(reader).dump(2) << "\n";
        } else {
            std::cerr << "unsupported extension: " << ext << "\n";
            return 2;
        }
    } catch (const std::exception& ex) {
        std::cerr << "gbinspect: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}
