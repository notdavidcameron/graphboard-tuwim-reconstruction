#pragma once

#include "graphboard/format.hpp"
#include "graphboard/image.hpp"

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

} // namespace graphboard
