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

// Supplies decoded frames for Transparent_Video_Holder entries that are
// currently playing. Returns width*height 8-bpp top-down pixels (drawn with
// `palette`, 256 RGBQUAD entries), or nullptr to fall back to the entry's
// still frame. Implemented by the interactive shell, which owns the
// BoardVideoDecoder cache; headless renders pass nothing and get still frames.
class VideoFrameSource {
public:
    virtual ~VideoFrameSource() = default;
    virtual const std::uint8_t* currentFrame(const std::string& component, int entry,
                                             const std::uint8_t*& palette) = 0;
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
