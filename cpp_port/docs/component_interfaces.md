# Component Interfaces (both directions)

Each GraphBoard component exposes three interfaces, all recovered here:

- **`IObject`** — the shared base every component implements (host-facing
  lifecycle + raw input).
- **`IMyObjectProperty`** — the **incoming** methods a page script *calls*
  (`Sprite_Holder.MoveTo`, `Sound_Holder.PlayDSound`, …).
- **`IMyEvents`** — the **outgoing** events a component *fires* back into the
  page script (`Sprite_Holder.MouseClickOnDown`, `Transparent_Video_Holder.TheEnd`).

Two independent sources, which agree:

1. Each component DLL embeds a compiled MSFT **type library**; its name table
   lists every interface, method, property and parameter. `tools/extract_typelib.py`
   parses it straight from the DLL bytes. This is the authoritative, complete
   list and the source for the incoming-method tables below.
2. The `.BDF` component wrapper carries a host-cached reflection of the
   component's **event** typeinfo — `cpp_port` parses it
   (`ComponentWrapper::functions`, from `GraphBrdCntrItem_Serialize`), and
   `gbinspect PAGE.BDF` now emits it with names + argument signatures. This is
   the source for the event signatures below (the human name is the record's
   `displayName`, the signature is `descriptionOrHelpName`; event members carry
   `flags = 0x18`).

## Why this matters for the runtime

These names ARE the triggers. To make the runtime interactive
(`runtime/page`), the component→script direction becomes mechanical:

- A left click at (x, y) → hit-test the top-most enabled hotspot/sprite/bitmap
  containing the point → fire that component's click event with the item index
  (`HotSpot_Holder.LeftButtonClickOn(i)`, `Sprite_Holder.MouseClickOnDown(i)`,
  `MultiBitmap.MouseClickOnDown(i, x, y, deep)`).
- Pointer entering/leaving an item → `MouseMoveIn` / `MouseMoveOut(i)`.
- A board-video reaching its last frame → `Transparent_Video_Holder.TheEnd(id)`.
- A sprite animation completing → `Sprite_Holder.EndAnimation(id)`.
- A synchro-text mark / sound completing → `Text_Holder.PlayMark(id, mark)`,
  `Sound_Holder.EndPlaySound(id)`.

The signatures give the exact argument count/order to pass. The runtime only
fires an event if the page defines the matching handler (already known from
`discoverHandlers`).

## Incoming methods (script -> component)

The methods a page script *calls* on a component, recovered from each DLL's
embedded MSFT type library (`tools/extract_typelib.py`) - the
`IMyObjectProperty` dispinterface. These resolve at runtime through the
component's `IDispatch::GetIDsOfNames` + `Invoke`, not the host-cached event
reflection.

### Shared base interface — `IObject` (every component)

Host-facing lifecycle + raw input, dispatched by the board, not usually by
script name:
```
IsYou
MinMaxDeep
Draw
SetBufferSize
EditProperties
ConnectToBoard
ConnnectionPoint
ComponentInfo
SetObjectMode
Serialize
LButtonDown
LButtonUp
MouseMove
LButtonDblClk
RButtonDown
RButtonUp
OnKeyDown
OnKeyUp
CheckRect
```

### HotSpot_Holder
```
EnableHotSpot
DisableHotSpot
MoveTo
PointInHotSpot
```

### Sprite_Holder
```
GotoXY
ChangePhase
MoveTo
ShowSprite
HideSprite
StopAnimation
ContinueAnimation
SetDeep
GetPosition
DisableDragMode
DisableTimers
EnableTimers
SynchronizeTimers
CompEnableDrag
CompDisableDrag
```

### Transparent_Video_Holder
```
Play
Stop
MoveTo
ShowFirsLastVideoFrame
HideFirsLastVideoFrame
ResetVideo
SetDeep
GetPosition
GetDeep
```

### Video_Holder
```
Play
PlayFromTo
Stop
NewOpen
NewPlay
NewPlayFromTo
NewStop
NewClose
```

