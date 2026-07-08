# Holder Recovery Notes for the Implementing Agent

Date: 2026-07-08. Recovered read-only from the Ghidra MCP server at
`localhost:8089` (programs `TextHolder.dll`, `Sound.dll`, `FontControl.dll`)
and validated byte-exact against `RZECZKA.BDF`. **Nothing was written to the
Ghidra project in this pass** — the function names cited below already existed.

## Goal

Implement the two missing component-private parsers so the `cpp_port` component
walker can traverse a full page:

- `Sound_Holder` (`parseSoundHolderState`)
- `Text_Holder` (`parseTextHolderState`, including its trailing FontControl block)

Once these land, `gbinspect` walks all six RZECZKA components end-to-end.
`HotSpot`, `Sprite`, `MultiBitmap`, and `TransparentVideoHolder` are already
implemented in `../src/holders.cpp` — follow their pattern:

1. Add the structs + doc comment to `../include/graphboard/holders.hpp`.
2. Add the parser to `../src/holders.cpp`; flip the registry entry's
   `privateStateParserImplemented` to `true`.
3. Add a dispatch branch in `../tools/gbinspect.cpp` (`componentListToJson`).
4. Add a synthetic round-trip test in `../tests/synthetic_parse.cpp`.
5. Verify against the real file (recipe at the bottom).

## Sound_Holder — VERIFIED layout

Serializer: `SoundHolder_SerializeSoundData` @ `Sound.dll:10004090`
(the DLL under `/extracted/GraphBoard 1.00/`). Same cache-file pattern as
TransparentVideoHolder: sounds live in a backing file at runtime; on save each
sound's byte range is copied into the archive.

On-disk layout, **verified walking all 14 RZECZKA sounds** (private block
`0x648c9c` → ends exactly at the MultiBitmap wrapper `0xb04568`):

```text
u32 version              // RZECZKA: 2. Load path SKIPS it (never branches on it)
u32 soundCount           // holder+0x1d8; RZECZKA: 14
repeat soundCount:
  u32 soundByteCount     // RZECZKA snd0: 0x8452
  u8  soundBytes[soundByteCount]   // a complete RIFF/WAVE file, verbatim
  u8  trailer[0x5c]      // see below
```

Trailer fields verified in RZECZKA (offsets relative to trailer start):

```text
+0x00 u32 stale pointer (waveformat ptr; 0 in RZECZKA)
+0x04 u32 archive offset of this sound's first RIFF byte   (snd0: 0x00648ca8)
+0x08 u32 archive offset just past the sound bytes         (snd0: 0x006510fa)
+0x0c char name[]        // NUL-padded ("rzeczka -" in snd0); exact field width
                         // unconfirmed, zeros continue to +0x58
+0x58 u32 waveFormatByteCount   // 0 in ALL 14 RZECZKA sounds
      u8  waveFormat[waveFormatByteCount]
```

**Version-skew warning:** the Ghidra copy of `Sound.dll` (GraphBoard 1.00,
Program Files) writes a **0x6c-byte** entry record with the archive offsets at
record+0x08/+0x0c, and always emits a nonzero `waveFormatByteCount`
(`WAVEFORMATEX.cbSize + 0x12`). The DLL that wrote RZECZKA.BDF in 1997 used the
**0x58-byte record + u32 wfSize** framing above. A robust parser should read
`u32 size + WAV + 0x58 record + u32 wfSize + wfSize bytes`; that handles both
if the newer files simply carry nonzero wfSize. If a file from the newer DLL
mis-walks, re-derive the record size the same way we did: brute-force the
trailer size that makes every next `u32 size` land on a `RIFF` tag and the last
entry end at the next wrapper CLSID.

Semantics from the decompile (matters for a future runtime, not for walking):
`GetObjectSchema` selects cache strategy — schema 0 copies sound bytes into a
temp file (`SoundArchive_ReadFileRange`); nonzero schema reopens the archive
file itself and just seeks past the bytes, keeping (start,end) offsets.

## Text_Holder — VERIFIED layout

Serializers: `TextHolder_SerializeDocument` @ `TextHolder.dll:10008200`,
`TextHolder_SerializeOneTextEntry` @ `TextHolder.dll:10007de0`.
Verified against RZECZKA (private block `0x642244`; text entries end `0x642508`;
the FontControl block then runs to exactly `0x64891c` = Sound wrapper).

```text
u32 version              // save writes 2; RZECZKA: 2
u32 textCount            // RZECZKA: 1
u8  color0, color1, color2, color3   // RZECZKA: 0x17, 0xbb, 0x37, 0x00
repeat textCount:        // TextHolder_SerializeOneTextEntry
  u8  entry[0xc4]                    // TextHolderTextEntry record (stale ptrs inside)
  if entry+0x70 (u32, pRenderCache) != 0:
    if entry+0x90 (u32, pSecondaryText) == 0:
      u32 streamByteCount            // render-object bytes (cache-file backed)
      u8  streamBytes[streamByteCount]
      u8  renderCache[0x68]          // +0x0c lineOffsetsPtr (stale), +0x10 lineCount
      if lineOffsetsPtr != 0 && lineCount != 0:
        u32 lineOffsets[lineCount]
    else:
      u8  renderCache[0x68]          // same +0x0c/+0x10 fields
      if lineOffsetsPtr != 0 && lineCount != 0:
        u32 lineOffsets[lineCount]   // absolute; load rebases by -renderCache[+0x04]
      CString secondaryText          // RZECZKA: "rzeczka.exs" (synchro audio ref)
  CString primaryText                // RZECZKA: the poem text, CRLF + cp1250
<FontControl block — see below; always appended when ctx->pFontControl != 0>
```

