# GraphBoard File Format Notes

Continuation from the initial `Tuwim.exe` serializer reconstruction, now cross-checked against `RZECZKA.BDF` and the running Ghidra MCP server.

The host serializers described below (`START.PRJ`, `.BDF`, `.GRP`, the shared
script-text block, the reflected component wrapper, and the script engine
state) have a source-style C++ reconstruction in
`ghidra_import/GraphBoardDocument_reconstructed.cpp`, kept compile-checkable via
`tools/compile_reconstructions.ps1`.

## Shared Encoding

- Integers are little-endian.
- Several records are written through MFC `CArchive`.
- `CString` uses the MFC archive string length prefix:
  - `uint8 length` when length is below `0xff`.
  - `0xff`, then `uint16 length` for longer strings.
  - `0xff 0xffff`, then `uint32 length` for very long strings.
  - String bytes are stored without a trailing NUL.
- Component payloads are polymorphic. The host stores a component CLSID and wrapper metadata, then calls the component DLL serialize method at component interface vtable slot `+0x44`.

## `START.PRJ`

Serializer: `GraphBrdDoc_SerializeProjectState` at `Tuwim.exe:00406020`.

Observed sample: `START.PRJ`, 827 bytes.

Layout:

```text
u32 version                         // currently 1
CString startupPage                 // sample: "intro.bdf"
CString currentPageOrGroupState     // sample: empty
u32 audioPresetIndex                // sample: 11
u32 pageCount
CString pageNames[pageCount]
u32 groupCount
CString groupNames[groupCount]
CString encodedSignature            // "Julian Tuwim", each byte + 0x21 on save
u32 globalScriptVersion             // currently 1
CString globalScript                // project-wide global-variable / handler script
```

The `audioPresetIndex` is the only project field owned by the audio manager: the
serializer reads/writes it from `audioManager(+0xbc)->[+0x20]`. The audio manager
does **not** serialize any trailing block into `START.PRJ`.

The trailing `(globalScriptVersion, globalScript)` pair is written by the group /
global script editor at document field `+0xa0`, via
`GraphBrdScriptEditor_SerializeText` (`Tuwim.exe:004230a0`) — the same routine the
page serializer uses for per-page script text (see below). On load the host runs
this text through `GraphBrdScript_RunGlobalSetupBlock` (`this+0xb4`), so it holds
the project's global variables and shared handlers.

Sample `START.PRJ` (827 bytes) verified end-to-end: version 1, `startupPage`
`"intro.bdf"`, empty `currentPageOrGroupState`, `audioPresetIndex` 11, 34 page
names, 6 group names, a valid `"Julian Tuwim"` signature, then a 339-byte trailing
block = `u32 1` + a 332-byte `globalScript` CString that begins with
`//Global variables ... int global1=0;` and consumes exactly to end-of-file.

## Script Editor Text Serialization

Serializer: `GraphBrdScriptEditor_SerializeText` at `Tuwim.exe:004230a0`.

Both the page script editor (document field `+0x9c`) and the group / global script
editor (document field `+0xa0`) are the same dialog class and share this routine:

```text
u32 version                         // currently 1
CString scriptText                  // editor buffer at editor+0xe0, no trailing NUL
```

`GraphBrdDoc_SerializePageData` invokes it on `+0x9c` for per-page script text;
`GraphBrdDoc_SerializeProjectState` invokes it on `+0xa0` for the `START.PRJ`
global script.

## `.BDF` Page Files

Serializer: `GraphBrdDoc_SerializePageData` at `Tuwim.exe:004066f0`.

Top-level layout:

