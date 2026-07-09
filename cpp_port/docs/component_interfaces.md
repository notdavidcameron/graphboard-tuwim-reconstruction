# Component Event Interfaces (component → script callbacks)

Every GraphBoard component wrapper carries a reflected copy of the component's
IDispatch typeinfo — `cpp_port` already parses it (`ComponentWrapper::functions`
/ `properties`, from `GraphBrdCntrItem_Serialize`). The event/source members are
the callbacks a component fires back into the page script; a page script binds
one by defining a top-level function named `Component_Name.Event(args)` (which
`discoverHandlers` classifies as a `ComponentCallback`).

This catalog is recovered directly from that reflected typeinfo — the human name
is the record's `displayName`, the argument list is `descriptionOrHelpName`, and
event members carry `flags = 0x18 (24)`. Dump it for any page with:

```
gbinspect PAGE.BDF   # each component now emits its functions[]/properties[]
```

The first seven functions of every component are the standard
IUnknown/IDispatch slots (QueryInterface … Invoke) and are omitted below.

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

## Catalog

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

## Open question: the incoming (script → component) method interface

The reflected `functions` table above is the **event/source** dispinterface
(all `flags = 0x18`). The methods a script *calls* — `Sprite_Holder.MoveTo`,
`ChangePhase`, `ShowBitmap`, `Sound_Holder.PlayDSound`, etc. — are NOT in this
reflected table. `GraphBrdScript_InvokeComponentDispatchMethod` (Tuwim.exe
00419950) resolves those by name against the component's live IDispatch
(`GetIDsOfNames` → `Invoke`), not the cached reflection. Recovering that full
incoming method list per component means reading each DLL's primary
`IDispatch::GetIDsOfNames` name table / typeinfo — the natural next
Ghidra-side task. Until then the runtime models the common incoming methods by
their (self-documenting) script names in `runtime/page`.
