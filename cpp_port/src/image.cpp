#include "graphboard/image.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>

namespace graphboard {

namespace {

std::uint32_t crc32(const std::uint8_t* data, std::size_t len) {
    static const std::array<std::uint32_t, 256> table = [] {
        std::array<std::uint32_t, 256> t{};
        for (std::uint32_t n = 0; n < 256; ++n) {
            std::uint32_t c = n;
            for (int k = 0; k < 8; ++k) {
                c = (c & 1) ? (0xedb88320u ^ (c >> 1)) : (c >> 1);
            }
            t[n] = c;
        }
        return t;
    }();
    std::uint32_t c = 0xffffffffu;
    for (std::size_t i = 0; i < len; ++i) {
        c = table[(c ^ data[i]) & 0xff] ^ (c >> 8);
    }
    return c ^ 0xffffffffu;
}

std::uint32_t adler32(const std::uint8_t* data, std::size_t len) {
    std::uint32_t s1 = 1, s2 = 0;
    for (std::size_t i = 0; i < len; ++i) {
        s1 = (s1 + data[i]) % 65521;
        s2 = (s2 + s1) % 65521;
    }
    return (s2 << 16) | s1;
}

void putBE32(std::vector<std::uint8_t>& out, std::uint32_t v) {
    out.push_back(static_cast<std::uint8_t>(v >> 24));
    out.push_back(static_cast<std::uint8_t>(v >> 16));
    out.push_back(static_cast<std::uint8_t>(v >> 8));
    out.push_back(static_cast<std::uint8_t>(v));
}

void putChunk(std::vector<std::uint8_t>& out, const char (&type)[5],
              const std::vector<std::uint8_t>& data) {
    putBE32(out, static_cast<std::uint32_t>(data.size()));
    const std::size_t typeStart = out.size();
    out.insert(out.end(), type, type + 4);
    out.insert(out.end(), data.begin(), data.end());
    out.push_back(0);
    out.push_back(0);
    out.push_back(0);
    out.push_back(0);
    const std::uint32_t crc = crc32(&out[typeStart], 4 + data.size());
    out[out.size() - 4] = static_cast<std::uint8_t>(crc >> 24);
    out[out.size() - 3] = static_cast<std::uint8_t>(crc >> 16);
    out[out.size() - 2] = static_cast<std::uint8_t>(crc >> 8);
    out[out.size() - 1] = static_cast<std::uint8_t>(crc);
}

// Wrap `raw` in a zlib stream using stored (uncompressed) DEFLATE blocks.
std::vector<std::uint8_t> zlibStored(const std::vector<std::uint8_t>& raw) {
    std::vector<std::uint8_t> out;
    out.push_back(0x78);   // CMF: deflate, 32K window
    out.push_back(0x01);   // FLG: no preset dict, check bits
    std::size_t pos = 0;
    while (pos < raw.size() || raw.empty()) {
        const std::size_t block = std::min<std::size_t>(raw.size() - pos, 0xffff);
        const bool last = pos + block >= raw.size();
        out.push_back(last ? 1 : 0);   // BFINAL, BTYPE=00 (stored)
        out.push_back(static_cast<std::uint8_t>(block));
        out.push_back(static_cast<std::uint8_t>(block >> 8));
        out.push_back(static_cast<std::uint8_t>(~block));
        out.push_back(static_cast<std::uint8_t>(~block >> 8));
        out.insert(out.end(), raw.begin() + pos, raw.begin() + pos + block);
        pos += block;
        if (last) {
            break;
        }
    }
    putBE32(out, adler32(raw.data(), raw.size()));
    return out;
}

} // namespace

bool writePng(const std::filesystem::path& path, const Image& image) {
    if (image.width <= 0 || image.height <= 0) {
        return false;
    }

    // Filtered scanlines: each row is a filter byte (0 = None) then RGB pixels.
    std::vector<std::uint8_t> raw;
    raw.reserve((static_cast<std::size_t>(image.width) * 3 + 1) * image.height);
    for (int y = 0; y < image.height; ++y) {
        raw.push_back(0);
        const std::size_t row = static_cast<std::size_t>(y) * image.width * 3;
        raw.insert(raw.end(), image.rgb.begin() + row,
                   image.rgb.begin() + row + static_cast<std::size_t>(image.width) * 3);
    }

    std::vector<std::uint8_t> png = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a};

    std::vector<std::uint8_t> ihdr;
    putBE32(ihdr, static_cast<std::uint32_t>(image.width));
    putBE32(ihdr, static_cast<std::uint32_t>(image.height));
    ihdr.push_back(8);   // bit depth
    ihdr.push_back(2);   // colour type: truecolour RGB
    ihdr.push_back(0);   // compression
    ihdr.push_back(0);   // filter
    ihdr.push_back(0);   // interlace
    putChunk(png, "IHDR", ihdr);
    putChunk(png, "IDAT", zlibStored(raw));
    putChunk(png, "IEND", {});

    std::ofstream out(path, std::ios::binary);
    if (!out) {
        return false;
    }
    out.write(reinterpret_cast<const char*>(png.data()), static_cast<std::streamsize>(png.size()));
    return static_cast<bool>(out);
}

} // namespace graphboard
