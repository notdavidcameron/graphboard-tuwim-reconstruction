#include "graphboard/render.hpp"

#include <algorithm>
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

namespace {

using runtime::ComponentState;

std::int64_t itemInt(const ComponentState& c, int id, const std::string& key) {
    const auto ci = c.items.find(id);
    if (ci == c.items.end()) return 0;
    const auto ki = ci->second.find(key);
    return ki == ci->second.end() ? 0 : ki->second.toInt();
}

// One blit request, resolved to a fixed rectangle of indexed pixels.
struct Drawable {
    std::int32_t layer = 0;
    std::size_t order = 0;               // stable tie-break (scene order)
    int x = 0, y = 0, w = 0, h = 0;
    const std::uint8_t* pixels = nullptr;   // 8-bpp indexed
    std::size_t stride = 0;                 // bytes per source row
    bool bottomUp = false;                  // DIB-order rows (still frames)
    bool useTransparent = true;
    std::uint8_t transparent = 0;
    const std::uint8_t* palette = nullptr;  // 256 RGBQUADs; null = page palette
};

// 8-bpp bottom-up DIB row stride.
std::size_t dibStride(std::size_t width) {
    return (width + 3) & ~std::size_t{3};
}

// Board videos are keyed on pure green: the palette index whose RGBQUAD is
// (0, 255, 0). Returns -1 when the palette has no such entry.
int greenKeyIndex(const std::uint8_t* palette) {
    if (!palette) return -1;
    for (int i = 0; i < 256; ++i) {
        const std::uint8_t* c = palette + static_cast<std::size_t>(i) * 4;
        if (c[0] == 0 && c[1] == 255 && c[2] == 0) return i;
    }
    return -1;
}

} // namespace

