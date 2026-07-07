#include "graphboard/format.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <iostream>

namespace {

using graphboard::BdfHeader;
using graphboard::ProjectManifest;

std::string lowerExtension(const std::filesystem::path& path) {
    auto ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return ext;
}

void printStringArray(const std::vector<std::string>& values, const char* indent) {
    std::cout << "[";
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i != 0) {
            std::cout << ",";
        }
        std::cout << "\n" << indent << "\"" << graphboard::jsonEscape(values[i]) << "\"";
    }
    if (!values.empty()) {
        std::cout << "\n  ";
    }
    std::cout << "]";
}

void printProject(const ProjectManifest& project) {
    std::cout
        << "{\n"
        << "  \"kind\": \"START.PRJ\",\n"
        << "  \"version\": " << project.version << ",\n"
        << "  \"startupPage\": \"" << graphboard::jsonEscape(project.startupPage) << "\",\n"
        << "  \"currentPageOrGroupState\": \"" << graphboard::jsonEscape(project.currentPageOrGroupState) << "\",\n"
        << "  \"audioPresetIndex\": " << project.audioPresetIndex << ",\n"
        << "  \"pageCount\": " << project.pageNames.size() << ",\n"
        << "  \"pages\": ";
    printStringArray(project.pageNames, "    ");
    std::cout
        << ",\n"
        << "  \"groupCount\": " << project.groupNames.size() << ",\n"
        << "  \"groups\": ";
    printStringArray(project.groupNames, "    ");
    std::cout
        << ",\n"
        << "  \"decodedSignature\": \"" << graphboard::jsonEscape(project.decodedSignature) << "\",\n"
        << "  \"trailingAudioManagerOffset\": " << project.trailingAudioManagerOffset << ",\n"
        << "  \"trailingAudioManagerBytes\": " << project.trailingAudioManagerBytes << "\n"
        << "}\n";
}

void printBdf(const BdfHeader& header) {
    std::cout
        << "{\n"
        << "  \"kind\": \"BDF\",\n"
        << "  \"banner\": \"" << graphboard::jsonEscape(header.banner) << "\",\n"
        << "  \"version\": " << header.version << ",\n"
        << "  \"pageRect\": [" << header.pageRect.left << ", " << header.pageRect.top << ", "
        << header.pageRect.right << ", " << header.pageRect.bottom << "],\n"
        << "  \"layerRange\": [" << header.minLayer << ", " << header.maxLayer << "],\n"
        << "  \"backgroundFlag\": " << header.backgroundFlag << ",\n"
        << "  \"backgroundColorIndex\": " << static_cast<int>(header.backgroundColorIndex) << ",\n"
        << "  \"paletteByteCount\": " << header.paletteByteCount << ",\n"
        << "  \"paletteOffset\": " << header.paletteOffset << ",\n"
        << "  \"dibByteCount\": " << header.dibByteCount << ",\n"
        << "  \"dibOffset\": " << header.dibOffset << ",\n"
        << "  \"componentListOffset\": " << header.componentListOffset << "\n"
        << "}\n";
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
            printProject(graphboard::parseProjectManifest(reader));
        } else if (ext == ".bdf") {
            printBdf(graphboard::parseBdfHeader(reader));
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
