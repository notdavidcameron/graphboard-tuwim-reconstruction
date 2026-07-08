#include "graphboard/holders.hpp"

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
                     "Text_Holder", "TextHolder.dll", "", false});
        t.push_back({HolderKind::SoundHolder,
                     Guid::fromString("1720D306-6932-11D0-B468-008048EB5D40"),
                     "Sound_Holder", "", "", false});
        t.push_back({HolderKind::MultiBitmap,
                     Guid::fromString("F7794041-1EB4-11D1-9B25-008048EB5D40"),
                     "MultiBitmap", "MultiBmp.dll", "", true});
        // Observed while walking the full Tuwim DATA folder. Implementing DLLs
        // are not yet confirmed against the disassembly (candidates by name:
        // BitmapHolder.dll, VideoHolder.dll, PanoramaHolder.dll, Panorama.dll,
        // Puzzle.dll, Recorder.dll — all open in the Ghidra project).
        t.push_back({HolderKind::BitmapHolder,
                     Guid::fromString("0D8A5736-5337-11D0-B444-008048EB5D40"),
                     "Bitmap_Holder", "", "", false});
        t.push_back({HolderKind::VideoHolder,
                     Guid::fromString("B2CDC8DE-5359-11D0-B445-008048EB5D40"),
                     "Video_Holder", "", "", false});
        t.push_back({HolderKind::PanoramaHolder,
                     Guid::fromString("B2CDC8D6-5359-11D0-B445-008048EB5D40"),
                     "Panorama_Holder", "", "", false});
        t.push_back({HolderKind::Panorama,
                     Guid::fromString("8B446B11-0746-11D1-9B09-008048EB5D40"),
                     "Panorama", "", "", false});
        t.push_back({HolderKind::Puzzle,
                     Guid::fromString("196E7596-AB2B-11D0-B2A5-008048EB5D40"),
                     "Puzzle", "", "", false});
        t.push_back({HolderKind::Recorder,
                     Guid::fromString("2B61D676-9627-11D0-B280-008048EB5D40"),
                     "Recorder", "", "", false});
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
        hotspot.layer = readI32(record, kOffLayer);
        hotspot.enabled = readI32(record, kOffEnabled);
        hotspot.name = reader.readArchiveString();
        state.hotspots.push_back(std::move(hotspot));
    }

    state.field1f0 = reader.readU32();
    state.field1c8 = reader.readU32();
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
            definition.width = readU32At(blob, 0x80);
            definition.height = readU32At(blob, 0x84);
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
        record.width = readU32At(metadata, 0x10);
        record.height = readU32At(metadata, 0x14);
        record.name = readNulPaddedName(metadata, 0x28, 12);
        record.pixelsAreRawIndexed =
            record.width != 0 && record.height != 0 &&
            static_cast<std::uint64_t>(record.width) * record.height == record.pixelByteCount;
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

        // The still frame is sized from the ENTRY's header copy, exactly as
        // TVH_LoadPrivateState computes it.
        const auto entryWidth = readU32At(entryBytes, 0x80);
        const auto entryHeight = readU32At(entryBytes, 0x84);
        entry.stillFrameByteCount = dibStride8(entryWidth) * entryHeight;
        entry.stillFrameOffset = reader.position();
        reader.skip(entry.stillFrameByteCount);

        state.entries.push_back(entry);
    }

    return state;
}

} // namespace graphboard
