# GraphBoard Studio writer recovery note

This note records the targeted Ghidra confirmation pass used by the native
authoring writer. The checked-in source-style host reconstructions remain the
authority for `START.PRJ`, BDF/GRP framing, component wrappers, script text,
schema-4 engine state, and the trailing zero `COleDocument` word. Addresses are
image-base virtual addresses in the named 32-bit DLL.

For newly authored fixed records, every byte not named below is initialized to
zero; recovered non-zero constructor defaults are then written explicitly.
Imported editable holders retain their complete original private byte block
until that holder is changed, while known non-authorable holders always retain
it. This keeps pointer-shaped/cache/reserved bytes deterministic for new data
and lossless for legacy data.

## SpriteHolder.dll

- `10008930` serializes private schema `1`, definition count, instance count,
  each length-prefixed definition blob, then `0x8c` bytes per instance. The
  definition reference at instance `+0x00` is an array index on disk and a
  pointer in memory.
- `100052f0` creates a zeroed `0xbc` definition and initializes `+0x18 = 1`
  and `+0x2c = 1`. The fixed definition name begins at `+0x04`.
- `10004170` creates a zeroed `0x8c` instance. It assigns the definition/index,
  initializes `+0x10 = 1`, and initializes `+0x88 = 1` (visible).
- Authored definition blobs use a frame table at `+0x6c`, `0x4c` bytes per
  frame, and indexed bottom-up scanlines beginning no earlier than `+0xb8`.
  Frame transparency is `+0x04`, pixel hit-test is `+0x08`, width is
  `+0x10/+0x14`, height is `+0x18`, duration is `+0x20`, and the pixel offset
  relative to `+0xb8` is `+0x48`.
- Authored instances write layer `+0x04/+0x18`, X/Y `+0x08/+0x0c`, draggable
  `+0x1c`, phase `+0x5c`, and visible `+0x88`.

## HotSpotHolder.dll

- `10004d10` writes schema `0`, the three holder bytes, count, one `0x64`
  record plus MFC `CString` name per item, active index, and auxiliary word.
- The constructor at `100045b0` confirms blank-holder defaults
  `+0x1d0 = 0`, `+0x1d1 = 0`, `+0x1d2 = 0xff`, with active/auxiliary state
  initially zero.
- A record stores left/top/right/bottom at `+0x00/+0x04/+0x08/+0x0c`, id at
  `+0x18`, layer at `+0x1c`, and enabled at `+0x20`. `+0x10` is pointer-shaped
  process state and is emitted as zero before the following `CString`.

## MultiBmp.dll

- `10004353` writes schema `1`, count, then for each item a byte count, indexed
  pixel plane, and fixed `0xc0` metadata record. Loading clears metadata
  `+0x3c`; scripts establish visible state through `ShowBitmap`.
- Authored records use transparent index `+0x04`, rectangle
  `+0x08..+0x14`, layer `+0x18`, and a 12-byte fixed name at `+0x28`.
  Pixel planes are 4-byte-stride, bottom-up 8-bit indexed rows.

## BitmapHolder.dll

- `10003d20` writes schema `1`, count, then a byte count and self-contained
  blob per bitmap. The import/replace helper at `10005de0` confirms indexed
  pixels at `+0x80` and a copied metadata/header area.
- Authored blobs use transparency `+0x04`, rectangle `+0x08..+0x14`, layer
  `+0x18`, hit-test flags `+0x20/+0x28`, initially-hidden `+0x2c`, a 12-byte
  fixed name at `+0x34`, stage X/Y at `+0x40/+0x44`, a bottom-up pixel plane
  at `+0x80`, and a zero `0x10` trailer.

## Sound.dll

- The recovered serializer at `10004090` belongs to GraphBoard 1.00 and writes
  schema `1`, `0x6c` record bytes, and a trailing format block. The shipped
  Tuwim files are a later binary variant: schema `2`, `0x58` record bytes, and
  a zero-length trailing format block. Real Tuwim files and the reconstructed
  parser are therefore the writer authority for this holder.
- The entry creation path at `10007430` confirms embedded backing data and a
  fixed record name (the 1.00 layout places it at `+0x10`). The shipped schema-2
  record places absolute WAV start/end archive offsets at `+0x04/+0x08` and a
  32-byte fixed name at `+0x0c`.
- When an imported private block moves, both offsets are regenerated from the
  new component-private start. They point to the first WAV byte and one past
  the last WAV byte.

## VideoHolder.dll

- `10003e20` writes schema `1`, count, then one `0x6c` record and one MFC
  `CString` filename per item.
- Import at `100059e0` zeroes the complete record, stores X/Y at
  `+0x10/+0x14`, mirrors the entry index at `+0x18/+0x1c`, and stores a title
  of at most nine bytes at `+0x20`. The external AVI filename follows the
  fixed record as a `CString`.

## Wrappers, scripts, and relocation

The static wrapper catalog is generated from known CLSIDs and reflected
events/methods. A callback function value is not a code address: it is the byte
offset of the opening `(` in the handler declaration, for example
`HotSpot_Holder.LeftButtonClickOn(`. Offsets are rebuilt from the CP1250 script
on every semantic export.

Schema-4 script metadata is rebuilt with page-event offsets, nested switch/case
blocks, and host builtin call offsets/kinds. The on-disk caches are capped at
100 records, matching the host arrays. `OnOpenPage(void)` and
`OnClosePage(void)` are mandatory validation targets.

All offsets are archive-absolute unless a recovered holder explicitly defines
them as relative. In addition to Sound ranges, preserved Transparent Video
entries relocate `+0x558/+0x55c` to the regenerated BoardVideo stream start/end.
Component order and within-holder item order are stable because equal-layer
rendering and hit testing depend on them.