```text
char banner[100]
u32 version
i32 pageLeft
i32 pageTop
i32 pageRight
i32 pageBottom
i32 minLayer
i32 maxLayer
u32 hasBackgroundBitmapOrFillMode
u8  backgroundColorIndex
u32 paletteByteCount
u8  paletteBytes[paletteByteCount]
u32 dibByteCount
u8  dibBytes[dibByteCount]
ComponentList pageComponents          // document field +0xa4
ScriptEditorText pageScriptText       // script editor dialog at document field +0x9c
ScriptEngineState pageScriptIndexes   // script engine at document field +0xb4
COleDocument archive state
```

For `RZECZKA.BDF`:

```text
file size: 14,661,455 bytes
banner: "YDP Board data file. Ver:1 1996.07.    File modification: Wednesday, November 12, 1997"
version: 1
page rectangle: 0,0,640,480
layer range: -1..8
background flag: 1
background color index: 132
palette bytes: 1024
dib bytes: 0
first component list offset: 0x048d
first component list version/count: 1 / 6
```

## `RZECZKA.BDF` Component List A

The six wrappers validate by the host `GraphBrdCntrItem_Serialize` shape: CLSID, wrapper version, reflected function/property records, duplicate CLSID, then component-private state.

```text
0: HotSpot_Holder
   CLSID DA768116-5341-11D0-B444-008048EB5D40
   wrapper offset 0x00000495, private offset 0x000008e6
   reflected functions/properties: 13 / 11
   private length to next wrapper: 827 bytes

1: Transparent_Video_Holder
   CLSID 2F3C5846-5413-11D0-B447-008048EB5D40
   wrapper offset 0x00000c21, private offset 0x000010b7
   reflected functions/properties: 12 / 16
   private length to next wrapper: 5,861,053 bytes
   private header starts: u32 3, u32 8, u32 0x00035331, "Board Video File Ver:0"

2: Sprite_Holder
   CLSID B64F3336-5368-11D0-B445-008048EB5D40
   wrapper offset 0x00597f74, private offset 0x00598572
   reflected functions/properties: 15 / 22
   private length to next wrapper: 693,880 bytes
   private header starts: u32 1, u32 5, u32 5, u32 0x00096108, u32 1, "rzeczka"

3: Text_Holder
   CLSID AD45ADB6-5290-11D0-B442-008048EB5D40
   wrapper offset 0x00641bea, private offset 0x00642244
   reflected functions/properties: 15 / 28
   private length to next wrapper: 26,328 bytes
   private header starts: u32 2, u32 1, u32 0x0037bb17

4: Sound_Holder
   CLSID 1720D306-6932-11D0-B468-008048EB5D40
   wrapper offset 0x0064891c, private offset 0x00648c9c
   reflected functions/properties: 8 / 12
   private length to next wrapper: 4,962,508 bytes
   private header starts: u32 2, u32 14, u32 0x8452, then RIFF/WAVE bytes

5: MultiBitmap
   CLSID F7794041-1EB4-11D1-9B25-008048EB5D40
   wrapper offset 0x00b04568, private offset 0x00b04955
   reflected functions/properties: 11 / 10
   private header starts: u32 1, u32 20, u32 0x0004b000
   private length to script editor text: 3,087,808 bytes
```

The original quick scan missed `MultiBitmap` because its display name does not end in `_Holder`.

## Reflected Wrapper Notes

Serializer: `GraphBrdCntrItem_Serialize` at `Tuwim.exe:0042b920`.

Layout:

```text
u32 version                         // currently 1
u32 reflectedFunctionCount
CString displayName
u32 reflectedPropertyCount
GUID clsid                          // duplicate of ComponentList CLSID
FunctionMemberRecord reflectedFunctionRecords[reflectedFunctionCount]
PropertyMemberRecord reflectedPropertyRecords[reflectedPropertyCount]
ComponentPrivateState
```

Function member records:

```text
u32 dispatchIdOrOffset
u32 rawNameByteCount
char rawName[rawNameByteCount + 1]
CString typeOrReturnName
CString displayName
CString descriptionOrHelpName
u16 flagsOrInvokeKind
```

Property member records:

```text
u32 rawNameByteCount
char rawName[rawNameByteCount + 1]
u32 variantTypeOrDispatchMetadata
CString displayName
u16 flagsOrInvokeKind
```

For `Sprite_Holder`, the final reflected property is `CompDisableDrag`; the component-private state begins immediately after its `u16 flags` at `0x00598572`.

## Script Engine State

Serializer: `GraphBrdScriptEngine_Serialize` at `Tuwim.exe:0041aad0`.

Appears in `.BDF` immediately after the page script text block (document field
`+0xb4`). It caches parse results for the page script; all offsets are
character offsets into the page script text.

```text
u32 schemaVersion                   // current exe writes 4; RZECZKA.BDF has 2
u32 switchBlockCount                // max 100
u32 parserState[4]                  // engine +0x38, +0x3c, +0x30, +0x34
repeat switchBlockCount:
  u32 blockStart                    // written twice; second copy wins on load
  u32 blockStart
  u32 caseCount
  u32 blockEnd
  u32 defaultBodyOffset
  repeat caseCount:
    u32 caseValue
    u32 caseBodyOffset
u32 builtinCallCount                // max 100
u32 builtinTokenOffsets[builtinCallCount]   // engine +0x11b4
u32 builtinCallKinds[builtinCallCount]      // engine +0x1344
if schema >= 2: u32 engine +0x44, +0x48, +0x4c, +0x50
if schema >= 3: u32 engine +0x54
if schema >= 4: u32 engine +0x58
```

The switch index is built by `GraphBrdScript_IndexSwitchCaseBlocks`
(`Tuwim.exe:00426480`) and the call records by
`GraphBrdScript_IndexBuiltInCalls` (`Tuwim.exe:00427dd0`); the call records
feed `GraphBrdScript_InvokeIndexedCall` at runtime.

Verified against `RZECZKA.BDF` (engine state at `0xdfb493`): schema 2,
11 switch blocks whose offsets all fall inside the 19,831-byte script text,
one built-in call record pointing at `SetTextMode(void)`, and the schema-2
scalar fields `19598, 0, 19730, 0` (values near the text length suggest these
are also text offsets, roles not yet recovered). The block ends 4 bytes before
EOF; the remaining u32 is the `COleDocument::Serialize` trailing state.

## Document Field Roles

`GraphBrdDoc_Construct` at `Tuwim.exe:00405780` resolves the document-owned helper objects:

```text
+0xa4 page component list dialog
+0xa8 group component list dialog
+0x9c page script editor dialog
+0xa0 group script editor dialog
+0xb4 script engine
+0xbc audio manager
```

The script engine stores links back to the document and both script editors:

```text
scriptEngine+0x20 = document
scriptEngine+0x24 = page script editor (+0x9c)
scriptEngine+0x28 = group script editor (+0xa0)
```

The page serializer calls `+0xa4`, `+0x9c`, then `+0xb4`. The project serializer (`GraphBrdDoc_SerializeProjectState`) calls the group / global script editor at `+0xa0`, which emits the `START.PRJ` trailing global-script block. The group serializer (`.GRP`) below calls `+0xa8`; its group script editor is not part of the cursor/group serializer recovered so far.

## `.GRP` Group Files

Serializer: `GraphBrdDoc_SerializeCursorAndGroupState` at `Tuwim.exe:004064c0`.

Top-level layout:

```text
u32 cursorBitmapCount
repeat cursorBitmapCount:
  CursorBitmapRecord2c record
  u32 pixelByteCount
  u8 pixels[pixelByteCount]
ComponentList groupComponents
```

`CursorBitmapRecord2c` is written as a raw 0x2c-byte in-memory record. The final pointer field is overwritten on load and should be ignored by external parsers.

Observed first cursor in `CURSORS.GRP`:

```text
cursorCount: 13
record[0]+0x00 = 1
record[0]+0x04 = transparent/background color index
record[0]+0x08 = width
record[0]+0x0c = height
record[0]+0x10/+0x14 = hotspot or edit coordinates
record[0]+0x18 = stored width copy
record[0]+0x1c = short title, copied from BMP file title, about 9 bytes plus padding
record[0]+0x28 = stale runtime pixel pointer, ignore on disk
pixelByteCount = width * height for 8bpp cursor images
```

Sample first record:

```text
name: "tu_nie"
width: 30
height: 28
pixel bytes: 840
```

After the cursor bitmap records, `GraphBrdDoc_SerializeCursorAndGroupState` delegates only to the group component list at document field `+0xa8`.

## `.EXS`, `.EXS_`, `.AVI`

Sample `.EXS_` (`RZECZKA.EXS_`) begins with a normal `RIFF/WAVE` header. The observed `.EXS` files are likely direct audio assets rather than GraphBoard archive containers.

`.AVI` files are standard AVI assets and are referenced by video-capable components.

## `Transparent_Video_Holder` Board Video Audio

Targeted check in `TransparentVideoHolder.dll`:

- AVI import path: `TransparentVideoHolder.dll:FUN_100020e0`.
- Holder serialize/load routine: `TransparentVideoHolder.dll:10007af0`, manually defined in Ghidra as `TVH_SerializeOrDeserialize`.
- Board-video stream setup/write helpers: `FUN_1000a620`, `FUN_1000a880`, `FUN_1000aab0`.
- Audio rewrite helper: `FUN_1000ab90`.
- Source-style C++ reconstruction: `ghidra_import/TransparentVideoHolder_reconstructed.cpp`.

The component-private block stores one or more custom streams whose header starts with the ASCII string `Board Video File Ver:0`. The block begins with:

```text
0x00 u32 component-private version = 3
+0x04 u32 board-video entry count
```

Each entry is serialized as:

```text
u32 boardVideoByteSize
u8 boardVideoBytes[boardVideoByteSize]  // begins with BoardVideoHeader[0x4e8], then chunk records
u8 holderEntry[0x568]                   // also begins with a BoardVideoHeader copy
u8 stillFrame[((width * 8 + 31) >> 3 & ~3) * height]
```

Important holder-entry fields, relative to `holderEntry`:

```text
+0x080 u32 width
+0x084 u32 height
+0x508 i32 originalX
+0x50c i32 originalY
+0x510 i32 originalZ
+0x54c i32 current/stage X
+0x550 i32 current/stage Y
+0x554 i32 current/stage Z
+0x558 i32 backing video start offset
+0x55c i32 backing video end offset
+0x560 void* runtime still-frame buffer, reallocated on load
+0x564 void* runtime scratch/secondary buffer, reallocated on load
```

This explains the previously observed "paired" `Board Video File` signatures: the first signature is the playable chunk stream, and the second is the serialized 0x568-byte holder-entry header copy. `graphboard_export_scene.py` uses the holder-entry `+0x54c/+0x550/+0x554` fields to place TransparentVideoHolder GIF layers in the HTML port.

Every board-video header starts at the `Board Video File Ver:0` string and has a 0x4e8-byte fixed header. Relevant header fields, relative to the string:

```text
+0x68 u32 magic = 0xada77777
+0x80 u32 frame width
+0x84 u32 frame height
+0x88 u32 chunk-record count
+0x8c u32 frame count
+0x90 u32 audio sample rate        // 22050 in RZECZKA
+0x94 u32 bits per sample          // 16 in RZECZKA
+0x98 u32 channels                 // 1 in RZECZKA
+0xe0 u32 audio byte rate          // 44100 in RZECZKA
+0xe4 u32 maximum compressed video chunk size
```

Chunk records begin at `header + 0x4e8`:

```text
u32 recordByteSize
u32 recordTag
u32 nextRecordByteSize
...
```

Known record tags:

```text
0xff000001 video frame/chunk record
0xff000002 audio record
```

