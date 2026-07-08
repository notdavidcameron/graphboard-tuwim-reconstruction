#include "graphboard/component.hpp"
#include "graphboard/format.hpp"
#include "graphboard/holders.hpp"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>
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
        {"startupPage", project.startupPage},
        {"currentPageOrGroupState", project.currentPageOrGroupState},
        {"audioPresetIndex", project.audioPresetIndex},
        {"pageCount", project.pageNames.size()},
        {"pages", project.pageNames},
        {"groupCount", project.groupNames.size()},
        {"groups", project.groupNames},
        {"decodedSignature", project.decodedSignature},
        {"globalScriptVersion", project.globalScriptVersion},
        {"globalScriptByteCount", project.globalScript.size()},
        {"globalScript", project.globalScript},
    };
}

json headerToJson(const BdfHeader& header) {
    return {
        {"banner", header.banner},
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
        {"displayName", wrapper.displayName},
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

json hotspotStateToJson(const HotSpotHolderState& state) {
    json hotspots = json::array();
    for (const auto& hotspot : state.hotspots) {
        hotspots.push_back({
            {"rect", {hotspot.left, hotspot.top, hotspot.right, hotspot.bottom}},
            {"layer", hotspot.layer},
            {"enabled", hotspot.enabled},
            {"name", hotspot.name},
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
        if (info && info->kind == graphboard::HolderKind::HotSpotHolder) {
            component["privateStateParsed"] = true;
            component["privateState"] = hotspotStateToJson(graphboard::parseHotSpotHolderState(reader));
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
    const auto header = graphboard::parseBdfHeader(reader);
    return {
        {"kind", "BDF"},
        {"header", headerToJson(header)},
        {"componentList", componentListToJson(reader)},
    };
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
