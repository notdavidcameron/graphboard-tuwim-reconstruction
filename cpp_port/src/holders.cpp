#include "graphboard/holders.hpp"

#include <algorithm>
#include <array>
#include <iterator>

namespace graphboard {

namespace {

const std::vector<HolderInfo>& registry() {
    // Built once. CLSIDs and display names are the wrapper values observed in
    // RZECZKA.BDF; dll/anchor are cited only where verified in the disassembly.
    static const std::vector<HolderInfo> table = [] {
        std::vector<HolderInfo> t;
        t.push_back({HolderKind::HotSpotHolder,
                     Guid::fromString("DA768116-5341-11D0-B444-008048EB5D40"),
                     "HotSpot_Holder", "HotSpotHolder.dll",
                     "HotSpotHolder.dll:10004d10", true});
        t.push_back({HolderKind::TransparentVideoHolder,
                     Guid::fromString("2F3C5846-5413-11D0-B447-008048EB5D40"),
                     "Transparent_Video_Holder", "TransparentVideoHolder.dll",
                     "TransparentVideoHolder.dll:10007af0", true});
        t.push_back({HolderKind::SpriteHolder,
                     Guid::fromString("B64F3336-5368-11D0-B445-008048EB5D40"),
                     "Sprite_Holder", "SpriteHolder.dll",
                     "SpriteHolder.dll:10008930", true});
        t.push_back({HolderKind::TextHolder,
                     Guid::fromString("AD45ADB6-5290-11D0-B442-008048EB5D40"),
                     "Text_Holder", "TextHolder.dll",
                     "TextHolder.dll:10008200", true});
        t.push_back({HolderKind::SoundHolder,
                     Guid::fromString("1720D306-6932-11D0-B468-008048EB5D40"),
                     "Sound_Holder", "Sound.dll",
                     "Sound.dll:10004090", true});
        t.push_back({HolderKind::MultiBitmap,
                     Guid::fromString("F7794041-1EB4-11D1-9B25-008048EB5D40"),
                     "MultiBitmap", "MultiBmp.dll",
                     "MultiBmp.dll:10004353", true});
        // Recovered by walking the full Tuwim DATA folder; serializer anchors
        // confirmed against the disassembly by the Ghidra rename pass.
        t.push_back({HolderKind::BitmapHolder,
                     Guid::fromString("0D8A5736-5337-11D0-B444-008048EB5D40"),
                     "Bitmap_Holder", "BitmapHolder.dll",
                     "BitmapHolder.dll:10003d20", true});
        t.push_back({HolderKind::VideoHolder,
                     Guid::fromString("B2CDC8DE-5359-11D0-B445-008048EB5D40"),
                     "Video_Holder", "VideoHolder.dll",
                     "VideoHolder.dll:10003e20", true});
        t.push_back({HolderKind::PanoramaHolder,
                     Guid::fromString("B2CDC8D6-5359-11D0-B445-008048EB5D40"),
                     "Panorama_Holder", "PanoramaHolder.dll",
                     "PanoramaHolder.dll:10005e20", true});
        t.push_back({HolderKind::Panorama,
                     Guid::fromString("8B446B11-0746-11D1-9B09-008048EB5D40"),
                     "Panorama", "Panorama.dll",
                     "Panorama.dll:10004b6a", true});
        t.push_back({HolderKind::Puzzle,
                     Guid::fromString("196E7596-AB2B-11D0-B2A5-008048EB5D40"),
                     "Puzzle", "Puzzle.dll",
                     "Puzzle.dll:10003470", true});
        t.push_back({HolderKind::Recorder,
                     Guid::fromString("2B61D676-9627-11D0-B280-008048EB5D40"),
                     "Recorder", "Recorder.dll", "", true});
        return t;
    }();
    return table;
}

std::int32_t readI32(const std::vector<std::uint8_t>& buffer, std::size_t offset) {
    return static_cast<std::int32_t>(
        static_cast<std::uint32_t>(buffer[offset]) |
        (static_cast<std::uint32_t>(buffer[offset + 1]) << 8) |
        (static_cast<std::uint32_t>(buffer[offset + 2]) << 16) |
        (static_cast<std::uint32_t>(buffer[offset + 3]) << 24));
}

// Field offsets inside the 100-byte CHotSpot record (HotSpotHolder.dll).
constexpr std::size_t kHotSpotRecordBytes = 100;
constexpr std::size_t kOffLeft = 0x00;
constexpr std::size_t kOffTop = 0x04;
constexpr std::size_t kOffRight = 0x08;
constexpr std::size_t kOffBottom = 0x0c;
// The script-visible hotspot id ("rectID"), NOT the array index: the engine
// passes record+0x18 to LeftButtonClickOn and matches EnableHotSpot(id) against
// it (HotSpotHolder.dll LButtonDown @ 100050a0, IsYou @ 100047c0). RZECZKA.BDF
// stores ids 0,1,3,4,6,5,7,8 for array indices 0..7.
constexpr std::size_t kOffHotSpotId = 0x18;
constexpr std::size_t kOffLayer = 0x1c;
constexpr std::size_t kOffEnabled = 0x20;

} // namespace

const HolderInfo* lookupHolder(const Guid& clsid) {
    for (const auto& info : registry()) {
        if (info.clsid == clsid) {
            return &info;
        }
    }
    return nullptr;
}

const std::vector<HolderInfo>& knownHolders() {
    return registry();
}

HotSpotHolderState parseHotSpotHolderState(BinaryReader& reader) {
    HotSpotHolderState state;
    state.version = reader.readU32();
    state.flag0 = reader.readU8();
    state.flag1 = reader.readU8();
    state.flag2 = reader.readU8();

    const auto hotspotCount = reader.readU32();
    state.hotspots.reserve(hotspotCount);
    for (std::uint32_t i = 0; i < hotspotCount; ++i) {
        const auto record = reader.readBytes(kHotSpotRecordBytes);
        HotSpot hotspot;
        hotspot.left = readI32(record, kOffLeft);
        hotspot.top = readI32(record, kOffTop);
        hotspot.right = readI32(record, kOffRight);
        hotspot.bottom = readI32(record, kOffBottom);
        hotspot.id = readI32(record, kOffHotSpotId);
        hotspot.layer = readI32(record, kOffLayer);
        hotspot.enabled = readI32(record, kOffEnabled);
        hotspot.name = reader.readArchiveString();
        state.hotspots.push_back(std::move(hotspot));
    }

    state.activeIndex = reader.readU32();
    state.auxStateWord = reader.readU32();
    return state;
}

namespace {

std::uint32_t readU32At(const std::vector<std::uint8_t>& buffer, std::size_t offset) {
    return static_cast<std::uint32_t>(buffer[offset]) |
           (static_cast<std::uint32_t>(buffer[offset + 1]) << 8) |
           (static_cast<std::uint32_t>(buffer[offset + 2]) << 16) |
           (static_cast<std::uint32_t>(buffer[offset + 3]) << 24);
}

std::string readNulPaddedName(const std::vector<std::uint8_t>& buffer, std::size_t offset,
                              std::size_t maxBytes) {
    std::string name;
    for (std::size_t i = 0; i < maxBytes && offset + i < buffer.size(); ++i) {
        const char c = static_cast<char>(buffer[offset + i]);
        if (c == '\0') break;
        name.push_back(c);
    }
    return name;
}

// Serialized CSpriteParam instance record size (SpriteHolder.dll).
constexpr std::size_t kSpriteInstanceBytes = 0x8c;
constexpr std::size_t kSpriteFrameTableOffset = 0x6c;  // within the definition blob
constexpr std::size_t kSpriteFrameBytes = 0x4c;
constexpr std::size_t kSpriteBlobPixelBase = 0xb8;     // frame[0x48] is relative to this
// Serialized MultiBitmap per-record metadata size.
constexpr std::size_t kMultiBitmapMetadataBytes = 0xc0;
// Serialized TVH holder-entry size and board-video header size.
constexpr std::size_t kTvhEntryBytes = 0x568;
constexpr std::size_t kBoardVideoHeaderBytes = 0x4e8;

// 8-bpp bottom-up DIB row stride (TransparentVideoHolder still frames).
std::uint32_t dibStride8(std::uint32_t width) {
    return ((width * 8 + 31) >> 3) & ~3u;
}

} // namespace

SpriteHolderState parseSpriteHolderState(BinaryReader& reader) {
    SpriteHolderState state;
    state.version = reader.readU32();
    const auto definitionCount = reader.readU32();
    const auto instanceCount = reader.readU32();

    state.definitions.reserve(definitionCount);
    for (std::uint32_t i = 0; i < definitionCount; ++i) {
        SpriteDefinition definition;
        definition.blobByteCount = reader.readU32();
        definition.blobOffset = reader.position();
        const auto blob = reader.readBytes(definition.blobByteCount);
        if (blob.size() >= 0x88) {
            definition.name = readNulPaddedName(blob, 0x04, 0x20);
            definition.phaseCount = readU32At(blob, 0x00);
            definition.width = readU32At(blob, 0x80);
            definition.height = readU32At(blob, 0x84);

            // Frame table: blob+0x6c, stride 0x4c, width +0x14 / height +0x18.
            // Trust phaseCount only as far as the blob actually reaches; the
            // bytes past the last frame are pixel data, not frame records.
            //
            // The per-pixel transparency test runs only when the definition
            // (def+0x24) and the frame (frame+0x08) both opt in; then the mask
            // is precomputed from the frame's pixels. Pixels for a phase start
            // at blob+0xb8+frame[0x48], laid out bottom-up with byte pitch
            // ((frame[0x10]+3)&~3), one index byte per pixel; a byte equal to
            // frame[0x04] is transparent. (instance+0x54 would add a horizontal
            // source offset, but it is 0 across every shipped sprite, so the
            // mask is definition-level.)
            const bool defWantsPixelTest = readU32At(blob, 0x24) != 0;
            for (std::uint32_t phase = 0; phase < definition.phaseCount; ++phase) {
                const std::size_t frame = kSpriteFrameTableOffset + phase * kSpriteFrameBytes;
                if (frame + 0x1c > blob.size()) {
                    break;
                }
                SpriteFrame f;
                f.width = readU32At(blob, frame + 0x14);
                f.height = readU32At(blob, frame + 0x18);
                const auto storedDuration = readU32At(blob, frame + 0x20);

                const bool frameFits = frame + kSpriteFrameBytes <= blob.size();
                if (frameFits && f.width > 0 && f.height > 0) {
                    f.transparentIndex = static_cast<std::uint8_t>(readU32At(blob, frame + 0x04));
                    const std::size_t rowWidth = readU32At(blob, frame + 0x10);
                    const auto storedCells = static_cast<std::size_t>(readU32At(blob, frame));
                    const std::size_t cellCount = storedCells > 0 && storedCells <= 64 &&
                        rowWidth >= static_cast<std::size_t>(f.width) * storedCells
                            ? storedCells : 1;
                    // The serialized time belongs to the complete horizontal
                    // animation strip. Dividing it between its cells matches
                    // the holder timer; treating it as a per-cell delay makes
                    // PLOTKI's three/four-pose reactions take several seconds.
                    if (storedDuration > 0 && storedDuration <= 5000) {
                        // Short values are the repeating cell interval (CUDA's
                        // wings: roughly 70 ms). Long values on click-reaction
                        // strips describe the whole gesture (PLOTKI: 500 ms).
                        f.cellDurationMs = storedDuration >= 250 && cellCount > 1
                            ? std::max<std::uint32_t>(1, storedDuration /
                                  static_cast<std::uint32_t>(cellCount))
                            : storedDuration;
                    } else {
                        f.cellDurationMs = 100;
                    }
                    const std::size_t stride = (rowWidth + 3) & ~std::size_t{3};
                    const std::size_t pixels = kSpriteBlobPixelBase + readU32At(blob, frame + 0x48);
                    if (rowWidth >= static_cast<std::size_t>(f.width) * cellCount &&
                        stride >= rowWidth && pixels + stride * f.height <= blob.size()) {
                        // Store the frame's pixels top-down for rendering; build
                        // the hit-test opacity mask from them when the def+frame
                        // both opt into the per-pixel test.
                        const bool wantsMask =
                            defWantsPixelTest && readU32At(blob, frame + 0x08) != 0;
                        f.animationPixels.resize(cellCount);
                        if (wantsMask) f.animationOpaque.resize(cellCount);
                        for (std::size_t cell = 0; cell < cellCount; ++cell) {
                            auto& cellPixels = f.animationPixels[cell];
                            cellPixels.assign(static_cast<std::size_t>(f.width) * f.height, 0);
                            auto* cellMask = wantsMask ? &f.animationOpaque[cell] : nullptr;
                            if (cellMask) cellMask->assign(cellPixels.size(), 0);
                            for (std::uint32_t row = 0; row < f.height; ++row) {
                                const std::size_t src = pixels + (f.height - 1 - row) * stride +
                                                        cell * f.width;
                                for (std::uint32_t col = 0; col < f.width; ++col) {
                                    const std::uint8_t px = blob[src + col];
                                    cellPixels[row * f.width + col] = px;
                                    if (cellMask) {
                                        (*cellMask)[row * f.width + col] =
                                            px != f.transparentIndex ? 1 : 0;
                                    }
                                }
                            }
                        }
                        f.pixels = f.animationPixels.front();
                        if (wantsMask) f.opaque = f.animationOpaque.front();
                    }
                }
                definition.frames.push_back(std::move(f));
            }


        }
        state.definitions.push_back(std::move(definition));
    }

    state.instances.reserve(instanceCount);
    for (std::uint32_t i = 0; i < instanceCount; ++i) {
        const auto record = reader.readBytes(kSpriteInstanceBytes);
        SpriteInstance instance;
        instance.definitionIndex = readU32At(record, 0x00);
        instance.field04 = readU32At(record, 0x04);
        instance.posX = static_cast<std::int32_t>(readU32At(record, 0x08));
        instance.posY = static_cast<std::int32_t>(readU32At(record, 0x0c));
        instance.layer = static_cast<std::int32_t>(readU32At(record, 0x18));
        instance.phase = static_cast<std::int32_t>(readU32At(record, 0x5c));
        instance.visible = static_cast<std::int32_t>(readU32At(record, 0x88));
        instance.dragEnabled = readU32At(record, 0x1c) == 1;
        instance.unknown38 = static_cast<std::int32_t>(readU32At(record, 0x38));
        state.instances.push_back(instance);
    }

    return state;
}

MultiBitmapState parseMultiBitmapState(BinaryReader& reader) {
    MultiBitmapState state;
    state.version = reader.readU32();
    const auto recordCount = reader.readU32();

    state.records.reserve(recordCount);
    for (std::uint32_t i = 0; i < recordCount; ++i) {
        MultiBitmapRecord record;
        record.pixelByteCount = reader.readU32();
        record.pixelOffset = reader.position();
        reader.skip(record.pixelByteCount);

        const auto metadata = reader.readBytes(kMultiBitmapMetadataBytes);
        record.left = static_cast<std::int32_t>(readU32At(metadata, 0x08));
        record.top = static_cast<std::int32_t>(readU32At(metadata, 0x0c));
        record.right = static_cast<std::int32_t>(readU32At(metadata, 0x10));
        record.bottom = static_cast<std::int32_t>(readU32At(metadata, 0x14));
        record.layer = static_cast<std::int32_t>(readU32At(metadata, 0x18));
        record.shown = static_cast<std::int32_t>(readU32At(metadata, 0x3c));
        record.width = record.right > record.left
                           ? static_cast<std::uint32_t>(record.right - record.left) : 0;
        record.height = record.bottom > record.top
                            ? static_cast<std::uint32_t>(record.bottom - record.top) : 0;
        record.name = readNulPaddedName(metadata, 0x28, 12);
        record.pixelsAreRawIndexed =
            record.width != 0 && record.height != 0 &&
            static_cast<std::uint64_t>((record.width + 3) & ~std::uint32_t{3}) *
                    record.height == record.pixelByteCount;
        state.records.push_back(std::move(record));
    }

    return state;
}

TransparentVideoHolderState parseTransparentVideoHolderState(BinaryReader& reader) {
    TransparentVideoHolderState state;
    state.version = reader.readU32();
    const auto entryCount = reader.readU32();

    state.entries.reserve(entryCount);
    for (std::uint32_t i = 0; i < entryCount; ++i) {
        TransparentVideoEntry entry;
        entry.stream.streamByteCount = reader.readU32();
        entry.stream.streamOffset = reader.position();

        // Peek the fixed board-video header at the stream start, then skip the
        // chunk records; walking the block does not require decoding them.
        if (entry.stream.streamByteCount < kBoardVideoHeaderBytes) {
            throw ParseError("Board-video stream shorter than its fixed header");
        }
        const auto header = reader.readBytes(kBoardVideoHeaderBytes);
        entry.stream.magic = readU32At(header, 0x68);
        entry.stream.transparentIndex = readU32At(header, 0x74);
        entry.stream.frameDurationMs = readU32At(header, 0x7c);
        entry.stream.width = static_cast<std::int32_t>(readU32At(header, 0x80));
        entry.stream.height = static_cast<std::int32_t>(readU32At(header, 0x84));
        entry.stream.chunkRecordCount = readU32At(header, 0x88);
        entry.stream.frameCount = readU32At(header, 0x8c);
        entry.stream.audioSampleRate = readU32At(header, 0x90);
        reader.skip(entry.stream.streamByteCount - kBoardVideoHeaderBytes);

        const auto entryBytes = reader.readBytes(kTvhEntryBytes);
        entry.originalX = static_cast<std::int32_t>(readU32At(entryBytes, 0x508));
        entry.originalY = static_cast<std::int32_t>(readU32At(entryBytes, 0x50c));
        entry.originalZ = static_cast<std::int32_t>(readU32At(entryBytes, 0x510));
        entry.stageX = static_cast<std::int32_t>(readU32At(entryBytes, 0x54c));
        entry.stageY = static_cast<std::int32_t>(readU32At(entryBytes, 0x550));
        entry.stageZ = static_cast<std::int32_t>(readU32At(entryBytes, 0x554));
        entry.showStillAtRest = readU32At(entryBytes, 0x504) != 0;

        // The still frame is sized from the ENTRY's header copy, exactly as
        // TVH_LoadPrivateState computes it.
        const auto entryWidth = readU32At(entryBytes, 0x80);
        const auto entryHeight = readU32At(entryBytes, 0x84);
        entry.stillFrameByteCount = dibStride8(entryWidth) * entryHeight;
        entry.stillFrameOffset = reader.position();
        // Persisted stills are stage snapshots and can use a different matte
        // from the BoardVideo stream. Recover the key from the complete image
        // perimeter: artwork may cover one or more corners, while the matte is
        // normally present along most of the serialized boundary. Looking at
        // the perimeter (rather than the whole rectangle) also avoids choosing
        // a large interior object such as FIGIELEK's curtain or a stamp.
        entry.transparentIndex = static_cast<std::uint8_t>(entry.stream.transparentIndex);
        const auto& file = reader.bytes();
        const auto stride = dibStride8(entryWidth);
        if (entryWidth != 0 && entryHeight != 0 &&
            entry.stillFrameOffset + entry.stillFrameByteCount <= file.size()) {
            std::array<std::size_t, 256> counts{};
            const auto pixel = [&](std::uint32_t x, std::uint32_t y) {
                return file[entry.stillFrameOffset + static_cast<std::size_t>(y) * stride + x];
            };
            for (std::uint32_t x = 0; x < entryWidth; ++x) {
                ++counts[pixel(x, 0)];
                if (entryHeight > 1) ++counts[pixel(x, entryHeight - 1)];
            }
            for (std::uint32_t y = 1; y + 1 < entryHeight; ++y) {
                ++counts[pixel(0, y)];
                if (entryWidth > 1) ++counts[pixel(entryWidth - 1, y)];
            }
            entry.transparentIndex = static_cast<std::uint8_t>(
                std::distance(counts.begin(), std::max_element(counts.begin(), counts.end())));
        }
        reader.skip(entry.stillFrameByteCount);

        state.entries.push_back(entry);
    }

    return state;
}

// Duration of a RIFF/WAVE clip in ms: walk the chunks for the fmt byte-rate and
// the data chunk size (duration = dataBytes * 1000 / byteRate). Returns 0 for
// anything that is not a canonical RIFF/WAVE.
static std::uint32_t wavDurationMs(const std::vector<std::uint8_t>& wav) {
    if (wav.size() < 12 || wav[0] != 'R' || wav[1] != 'I' || wav[2] != 'F' ||
        wav[3] != 'F' || wav[8] != 'W' || wav[9] != 'A' || wav[10] != 'V' || wav[11] != 'E') {
        return 0;
    }
    std::uint32_t byteRate = 0, dataBytes = 0;
    std::size_t pos = 12;
    while (pos + 8 <= wav.size()) {
        const std::size_t chunkSize = readU32At(wav, pos + 4);
        if (wav[pos] == 'f' && wav[pos + 1] == 'm' && wav[pos + 2] == 't' && wav[pos + 3] == ' ' &&
            pos + 8 + 16 <= wav.size()) {
            byteRate = readU32At(wav, pos + 8 + 8);   // fmt: +8 into the chunk data
        } else if (wav[pos] == 'd' && wav[pos + 1] == 'a' && wav[pos + 2] == 't' &&
                   wav[pos + 3] == 'a') {
            dataBytes = static_cast<std::uint32_t>(chunkSize);
        }
        pos += 8 + chunkSize + (chunkSize & 1);   // chunks are word-aligned
    }
    if (byteRate == 0) {
        return 0;
    }
    return static_cast<std::uint32_t>(static_cast<std::uint64_t>(dataBytes) * 1000 / byteRate);
}

SoundHolderState parseSoundHolderState(BinaryReader& reader) {
    // 0x58-byte record framing per the shipped 1997 DLL; the GraphBoard 1.00
    // DLL writes 0x6c-byte records (see holders.hpp / recovery notes).
    constexpr std::size_t kSoundRecordBytes = 0x58;

    SoundHolderState state;
    state.version = reader.readU32();
    const auto soundCount = reader.readU32();

    state.sounds.reserve(soundCount);
    for (std::uint32_t i = 0; i < soundCount; ++i) {
        SoundEntry sound;
        sound.soundByteCount = reader.readU32();
        sound.soundOffset = reader.position();
        if (sound.soundByteCount < 4) {
            throw ParseError("Sound entry shorter than a RIFF tag");
        }
        const auto soundBytes = reader.readBytes(sound.soundByteCount);
        sound.looksLikeRiff = soundBytes[0] == 'R' && soundBytes[1] == 'I' &&
                              soundBytes[2] == 'F' && soundBytes[3] == 'F';
        sound.durationMs = wavDurationMs(soundBytes);

        const auto record = reader.readBytes(kSoundRecordBytes);
        sound.looping = readU32At(record, 0x00) != 0;
        sound.archiveStart = readU32At(record, 0x04);
        sound.archiveEnd = readU32At(record, 0x08);
        sound.name = readNulPaddedName(record, 0x0c, 0x20);
        sound.waveFormatByteCount = reader.readU32();
        reader.skip(sound.waveFormatByteCount);
        state.sounds.push_back(std::move(sound));
    }

    return state;
}

TextHolderState parseTextHolderState(BinaryReader& reader) {
    constexpr std::size_t kTextEntryBytes = 0xc4;
    constexpr std::size_t kRenderCacheBytes = 0x68;
    constexpr std::size_t kFontSlotBytes = 0xc84;
    constexpr std::uint32_t kFontSlotCount = 50;
    constexpr std::size_t kGlyphCount = 256;

    TextHolderState state;
    state.version = reader.readU32();
    const auto textCount = reader.readU32();
    for (auto& color : state.colors) {
        color = reader.readU8();
    }

    state.entries.reserve(textCount);
    for (std::uint32_t i = 0; i < textCount; ++i) {
        const auto record = reader.readBytes(kTextEntryBytes);
        TextEntry entry;
        entry.left = static_cast<std::int32_t>(readU32At(record, 0x28));
        entry.top = static_cast<std::int32_t>(readU32At(record, 0x2c));
        entry.right = static_cast<std::int32_t>(readU32At(record, 0x30));
        entry.bottom = static_cast<std::int32_t>(readU32At(record, 0x34));
        entry.layer = static_cast<std::int32_t>(readU32At(record, 0x38));
        entry.hasRenderCache = readU32At(record, 0x70) != 0;
        entry.hasSecondaryText = readU32At(record, 0x90) != 0;
        // record+0x48 is the initial visibility flag: the poem body and title
        // carry 1 (shown on open) while the plain karaoke-overlay duplicate of
        // the same rect carries 0 (revealed only via ShowText during playback).
        entry.initiallyVisible = readU32At(record, 0x48) != 0;

        if (entry.hasRenderCache) {
            if (!entry.hasSecondaryText) {
                // Branch A: embedded render-object stream, then the cache record.
                entry.streamByteCount = reader.readU32();
                entry.streamOffset = reader.position();
                reader.skip(entry.streamByteCount);
            }
            const auto cache = reader.readBytes(kRenderCacheBytes);
            const auto lineOffsetsPtr = readU32At(cache, 0x0c);
            entry.lineCount = readU32At(cache, 0x10);
            if (lineOffsetsPtr != 0 && entry.lineCount != 0) {
                entry.lineOffsets.reserve(entry.lineCount);
                for (std::uint32_t line = 0; line < entry.lineCount; ++line) {
                    entry.lineOffsets.push_back(reader.readU32());
                }
            }
            if (entry.hasSecondaryText) {
                entry.secondaryText = reader.readArchiveString();
            }
        }
        entry.primaryText = reader.readArchiveString();
        state.entries.push_back(std::move(entry));
    }

    // FontControl block: always appended by TextHolder_SerializeDocument.
    for (std::uint32_t slot = 0; slot < kFontSlotCount; ++slot) {
        const auto present = reader.readU32();
        if (present == 0) {
            continue;
        }
        const auto slotRecord = reader.readBytes(kFontSlotBytes);
        FontSlot font;
        font.height = readU32At(slotRecord, 0xc00);
        for (std::size_t g = 0; g < kGlyphCount; ++g) {
            if (readU32At(slotRecord, g * 4) != 0) {
                const auto width = static_cast<std::uint32_t>(slotRecord[0x800 + g * 4]) |
                                   (static_cast<std::uint32_t>(slotRecord[0x801 + g * 4]) << 8);
                reader.skip(((width + 3) & ~3u) * font.height);
                ++font.glyphCount;
            }
        }
        state.fontSlots.push_back(font);
    }

    return state;
}

BitmapHolderState parseBitmapHolderState(BinaryReader& reader) {
    // Pixels begin at blob+0x80 (the base BitmapHolder_LButtonDown and the blit
    // path both index), followed by stride*height bytes and 0x10 trailing bytes,
    // so blobByteCount == 0x90 + stride*height. Reading them at +0x90 -- as an
    // earlier note assumed -- shears every row by 16 bytes (confirmed: the guzik
    // button's left/right symmetry drops from 97% to 55%).
    constexpr std::size_t kBitmapPixelBase = 0x80;
    constexpr std::size_t kBitmapTrailerBytes = 0x10;
    constexpr std::size_t kBitmapHeaderBytes = kBitmapPixelBase + kBitmapTrailerBytes;

    BitmapHolderState state;
    state.version = reader.readU32();
    const auto bitmapCount = reader.readU32();

    state.bitmaps.reserve(bitmapCount);
    for (std::uint32_t i = 0; i < bitmapCount; ++i) {
        BitmapHolderBitmap bitmap;
        bitmap.blobByteCount = reader.readU32();
        bitmap.blobOffset = reader.position();
        const auto blob = reader.readBytes(bitmap.blobByteCount);
        if (blob.size() >= kBitmapHeaderBytes) {
            bitmap.left = static_cast<std::int32_t>(readU32At(blob, 0x08));
            bitmap.top = static_cast<std::int32_t>(readU32At(blob, 0x0c));
            bitmap.right = static_cast<std::int32_t>(readU32At(blob, 0x10));
            bitmap.bottom = static_cast<std::int32_t>(readU32At(blob, 0x14));
            bitmap.layer = static_cast<std::int32_t>(readU32At(blob, 0x18));
            bitmap.name = readNulPaddedName(blob, 0x34, 12);
            bitmap.pixelOffset = bitmap.blobOffset + kBitmapPixelBase;
            const auto width = static_cast<std::uint32_t>(bitmap.right - bitmap.left);
            const auto height = static_cast<std::uint32_t>(bitmap.bottom - bitmap.top);
            const std::size_t stride = (width + 3) & ~std::size_t{3};
            bitmap.pixelSizeConsistent =
                bitmap.right > bitmap.left && bitmap.bottom > bitmap.top &&
                static_cast<std::uint64_t>(stride) * height ==
                    bitmap.blobByteCount - kBitmapHeaderBytes;

            // Store pixels top-down for rendering; build the hit-test opacity
            // mask from them when the record opts into the per-pixel test
            // (record+0x20 and +0x28 both != 0; BitmapHolder_LButtonDown @
            // 10003f50). Pixels are bottom-up from blob+0x80, transparent index
            // at blob+0x04.
            bitmap.transparentIndex = static_cast<std::uint8_t>(readU32At(blob, 0x04));
            bitmap.stateWord = readU32At(blob, 0x00);
            // blob+0x00 persists the shown state for placed overlays. Complete
            // page-art records commonly store zero despite being the backing
            // canvas, so the page compositor handles that structural case.
            // blob+0x2c is retained for further recovery but is not visibility.
            bitmap.initiallyHidden = readU32At(blob, 0x2c) != 0;
            const bool wantsPixelTest =
                readU32At(blob, 0x20) != 0 && readU32At(blob, 0x28) != 0;
            if (width > 0 && height > 0 &&
                kBitmapPixelBase + stride * height <= blob.size()) {
                bitmap.pixels.assign(static_cast<std::size_t>(width) * height, 0);
                if (wantsPixelTest) {
                    bitmap.opaque.assign(static_cast<std::size_t>(width) * height, 0);
                }
                for (std::uint32_t row = 0; row < height; ++row) {
                    const std::size_t src = kBitmapPixelBase + (height - 1 - row) * stride;
                    for (std::uint32_t col = 0; col < width; ++col) {
                        const std::uint8_t px = blob[src + col];
                        bitmap.pixels[row * width + col] = px;
                        if (wantsPixelTest) {
                            bitmap.opaque[row * width + col] =
                                px != bitmap.transparentIndex ? 1 : 0;
                        }
                    }
                }
            }
        }
        state.bitmaps.push_back(std::move(bitmap));
    }

    return state;
}

PuzzleState parsePuzzleState(BinaryReader& reader) {
    constexpr std::size_t kBoardBytes = 0x60;
    constexpr std::size_t kChipBytes = 0x48;
    constexpr std::size_t kSubRecordBytes = 0x2c;

    PuzzleState state;
    state.version = reader.readU32();
    const auto boardCount = reader.readU32();

    state.boards.reserve(boardCount);
    for (std::uint32_t b = 0; b < boardCount; ++b) {
        const auto board = reader.readBytes(kBoardBytes);
        const auto chipCount = readU32At(board, 0x0c);

        PuzzleBoard puzzleBoard;
        puzzleBoard.chips.reserve(chipCount);
        for (std::uint32_t c = 0; c < chipCount; ++c) {
            const auto chip = reader.readBytes(kChipBytes);
            PuzzleChip puzzleChip;
            puzzleChip.left = static_cast<std::int32_t>(readU32At(chip, 0x00));
            puzzleChip.top = static_cast<std::int32_t>(readU32At(chip, 0x04));
            puzzleChip.right = static_cast<std::uint16_t>(
                static_cast<std::uint32_t>(chip[0x08]) |
                (static_cast<std::uint32_t>(chip[0x09]) << 8));
            puzzleChip.bottom = static_cast<std::int32_t>(readU32At(chip, 0x0c));
            puzzleChip.subRecordCount = readU32At(chip, 0x20);

            puzzleChip.pixelByteCount = reader.readU32();
            puzzleChip.pixelOffset = reader.position();
            reader.skip(puzzleChip.pixelByteCount);
            puzzleChip.connections.reserve(puzzleChip.subRecordCount);
            for (std::uint32_t sub = 0; sub < puzzleChip.subRecordCount; ++sub) {
                const auto connection = reader.readBytes(kSubRecordBytes);
                puzzleChip.connections.push_back({
                    static_cast<std::int32_t>(readU32At(connection, 0x00)),
                    static_cast<std::int32_t>(readU32At(connection, 0x04)),
                    static_cast<std::int32_t>(readU32At(connection, 0x08)),
                });
            }
            puzzleBoard.chips.push_back(puzzleChip);
        }
        state.boards.push_back(std::move(puzzleBoard));
    }

    return state;
}

RecorderState parseRecorderState(BinaryReader& reader) {
    constexpr std::size_t kRecorderRecordBytes = 0x68;
    RecorderState state;
    state.version = reader.readU32();
    state.recordOffset = reader.position();
    reader.skip(kRecorderRecordBytes);
    return state;
}

VideoHolderState parseVideoHolderState(BinaryReader& reader) {
    constexpr std::size_t kVideoRecordBytes = 0x6c;
    VideoHolderState state;
    state.version = reader.readU32();
    const auto entryCount = reader.readU32();

    state.entries.reserve(entryCount);
    for (std::uint32_t i = 0; i < entryCount; ++i) {
        VideoEntry entry;
        entry.recordOffset = reader.position();
        const auto record = reader.readBytes(kVideoRecordBytes);
        entry.posX = static_cast<std::int32_t>(readU32At(record, 0x10));
        entry.posY = static_cast<std::int32_t>(readU32At(record, 0x14));
        entry.name = reader.readArchiveString();
        state.entries.push_back(std::move(entry));
    }

    return state;
}

PanoramaHolderState parsePanoramaHolderState(BinaryReader& reader) {
    constexpr std::size_t kSceneRecordBytes = 0x224;
    constexpr std::size_t kRegionRecordBytes = 0x48;

    PanoramaHolderState state;
    state.version = reader.readU32();
    const auto sceneCount = reader.readU32();

    state.scenes.reserve(sceneCount);
    for (std::uint32_t i = 0; i < sceneCount; ++i) {
        PanoramaHolderScene scene;
        scene.recordOffset = reader.position();
        reader.skip(kSceneRecordBytes);

        scene.dibByteCount = reader.readU32();
        scene.dibOffset = reader.position();
        reader.skip(scene.dibByteCount);

        scene.subImageCount = reader.readU32();
        for (std::uint32_t s = 0; s < scene.subImageCount; ++s) {
            reader.skip(reader.readU32());
        }

        scene.regionCount = reader.readU32();
        reader.skip(static_cast<std::size_t>(scene.regionCount) * kRegionRecordBytes);

        state.scenes.push_back(scene);
    }

    return state;
}

PanoramaState parsePanoramaState(BinaryReader& reader) {
    constexpr std::size_t kSceneRecordBytes = 0x224;
    constexpr std::size_t kLayerRecordBytes = 0x78;
    constexpr std::size_t kRegionRecordBytes = 0x34;

    PanoramaState state;
    state.version = reader.readU32();
    const auto sceneCount = reader.readU32();

    state.scenes.reserve(sceneCount);
    for (std::uint32_t i = 0; i < sceneCount; ++i) {
        PanoramaScene scene;
        scene.recordOffset = reader.position();
        const auto record = reader.readBytes(kSceneRecordBytes);
        scene.width = readU32At(record, 0x04);
        scene.height = readU32At(record, 0x08);
        scene.subImageCount = readU32At(record, 0x150);
        scene.regionCount = readU32At(record, 0x21c);

        // Base image: width*height bytes, size derived from the record fields.
        scene.pixelOffset = reader.position();
        reader.skip(static_cast<std::uint64_t>(scene.width) * scene.height);

        for (std::uint32_t s = 0; s < scene.subImageCount; ++s) {
            const auto layer = reader.readBytes(kLayerRecordBytes);
            const auto lw = readU32At(layer, 0x24);
            const auto lh = readU32At(layer, 0x2c);
            reader.skip(static_cast<std::uint64_t>(lw) * lh);
        }

        reader.skip(static_cast<std::size_t>(scene.regionCount) * kRegionRecordBytes);
        state.scenes.push_back(scene);
    }

    return state;
}

} // namespace graphboard
