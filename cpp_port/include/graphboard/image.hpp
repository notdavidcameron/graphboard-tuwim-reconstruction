#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>

namespace graphboard {

// A simple 24-bit RGB image, top-down, row-major. Enough to composite an
// indexed-colour GraphBoard page (background + sprites + bitmaps) for a visual
// render of the headless runtime's state.
struct Image {
    int width = 0;
    int height = 0;
    std::vector<std::uint8_t> rgb;   // width*height*3, row-major top-down

    Image() = default;
    Image(int w, int h, std::uint8_t r = 0, std::uint8_t g = 0, std::uint8_t b = 0)
        : width(w), height(h),
          rgb(static_cast<std::size_t>(w) * h * 3) {
        for (std::size_t i = 0; i + 2 < rgb.size(); i += 3) {
            rgb[i] = r; rgb[i + 1] = g; rgb[i + 2] = b;
        }
    }

    // Set a pixel if it is inside the frame (out-of-range is ignored so callers
    // can blit clipped sprites without bounds-checking every pixel).
    void set(int x, int y, std::uint8_t r, std::uint8_t g, std::uint8_t b) {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return;
        }
        const std::size_t i = (static_cast<std::size_t>(y) * width + x) * 3;
        rgb[i] = r; rgb[i + 1] = g; rgb[i + 2] = b;
    }
};

// Write `image` as a PNG (24-bit truecolour, uncompressed DEFLATE blocks so the
// encoder stays small and dependency-free). Returns false on I/O failure.
bool writePng(const std::filesystem::path& path, const Image& image);

} // namespace graphboard
