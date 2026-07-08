#pragma once

#include "graphboard/binary_reader.hpp"

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace graphboard {

struct Rect {
    std::int32_t left = 0;
    std::int32_t top = 0;
    std::int32_t right = 0;
    std::int32_t bottom = 0;
};

struct ProjectManifest {
    std::uint32_t version = 0;
    std::string startupPage;
    std::string currentPageOrGroupState;
    std::uint32_t audioPresetIndex = 0;   // audioManager+0x20; only project field it owns
    std::vector<std::string> pageNames;
    std::vector<std::string> groupNames;
    std::string encodedSignature;
    std::string decodedSignature;
    // Trailing block is the group/global script editor's text (doc +0xa0),
    // written by GraphBrdScriptEditor_SerializeText, NOT audio-manager state.
    std::uint32_t globalScriptVersion = 0;
    std::string globalScript;
};

struct BdfHeader {
    std::string banner;
    std::uint32_t version = 0;
    Rect pageRect;
    std::int32_t minLayer = 0;
    std::int32_t maxLayer = 0;
    std::uint32_t backgroundFlag = 0;
    std::uint8_t backgroundColorIndex = 0;
    std::uint32_t paletteByteCount = 0;
    std::size_t paletteOffset = 0;
    std::uint32_t dibByteCount = 0;
    std::size_t dibOffset = 0;
    std::size_t componentListOffset = 0;
};

ProjectManifest parseProjectManifest(BinaryReader& reader);
BdfHeader parseBdfHeader(BinaryReader& reader);

std::string decodeShiftedSignature(const std::string& encoded);

} // namespace graphboard