### Text_Holder
```
ShowText
HideText
SetText
SetTextColor
StartScroll
StopScroll
ScrollUp
ScrollDown
SetTextOffsets
GetTextSize
MoveTextToPosition
PlaySynchroText
StopSynchroText
Pause
ContinuePlay
SetCurrentSelect
GetTextOffsets
SetSoundParameters
SetPosition
EnableMouse
DisableMouse
```

### Sound_Holder
```
PlayDSound
PlayDSoundEx
SetPlayDSoundParameters
Stop
StopAll
PlayExtern
PlayExternEx
```

### MultiBitmap
```
ShowBitmap
HideBitmap
MoveTo
```

### Bitmap_Holder
```
ShowBitmap
HideBitmap
MoveTo
GetPixel
EnableDrag
DisableDrag
```

### Puzzle
```
OpenPuzzle
ClosePuzzle
Mix
ShowFullBitmap
EndShowFullBitmap
```

### Panorama
```
OpenPanorama
ClosePanorama
RemoveSprite
StartSpriteAnimation
ResetSprite
Enable
Disable
SetHorAngle
SetVertOffset
SetZoom
```

### Panorama_Holder
```
OpenPanorama
ClosePanorama
ZoomIn
ZoomOut
GoLeft
GoRight
GoTop
GoBottom
Stop
SetHorizontalAngle
SetVerticalAngle
ShowSprite
HideSprite
ResetSprite
StartSpriteAnimation
SetZoom
```

### Recorder
```
OpenFile
EmptyFile
IsEmpty
Record
Stop
Play
CloseFile
```

## Events catalog

### HotSpot_Holder
```
LeftButtonClickOn(int rectID)
RightButtonClickOn(int rectID)
DBLClickOn(int rectID)
LeftButtonClickOnUp(int rectID)
MouseMoveIn(int rectID)
MouseMoveOut(int rectID)
```

### Sprite_Holder
```
MouseClickOnDown(int spriteID)
MouseClickOnUp(int spriteID)
MouseMoveIn(int spriteID)
MouseMoveOut(int spriteID)
MouseDrop(int spriteID,int left,int top,int right,int bottom)
InPlace(int spriteID,int x,int y)
Crash(int spriteID,int x,int y,int angle)
EndAnimation(int spriteID)
```

### Transparent_Video_Holder
```
TheEnd(int videoID)
MouseClickOnDown(int videoID)
MouseClickOnUp(int videoID)
MouseMoveIn(int videoID)
MouseMoveOut(int videoID)
```

### Video_Holder
```
TheEnd(int videoID)
MouseClickOn(int videoID)
```

### Text_Holder
```
ClickOnText(int textID)
EndOfSynchroText(int textID)
PlayMark(int textID,int markNo)
ScrollTextUp(int textID)
ScrollTextDown(int textID)
EndScrollText(int textID)
MouseMoveIn(int textID)
MouseMoveOut(int textID)
```

### Sound_Holder
```
EndPlaySound(int soundID)
```

### MultiBitmap
```
MouseClickOnDown(int bitmapID,int x,int y,int deep)
MouseClickOnUp(int bitmapID)
MouseMoveIn(int bitmapID)
MouseMoveOut(int bitmapID)
```

### Bitmap_Holder
```
MouseClickOnDown(int bitmapID)
MouseClickOnUp(int bitmapID)
MouseMoveIn(int bitmapID)
MouseMoveOut(int bitmapID)
MouseDrop(int bitmapID,int left,int top,int right,int bottom)
```

### Puzzle
```
MouseMoveIn()
MouseMoveOut()
MouseStartDrag()
MouseDrop()
ChipLock()
GameOver(int puzzleID)
EndShowBitmap(int puzzleID)
```

### Recorder
```
DiskFull()
EndRecordSound()
EndPlaySound()
Progress(int percentFull)
```

