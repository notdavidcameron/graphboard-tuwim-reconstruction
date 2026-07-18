#pragma once

#include "graphboard/format.hpp"
#include "graphboard/image.hpp"
#include "graphboard/runtime/page.hpp"

#include <cstdint>
#include <vector>

namespace graphboard {

// Render a page's static background to an RGB image: decode the packed 8-bit DIB
// when the page has one, otherwise fill with the page palette's background
// colour. `bytes` is the whole .BDF; `header` is its parsed header. The image is
// sized from the DIB (or the page rect when there is no DIB).
//
// This is the base layer of the visual "head" for the headless runtime; live
// components (sprites, bitmaps) composite on top of it.
Image renderBackground(const std::vector<std::uint8_t>& bytes, const BdfHeader& header);

// Supplies the native TVH draw view for a currently playing entry. Persistent
// streams expose their full retained DIB; direct streams expose only the current
// record rectangle. Pixels are 8-bpp page-palette indices, exactly as the DLL
// writes them into the board backbuffer.
class VideoFrameSource {
public:
    struct FrameView {
        const std::uint8_t* pixels = nullptr;
        std::size_t stride = 0;
        int x = 0, y = 0, width = 0, height = 0;
        bool useTransparency = false;
        std::uint8_t transparentIndex = 0;
    };

    virtual ~VideoFrameSource() = default;
    virtual FrameView currentFrame(const std::string& component, int entry) = 0;
};

// Render the page's live scene: the background, then every visible sprite,
// bitmap and video entry, plus the active group's sprites, composited at their
// current position, layer and phase
// (lowest layer first), using the page palette and per-item transparency.
// `page` supplies the live, script-mutated state; `bytes`/`header` supply the
// palette and static background. This is the visual "head" of the headless
// runtime.
Image renderPage(const runtime::Page& page, const std::vector<std::uint8_t>& bytes,
                 const BdfHeader& header, VideoFrameSource* videoFrames = nullptr);

} // namespace graphboard
