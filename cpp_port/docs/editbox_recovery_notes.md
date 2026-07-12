# EditBox.dll recovery notes

Recovered read-only from `EditBox.dll` (GraphBoard 1.00, Ghidra project, image
base `0x10000000`). **Caveat:** no `.BDF` in the Tuwim title — nor any other
title available here — instantiates EditBox, so unlike the shipped holders
(HotSpot/Sprite/Bitmap/Video/…, all verified byte-exact against real files) the
serialized format below is reconstructed from the DLL only and **cannot be
verified against real data**. Treat the on-disk layout as provisional.

## What it is

EditBox is a **fill-in-the-blank / arrange-the-letters text component**. A
template holds display text with embedded editable *fields*; the child fills or
rearranges characters in those fields. It is not a plain edit control — the
recovered click path is `EditBox_OnLButtonUp_CommitDragOrCharSwap`
(`10007420`), i.e. the child can **drag characters between blanks and swap
them**, the classic "put the letters in order" exercise.

- ProgID `CEditBox.CEditBox1` (info string: `"To jest informacja nt. obiektu
  EditBox"`).
- Depends on **FontControl.dll** (`"Can't find FontControl component"`); glyphs
  are laid out and measured through the FontControl interface (vtable slot
  `+0x4c` selects a font by index), exactly like `Text_Holder`.
- Renders palettised text into a DIB (`CreateCompatibleBitmap/DC`, `DrawTextA`,
  `GetDIBits`, `SelectPalette`/`RealizePalette`), transparent index sentinel
  `1000` (the same "no transparent" marker the other holders use).
- Has a timer window (`"EBTimer Wnd"`, `SetTimer`/`KillTimer`) — caret blink /
  field animation.
- A sibling, **EditBox1.dll** (`Create_EditBox1ComObject`), is a simpler variant
  that edits through real Win32 child edit windows (`EditBox1FieldEditWindow`,
  `CommitWindowTextAndNotifyLayout`) rather than in-DIB character dragging.

## Template markup

The field syntax is embedded in each entry's primary text as bracketed markers
(format strings `"[0/l%d %s]"`, `"[0/l%d ]"`, `"[0/l%d "`):

```
[0/l<len> <content>]
```

`ParseAndSelectFromTemplateText` (`100060b0`) walks the primary text char by
char, tracks `[` … `]` nesting (`bracketCloseCount`/`expectedCloseCount`), reads
the `/l<digits>` length option (`isdigit`/`atoi`), and lays out each glyph at an
accumulating (x,y) using FontControl extents plus the entry's scroll offset.
Bracketed spans become the editable fields; everything else is static text.

## In-memory model (recovered offsets)

Model object (`EditBoxContext` / "Editor"):

| Offset | Meaning |
|--------|---------|
| `+0x44` | base of the template-entry pointer array (`entry* slots[]`) |
| `+0x10c` | template-entry count / current index |
| `+0x1fc` (`fontControlInterface`) | FontControl COM interface pointer |
| `+0x208` | render scratch (reset to 0 before a parse) |

Template entry — `operator_new(0x288)` in `EditBox_AllocateTemplateEntry`
(`10009080`), zero-initialised (0xa2 dwords):

| Offset | Meaning |
|--------|---------|
| `+0x08` | display `RECT` (left,top,right,bottom) via `SetRect(x,y,x+w,y+h)` |
| `+0x228` | flag, init 1 |
| `+0x22c` | flag, init 1 (`hasTextContent`-style gate on parse) |
| `+0x230` | `CString* pPrimaryText` — the display text with `[0/l…]` markup |
| `+0x234` | `CString*` (secondary — user entry / answer key) |
| `+0x238` | `CString*` (tertiary — style/format ref) |
| `+0x244` | flag, init 0 |
| … | `fontIndex`, `nScrollXOffset`, `nScrollYOffset`, per-glyph `data[]` |

Three heap `CString`s are constructed per entry, matching the three text slots.

## Interface / behaviour surface

- `AddNewTemplateEntry` (`1000b770`), `DeleteTemplate` (`1000b4c0`),
  `MoveTemplateToFront/Back` (`1000b650`/`1000b6c0`) — editor-side field
  management (these live in the property-sheet path, `EditBoxFieldPropertyPage`,
  `ShowEditBoxPropertySheetModal` @ `100057d0`).
- `HitTestAndSelectTemplateField` (`100066c0`) + `SelectTemplateField`
  (`10005f60`) — `PtInRect` over the field rects, pick the active field.
- `EditBox_OnLButtonUp_CommitDragOrCharSwap` (`10007420`) — commit a typed/
  dragged character or swap two characters between positions.
- `RenderFormattedText` (`10004350`) — lay out + blit the entry's text (static +
  filled fields) into the board DIB.

## Reconstruction plan (if a general GraphBoard player wants it)

1. Register the EditBox CLSID in `holders` (CLSID still to be pulled from the
   DLL's `DllRegisterServer`; ProgID `CEditBox.CEditBox1`).
2. Model the entry: `RECT`, `fontIndex`, three cp1250 strings, scroll offsets,
   flags. Reuse the existing `Text_Holder` + FontControl glyph path for layout.
3. Port the `[0/l<len> <content>]` parser to split static text from fields.
4. Runtime: hit-test field rects on click; model type / drag / swap into the
   selected field; re-render.
5. **Blocked on verification** until a `.BDF` (or `.grp`) that actually contains
   an EditBox private block is found — the byte layout above is unconfirmed.

## Bottom line

The runtime model and the template markup are recovered and coherent, and the
component is a bounded, holder-sized job (472 functions, but most are MFC
property-sheet UI that a headless player skips). The blocker is not difficulty —
it is that **no available data exercises it**, so it can be reconstructed but not
proven, and it adds nothing to the Tuwim port specifically.
