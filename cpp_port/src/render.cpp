#include "graphboard/render.hpp"

#include <cstdint>

namespace graphboard {

namespace {

std::uint32_t readU32(const std::vector<std::uint8_t>& b, std::size_t off) {
    if (off + 4 > b.size()) {
        return 0;
    }
    return static_cast<std::uint32_t>(b[off]) | (static_cast<std::uint32_t>(b[off + 1]) << 8) |
           (static_cast<std::uint32_t>(b[off + 2]) << 16) |
           (static_cast<std::uint32_t>(b[off + 3]) << 24);
}

std::int32_t readI32(const std::vector<std::uint8_t>& b, std::size_t off) {
    return static_cast<std::int32_t>(readU32(b, off));
}

std::uint16_t readU16(const std::vector<std::uint8_t>& b, std::size_t off) {
    if (off + 2 > b.size()) {
        return 0;
    }
    return static_cast<std::uint16_t>(b[off] | (b[off + 1] << 8));
}

} // namespace

Image renderBackground(const std::vector<std::uint8_t>& bytes, const BdfHeader& header) {
    // A packed DIB: BITMAPINFOHEADER + RGBQUAD palette + 8-bit bottom-up pixels.
    if (header.dibByteCount >= 40 && header.dibOffset + 40 <= bytes.size()) {
        const std::size_t base = header.dibOffset;
        const std::uint32_t headerSize = readU32(bytes, base);
        const std::int32_t width = readI32(bytes, base + 4);
        const std::int32_t rawHeight = readI32(bytes, base + 8);
        const std::uint16_t bitCount = readU16(bytes, base + 14);
        const std::uint32_t colorsUsed = readU32(bytes, base + 32);
        const bool topDown = rawHeight < 0;
        const std::int32_t height = topDown ? -rawHeight : rawHeight;

        if (bitCount == 8 && width > 0 && height > 0 && headerSize >= 40) {
            const std::uint32_t paletteEntries = colorsUsed ? colorsUsed : 256u;
            const std::size_t paletteOff = base + headerSize;
            const std::size_t pixels = paletteOff + static_cast<std::size_t>(paletteEntries) * 4;
            const std::size_t stride = (static_cast<std::size_t>(width) + 3) & ~std::size_t{3};

            Image img(width, height);
            for (std::int32_t y = 0; y < height; ++y) {
                const std::int32_t srcRow = topDown ? y : (height - 1 - y);
                const std::size_t rowOff = pixels + static_cast<std::size_t>(srcRow) * stride;
                for (std::int32_t x = 0; x < width; ++x) {
                    const std::size_t pi = rowOff + static_cast<std::size_t>(x);
                    const std::uint8_t idx = pi < bytes.size() ? bytes[pi] : 0;
                    const std::size_t c = paletteOff + static_cast<std::size_t>(idx) * 4;
                    // RGBQUAD is blue, green, red, reserved.
                    const std::uint8_t bl = c + 0 < bytes.size() ? bytes[c + 0] : 0;
                    const std::uint8_t gr = c + 1 < bytes.size() ? bytes[c + 1] : 0;
                    const std::uint8_t rd = c + 2 < bytes.size() ? bytes[c + 2] : 0;
                    img.set(x, y, rd, gr, bl);
                }
            }
            return img;
        }
    }

    // No DIB: a solid page sized from the page rect, filled with the background
    // colour from the page palette (256 RGBQUAD entries at paletteOffset).
    const int width = header.pageRect.right > header.pageRect.left
                          ? header.pageRect.right - header.pageRect.left : 640;
    const int height = header.pageRect.bottom > header.pageRect.top
                           ? header.pageRect.bottom - header.pageRect.top : 480;
    std::uint8_t r = 0, g = 0, b = 0;
    if (header.paletteByteCount >= 1024) {
        const std::size_t c = header.paletteOffset +
                              static_cast<std::size_t>(header.backgroundColorIndex) * 4;
        b = c + 0 < bytes.size() ? bytes[c + 0] : 0;
        g = c + 1 < bytes.size() ? bytes[c + 1] : 0;
        r = c + 2 < bytes.size() ? bytes[c + 2] : 0;
    }
    return Image(width, height, r, g, b);
}

} // namespace graphboard
