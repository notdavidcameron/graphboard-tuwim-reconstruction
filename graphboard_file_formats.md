# GraphBoard File Format Notes

Continuation from the initial `Tuwim.exe` serializer reconstruction, now cross-checked against `RZECZKA.BDF` and the running Ghidra MCP server.

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
AudioManagerProjectState trailing block
```

Sample `START.PRJ` contains 34 page names, 6 group names, and 339 trailing bytes after the project manifest, attributed to the audio manager serialize call at document field `+0xa0`.

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

The page serializer calls `+0xa4`, `+0x9c`, then `+0xb4`. The group serializer below calls `+0xa8`; the group script editor is not part of the cursor/group serializer recovered so far.

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

## `HotSpot_Holder` Private State

Targeted Ghidra check in `/extracted/GraphBoard 1.00/HotSpotHolder.dll`:

- Object constructor: `Create_HotSpotHolderComObject` at `10002660`.
- Main component vtable: `10007d38`.
- Component serialize/load vtable slot `+0x44`: `HotSpotHolder_SerializePrivateState` at `10004d10`.

The component-private block begins with:

```text
+0x00 u32 versionOrReserved = 0
+0x04 u8  colorOrStateA
+0x05 u8  colorOrStateB
+0x06 u8  colorOrStateC
+0x07 u32 hotspotCount
```

Each hotspot is serialized as a raw 100-byte record followed by an MFC `CString` label/name:

```text
record+0x00 i32 left
record+0x04 i32 top
record+0x08 i32 right
record+0x0c i32 bottom
record+0x10 u32 staleCStringPointer   // ignored on load
record+0x18 i32 hotspotId
record+0x1c i32 zLayer
record+0x20 i32 enabled
...
CString label
```

After the repeated records there are two trailing `u32` values. The first often matches the next/upper hotspot id value; the second is usually zero in checked pages. `graphboard_export_scene.py` uses these records to recover exact browser hitboxes for `HotSpot_Holder` components.

## `Transparent_Video_Holder` Board Video Audio

Targeted check in `TransparentVideoHolder.dll`:

- AVI import path: `TransparentVideoHolder.dll:FUN_100020e0`.
- Holder serialize/load routine: `TransparentVideoHolder.dll:10007af0`, manually defined in Ghidra as `TVH_SerializeOrDeserialize`.
- Board-video stream setup/write helpers: `FUN_1000a620`, `FUN_1000a880`, `FUN_1000aab0`.
- Audio rewrite helper: `FUN_1000ab90`.

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

RLE payload commands:

```text
0x00, u8 count, u8 value       repeated value run
0xff, u16 count, u8 value      long repeated value run
u8 count, u8 values[count]     literal run
```

Rows are traversed left-to-right inside the changed rectangle. GIF reconstruction now uses the dominant first-frame background index as transparency; this matches `RZECZKA` better than blindly trusting the header field for all streams. The GIF writer uses that transparent index as the logical screen background and restore-to-background disposal so browser playback replaces old frame pixels instead of leaving trails.

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

## Live RZECZKA x32dbg Notes

Debug target was `Tuwim.exe` with `john-mayhem/x32dbgMCP` exposed at `http://127.0.0.1:8888`.

Useful loaded module bases from the live session:

```text
tuwim.exe                  0x00400000
TransparentVideoHolder.dll 0x05ff0000   // current restarted session
MultiBmp.dll               0x06a20000   // current restarted session
HotSpotHolder.dll          0x06030000
SpriteHolder.dll           0x10000000
WING32.DLL                 0x20000000
gdi32.dll                  0x75b80000
```

Confirmed breakpoint:

```text
0x05ff8120 = TransparentVideoHolder.dll + 0x8120 in the current session
label: TVH_Play_likely
source note: recovered FUN_10008120
```

Older notes in this file may mention `0x030a8120`; that was the same DLL-relative breakpoint from the previous load base.

This is the TransparentVideoHolder play path. At a hit during RZECZKA runtime, stack arguments identified `videoId = 0`, followed by later `videoId = 1`; entry count was `8`. The live holder entries matched exported geometry, for example:

```text
0: 197x190 at 1,166 z=3
1: 408x150 at 197,147 z=2
2: 248x85  at 392,220 z=2
```

This confirms RZECZKA automatically cycles the fish TVH videos in the original after startup; other click/event animations should stay hidden until their scripts show/play them.

Screenshot comparison from the original paused RZECZKA state shows the visible page is a full-screen MultiBitmap-like underwater scene plus TVH fish layers, not a flat palette background. A rough pixel match against exported `niespodzi` frames ranked `05_MultiBitmap_006_niespodzi.bmp`, `001`, and `000` closest, but the screenshot contains TVH overlays and cursor pixels, so debugger draw-state should be treated as final authority.

