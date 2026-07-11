#pragma once

#include "graphboard/binary_reader.hpp"
#include "graphboard/guid.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace graphboard {

// The GraphBoard component ("holder") kinds recovered so far. The first six
// wrapper CLSIDs were observed in RZECZKA.BDF; the rest were collected by
// walking every page in the Tuwim DATA folder. Anything not in the registry
// parses as Unknown.
enum class HolderKind {
    Unknown,
    HotSpotHolder,
    TransparentVideoHolder,
    SpriteHolder,
    TextHolder,
    SoundHolder,
    MultiBitmap,
    BitmapHolder,
    VideoHolder,
    PanoramaHolder,
    Panorama,
    Puzzle,
    Recorder,
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
//       u8 record[100]     (rects at +0/+4/+8/+0xc, id +0x18, layer +0x1c,
//                           enabled +0x20)
//       CString name
//   u32 activeIndex   (holder +0x1f0: active/selected hotspot index)
//   u32 auxStateWord  (holder +0x1c8)
// -------------------------------------------------------------------------
struct HotSpot {
    std::int32_t left = 0;
    std::int32_t top = 0;
    std::int32_t right = 0;
    std::int32_t bottom = 0;
    // Script-visible "rectID": what LeftButtonClickOn receives and what
    // EnableHotSpot/DisableHotSpot match on. Not the array index -- RZECZKA.BDF
    // stores ids 0,1,3,4,6,5,7,8 for indices 0..7.
    std::int32_t id = 0;         // record +0x18
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
    std::uint32_t activeIndex = 0;   // holder +0x1f0
    std::uint32_t auxStateWord = 0;  // holder +0x1c8
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
// One animation frame of a definition. The blob carries a frame table at
// blob+0x6c with stride 0x4c; SpriteHolder::LButtonDown (@ 10008c80) computes
// the hit rect from `frame = def + 0x6c + phase*0x4c`, reading width at
// frame+0x14 and height at frame+0x18. For phase 0 that is blob+0x80/+0x84,
// which is why the definition header's width/height agree with frames[0].
//
// A rect hit is refined per-pixel when the frame opts in (frame+0x08 != 0) and
// the definition opts in (def+0x24 != 0): the click misses if the pixel under
// it is the transparent colour (frame+0x04). `opaque`, when non-empty, is that
// test precomputed: width*height, row-major top-down, 1 = opaque. Empty means
// rect-only (the common case). Verified against CUDA.BDF's "motylek" (the mask
// reconstructs the butterfly silhouette exactly).
struct SpriteFrame {
    std::uint32_t width = 0;    // frame+0x14
    std::uint32_t height = 0;   // frame+0x18
    std::vector<std::uint8_t> opaque;  // width*height, or empty for rect-only
    // Indexed pixels for rendering: width*height, row-major top-down. Colour
    // comes from the page palette; `transparentIndex` (frame+0x04) is skipped.
    std::vector<std::uint8_t> pixels;
    std::uint8_t transparentIndex = 0;
};

struct SpriteDefinition {
    std::uint32_t blobByteCount = 0;
    std::size_t blobOffset = 0;   // file offset of the blob's first byte
    std::string name;             // blob+0x04
    std::uint32_t phaseCount = 0; // blob+0x00: number of valid frame records
    std::uint32_t width = 0;      // blob+0x80 (== frames[0].width)
    std::uint32_t height = 0;     // blob+0x84 (== frames[0].height)
    std::vector<SpriteFrame> frames;   // phaseCount entries
};

struct SpriteInstance {
    std::uint32_t definitionIndex = 0;  // record+0x00 (index on disk)
    std::uint32_t field04 = 0;          // record+0x04 (mirrored to +0x18 on load)
    std::int32_t posX = 0;              // record+0x08
    std::int32_t posY = 0;              // record+0x0c
    // Verified against RZECZKA.BDF (layers 0,8,8,1,2; visible 1,1,1,0,0).
    std::int32_t layer = 0;             // record+0x18 ("deep"; mirrors +0x04)
    std::int32_t phase = 0;             // record+0x5c (current animation frame)
    std::int32_t visible = 0;           // record+0x88
    // record+0x1c == 1 means the sprite can be click-dragged (SpriteHolder
    // LButtonUp @ 10008fd0 fires MouseDrop only for these). CUDA's butterflies
    // are 1; DYZIO's flying food is 0.
    bool dragEnabled = false;
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
    // entry+0x504 (initialFrameScratch2): 1 => the board paints this entry's
    // still frame at rest (its resting pose), 0 => it stays blank until played.
    // WYBORW's 25 menu characters carry 1; its two menu-overlay entries carry 0.
    bool showStillAtRest = false;
};

struct TransparentVideoHolderState {
    std::uint32_t version = 0;
    std::vector<TransparentVideoEntry> entries;
};

TransparentVideoHolderState parseTransparentVideoHolderState(BinaryReader& reader);

// -------------------------------------------------------------------------
// Sound_Holder component-private state.
// Serializer: SoundHolder_SerializeSoundData (Sound.dll:10004090); layout
// recovered read-only and verified walking all 14 RZECZKA.BDF sounds
// (0x648c9c..0xb04568). See docs/holder_recovery_notes.md, including the
// version-skew caveat: the shipped 1997 DLL wrote a 0x58-byte record + u32
// waveFormatByteCount (0 in every observed sound); the GraphBoard 1.00 DLL in
// the Ghidra project writes a 0x6c record and a nonzero wave-format block.
//
//   u32 version              // ignored on load; RZECZKA: 2
//   u32 soundCount
//   repeat soundCount:
//       u32 soundByteCount
//       u8  soundBytes[soundByteCount]  // complete RIFF/WAVE file
//       u8  record[0x58]     // +0x04/+0x08 archive start/end, +0x0c name
//       u32 waveFormatByteCount
//       u8  waveFormat[waveFormatByteCount]
// -------------------------------------------------------------------------
struct SoundEntry {
    std::uint32_t soundByteCount = 0;
    std::size_t soundOffset = 0;      // file offset of the RIFF bytes
    std::string name;                 // record+0x0c, NUL padded
    std::uint32_t archiveStart = 0;   // record+0x04 (matches soundOffset on disk)
    std::uint32_t archiveEnd = 0;     // record+0x08
    std::uint32_t waveFormatByteCount = 0;
    bool looksLikeRiff = false;       // soundBytes begin with "RIFF"
    // Playback length in ms, from the WAV (data-chunk bytes / fmt byte-rate);
    // 0 if the bytes are not a parseable RIFF/WAVE. Used to schedule EndPlaySound.
    std::uint32_t durationMs = 0;
};

struct SoundHolderState {
    std::uint32_t version = 0;
    std::vector<SoundEntry> sounds;
};

SoundHolderState parseSoundHolderState(BinaryReader& reader);

// -------------------------------------------------------------------------
// Text_Holder component-private state (including the trailing FontControl
// block). Serializers: TextHolder_SerializeDocument (TextHolder.dll:10008200),
// TextHolder_SerializeOneTextEntry (TextHolder.dll:10007de0),
// FontControl_SerializeFontSlots (FontControl.dll:10004230). Layout recovered
// read-only and verified against RZECZKA.BDF: text entries 0x642244..0x642508,
// FontControl block ends exactly at the Sound wrapper 0x64891c.
//
//   u32 version (=2)
//   u32 textCount
//   u8  color0..color3
//   repeat textCount:
//       u8 entry[0xc4]                       // stale ptrs +0x70/+0x90 act as flags
//       if entry+0x70 != 0:
//           if entry+0x90 == 0:
//               u32 streamByteCount; u8 stream[streamByteCount]
//               u8 renderCache[0x68]; [u32 lineOffsets[lineCount]]
//           else:
//               u8 renderCache[0x68]; [u32 lineOffsets[lineCount]]
//               CString secondaryText        // e.g. "rzeczka.exs" synchro ref
//       CString primaryText                  // the display text (cp1250)
//   FontControl block: exactly 50 slots of
//       u32 present; if present: u8 slot[0xc84] + per-glyph bitmaps
// -------------------------------------------------------------------------
struct TextEntry {
    bool hasRenderCache = false;      // entry+0x70 != 0
    bool hasSecondaryText = false;    // entry+0x90 != 0
    std::uint32_t streamByteCount = 0;   // embedded render stream (branch A only)
    std::size_t streamOffset = 0;
    std::uint32_t lineCount = 0;         // renderCache+0x10
    std::vector<std::uint32_t> lineOffsets;
    std::string secondaryText;           // branch B only
    std::string primaryText;
};

struct FontSlot {
    std::uint32_t height = 0;         // slot+0xc00
    std::uint32_t glyphCount = 0;     // glyphs with bitmaps present
};

struct TextHolderState {
    std::uint32_t version = 0;
    std::uint8_t colors[4] = {0, 0, 0, 0};
    std::vector<TextEntry> entries;
    std::vector<FontSlot> fontSlots;  // present slots only, in slot order
};

TextHolderState parseTextHolderState(BinaryReader& reader);

// -------------------------------------------------------------------------
// Bitmap_Holder component-private state.
// Serializer: BitmapHolder_SerializeBitmaps (BitmapHolder.dll:10003d20).
// Verified against GRZESIU.BDF (private block 0xa54ae8: one 452x359 bitmap
// "autobus09"; the walk lands exactly on the next wrapper's CLSID).
//
//   u32 version (=1)         // ignored on load
//   u32 bitmapCount          // holder+0x1dc
//   repeat bitmapCount:
//       u32 blobByteCount
//       u8  blob[blobByteCount]
//
// Blob fields (verified): rect left/top/right/bottom i32 at +0x08..+0x14,
// layer ("deep") i32 at +0x18, name char[12] at +0x34, current-position copies
// at +0x40/+0x44 (load copies +0x08/+0x0c over them), 8-bpp pixels from +0x90
// with stride ((right-left)+3 & ~3) and (bottom-top) rows.
//
// The click path (BitmapHolder_LButtonDown_HitTestAndFireClick, @ 10003f50)
// hit-tests by rect + layer only (no visibility gate) and fires
// MouseClickOnDown(index). It can refine a rect hit with a per-pixel
// transparency test when the record opts in (record+0x20 and +0x28 both != 0;
// 43 of 203 bitmaps in the title) — not yet modelled here.
// -------------------------------------------------------------------------
struct BitmapHolderBitmap {
    std::uint32_t blobByteCount = 0;
    std::size_t blobOffset = 0;
    std::int32_t left = 0, top = 0, right = 0, bottom = 0;
    std::int32_t layer = 0;           // blob+0x18 ("deep")
    bool initiallyHidden = false;     // blob+0x2c: revealed later via ShowBitmap
    std::string name;                 // blob+0x34
    std::size_t pixelOffset = 0;      // blobOffset + 0x80 (pixels; 0x10 trailer follows)
    bool pixelSizeConsistent = false; // stride*(bottom-top) == blobByteCount-0x90
    // Per-pixel transparency mask, when the record opts in (see the .cpp): a
    // click on a transparent pixel misses. width*height, row-major top-down,
    // 1 = opaque; empty for rect-only bitmaps.
    std::vector<std::uint8_t> opaque;
    // Indexed pixels for rendering: (right-left)*(bottom-top), row-major
    // top-down. `transparentIndex` (blob+0x04) is skipped when compositing.
    std::vector<std::uint8_t> pixels;
    std::uint8_t transparentIndex = 0;
};

struct BitmapHolderState {
    std::uint32_t version = 0;
    std::vector<BitmapHolderBitmap> bitmaps;
};

BitmapHolderState parseBitmapHolderState(BinaryReader& reader);

// -------------------------------------------------------------------------
// Puzzle component-private state.
// Serializer: PuzzleContext_SerializeBoards (Puzzle.dll:10003470).
//
//   u32 version (=1)         // ignored on load
//   u32 boardCount           // ctx+0x110
//   repeat boardCount:
//       u8 board[0x60]       // +0x0c chipCount (ptr table at +0x08 is stale)
//       repeat chipCount:
//           u8  chip[0x48]   // +0x00 i32 left, +0x04 i32 top, +0x08 u16 right,
//                            // +0x0c i32 bottom, +0x20 subRecordCount
//           u32 pixelByteCount   // save computes (bottom-top) *
//                                // ((right-left)+3 & ~3); load trusts the u32
//           u8  pixels[pixelByteCount]      // 8-bpp chip image
//           u8  subRecords[subRecordCount][0x2c]
// -------------------------------------------------------------------------
struct PuzzleChip {
    std::int32_t left = 0, top = 0, bottom = 0;
    std::uint16_t right = 0;
    std::uint32_t pixelByteCount = 0;
    std::size_t pixelOffset = 0;
    std::uint32_t subRecordCount = 0;
};

struct PuzzleBoard {
    std::vector<PuzzleChip> chips;
};

struct PuzzleState {
    std::uint32_t version = 0;
    std::vector<PuzzleBoard> boards;
};

PuzzleState parsePuzzleState(BinaryReader& reader);

// -------------------------------------------------------------------------
// Recorder component-private state.
// Serializer: Recorder_SerializePrivateState (Recorder.dll:10003ef0) — named by
// the Ghidra pass; it writes schema 1, a holder+0x34 field, and a fixed state
// block. On disk this port sees u32 version(=1) + a 0x68-byte record (108 bytes
// total), verified identical across ABEC_R/CUDA_R/DYZIO_R/FIG_R, each ending
// exactly at the next wrapper CLSID.
// -------------------------------------------------------------------------
struct RecorderState {
    std::uint32_t version = 0;
    std::size_t recordOffset = 0;   // file offset of the 0x68-byte record
};

RecorderState parseRecorderState(BinaryReader& reader);

// -------------------------------------------------------------------------
// Video_Holder component-private state.
// Serializer: VideoHolder_SerializePrivateState (VideoHolder.dll:10003e20).
// Verified byte-exact against ABEC_T.BDF (private block at 0x865: version 1,
// 9 AVI entries; the walk lands exactly on the next wrapper CLSID). Video_Holder
// plays external .AVI assets referenced by name.
//
//   u32 version (=1)
//   u32 entryCount           // holder+0x118
//   repeat entryCount:
//       u8 record[0x6c]      // +0x10 posX, +0x14 posY, +0x20 embedded name copy
//       CString aviName
// -------------------------------------------------------------------------
struct VideoEntry {
    std::size_t recordOffset = 0;
    std::int32_t posX = 0;   // record+0x10
    std::int32_t posY = 0;   // record+0x14
    std::string name;        // trailing CString: the .avi filename
};

struct VideoHolderState {
    std::uint32_t version = 0;
    std::vector<VideoEntry> entries;
};

VideoHolderState parseVideoHolderState(BinaryReader& reader);

// -------------------------------------------------------------------------
// Panorama_Holder component-private state.
// Serializer: PanoramaHolder_SerializePrivateState (PanoramaHolder.dll:10005e20).
// Verified byte-exact against MROZ.BDF (lands on the next wrapper CLSID). The
// per-block byte counts are length-prefixed on disk, so the block self-describes.
//
//   u32 version (=0)
//   u32 sceneCount           // holder+0x40
//   repeat sceneCount:
//       u8  sceneRecord[0x224]
//       u32 dibByteCount;    u8 dib[dibByteCount]        // BITMAPINFO+palette+pixels
//       u32 subImageCount;   repeat: u32 size; u8 data[size]   // record+0x88...
//       u32 regionCount;     repeat: u8 region[0x48]           // record+0x154...
// -------------------------------------------------------------------------
struct PanoramaHolderScene {
    std::size_t recordOffset = 0;
    std::uint32_t dibByteCount = 0;
    std::uint32_t subImageCount = 0;
    std::uint32_t regionCount = 0;
};

struct PanoramaHolderState {
    std::uint32_t version = 0;
    std::vector<PanoramaHolderScene> scenes;
};

PanoramaHolderState parsePanoramaHolderState(BinaryReader& reader);

// -------------------------------------------------------------------------
// Panorama (non-holder) component-private state.
// Serializer: PanoramaState_Serialize (Panorama.dll:10004b6a). Verified against
// OKULARY.BDF (1224x480 scene, 19 layers; lands on the next wrapper CLSID).
// Unlike Panorama_Holder, image byte counts are NOT length-prefixed: they are
// computed from width*height fields inside the just-read fixed records, and the
// sub-image/region counts live inside the 0x224 scene record itself.
//
//   u32 version (=1)
//   u32 sceneCount           // state+0x38
//   repeat sceneCount:
//       u8  sceneRecord[0x224]           // width +0x04, height +0x08,
//                                         // subImageCount +0x150, regionCount +0x21c
//       u8  basePixels[width*height]
//       repeat subImageCount:            // record+0x88...
//           u8 layerRecord[0x78]         // width +0x24, height +0x2c
//           u8 layerPixels[width*height]
//       repeat regionCount:              // record+0x154...
//           u8 region[0x34]
// -------------------------------------------------------------------------
struct PanoramaScene {
    std::size_t recordOffset = 0;
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    std::uint32_t subImageCount = 0;
    std::uint32_t regionCount = 0;
};

struct PanoramaState {
    std::uint32_t version = 0;
    std::vector<PanoramaScene> scenes;
};

PanoramaState parsePanoramaState(BinaryReader& reader);

} // namespace graphboard