Image renderPage(const runtime::Page& page, const std::vector<std::uint8_t>& bytes,
                 const BdfHeader& header, VideoFrameSource* videoFrames) {
    Image img = renderBackground(bytes, header);

    const std::uint8_t* pagePalette =
        header.paletteByteCount >= 1024 && header.paletteOffset + 1024 <= bytes.size()
            ? bytes.data() + header.paletteOffset : nullptr;

    std::vector<Drawable> draws;
    std::size_t order = 0;
    auto collect = [&](const std::vector<runtime::ComponentState>& components) {
      for (const auto& c : components) {
        if (c.kind == graphboard::HolderKind::SpriteHolder) {
            for (const auto& [id, item] : c.items) {
                if (itemInt(c, id, "visible") == 0) continue;
                if (id < 0 || static_cast<std::size_t>(id) >= c.sprites.size()) continue;
                const auto& geom = c.sprites[static_cast<std::size_t>(id)];
                const auto phase = itemInt(c, id, "phase");
                if (phase < 0 || phase >= static_cast<std::int64_t>(geom.frames.size())) continue;
                const auto& f = geom.frames[static_cast<std::size_t>(phase)];
                if (f.pixels.empty()) continue;
                Drawable d;
                d.layer = static_cast<std::int32_t>(itemInt(c, id, "z"));
                d.order = order++;
                d.x = static_cast<int>(itemInt(c, id, "x"));
                d.y = static_cast<int>(itemInt(c, id, "y"));
                d.w = static_cast<int>(f.width);
                d.h = static_cast<int>(f.height);
                d.pixels = f.pixels.data();
                d.stride = f.width;
                d.transparent = f.transparentIndex;
                draws.push_back(d);
            }
        } else if (c.kind == graphboard::HolderKind::BitmapHolder) {
            for (const auto& [id, item] : c.items) {
                if (itemInt(c, id, "visible") == 0) continue;
                if (id < 0 || static_cast<std::size_t>(id) >= c.bitmaps.size()) continue;
                const auto& geom = c.bitmaps[static_cast<std::size_t>(id)];
                if (geom.pixels.empty()) continue;
                Drawable d;
                d.layer = static_cast<std::int32_t>(itemInt(c, id, "z"));
                d.order = order++;
                d.x = static_cast<int>(itemInt(c, id, "x"));
                d.y = static_cast<int>(itemInt(c, id, "y"));
                d.w = geom.width;
                d.h = geom.height;
                d.pixels = geom.pixels.data();
                d.stride = static_cast<std::size_t>(geom.width);
                d.transparent = geom.transparentIndex;
                draws.push_back(d);
            }
        } else if (c.kind == graphboard::HolderKind::TransparentVideoHolder) {
            for (const auto& [id, item] : c.items) {
                if (id < 0 || static_cast<std::size_t>(id) >= c.videos.size()) continue;
                const auto& geom = c.videos[static_cast<std::size_t>(id)];
                const bool playing = itemInt(c, id, "playing") != 0;
                // Idle videos that never played draw nothing: the page
                // background contains the idle scene, and the persisted
                // last frame (visible == 1) survives only until the script's
                // HideFirsLastVideoFrame.
                const bool shown = itemInt(c, id, "visible") != 0;
                if (!playing && !shown) continue;
                Drawable d;
                d.layer = static_cast<std::int32_t>(itemInt(c, id, "z"));
                d.order = order++;
                d.x = static_cast<int>(itemInt(c, id, "x"));
                d.y = static_cast<int>(itemInt(c, id, "y"));
                d.w = geom.width;
                d.h = geom.height;
                if (videoFrames) {
                    const std::uint8_t* palette = nullptr;
                    const std::uint8_t* frame =
                        videoFrames->currentFrame(c.displayName, id, palette);
                    if (frame) {
                        d.pixels = frame;
                        d.stride = static_cast<std::size_t>(geom.width);
                        d.palette = palette;
                        const int key = greenKeyIndex(palette ? palette : pagePalette);
                        d.useTransparent = key >= 0;
                        d.transparent = static_cast<std::uint8_t>(key >= 0 ? key : 0);
                        draws.push_back(d);
                        continue;
                    }
                }
                // Still frame: the file's saved 8-bpp bottom-up, stride-padded
                // DIB, indexed against the VIDEO stream's own palette (at
                // streamOffset+0xe8) -- not the page palette. Used both headless
                // and, in the shell, whenever the entry is not actively playing.
                if (!shown) continue;
                const std::size_t stride = dibStride(static_cast<std::size_t>(geom.width));
                if (geom.stillFrameOffset + stride * geom.height > bytes.size()) continue;
                const std::uint8_t* videoPalette =
                    geom.paletteOffset + 1024 <= bytes.size() ? bytes.data() + geom.paletteOffset
                                                              : nullptr;
                d.pixels = bytes.data() + geom.stillFrameOffset;
                d.stride = stride;
                d.bottomUp = true;
                d.palette = videoPalette;
                const int key = greenKeyIndex(videoPalette ? videoPalette : pagePalette);
                d.useTransparent = key >= 0;
                d.transparent = static_cast<std::uint8_t>(key >= 0 ? key : 0);
                draws.push_back(d);
            }
        }
      }
    };
    collect(page.components());
    collect(page.groupComponents());

    // Lowest layer first; scene order breaks ties (the board draws page
    // components in list order per layer).
    std::stable_sort(draws.begin(), draws.end(), [](const Drawable& a, const Drawable& b) {
        return a.layer != b.layer ? a.layer < b.layer : a.order < b.order;
    });

    for (const auto& d : draws) {
        const std::uint8_t* palette = d.palette ? d.palette : pagePalette;
        for (int row = 0; row < d.h; ++row) {
            const int srcRow = d.bottomUp ? d.h - 1 - row : row;
            const std::uint8_t* src = d.pixels + static_cast<std::size_t>(srcRow) * d.stride;
            for (int col = 0; col < d.w; ++col) {
                const std::uint8_t idx = src[col];
                if (d.useTransparent && idx == d.transparent) continue;
                std::uint8_t r = idx, g = idx, b = idx;
                if (palette) {
                    const std::uint8_t* c = palette + static_cast<std::size_t>(idx) * 4;
                    b = c[0]; g = c[1]; r = c[2];
                }
                img.set(d.x + col, d.y + row, r, g, b);
            }
        }
    }
    return img;
}

} // namespace graphboard