RZECZKA script evidence for the active background:

```text
OnOpenPage:
  int curView=0;
  int curmusic=3;
  Sound_Holder.PlayDSound(curmusic);
  SetTextMode();

SetTextMode, normal mode:
  Transparent_Video_Holder.Play(0,0);

surprise button path:
  MultiBitmap.ShowBitmap(curView+10,363,416,4); // small przycisk overlay
  curView++;
  if(curView>9) curView=0;
  curmusic=curView+3;
  MultiBitmap.ShowBitmap(curView,0,0,3);        // full-screen niespodzi frame
  hide previous curView frame
```

This means startup begins with `curView == 0`, and the `niespodzi` full-screen frames `0..9` are the rotating backgrounds. The original paused screenshot looked closest to exported frame `006`, but that state can be reached after the button cycle; startup should use frame `000` unless debugger state at page-open shows otherwise.

Final page-copy evidence:

```text
0x200012a0 = WING32.DLL!WinGBitBlt
hit args:
  hdcDest = 0x80010abe
  xDest,yDest = 960,480
  width,height = 640,480
  hdcSrc = 0x8d010b4e
  xSrc,ySrc = 0,0
  return = 0x0040c5b0

0x75b84960 = gdi32.dll!BitBlt, called inside WinG path
same effective copy:
  dest offset = 960,480
  size = 640x480
  source = 0x8d010b4e at 0,0
  ROP = 0x00cc0020 (SRCCOPY)
```

Breakpoint `0x0040c5b0` was set as `Tuwim_after_WinGBitBlt_page_copy`. At the hit:

```text
EIP = 0x0040c5b0
instruction = mov eax, dword ptr ds:[esi+0xD0]
ESI = 0x00dd63e8   // Tuwim view/page renderer object
EDI = 0x02dec2b4   // object whose +0x04/+0x08 are the destination HDC

[ESI+0x054] = 0x8d010b4e   // source/backbuffer HDC seen in WinGBitBlt args
[ESI+0x080] = 641
[ESI+0x084] = 483
[ESI+0x090] = 14
[ESI+0x094] = 960
[ESI+0x098] = 480
[ESI+0x09c] = 1600
[ESI+0x0a0] = 960
[ESI+0x0ac] = 960
[ESI+0x0b0] = 480
[ESI+0x0b8] = 640
[ESI+0x0bc] = 480

[EDI+0x04] = 0x80010abe
[EDI+0x08] = 0x80010abe
```

The immediate caller return site was `0x0040c8b1`. At that hit:

```text
EIP = 0x0040c8b1
ESI = 0x00dd63e8
EDI = 0x02dec2b4
EBP = 0x06777c20
ESP = 0x001afe30

stack tuwim.exe return/code pointers included:
  [ESP+0x14] = 0x00430358
  [ESP+0x54] = 0x004054bc
  [ESP+0x60] = 0x0043cf30
  [ESP+0x64] = 0x00405390
  [ESP+0x84] = 0x0042ebef
  [ESP+0x98] = 0x0042eda5
```

Disassembly around the higher-level caller shows:

```text
0040c850: and eax, 0
0040c856: sub esp, 8
...
0040c8a3: mov ecx, dword ptr ss:[esp+0x28]
0040c8a7: push ecx
0040c8a8: push edi
0040c8a9: push eax
0040c8aa: mov ecx, esi
0040c8ac: call 0x0040c2c0
0040c8b1: mov edx, dword ptr ds:[edi+0x04]
0040c8b4: mov eax, dword ptr ds:[esi+0x20]
0040c8b7: push edx
0040c8b8: push eax
0040c8b9: call dword ptr ds:[0x00432834]
```

Next breakpoint set:

```text
0x0040c8ac
label: Tuwim_before_page_copy_call
expected hit: EIP=0x0040c8ac just before `call 0x0040c2c0`;
              ECX should be loaded from ESI (the 0x00dd63e8 view object);
              continuing should usually hit 0x0040c5b0 inside/after the final copy.
```

Avoid broad API breakpoints unless conditionalized to `640x480`: tiny `WinGBitBlt` hits like `28x15` are cursor/UI noise. The useful final compositor path is the Tuwim call chain above, not the WinG/GDI API breakpoint itself.

## Open Questions

- Recover component-private serializers from:
  - `HotSpotHolder.dll`
  - `SpriteHolder.dll`
  - `TextHolder.dll`
  - `Sound.dll`
  - `MultiBmp.dll`
- Recover exact semantic names for `ScriptEngineState` fields and indexed block records.
- Continue the dedicated pass through `START.PRJ` audio manager state.
- Validate cursor record fields at `+0x10/+0x14` against cursor selection/rendering code.
