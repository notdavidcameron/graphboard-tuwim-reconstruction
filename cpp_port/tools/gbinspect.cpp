#include "graphboard/component.hpp"
#include "graphboard/format.hpp"
#include "graphboard/holders.hpp"
#include "graphboard/runtime/script.hpp"
#include "graphboard/text.hpp"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>
#include <set>
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
    json out = {
        {"displayName", t(wrapper.displayName)},
        {"clsid", wrapper.clsid.toString()},
        {"holderKind", info ? info->displayName : std::string("Unknown")},
        {"wrapperVersion", wrapper.wrapperVersion},
        {"functionCount", wrapper.functions.size()},
        {"propertyCount", wrapper.properties.size()},
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
        definitions.push_back({
            {"name", t(definition.name)},
            {"width", definition.width},
            {"height", definition.height},
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
        bitmaps.push_back({
            {"name", t(bitmap.name)},
            {"rect", {bitmap.left, bitmap.top, bitmap.right, bitmap.bottom}},
            {"blobByteCount", bitmap.blobByteCount},
            {"pixelOffset", bitmap.pixelOffset},
            {"pixelSizeConsistent", bitmap.pixelSizeConsistent},
        });
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

} // namespace

int main(int argc, char** argv) {
    const auto args = collectArgs(argc, argv);
    if (args.size() != 2) {
        std::cerr << "usage: gbinspect <START.PRJ|PAGE.BDF>\n";
        return 2;
    }

    try {
        const std::filesystem::path path = args[1];
        auto reader = graphboard::BinaryReader::fromFile(path);
        const auto ext = lowerExtension(path);
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
