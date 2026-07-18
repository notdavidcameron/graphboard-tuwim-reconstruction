#pragma once

#include "graphboard/runtime/page.hpp"

#include <cstdint>
#include <vector>

namespace graphboard {

// Decoder for one embedded "Board Video File" stream (Transparent_Video_Holder
// entry). The stream is a 0x4e8 header (with its own 256-entry RGBQUAD palette
// at +0xe8) followed by chunk records: 0x48-byte record headers tagged video
// (0xff000001, an RLE-encoded changed rect per frame) or audio (0xff000002,
// raw PCM). The RLE codec is the recovered TVH_DrawRleRect (
// TransparentVideoHolder.dll:1000b8a0; see
// reconstructed/TransparentVideoHolder_reconstructed.cpp).
//
// Changed records are cumulative deltas against the preceding decoded frame;
// records explicitly marked unchanged retain it. The original holder draws
// each changed rectangle into a persistent backing DIB before compositing it.
class BoardVideoDecoder {
public:
    // `fileBytes` is the whole .BDF; `geom` locates the stream inside it.
    BoardVideoDecoder(const std::vector<std::uint8_t>& fileBytes,
                      const runtime::VideoGeometry& geom);

    int frameCount() const { return frameCount_; }
    int width() const { return width_; }
    int height() const { return height_; }

    // 8-bpp indexed pixels of frame `index` (clamped to the stream's range),
    // width*height, row-major top-down. Empty if the stream is malformed.
    const std::vector<std::uint8_t>& frameAt(int index);

    // Index of the frame currently decoded in the buffer (-1 before any decode).
    int lastDecodedFrame() const { return decodedFrame_; }

    // The original DLL has two playback modes. Streams with header+0x6c set
    // retain all decoded deltas in a full-size backing DIB. Other streams draw
    // only the current record rectangle into the board's freshly composed
    // surface. These accessors let the renderer reproduce that distinction.
    bool persistentBacking() const { return persistentBacking_; }
    bool transparencyEnabled() const { return transparencyEnabled_; }
    std::uint8_t transparentIndex() const { return transparentIndex_; }
    int currentLeft() const { return currentLeft_; }
    int currentTop() const { return currentTop_; }
    int currentRight() const { return currentRight_; }
    int currentBottom() const { return currentBottom_; }

    // The stream's own palette: 256 RGBQUAD entries (blue, green, red, pad).
    const std::vector<std::uint8_t>& palette() const { return palette_; }

    // Concatenated PCM payload of the audio records, and its format.
    const std::vector<std::uint8_t>& audioPcm() const { return audioPcm_; }
    std::uint32_t audioSampleRate() const { return audioSampleRate_; }
    std::uint32_t audioBitsPerSample() const { return audioBitsPerSample_; }
    std::uint32_t audioChannels() const { return audioChannels_; }

private:
    struct VideoRecord {
        std::size_t payloadOffset = 0;   // into records_
        std::uint32_t payloadBytes = 0;
        int left = 0, top = 0, right = 0, bottom = 0;
        bool unchanged = false;
    };

    void applyRecord(const VideoRecord& rec);
    int width_ = 0, height_ = 0, frameCount_ = 0;
    std::uint8_t transparentIndex_ = 0;
    bool persistentBacking_ = false;
    bool transparencyEnabled_ = false;
    std::uint32_t audioSampleRate_ = 0, audioBitsPerSample_ = 0, audioChannels_ = 0;
    std::vector<std::uint8_t> records_;       // raw chunk-record bytes
    std::vector<VideoRecord> videoRecords_;   // one per frame, in frame order
    std::vector<std::uint8_t> palette_;
    std::vector<std::uint8_t> audioPcm_;

    std::vector<std::uint8_t> frame_;         // width*height top-down, current state
    int decodedFrame_ = -1;                   // index frame_ holds, -1 = none
    int currentLeft_ = 0, currentTop_ = 0, currentRight_ = 0, currentBottom_ = 0;
};

} // namespace graphboard
