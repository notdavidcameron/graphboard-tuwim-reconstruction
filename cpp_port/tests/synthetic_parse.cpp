#include "graphboard/format.hpp"

#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

namespace {

void appendU8(std::vector<std::uint8_t>& bytes, std::uint8_t value) {
    bytes.push_back(value);
}

void appendU16(std::vector<std::uint8_t>& bytes, std::uint16_t value) {
    bytes.push_back(static_cast<std::uint8_t>(value & 0xff));
    bytes.push_back(static_cast<std::uint8_t>((value >> 8) & 0xff));
}

void appendU32(std::vector<std::uint8_t>& bytes, std::uint32_t value) {
    bytes.push_back(static_cast<std::uint8_t>(value & 0xff));
    bytes.push_back(static_cast<std::uint8_t>((value >> 8) & 0xff));
    bytes.push_back(static_cast<std::uint8_t>((value >> 16) & 0xff));
    bytes.push_back(static_cast<std::uint8_t>((value >> 24) & 0xff));
}

void appendArchiveString(std::vector<std::uint8_t>& bytes, const std::string& value) {
    assert(value.size() < 0xff);
    appendU8(bytes, static_cast<std::uint8_t>(value.size()));
    bytes.insert(bytes.end(), value.begin(), value.end());
}

void appendFixedString(std::vector<std::uint8_t>& bytes, const std::string& value, std::size_t width) {
    assert(value.size() <= width);
    bytes.insert(bytes.end(), value.begin(), value.end());
    bytes.resize(bytes.size() + (width - value.size()), 0);
}

std::string encodeSignature(const std::string& value) {
    std::string out;
    for (const unsigned char c : value) {
        out.push_back(static_cast<char>(c + 0x21));
    }
    return out;
}

} // namespace

int main() {
    {
        std::vector<std::uint8_t> bytes;
        appendU32(bytes, 1);
        appendArchiveString(bytes, "intro.bdf");
        appendArchiveString(bytes, "");
        appendU32(bytes, 11);
        appendU32(bytes, 2);
        appendArchiveString(bytes, "intro.bdf");
        appendArchiveString(bytes, "radio.bdf");
        appendU32(bytes, 1);
        appendArchiveString(bytes, "cursors.grp");
        appendArchiveString(bytes, encodeSignature("Julian Tuwim"));
        appendU32(bytes, 0x12345678);

        graphboard::BinaryReader reader(bytes);
        const auto project = graphboard::parseProjectManifest(reader);
        assert(project.version == 1);
        assert(project.startupPage == "intro.bdf");
        assert(project.audioPresetIndex == 11);
        assert(project.pageNames.size() == 2);
        assert(project.groupNames.size() == 1);
        assert(project.decodedSignature == "Julian Tuwim");
        assert(project.trailingAudioManagerBytes == 4);
    }

    {
        std::vector<std::uint8_t> bytes;
        appendFixedString(bytes, "YDP Board data file. Ver:1 1996.07.", 100);
        appendU32(bytes, 1);
        appendU32(bytes, 0);
        appendU32(bytes, 0);
        appendU32(bytes, 640);
        appendU32(bytes, 480);
        appendU32(bytes, 0xffffffffu);
        appendU32(bytes, 8);
        appendU32(bytes, 1);
        appendU8(bytes, 132);
        appendU32(bytes, 4);
        appendU8(bytes, 1);
        appendU8(bytes, 2);
        appendU8(bytes, 3);
        appendU8(bytes, 4);
        appendU32(bytes, 3);
        appendU8(bytes, 9);
        appendU8(bytes, 8);
        appendU8(bytes, 7);
        appendU32(bytes, 1);

        graphboard::BinaryReader reader(bytes);
        const auto header = graphboard::parseBdfHeader(reader);
        assert(header.version == 1);
        assert(header.pageRect.right == 640);
        assert(header.pageRect.bottom == 480);
        assert(header.minLayer == -1);
        assert(header.backgroundColorIndex == 132);
        assert(header.paletteByteCount == 4);
        assert(header.dibByteCount == 3);
        assert(header.componentListOffset == 148);
    }
}