### Panorama
```
ClickOnPanorama(int panoramaNo)
ClickOnSprite(int panoramaNo,int spriteNo)
SpriteEndAnimation(int panoramaNo,int spriteNo)
SpriteMouseMoveIn(int panoramaNo,int spriteNo)
SpriteMouseMoveOut()
ZoomIn(int panoramaNo)
ZoomOut(int panoramaNo)
ZoomEnd()
MoveLeft(int panoramaNo)
MoveRight(int panoramaNo)
MoveTop(int panoramaNo)
MoveBottom(int panoramaNo)
HorMoveEnd()
VertMoveEnd()
```

### Panorama_Holder
```
ClickOnPanorama(int panoramaNo,int x,int y)
ClickOnHotspot(int panoramaNo,int hotspotNo,int x,int y)
ClickOnSprite(int panoramaNo,int spriteNo,int x,int y)
DblClickOnSprite(int panoramaNo,int spriteNo)
DblClickOnHotspot(int panoramaNo,int hotspotNo)
SpriteEndAnimation(int panoramaNo,int spriteNo)
SpriteMouseMoveIn(int panoramaNo,int spriteNo)
SpriteMouseMoveOut(int panoramaNo,int spriteNo)
HotspotMouseMoveIn(int panoramaNo,int hotspotNo)
HotspotMouseMoveOut(int panoramaNo,int hotspotNo)
ZoomIn(int panoramaNo)
ZoomOut(int panoramaNo)
ZoomEnd(int panoramaNo)
MoveLeft(int panoramaNo)
MoveRight(int panoramaNo)
MoveTop(int panoramaNo)
MoveBottom(int panoramaNo)
MoveEnd(int panoramaNo)
```

## How the board routes raw input (recovered 2026-07-10)

Read out of `SpriteHolder.dll` and `HotSpotHolder.dll` with Ghidra, plus
`Tuwim.exe`. Every component implements a common `IObject` vtable; the slots
that matter here, given the interface pointer `B`:

| slot | meaning | SpriteHolder | HotSpotHolder |
|------|---------|--------------|---------------|
| `B+0x1c` | `IsYou(x, y, deep, BOOL* out)` — pure query | `10007fa0` | `100047c0` |
| `B+0x20` | `MinMaxDeep(int* outMin, int* outMax)` | `10008240` | `10004930` |
| `B+0x44` | `Serialize` | `10008930` | `10004d10` |
| `B+0x48` | `LButtonDown(POINT* pt, int deep, BOOL* handled)` | `10008c80` | `100050a0` |
| `B+0x6c` | the slot `GraphBrdViewCallback_HitTestComponents` calls | `10008090` | — |

Anchors: `B+0x20` is fixed by `GraphBrdViewCallback_RecomputeComponentTimingBounds`
(Tuwim.exe `0040cda0`), which calls it with two out-params and folds the results
into `doc+0x100`/`doc+0x104` — the page's min/max layer (`kOffPageMinLayer`).
`B+0x44` is fixed by both DLLs' known serializers.

**The board dispatches raw mouse input one layer at a time.** `LButtonDown`
receives the current layer in `deep`, and each holder considers *only* items
whose layer field equals it:

- `Sprite_Holder`: instance `+0x18` is the layer. (`MinMaxDeep` scans exactly
  this field for its min/max, which is what proves the field's meaning.)
- `HotSpot_Holder`: record `+0x1c` is the layer.
- `Bitmap_Holder`: record `+0x18` is the layer
  (`BitmapHolder_LButtonDown_HitTestAndFireClick` @ 10003f50).
- `MultiBitmap`: record `+0x18` is the layer
  (`MultiBmp_LButtonDown_HitTestAndFireClick` @ 100047a5).

Per-holder differences within that common frame, all read from the DLLs:

| holder | rect edges | gate before the click | callback | id passed |
|--------|-----------|-----------------------|----------|-----------|
| HotSpot_Holder | all inclusive | `enabled` (+0x20) | `LeftButtonClickOn` / `RightButtonClickOn` | stored id (+0x18) |
| Sprite_Holder | all inclusive | `visible` (+0x88) | `MouseClickOnDown` | instance index |
| Bitmap_Holder | `PtInRect` (r/b exclusive) | *none* | `MouseClickOnDown` | record index |
| MultiBitmap | `PtInRect` (r/b exclusive) | `shown` (+0x3c) | `MouseClickOnDown(id, x, y, deep)` | record index |

`Bitmap_Holder` has no right-click event and no visibility gate in the click
path. `MultiBitmap`'s callback is the only 4-arg one, and its `x`/`y` are the
bitmap's own `left`/`top` (record +0x08/+0x0c), not the mouse point; `deep` is
its layer.

**Bitmap_Holder pixel data lives at `blob+0x80`**, not `+0x90`. The blob is
`[header .. 0x80)`, then `stride*height` pixel bytes (`stride = (width+3)&~3`,
bottom-up, transparent index at `blob+0x04`), then a `0x10` trailer — so
`blobByteCount == 0x90 + stride*height`, which is what made `+0x90` look like the
start. It isn't: reading there shears each row 16 bytes (the `guzik` button's
horizontal symmetry falls from 97% to 55%). `graphboard_extract_assets.py` still
reads at `+0x90` and so emits sheared bitmap PNGs — a latent bug to fix there.
(Sprites differ: their per-phase pixels are at `blob+0xb8 + frame[0x48]`.)

Since the document keeps the aggregate min/max layer, the board walks layers
top-down and stops at the first component that sets `*handled` — so the highest
layer containing the point wins, and on a tie the earlier component in the list
order wins. (`HitTestComponents` shows the list order explicitly: page list
first, then group list.)

**Within a holder, items are scanned last → first.** Both `LButtonDown`
implementations count down from `count-1` and return on the first match, so a
later item wins an overlap on the same layer.

**Hit rectangles.** The *click* path and the *query* path do not agree on edges:

| | `LButtonDown` (click routing) | `IsYou` (query) |
|---|---|---|
| HotSpot | `left<=x<=right && top<=y<=bottom` | `left<=x<=right && top<=y<bottom` |
| Sprite | `x<=px<=x+w && y<=py<=y+h` | `x<=px<x+w && y<=py<y+h` |

i.e. `IsYou` is exclusive on the right/bottom edge, `LButtonDown` is inclusive
on every edge. `IsYou` additionally treats `deep < 0` as "any layer", checks the
sprite's visible flag (instance `+0x88`) and skips the holder's excluded index
(`holder+0x508`).

For a sprite, `w`/`h` come from the **current phase's** frame record, not the
definition header: `frame = def + 0x6c + phase*0x4c`, with `w` at `frame+0x14`
and `h` at `frame+0x18`. (For phase 0 that is `def+0x80`/`def+0x84`, which is
what `SpriteDefinition::width/height` already parse.) The instance's live
position is a float read through `ftol()`; the *on-disk* `posX`/`posY` at
instance `+0x08`/`+0x0c` are plain `int32` — confirmed against RZECZKA.BDF,
where they hold sensible coordinates (707, 18, 710, 107, …) that would be
denormal garbage if reinterpreted as floats.

The frame table's length is `def+0x00`, the **phase count**. Verified on
RZECZKA.BDF: definitions declaring 1, 2, 2, 1 and 13 phases have exactly that
many well-formed frame records at `+0x6c` stride `0x4c` (the bytes after the
last one are pixel data and decode as garbage). This supersedes the heuristic
`infer_phase_count` in `graphboard_extract_assets.py`, which guesses the count
from `blob_size / (width*height)` and assumes every phase shares one size — the
table permits per-phase dimensions.

Sprite instance fields used by the click path, all verified against
RZECZKA.BDF (layers `0,8,8,1,2`; visible `1,1,1,0,0`):