RZECZKA takes the `pSecondaryText != 0` branch (renderCache 0x68, lineCount 13,
13 u32 line offsets, then the two CStrings). The other branch (embedded render
object stream) is recovered from the decompile but **not yet byte-verified** —
find a page whose Text_Holder has `entry+0x90 == 0` to confirm before trusting
gbinspect output for it. Both stale pointers (+0x70/+0x90) act purely as
"present" flags for the disk format.

Note: whether the FontControl block is present depends on a runtime pointer
(`ctx->pFontControl`), not on a disk flag. In practice TextHolder always creates
its FontControl; treat the block as unconditional unless a counterexample shows
up (a missing block will mis-walk loudly, landing off the next wrapper CLSID).

## FontControl trailing block — VERIFIED layout

Serializer: `FontControl_SerializeFontSlots` @ `FontControl.dll:10004230`.
Called via FontControl vtable +0x2c at the end of `TextHolder_SerializeDocument`
(both save and load paths).

```text
repeat exactly 50 (0x32) slots:
  u32 present            // save writes the raw slot POINTER; nonzero = present
  if present != 0:
    u8 slotRecord[0xc84]
    // slotRecord fields: +0x000 u32 glyphPtr[256] (stale, nonzero = bitmap follows)
    //                    +0x800 glyph widths, u16 read at stride 4 (u16 at +0x800+4*g)
    //                    +0xc00 u32 slotHeight
    //                    +0xc28 u32 flag, forced to 1 on save
    repeat g in 0..255 where glyphPtr[g] != 0:
      u8 glyphBitmap[ ((width[g] + 3) & ~3) * slotHeight ]
```

RZECZKA: 2 slots present (heights 19 and 21, 43 glyph bitmaps each); the walk
lands on the Sound wrapper to the byte. Parser tip: the per-glyph "present"
flags and widths come from inside `slotRecord`, so parse it before the bitmaps.

## Verification recipe (RZECZKA.BDF ground truth)

| what | offset |
|---|---|
| Text_Holder private start | `0x642244` |
| text entries end / FontControl start | `0x642508` |
| FontControl end / Sound wrapper CLSID | `0x64891c` |
| Sound private start | `0x648c9c` |
| Sound private end / MultiBitmap wrapper CLSID | `0xb04568` |
| MultiBitmap private start | `0xb04955` |
| MultiBitmap end / page script text block | `0xdf6715` (starts `u32 1`, CString script) |

After implementing, run:

```powershell
cmake --build cpp_port/build/cmake
ctest --test-dir cpp_port/build/cmake
.\cpp_port\build\cmake\gbinspect.exe <DATA>\RZECZKA.BDF
```

and check `componentsParsed == 6` with no stop note. Then spot-check other
pages (`MURZYNEK.BDF` stops at its first TVH today only because RZECZKA-style
walking was the goal; it should walk further once Text/Sound land). The end of
the component list should sit exactly at the page script text block
(`u32 1`, then the script CString) — parse those two fields as a cheap
end-to-end assertion.

## Parser priority (full-DATA-folder survey, 2026-07-08)

Walking all 202 `*.BDF` files with the four implemented parsers, the histogram
of "which holder kind stopped the walk" is:

```text
Puzzle          102   (Puzzle.dll open in Ghidra)
Bitmap_Holder    45   (BitmapHolder.dll open in Ghidra)
Recorder         24   (Recorder.dll open in Ghidra)
Text_Holder      12   (recovered above - implement first, it unblocks RZECZKA)
Sound_Holder     12   (recovered above)
Panorama          3
Panorama_Holder   2
Video_Holder      1
```

So after Text/Sound, `Puzzle` and `Bitmap_Holder` are by far the highest-value
serializers to recover next. All six new CLSIDs are registered in
`../src/holders.cpp` with their wrapper display names; their implementing DLLs
are unconfirmed (match by name in the Ghidra project). One DATA file,
`KOTEK2.BDF`, is a misnamed 16-bit NE executable, not a board file — gbinspect
now rejects it by banner.

`tools/verify_scenes.ps1` asserts recovered ground truth for START.PRJ,
RZECZKA, GRZESIU, and INTRO — add each newly parsed scene there as its values
are verified by hand.

## Open questions for later passes

- Exact width of the Sound trailer name field (+0x0c..? — zeros in RZECZKA past
  the visible name; the Ghidra-copy record is 0x6c total, suggesting name may
  run to +0x58 in the old format too).
- Text entry record (0xc4) field semantics beyond +0x70/+0x90 (position, layer,
  font indices — `nPrimaryFontIndex`/`nSecondaryFontIndex` exist in the Ghidra
  typed struct; map them to disk offsets).
- The embedded-render-object branch of the text entry (stream path) needs a
  real-file example.
- Sprite definition blob: pixel/phase framing after the 0x108-ish header
  (RZECZKA def0: blobSize 0x96108, width 1280, height 480, pixels are the
  trailing width*height bytes; def3/def4 don't fit `width*height*phases` —
  the phase table (0x4c-byte entries) mentioned in older notes likely sits
  between header and pixels).
- MultiBitmap serializer function in `MultiBmp.dll` has not been located/named
  yet (layout is empirical); finding it would confirm the 0xc0 metadata record.
