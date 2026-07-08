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
                     "TransparentVideoHolder.dll:10007af0", false});
        t.push_back({HolderKind::SpriteHolder,
                     Guid::fromString("B64F3336-5368-11D0-B445-008048EB5D40"),
                     "Sprite_Holder", "SpriteHolder.dll",
                     "SpriteHolder.dll:10008930", false});
        t.push_back({HolderKind::TextHolder,
                     Guid::fromString("AD45ADB6-5290-11D0-B442-008048EB5D40"),
                     "Text_Holder", "TextHolder.dll", "", false});
        t.push_back({HolderKind::SoundHolder,
                     Guid::fromString("1720D306-6932-11D0-B468-008048EB5D40"),
                     "Sound_Holder", "", "", false});
        t.push_back({HolderKind::MultiBitmap,
                     Guid::fromString("F7794041-1EB4-11D1-9B25-008048EB5D40"),
                     "MultiBitmap", "MultiBmp.dll", "", false});
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

} // namespace graphboard