For audio records, `recordByteSize == audioByteRate + 0x4c` in the observed files. PCM bytes begin at `record + 0x48`, and the usable PCM length is `recordByteSize - 0x4c`. In `RZECZKA.BDF`, this produces ordinary 22050 Hz, mono, 16-bit signed PCM WAV files.

Video records use a simple changed-rectangle RLE stream. The current decoder treats `record + 0x14..0x24` as `left, top, right, bottom, unchangedFlag`, and decodes payload bytes from `record + 0x48` through `record + recordByteSize - 4`.

Ghidra confirms the matching encoder/decoder pair in `TransparentVideoHolder.dll`: `FUN_1000b370` writes the RLE payload, `FUN_1000b8a0` draws an unclipped payload into an 8-bpp bottom-up DIB, and `FUN_1000b670` handles clipped rectangles. The decoder skips pixels equal to the `transparentIndex` argument when that argument is at most `0xff`; callers pass `1000` to disable transparency.

RLE payload commands:

```text
0x00, u8 count, u8 value       repeated value run
0xff, u16 count, u8 value      long repeated value run
u8 count, u8 values[count]     literal run
```

Rows are traversed left-to-right inside the changed rectangle. GIF reconstruction now uses the dominant first-frame background index as transparency; this matches `RZECZKA` better than blindly trusting the header field for all streams.

`RZECZKA.BDF` `Transparent_Video_Holder` contains 8 serialized entries and therefore 16 visible `Board Video File` signatures when naively scanning for the marker. The extracted playable audio streams are:

```text
0x000010c3: 4 audio records, 4.0 seconds
0x0003fdd0: 6 audio records, 6.0 seconds
0x000a8ae2: 4 audio records, 4.0 seconds
0x000de9c9: 3 audio records, 3.0 seconds
0x00147dac: 9 audio records, 9.0 seconds
0x0022e5eb: 9 audio records, 9.0 seconds
0x0032f446: 10 audio records, 10.0 seconds
0x004b8fe0: 9 audio records, 9.0 seconds
```

`graphboard_extract_assets.py` now writes these as `board_video_pcm_wave` outputs under each page's `embedded` folder.

## `MultiBitmap` Bitmap Banks

No top-level `.GIF` or `.BMP` files were found in the DATA folder, and no plausible embedded GIF/BMP file signatures were found inside BDF files. The visible bitmap banks are stored in component-private `MultiBitmap` blocks instead.

Observed `MultiBitmap` layout:

```text
u32 version                         // observed: 1
u32 recordCount
repeat recordCount:
  u32 pixelByteCount
  u8  pixelOrPrivateBytes[pixelByteCount]
  u8  metadata[0xc0]
```

Useful metadata fields, relative to the 0xc0-byte metadata record:

```text
+0x10 u32 width
+0x14 u32 height
+0x28 char name[12]                 // cp1250-ish short name, NUL padded
+0x34 u32 original pixel pointer    // stale runtime pointer on disk
+0x38 u32 original metadata pointer // stale runtime pointer on disk
```

When `pixelByteCount == width * height`, the bytes are raw 8-bit indexed pixels and can be written as a BMP using the page palette. Some records have smaller private byte arrays where `pixelByteCount != width * height`; those are preserved as raw files for now.

Current extraction results across the DATA folder:

```text
multibitmap_bmp: 23
multibitmap_raw: 178
```

Pages with `MultiBitmap` records:

```text
ABEC_PS:   1 BMP
CUDA_PS1:  1 raw
CUDA_PS2:  1 raw
CUDA_PS3:  1 raw
RYCERZ1:   3 BMP, 9 raw
RZECZKA:  10 BMP, 10 raw
SLON:      7 BMP, 156 raw
TAN_PS:    1 BMP
ZOSIA_PS:  1 BMP
```