| offset | meaning |
|--------|---------|
| `+0x00` | definition index (a pointer once loaded) |
| `+0x04` | layer, mirrored to `+0x18` on load |
| `+0x08` / `+0x0c` | position x / y (`int32`) |
| `+0x18` | layer ("deep") — the field `MinMaxDeep` scans |
| `+0x5c` | current phase |
| `+0x88` | visible flag |

A sprite's script-visible id is simply its **instance index**: the loop counter
`LButtonDown` counts down is handed straight to the callback. There is no stored
id field as there is for hotspots.

**Per-pixel transparency test.** After a rect hit, `SpriteHolder::LButtonDown`
looks up the pixel under the click and treats a transparent pixel as a miss —
this is what makes an irregular sprite (a butterfly, a rounded key) clickable
only on its opaque body. It runs only when **both** the definition and the frame
opt in: `def+0x24 != 0` **and** `frame+0x08 != 0`. That is 112 of the 913 sprite
definitions in the title (e.g. CUDA's `motylek`, `klawisz`); the other 801 are
rect-only. The lookup:

```
stride   = (frame[0x10] + 3) & ~3          // byte pitch; frame[0x10] is the row width
pixels   = blob + 0xb8 + frame[0x48]        // start of this phase's pixel plane
row      = height - 1 - (clickY - top)      // bottom-up storage
byte     = pixels[row*stride + (clickX - left) + instance[0x54]*width]
hit      = byte != frame[0x04]              // frame[0x04] is the transparent index
```

`instance+0x54` is a horizontal source offset, but it is 0 across every one of
the 919 shipped instances, so the mask depends only on the definition. Two
independent checks confirm the layout: rendering `motylek`'s mask draws the
butterfly silhouette exactly (transparent corners, ~46% opaque), and the pixel
plane it selects agrees with the asset extractor's `blob_end - width*height`
reasoning (`0xb8 + frame[0x48]` lands at the end of the frame table, where the
pixels begin).

### Hotspots are addressed by a stored id, not their array index

`HotSpotHolder::LButtonDown` fires the click callback with **record `+0x18`**,
and `IsYou` / `EnableHotSpot` / `DisableHotSpot` match against the same field.
It is *not* the position in the array. RZECZKA.BDF:

| array index | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
|---|---|---|---|---|---|---|---|---|
| stored id (`+0x18`) | 0 | 1 | 3 | 4 | 6 | 5 | 7 | 8 |

So `2` is absent and `5`/`6` are transposed. Passing the index instead of the id
silently mis-fires every script `switch(rectID)` from index 2 onward — e.g.
clicking RZECZKA's `(434,241)-(528,319)` rect (index 3, id 4) must fire
`LeftButtonClickOn(4)` and play video 4, not video 3. `HotSpot::id` carries this
field, and `runtime::Page` keys its hotspot item state by id.

Note the record's `enabled` flag is `+0x20`, checked in the runtime branch of
`LButtonDown` before the callback fires.

## Notes

- The `.BDF` wrapper only caches the **event** typeinfo (`flags = 0x18`); the
  incoming methods above come from the DLL type library instead.
  `GraphBrdScript_InvokeComponentDispatchMethod` (Tuwim.exe 00419950) resolves a
  script `Component.Method(args)` call by name against the component's live
  `IDispatch` (`GetIDsOfNames` → `Invoke`) — the type library is exactly that
  name table.
- Script names sometimes differ from the runtime's generic modelling: sprites
  use `ShowSprite`/`HideSprite` (not `ShowBitmap`), video/TVH use `Play`/`Stop`,
  panoramas open with `OpenPanorama`. `runtime/page` can now dispatch these by
  their real names.
- Still deeper (optional): the full `FUNCDESC` for each incoming method
  (parameter types + `[out]`/`[in]`, DISPIDs). The parameter *names* are already
  in the type library name table (e.g. sprite `phase`, `x`, `y`, `deep`,
  `ref_x`); wiring them to methods means walking the typelib `FUNCDESC` records,
  not just the name table `extract_typelib.py` reads today.
