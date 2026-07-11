#include "graphboard/video.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>
#include <vector>

using graphboard::BoardVideoDecoder;
using graphboard::runtime::VideoGeometry;

namespace {

void putU32(std::vector<std::uint8_t>& bytes, std::size_t offset, std::uint32_t value) {
    if (bytes.size() < offset + 4) bytes.resize(offset + 4);
    bytes[offset] = static_cast<std::uint8_t>(value);
    bytes[offset + 1] = static_cast<std::uint8_t>(value >> 8);
    bytes[offset + 2] = static_cast<std::uint8_t>(value >> 16);
    bytes[offset + 3] = static_cast<std::uint8_t>(value >> 24);
}

void appendRecord(std::vector<std::uint8_t>& bytes, std::uint32_t tag,
                  int left, int top, int right, int bottom,
                  const std::vector<std::uint8_t>& payload) {
    const std::size_t start = bytes.size();
    bytes.resize(start + 0x48, 0);
    putU32(bytes, start, static_cast<std::uint32_t>(0x48 + payload.size()));
    putU32(bytes, start + 0x04, tag);
    putU32(bytes, start + 0x14, static_cast<std::uint32_t>(left));
    putU32(bytes, start + 0x18, static_cast<std::uint32_t>(top));
    putU32(bytes, start + 0x1c, static_cast<std::uint32_t>(right));
    putU32(bytes, start + 0x20, static_cast<std::uint32_t>(bottom));
    bytes.insert(bytes.end(), payload.begin(), payload.end());
}

void testDecodeAndSeek() {
    std::vector<std::uint8_t> bytes(0x4e8, 0);
    putU32(bytes, 0x90, 22050);
    putU32(bytes, 0x94, 16);
    putU32(bytes, 0x98, 1);
    for (int i = 0; i < 256; ++i) {
        bytes[0xe8 + static_cast<std::size_t>(i) * 4 + 2] = static_cast<std::uint8_t>(i);
    }

    // One 260-pixel row: literal 1,2,3; short run 4,4; long run of 255 fives.
    appendRecord(bytes, 0xff000001u, 0, 0, 260, 1,
                 {3, 1, 2, 3, 0, 2, 4, 0xff, 0xff, 0x00, 5});
    appendRecord(bytes, 0xff000002u, 0, 0, 0, 0, {0xaa, 0xbb});
    appendRecord(bytes, 0xff000001u, 0, 0, 3, 1, {3, 9, 8, 7});

    VideoGeometry geometry;
    geometry.width = 260;
    geometry.height = 1;
    geometry.frameCount = 2;
    geometry.frameDurationMs = 100;
    geometry.streamOffset = 0;
    geometry.streamByteCount = static_cast<std::uint32_t>(bytes.size());

    BoardVideoDecoder decoder(bytes, geometry);
    assert(decoder.frameCount() == 2);
    assert(decoder.palette().size() == 1024);
    assert(decoder.audioSampleRate() == 22050);
    assert(decoder.audioBitsPerSample() == 16);
    assert(decoder.audioChannels() == 1);
    assert(decoder.audioPcm() == std::vector<std::uint8_t>({0xaa, 0xbb}));

    const auto& first = decoder.frameAt(0);
    assert(first.size() == 260);
    assert(first[0] == 1 && first[1] == 2 && first[2] == 3);
    assert(first[3] == 4 && first[4] == 4 && first[259] == 5);

    const auto& second = decoder.frameAt(1);
    assert(second[0] == 9 && second[1] == 8 && second[2] == 7);
    assert(second[3] == 4 && second[259] == 5);

    const auto& replayed = decoder.frameAt(0);
    assert(replayed[0] == 1 && replayed[1] == 2 && replayed[2] == 3);
    assert(decoder.lastDecodedFrame() == 0);
}

void testMalformedStream() {
    std::vector<std::uint8_t> bytes(32, 0);
    VideoGeometry geometry;
    geometry.width = 8;
    geometry.height = 8;
    geometry.frameCount = 1;
    geometry.streamByteCount = static_cast<std::uint32_t>(bytes.size());
    BoardVideoDecoder decoder(bytes, geometry);
    assert(decoder.frameCount() == 0);
    assert(decoder.frameAt(0).empty());
}

} // namespace

int main() {
    testDecodeAndSeek();
    testMalformedStream();
    return 0;
}