For `RZECZKA.BDF`, the first 10 `MultiBitmap` records are full 640x480 frames named `niespodzi`; the remaining 10 smaller records are named `przycisk`.

## Script State in `RZECZKA.BDF`

The page script text has a standard editor-text shape inside the script state:

```text
offset 0x00df6715: u32 version = 1
offset 0x00df6719: CString length prefix ff 77 4d
offset 0x00df671c: script text, 0x4d77 bytes
offset 0x00dfb493: script-engine index/runtime state
```

The script begins:

```text
//###########################################################################
//Script code
//###########################################################################
```

The bytes immediately before `0x00df6715` are the end of the `MultiBitmap` private payload. Earlier notes suspected a second component list here; `GraphBrdDoc_Construct` and `GraphBrdDoc_SerializePageData` now rule that out for page `.BDF` files.

`GraphBrdScriptEngine_Serialize` was recovered from the script engine vtable at `0x004344d8 + 8`, target `Tuwim.exe:0041aad0`. Ghidra did not initially define a function there; it was created as `GraphBrdScriptEngine_Serialize`.

Script engine state layout:

```text
u32 version                         // save path currently writes 4; RZECZKA sample is 2
u32 indexedBlockCount               // script engine field +0x11b0
u32 handlerOrOffset38
u32 handlerOrOffset3c
u32 handlerOrOffset30
u32 handlerOrOffset34
repeat indexedBlockCount:
  u32 startOffset
  u32 duplicateStartOffset           // save path writes the same field twice
  u32 caseOrTargetCount
  u32 endOrBodyOffset
  u32 auxOffset
  repeat caseOrTargetCount:
    u32 caseValueOrKind
    u32 targetOffset
u32 tableCount                       // script engine field +0x14d4
u32 tableA[tableCount]               // copied from engine +0x11b4
u32 tableB[tableCount]               // copied from engine +0x1344
if version > 1:
  u32 field44
  u32 field48
  u32 field4c
  u32 field50
if version > 2:
  u32 field54
if version > 3:
  u32 field58
```

`RZECZKA.BDF` script-engine state:

```text
offset: 0x00dfb493
version: 2
indexedBlockCount: 11
handlerOrOffset38: 0x4c0b
handlerOrOffset3c: 0
handlerOrOffset30: 0
handlerOrOffset34: 0x017e
tableCount: 1
tableA[0]: 0x0482
tableB[0]: 8
v2 tail fields: 0x4c8e, 0, 0x4d12, 0
state end: 0x00dfb74b
file bytes remaining after state: 4 zero bytes
```

## Ghidra MCP Notes

- The running server at `localhost:8089` exposes the Ghidra plugin HTTP endpoints directly.
- `/mcp/schema`, `/list_open_programs`, `/search_functions`, `/decompile_function`, and related endpoints work.
- `/mcp/instance_info` returned `404 No context found for request` in this session, but this did not block analysis.
- `Tuwim.exe` remains the primary program for host file-format recovery and has the recovered serializer labels.
- The Ghidra project also contains the GraphBoard component DLLs under `/extracted/GraphBoard 1.00`. The six DLLs relevant to `RZECZKA.BDF` can be opened when a component-private payload question requires them, but broad exploration should stay centered on `Tuwim.exe`.

## Open Questions

- Recover component-private serializers from:
  - `HotSpotHolder.dll`
  - `SpriteHolder.dll`
  - `TextHolder.dll`
  - `Sound.dll`
  - `MultiBmp.dll`
- Recover exact semantic names for `ScriptEngineState` fields and indexed block records.
- `START.PRJ` layout is now fully recovered and byte-exact (the former "audio
  manager trailing block" is actually the global script editor's text; the audio
  manager owns only `audioPresetIndex`). Remaining audio-manager work is to map
  the runtime meaning of `audioPresetIndex` and the `+0xbc` object's other fields.
- Validate cursor record fields at `+0x10/+0x14` against cursor selection/rendering code.
