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

// -------------------------------------------------------------------------
// Sprite_Holder component-private state.
// Serializer: SpriteHolder.dll:10008930 (SpriteHolder_SerializePrivateState).
// Verified byte-exact against RZECZKA.BDF (private block 0x598572..0x641bea:
// version 1, 5 definitions, 5 instances).
//
//   u32 version (=1)
//   u32 definitionCount
//   u32 instanceCount
//   repeat definitionCount:
//       u32 blobByteCount
//       u8  blob[blobByteCount]     (CSpriteDef; fields below are blob-relative)
//   repeat instanceCount:
//       u8  record[0x8c]            (CSpriteParam; +0x00 is a definition INDEX
//                                    on disk, rebound to a pointer on load)
//
// Blob fields verified in RZECZKA.BDF: name is a NUL-padded string at +0x04,
// width u32 at +0x80, height u32 at +0x84. (The in-memory CSpriteDef offsets
// quoted in older notes — name +0x0c, width +0x88 — are shifted; the disk blob
// uses the offsets here.) Pixel/phase data trails the blob header; its exact
// framing is still being recovered, so the blob bytes are exposed by range.
// -------------------------------------------------------------------------
struct SpriteDefinition {
    std::uint32_t blobByteCount = 0;
    std::size_t blobOffset = 0;   // file offset of the blob's first byte
    std::string name;             // blob+0x04
    std::uint32_t width = 0;      // blob+0x80
    std::uint32_t height = 0;     // blob+0x84
};

struct SpriteInstance {
    std::uint32_t definitionIndex = 0;  // record+0x00 (index on disk)
    std::uint32_t field04 = 0;          // record+0x04 (mirrored to +0x18 on load)
    std::int32_t posX = 0;              // record+0x08 (placement-like, observed)
    std::int32_t posY = 0;              // record+0x0c (placement-like, observed)
};

struct SpriteHolderState {
    std::uint32_t version = 0;
    std::vector<SpriteDefinition> definitions;
    std::vector<SpriteInstance> instances;
};

SpriteHolderState parseSpriteHolderState(BinaryReader& reader);

// -------------------------------------------------------------------------
// MultiBitmap component-private state.
// Serializer: MultiBmp.dll (anchor not yet named in Ghidra); layout recovered
// empirically and used by graphboard_extract_assets.py across the DATA folder.
//
//   u32 version (observed: 1)
//   u32 recordCount
//   repeat recordCount:
//       u32 pixelByteCount
//       u8  pixels[pixelByteCount]
//       u8  metadata[0xc0]          (width u32 +0x10, height u32 +0x14,
//                                    name char[12] +0x28; +0x34/+0x38 are stale
//                                    runtime pointers)
//
// When pixelByteCount == width*height the pixels are raw 8-bit indexed and can
// be rendered with the page palette; otherwise the bytes are component-private.
// -------------------------------------------------------------------------
struct MultiBitmapRecord {
    std::uint32_t pixelByteCount = 0;
    std::size_t pixelOffset = 0;   // file offset of the pixel bytes
    std::uint32_t width = 0;       // metadata+0x10
    std::uint32_t height = 0;      // metadata+0x14
    std::string name;              // metadata+0x28, cp1250-ish, NUL padded
    bool pixelsAreRawIndexed = false;  // pixelByteCount == width*height
};

struct MultiBitmapState {
    std::uint32_t version = 0;
    std::vector<MultiBitmapRecord> records;
};

MultiBitmapState parseMultiBitmapState(BinaryReader& reader);

// -------------------------------------------------------------------------
// Transparent_Video_Holder component-private state.
// Serializer: TransparentVideoHolder.dll:10007af0 (TVH_SerializeOrDeserialize);
// full save/load reconstruction in ../reconstructed/TransparentVideoHolder_reconstructed.cpp.
//
//   u32 version (=3)
//   u32 entryCount
//   repeat entryCount:
//       u32 streamByteCount
//       u8  stream[streamByteCount]   (BoardVideoHeader[0x4e8] + chunk records)
//       u8  entry[0x568]              (starts with a BoardVideoHeader copy;
//                                      original x/y/z +0x508/+0x50c/+0x510,
//                                      stage x/y/z +0x54c/+0x550/+0x554,
//                                      stream start/end offsets +0x558/+0x55c)
//       u8  stillFrame[stride * height]  with stride = ((width*8+31)>>3) & ~3
//
// The still-frame size is computed from the entry header's width/height, so the
// whole block can be walked without decoding the chunk stream.
// -------------------------------------------------------------------------
struct BoardVideoStreamInfo {
    std::uint32_t streamByteCount = 0;
    std::size_t streamOffset = 0;   // file offset of "Board Video File Ver:0"
    std::uint32_t magic = 0;        // header+0x68, expected 0xada77777
    std::uint32_t frameDurationMs = 0;  // header+0x7c
    std::int32_t width = 0;         // header+0x80
    std::int32_t height = 0;        // header+0x84
    std::uint32_t chunkRecordCount = 0; // header+0x88
    std::uint32_t frameCount = 0;   // header+0x8c
    std::uint32_t audioSampleRate = 0;  // header+0x90
};

struct TransparentVideoEntry {
    BoardVideoStreamInfo stream;
    std::int32_t originalX = 0;     // entry+0x508
    std::int32_t originalY = 0;     // entry+0x50c
    std::int32_t originalZ = 0;     // entry+0x510
    std::int32_t stageX = 0;        // entry+0x54c
    std::int32_t stageY = 0;        // entry+0x550
    std::int32_t stageZ = 0;        // entry+0x554
    std::uint32_t stillFrameByteCount = 0;
    std::size_t stillFrameOffset = 0;
};

struct TransparentVideoHolderState {
    std::uint32_t version = 0;
    std::vector<TransparentVideoEntry> entries;
};

TransparentVideoHolderState parseTransparentVideoHolderState(BinaryReader& reader);

} // namespace graphboard
