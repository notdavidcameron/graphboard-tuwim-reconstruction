#include "graphboard/format.hpp"

namespace graphboard {

namespace {

constexpr std::size_t kBdfBannerBytes = 100;

std::vector<std::string> readStringTable(BinaryReader& reader) {
    const auto count = reader.readU32();
    std::vector<std::string> values;
    values.reserve(count);
    for (std::uint32_t i = 0; i < count; ++i) {
        values.push_back(reader.readArchiveString());
    }
    return values;
}

std::string trimTrailingNulAndSpace(std::string value) {
    while (!value.empty() && (value.back() == '\0' || value.back() == ' ')) {
        value.pop_back();
    }
    return value;
}

} // namespace

ProjectManifest parseProjectManifest(BinaryReader& reader) {
    ProjectManifest project;
    project.version = reader.readU32();
    project.startupPage = reader.readArchiveString();
    project.currentPageOrGroupState = reader.readArchiveString();
    project.audioPresetIndex = reader.readU32();
    project.pageNames = readStringTable(reader);
    project.groupNames = readStringTable(reader);
    project.encodedSignature = reader.readArchiveString();
    project.decodedSignature = decodeShiftedSignature(project.encodedSignature);
    project.trailingAudioManagerOffset = reader.position();
    project.trailingAudioManagerBytes = reader.remaining();
    return project;
}

BdfHeader parseBdfHeader(BinaryReader& reader) {
    BdfHeader header;
    header.banner = trimTrailingNulAndSpace(reader.readBytesAsString(kBdfBannerBytes));
    header.version = reader.readU32();
    header.pageRect.left = reader.readI32();
    header.pageRect.top = reader.readI32();
    header.pageRect.right = reader.readI32();
    header.pageRect.bottom = reader.readI32();
    header.minLayer = reader.readI32();
    header.maxLayer = reader.readI32();
    header.backgroundFlag = reader.readU32();
    header.backgroundColorIndex = reader.readU8();
    header.paletteByteCount = reader.readU32();
    header.paletteOffset = reader.position();
    reader.skip(header.paletteByteCount);
    header.dibByteCount = reader.readU32();
    header.dibOffset = reader.position();
    reader.skip(header.dibByteCount);
    header.componentListOffset = reader.position();
    return header;
}

std::string decodeShiftedSignature(const std::string& encoded) {
    std::string decoded;
    decoded.reserve(encoded.size());
    for (const unsigned char c : encoded) {
        decoded.push_back(static_cast<char>(c - 0x21));
    }
    return decoded;
}

} // namespace graphboard
