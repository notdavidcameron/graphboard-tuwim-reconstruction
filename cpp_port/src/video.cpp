#include "graphboard/video.hpp"

#include <algorithm>
#include <cstring>
#include <limits>

namespace graphboard {

namespace {

constexpr std::uint32_t kVideoRecordTag = 0xff000001u;
constexpr std::uint32_t kAudioRecordTag = 0xff000002u;
constexpr std::size_t kBoardVideoHeaderBytes = 0x4e8;
constexpr std::size_t kChunkRecordHeaderBytes = 0x48;

std::uint32_t readU32(const std::vector<std::uint8_t>& b, std::size_t off) {
    if (off + 4 > b.size()) {
        return 0;
    }
    return static_cast<std::uint32_t>(b[off]) | (static_cast<std::uint32_t>(b[off + 1]) << 8) |
           (static_cast<std::uint32_t>(b[off + 2]) << 16) |
           (static_cast<std::uint32_t>(b[off + 3]) << 24);
}

} // namespace

BoardVideoDecoder::BoardVideoDecoder(const std::vector<std::uint8_t>& fileBytes,
                                     const runtime::VideoGeometry& geom) {
    width_ = geom.width;
    height_ = geom.height;
    transparentIndex_ = geom.streamTransparentIndex;
    const int declaredFrameCount = geom.frameCount;
    frameCount_ = 0;
    const auto width = static_cast<std::size_t>(width_ > 0 ? width_ : 0);
    const auto height = static_cast<std::size_t>(height_ > 0 ? height_ : 0);
    if (width_ <= 0 || height_ <= 0 || declaredFrameCount <= 0 ||
        width > (std::numeric_limits<std::size_t>::max)() / height ||
        geom.streamByteCount < kBoardVideoHeaderBytes ||
        geom.streamOffset > fileBytes.size() ||
        geom.streamByteCount > fileBytes.size() - geom.streamOffset) {
        return;
    }

    const std::size_t base = geom.streamOffset;
    audioSampleRate_ = readU32(fileBytes, base + 0x90);
    audioBitsPerSample_ = readU32(fileBytes, base + 0x94);
    audioChannels_ = readU32(fileBytes, base + 0x98);
    palette_.assign(fileBytes.begin() + base + 0xe8, fileBytes.begin() + base + 0x4e8);

    // Keep the chunk records only; offsets below are into this copy.
    records_.assign(fileBytes.begin() + base + kBoardVideoHeaderBytes,
                    fileBytes.begin() + base + geom.streamByteCount);

    std::size_t pos = 0;
    while (pos + kChunkRecordHeaderBytes <= records_.size()) {
        const std::uint32_t byteSize = readU32(records_, pos);
        if (byteSize < kChunkRecordHeaderBytes || byteSize > records_.size() - pos) {
            break;
        }
        const std::uint32_t tag = readU32(records_, pos + 0x04);
        if (tag == kVideoRecordTag) {
            VideoRecord rec;
            rec.payloadOffset = pos + kChunkRecordHeaderBytes;
            rec.payloadBytes = byteSize - kChunkRecordHeaderBytes;
            rec.left = static_cast<std::int32_t>(readU32(records_, pos + 0x14));
            rec.top = static_cast<std::int32_t>(readU32(records_, pos + 0x18));
            rec.right = static_cast<std::int32_t>(readU32(records_, pos + 0x1c));
            rec.bottom = static_cast<std::int32_t>(readU32(records_, pos + 0x20));
            rec.unchanged = readU32(records_, pos + 0x24) != 0;
            videoRecords_.push_back(rec);
        } else if (tag == kAudioRecordTag) {
            audioPcm_.insert(audioPcm_.end(),
                             records_.begin() + pos + kChunkRecordHeaderBytes,
                             records_.begin() + pos + byteSize);
        } else {
            break;
        }
        pos += byteSize;
    }

    frameCount_ = std::min<int>(declaredFrameCount, static_cast<int>(videoRecords_.size()));
    if (frameCount_ <= 0) return;
    frame_.assign(width * height, transparentIndex_);
}

// Decode one RLE rect into frame_ (top-down here; the original writes into a
// bottom-up DIB, which is the same walk mirrored). Ops: 0x00 = short run
// (u8 count, u8 value), 0xff = long run (u16 count, u8 value), else = literal
// (`op` raw bytes). Recovered from TVH_DrawRleRect (1000b8a0).
void BoardVideoDecoder::applyRecord(const VideoRecord& rec) {
    if (rec.unchanged || rec.payloadBytes == 0) {
        return;
    }
    const std::int64_t rectW64 = static_cast<std::int64_t>(rec.right) - rec.left;
    const std::int64_t rectH64 = static_cast<std::int64_t>(rec.bottom) - rec.top;
    if (rectW64 <= 0 || rectH64 <= 0 ||
        rectW64 > (std::numeric_limits<std::int64_t>::max)() / rectH64) {
        return;
    }
    const int rectW = static_cast<int>(std::min<std::int64_t>(rectW64, (std::numeric_limits<int>::max)()));
    const std::uint8_t* rle = records_.data() + rec.payloadOffset;
    const std::uint8_t* end = rle + rec.payloadBytes;

    std::int64_t x = rec.left, y = rec.top;
    auto put = [&](std::uint8_t value) {
        if (x >= 0 && x < width_ && y >= 0 && y < height_) {
            frame_[static_cast<std::size_t>(y) * static_cast<std::size_t>(width_) +
                   static_cast<std::size_t>(x)] = value;
        }
        if (++x - rec.left >= rectW) {
            x = rec.left;
            ++y;
        }
    };

    std::int64_t remaining = rectW64 * rectH64;
    while (remaining > 0 && rle < end) {
        const std::uint8_t op = *rle++;
        if (op == 0x00) {
            if (rle + 2 > end) break;
            std::uint32_t count = *rle++;
            const std::uint8_t value = *rle++;
            for (; count > 0 && remaining > 0; --count, --remaining) put(value);
        } else if (op == 0xff) {
            if (rle + 3 > end) break;
            std::uint32_t count = static_cast<std::uint32_t>(rle[0]) | (rle[1] << 8);
            rle += 2;
            const std::uint8_t value = *rle++;
            for (; count > 0 && remaining > 0; --count, --remaining) put(value);
        } else {
            std::uint32_t count = op;
            for (; count > 0 && remaining > 0 && rle < end; --count, --remaining) put(*rle++);
        }
    }
}

const std::vector<std::uint8_t>& BoardVideoDecoder::frameAt(int index) {
    if (frame_.empty() || videoRecords_.empty()) {
        return frame_;
    }
    index = std::clamp(index, 0, static_cast<int>(videoRecords_.size()) - 1);
    if (index < decodedFrame_) {
        std::fill(frame_.begin(), frame_.end(), transparentIndex_);
        decodedFrame_ = -1;
    }
    while (decodedFrame_ < index) {
        ++decodedFrame_;
        const auto& record = videoRecords_[static_cast<std::size_t>(decodedFrame_)];
        applyRecord(record);
    }
    return frame_;
}

} // namespace graphboard
