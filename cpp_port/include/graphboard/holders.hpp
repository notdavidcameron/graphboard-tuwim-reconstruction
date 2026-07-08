#pragma once

#include "graphboard/binary_reader.hpp"
#include "graphboard/guid.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace graphboard {

// The GraphBoard component ("holder") kinds recovered so far. These are the six
// wrapper CLSIDs observed in RZECZKA.BDF; the full component set is larger (each
// *.dll under "GraphBoard 1.00" is a candidate) and can be added as its CLSID is
// confirmed. Anything not in the registry parses as Unknown.
enum class HolderKind {
    Unknown,
    HotSpotHolder,
    TransparentVideoHolder,
    SpriteHolder,
    TextHolder,
    SoundHolder,
    MultiBitmap,
};

// Registry entry mapping a wrapper CLSID to a known holder identity. `dll` and
// `serializerAnchor` are filled in only where verified against the disassembly;
// empty means "not yet confirmed", not "none". `privateStateParserImplemented`
// reports whether this port can currently parse the component-private payload
// (needed to walk past the component in a real file).
struct HolderInfo {
    HolderKind kind = HolderKind::Unknown;
    Guid clsid;
    std::string displayName;
    std::string dll;               // implementing component DLL, "" if unconfirmed
    std::string serializerAnchor;  // e.g. "HotSpotHolder.dll:10004d10", "" if not recovered
    bool privateStateParserImplemented = false;
};

// Look up a wrapper CLSID. Returns nullptr for CLSIDs not yet in the registry.
const HolderInfo* lookupHolder(const Guid& clsid);

// All registry entries (stable order), for tooling/enumeration.
const std::vector<HolderInfo>& knownHolders();

// -------------------------------------------------------------------------
// HotSpot_Holder component-private state.
// Serializer: HotSpotHolder.dll:10004d10 (HotSpotHolder_SerializePrivateState).
// Verified byte-exact against RZECZKA.BDF (private block at 0x000008e6).
//
//   u32 version (=0)
//   u8  flag0, flag1, flag2
//   u32 hotspotCount
//   repeat hotspotCount:
//       u8 record[100]     (rects at +0/+4/+8/+0xc, layer +0x1c, enabled +0x20)
//       CString name
//   u32 field1f0
//   u32 field1c8
// -------------------------------------------------------------------------
struct HotSpot {
    std::int32_t left = 0;
    std::int32_t top = 0;
    std::int32_t right = 0;
    std::int32_t bottom = 0;
    std::int32_t layer = 0;
    std::int32_t enabled = 0;
    std::string name;
};

struct HotSpotHolderState {
    std::uint32_t version = 0;
    std::uint8_t flag0 = 0;
    std::uint8_t flag1 = 0;
    std::uint8_t flag2 = 0;
    std::vector<HotSpot> hotspots;
    std::uint32_t field1f0 = 0;
    std::uint32_t field1c8 = 0;
};

HotSpotHolderState parseHotSpotHolderState(BinaryReader& reader);

} // namespace graphboard
