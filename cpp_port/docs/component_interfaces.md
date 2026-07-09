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
