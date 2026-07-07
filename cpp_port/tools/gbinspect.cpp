#include "graphboard/format.hpp"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>

namespace {

using graphboard::BdfHeader;
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
        {"trailingAudioManagerOffset", project.trailingAudioManagerOffset},
        {"trailingAudioManagerBytes", project.trailingAudioManagerBytes},
    };
}

json bdfToJson(const BdfHeader& header) {
    return {
        {"kind", "BDF"},
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

} // namespace

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: gbinspect <START.PRJ|PAGE.BDF>\n";
        return 2;
    }

    try {
        const std::filesystem::path path(argv[1]);
        auto reader = graphboard::BinaryReader::fromFile(path);
        const auto ext = lowerExtension(path);
        if (ext == ".prj") {
            std::cout << projectToJson(graphboard::parseProjectManifest(reader)).dump(2) << "\n";
        } else if (ext == ".bdf") {
            std::cout << bdfToJson(graphboard::parseBdfHeader(reader)).dump(2) << "\n";
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
