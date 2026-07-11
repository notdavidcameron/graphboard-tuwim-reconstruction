# Important Ghidra Function Renames

Date: 2026-07-08

This note records high-value function names applied after the duplicate-DLL
merge pass. All targets are the kept `/extracted/GraphBoard 1.00/` component
programs.

## HotSpotHolder.dll

```text
10004d10 HotSpotHolder_SerializePrivateState
```

This is the component-private serializer for `HotSpot_Holder`. It matches the
layout documented in `graphboard_file_formats.md` and
`cpp_port/reconstructed/ComponentHolders_reconstructed.cpp`:

```text
u32 version
u8 flag0
u8 flag1
u8 flag2
u32 hotspotCount
repeat hotspotCount:
  u8 record[100]
  CString name
u32 selected/active index
u32 trailing field
```

## AudioHolder.dll

```text
10003ca0 AudioHolder_SerializePrivateState
```

This function was missing from Ghidra's function table and was created at the
aligned entry between `AudioArchive_ReadFileRange` and
`AudioHolder_StartPlaybackForEntry`. It reads/writes the component-private audio
entry table and backing temp/data file ranges. It calls:

```text
10003b40 AudioArchive_WriteFileRange
10003bf0 AudioArchive_ReadFileRange
```

The stored schema observed in the decompile is `2`, with each entry record
using a `0x5c`-byte payload after its backing byte range.

## VideoHolder.dll

The following previously missing functions were created and named:

```text
10004120 VideoHolder_OpenVideoWindowForEntry
10004190 VideoHolder_SetParentWindowPointer
100041b0 VideoHolder_SetEntryCallbackPointer
100041c0 VideoHolder_OpenAviFileForEntry
10004480 VideoHolder_OpenAviFileHiddenForEntry
```

`VideoHolder_OpenVideoWindowForEntry` creates the MCI window and installs
`VideoHolder_MciWindowSubclassProc`.

`VideoHolder_OpenAviFileForEntry` and
`VideoHolder_OpenAviFileHiddenForEntry` open the entry's AVI path through the
MCI window. The hidden/ranged variant additionally sets playback positions with
the caller-provided parameters before showing the window.

## Core holder UI/runtime rename pass

Applied with the local `bethington/ghidra-mcp` HTTP bridge at
`http://127.0.0.1:8089`, always using exact `/extracted/GraphBoard 1.00/...`
program paths because duplicate component programs are open in the project.

### AudioHolder.dll

These were high-confidence settings-page handlers around the already named
`AudioHolder_StartPlaybackForEntry` / `AudioHolder_StopPlaybackForEntry`
runtime functions:

```text
10005290 AudioSettingsPage_DoDataExchange
10005440 AudioSettingsPage_UpdateControls
10005f30 AudioSettingsPage_PlaySelectedAudio
10005f60 AudioSettingsPage_StopSelectedAudio
10006040 AudioSettingsPage_OnInitDialog
10006090 AudioSettingsPage_StopAllAudio
100060d0 AudioSettingsPage_OnTimer
```

Readback after the rename pass showed the default-name count drop from 85 to
78 for `/extracted/GraphBoard 1.00/AudioHolder.dll`, and the program was saved.

### TextHolder.dll

These were high-confidence text/settings and waveform-sync helpers. The
wave-sync helper is called by the existing `TextHolderEffectsPage_*` playback
and import handlers and builds the per-line/audio offset table:

```text
100074c0 TextHolderEffectsPage_BuildWaveSyncOffsets
1000c260 TextHolderSettingsPage_RebuildTextList
1000c390 TextHolderSettingsPage_DeleteSelectedText
1000c590 TextHolderSettingsPage_MoveSelectedTextToTop
1000c630 TextHolderSettingsPage_MoveSelectedTextToBottom
1000c6f0 TextHolderSettingsPage_AddTextEntry
1000c990 TextHolderSettingsPage_OnApply
1000ca00 TextHolderSettingsPage_OnInitDialog
1000ca60 TextHolderSettingsPage_ImportTextHolderFile
```

Readback after the rename pass showed the default-name count drop from 118 to
109 for `/extracted/GraphBoard 1.00/TextHolder.dll`, and the program was saved.

### Deferred / confirmed existing

The pasted holder recovery notes from 2026-07-08 confirm that these serializer
names already existed and should be treated as the authoritative anchors for
future parser work:

```text
Sound.dll:10004090       SoundHolder_SerializeSoundData
TextHolder.dll:10008200  TextHolder_SerializeDocument
TextHolder.dll:10007de0  TextHolder_SerializeOneTextEntry
FontControl.dll:10004230 FontControl_SerializeFontSlots
```

Several remaining high-xref default functions in holder DLLs decompile as
generic COM type-info cache, BSTR cleanup, registry helper, import thunk, or
compiler stack-probe routines. They were intentionally left unnamed in this
core pass unless their component-specific role was clear from callers/callees.

### VideoHolder.dll

These settings-page handlers sit around the already named
`VideoSettingsPage_UpdateControls`, `VideoHolder_OpenAviFileForEntry`, and
`VideoHolder_PlayActiveVideo` runtime functions. The import helper copies an
AVI into the holder's managed backing path.

```text
100054d0 VideoSettingsPage_DeleteSelectedVideo
10005650 VideoSettingsPage_OnVideoListSelectionChanged
100056c0 VideoSettingsPage_MoveSelectedVideoToTop
10005740 VideoSettingsPage_MoveSelectedVideoToBottom
100057e0 VideoSettingsPage_CopyFileBytes
100059e0 VideoSettingsPage_ImportAviFile
10005d90 VideoSettingsPage_RebuildVideoList
10005e50 VideoSettingsPage_OnInitDialog
```

Readback after the rename pass confirmed all eight names, and
`/extracted/GraphBoard 1.00/VideoHolder.dll` was saved. The remaining high-xref
default functions in `Recorder.dll` inspected during this pass were left
unnamed because they were generic type-info, registry cleanup, stack-probe, or
tiny release/delete helper routines rather than holder-specific core behavior.

### SpriteHolder.dll

These property-page handlers are adjacent to the already recovered
`SpriteHolder_SerializeImagesAndSprites`, `SpriteHolder_DrawSpritesForLayer`,
and sprite definition import/replace routines. The first page manages sprite
instances; the second manages sprite definitions.

```text
10004010 SpriteSettingsPage_RebuildSpriteLists
10004170 SpriteSettingsPage_AddSpriteInstance
10004290 SpriteSettingsPage_DeleteSpriteInstanceAtIndex
10004360 SpriteSettingsPage_MoveSelectedSpriteInstanceToTop
100043d0 SpriteSettingsPage_MoveSelectedSpriteInstanceToBottom
100044a0 SpriteSettingsPage_OnSetActive
100044d0 SpriteSettingsPage_OnApply
100053e0 SpriteDefinitionPage_RebuildDefinitionList
100054e0 SpriteDefinitionPage_MoveSelectedDefinitionToTop
10005580 SpriteDefinitionPage_MoveSelectedDefinitionToBottom
10005670 SpriteDefinitionPage_OnSetActive
```

Readback after the rename pass confirmed all eleven names, and
`/extracted/GraphBoard 1.00/SpriteHolder.dll` was saved. A follow-up inspection
of `HotSpotHolder.dll` left remaining candidates unnamed because they were
generic registry/type-info/destructor support or low-level drawing helpers whose
component-specific role was not yet clear enough for this core pass.

### MultiBmp.dll and BitmapHolder.dll

These names cover one clear MultiBitmap property-page list helper and two
indexed-bitmap pixel helpers relevant to recovered bitmap payloads and
transparent blitting:

```text
MultiBmp.dll:10001e30      MultiBmpSettingsPage_RebuildBitmapList
BitmapHolder.dll:100071d0  BitmapUtil_RemapIndexedPixelValue
BitmapHolder.dll:10007250  BitmapUtil_BlitIndexedRectWithTransparency
```

Readback confirmed all three names, and both
`/extracted/GraphBoard 1.00/MultiBmp.dll` and
`/extracted/GraphBoard 1.00/BitmapHolder.dll` were saved. Other inspected
MultiBmp/BitmapHolder defaults were left unnamed as COM dispatch, registry,
CRT/MFC startup, stack-probe, or tiny thunk helpers.

### Panorama.dll and PanoramaHolder.dll

Although panorama support is peripheral to the current static viewer, these
three names were high confidence from caller/callee evidence and matching
existing timer/blit/list patterns:

```text
Panorama.dll:1000705c       PanoramaSettingsPage_RebuildSceneList
PanoramaHolder.dll:10001390 PanoramaHolderTimerWindow_SetTrackedTimer
PanoramaHolder.dll:1000dd20 PanoramaHolder_BlitIndexedRectWithTransparency
```

Readback confirmed all three names, and both
`/extracted/GraphBoard 1.00/Panorama.dll` and
`/extracted/GraphBoard 1.00/PanoramaHolder.dll` were saved.

### Sound.dll, Recorder.dll, and HotSpotHolder.dll

These were shared registry helpers that are clearly component-scoped even
though the code pattern is generic:

```text
Sound.dll:10002c80          SoundRegistry_CloseKey
Sound.dll:10002c60          SoundRegistry_CloseKeyQuietly
Sound.dll:10002ca0          SoundRegistry_CreateOrReplaceKey
Sound.dll:10002d80          SoundRegistry_WriteStringValue
Sound.dll:10003140          SoundRegistry_RemoveTree
Recorder.dll:10002d50       RecorderRegistry_CloseKey
Recorder.dll:10002d30       RecorderRegistry_CloseKeyQuietly
Recorder.dll:10002d70       RecorderRegistry_CreateOrReplaceKey
Recorder.dll:10002de0       RecorderRegistry_WriteStringValue
Recorder.dll:10003210       RecorderRegistry_RemoveTree
HotSpotHolder.dll:10003700  HotSpotHolderRegistry_CloseKey
HotSpotHolder.dll:100036e0  HotSpotHolderRegistry_CloseKeyQuietly
HotSpotHolder.dll:10003720  HotSpotHolderRegistry_CreateOrReplaceKey
HotSpotHolder.dll:10003800  HotSpotHolderRegistry_WriteStringValue
HotSpotHolder.dll:10003bc0  HotSpotHolderRegistry_RemoveTree
```

Readback confirmed all fifteen names, and the three programs were saved. The
remaining defaults in those DLLs are mostly trivial no-ops, CRT helpers, or
type-info/COM scaffolding that did not yet clear the high-confidence bar.

### SpriteHolder.dll follow-up pass

I kept the already-good `SpriteHolder_SerializeImagesAndSprites` anchor in
place and only renamed the sprite-definition page actions that were still
plain `FUN_*`:

```text
SpriteHolder.dll:100052f0  SpriteDefinitionPage_AddDefinition
SpriteHolder.dll:10002180  SpriteDefinitionPage_DeleteSelectedDefinition
SpriteHolder.dll:10003090  SpriteDefinitionPage_DeleteSelectedDefinitionAndCompactList
SpriteHolder.dll:10003400  SpriteDefinitionPage_InsertBlankDefinitionAtSelectedIndex
```

Readback confirmed all four names, and `/extracted/GraphBoard 1.00/SpriteHolder.dll`
was saved again after the pass. I deferred the nearby low-confidence helpers at
`10001000`, `100010b0`, `10001210`, `10001270`, `10002120`, `100021b0`, and
`10005640`; they are still UI glue or selection-refresh paths, but not yet
distinct enough for this core-only sweep.

### Sound.dll follow-up pass

These were the sound playback/runtime helpers that still had clear behavior
after the earlier settings-page pass:

```text
Sound.dll:10007a30  SoundSettingsPage_OnTimer
Sound.dll:10004f60  SoundHolder_UpdatePlaybackForAllEntries
Sound.dll:10001140  SoundHolder_ReadAndConvertPlaybackChunk
```

`SoundSettingsPage_OnTimer` is the timer handler that drives
`SoundHolder_UpdatePlaybackForAllEntries`, and the playback helper at
`10001140` is the chunk reader/converter used by both the start-playback and
update paths. Readback confirmed all three names, and `/extracted/GraphBoard 1.00/Sound.dll`
was saved after the pass. I left the remaining `Sound.dll` defaults unnamed
because they are still mostly COM/type-info, small thunks, or helper functions
that need more evidence than this pass was aiming to collect.

### HotSpotHolder.dll follow-up pass

The HotSpot properties dialog pair was the only remaining low-risk UI naming
opportunity I found in this pass:

```text
HotSpotHolder.dll:10001390  HotSpotHolderPropertiesPage_Construct
HotSpotHolder.dll:10001d00  HotSpotHolderPropertiesSheet_Construct
```

Readback confirmed both names, and `/extracted/GraphBoard 1.00/HotSpotHolder.dll`
was saved after the pass. I left the other remaining `HotSpotHolder.dll`
defaults unnamed because they were still no-op stubs, CRT helpers, or generic
dialog plumbing without enough evidence to justify more renames here.

### Recorder.dll follow-up pass

I kept this pass focused on the preview playback and COM-registration path:

```text
Recorder.dll:10001140  Recorder_SetPreviewTimer
Recorder.dll:10001180  Recorder_ClearPreviewTimerById
Recorder.dll:10002910  Recorder_RegisterComServer
Recorder.dll:10002e50  RecorderRegistry_WriteDefaultStringValue
```

`10001140` and `10001180` are the paired timer helpers used by
`Recorder_StartPlaybackPreview` / `Recorder_StopPlaybackPreview`, while
`10002910` is the server registration routine that writes the CLSID and
ProgID registry keys. `10002e50` is the small helper that writes the default
string value for a registry subkey. Readback confirmed all four names, and
`/extracted/GraphBoard 1.00/Recorder.dll` was saved after the pass. I deferred
the remaining Recorder defaults that still looked like COM support, callback
thunks, or unproven one-off helpers.

### VideoHolder.dll follow-up pass

The remaining safe `VideoHolder.dll` names in this pass were the settings sheet
lifecycle pair:

```text
VideoHolder.dll:10001230  VideoSettingsSheet_ScalarDeletingDestructor
VideoHolder.dll:10001250  VideoSettingsSheet_Destroy
```

These are the scalar-deleting destructor and the real destructor for the
video settings property sheet. Readback confirmed both names, and
`/extracted/GraphBoard 1.00/VideoHolder.dll` was saved after the pass. I left
the other VideoHolder defaults unnamed because they were still mostly helper
glue, page plumbing, or lower-confidence runtime support.

### Puzzle.dll follow-up pass

I picked up the remaining easy persistence and startup helpers in the puzzle
component:

```text
Puzzle.dll:10006f90  PuzzleRegistry_CloseKey
Puzzle.dll:10006fb0  PuzzleRegistry_CreateOrReplaceKey
Puzzle.dll:10007010  PuzzleRegistry_OpenKey
Puzzle.dll:10007050  PuzzleRegistry_CreateOrReplaceKeyAndWriteStringValue
Puzzle.dll:100070e0  PuzzleRegistry_WriteStringValue
Puzzle.dll:10007110  PuzzleRegistry_RemoveTree
Puzzle.dll:10007300  PuzzleApp_InitializeGlobalState
```

`10006f90` through `10007110` are the registry wrapper family used by the COM
registration code path, and `10007300` is the app-start state initializer
called from `PuzzleApp_InitInstance`. Readback confirmed all seven names, and
`/extracted/GraphBoard 1.00/Puzzle.dll` was saved after the pass. I deferred
`Puzzle.dll:10007220` because it is still a shared QueryInterface-style helper
with multiple callers and not yet specific enough for this core-only sweep.

### TransparentVideoHolder.dll follow-up pass

I picked up the remaining page lifecycle helpers in the TVH properties UI:

```text
TransparentVideoHolder.dll:10001250  TVH_PropertiesPage_OnOK
TransparentVideoHolder.dll:100012c0  TVH_PropertiesPage_Create
TransparentVideoHolder.dll:10001340  TVH_PropertiesPage_Construct
TransparentVideoHolder.dll:100014f0  TVH_PropertiesPage_ScalarDeletingDestructor
TransparentVideoHolder.dll:10001540  TVH_PropertiesPage_Destroy
TransparentVideoHolder.dll:10009320  TVH_PropertiesSheet_Construct
TransparentVideoHolder.dll:100093f0  TVH_PropertiesSheet_Destroy
```

`10001250` is the page OK handler, `100012c0` / `10001340` are the page
factory and constructor, `100014f0` / `10001540` are the page destructor pair,
and `10009320` / `100093f0` are the owning property sheet constructor and
destructor. Readback confirmed all seven names, and
`/extracted/GraphBoard 1.00/TransparentVideoHolder.dll` was saved after the
pass. I left `TransparentVideoHolder.dll:10001000` unnamed because it still
looks like a separate dialog constructor without caller evidence strong enough
for this core-only sweep.

### MultiBmp.dll follow-up pass

I picked up the remaining settings-page lifecycle helpers in the multi-bitmap
component:

```text
MultiBmp.dll:1000104e  MultiBmpSettingsPage_Construct
MultiBmp.dll:100011f8  MultiBmpSettingsPage_Destroy
MultiBmp.dll:1000131c  MultiBmpSettingsPage_DoDataExchange
```

`1000104e` constructs the property page, `100011f8` tears it down, and
`1000131c` is the DDX/DDV exchange routine that wires the controls to the page
state. Readback confirmed all three names, and `/extracted/GraphBoard 1.00/MultiBmp.dll`
was saved after the pass. I left `MultiBmp.dll:10001035` alone because it is
still the raw delete helper, not a component-specific runtime routine.

### PanoramaHolder.dll follow-up pass

I picked up the remaining COM registration path in the panorama holder:

```text
PanoramaHolder.dll:10003f50  PanoramaHolder_RegisterComServer
PanoramaHolder.dll:10004490  PanoramaHolder_UnregisterComServer
PanoramaHolder.dll:10004980  PanoramaHolderComRegistration_UpdateRegistry
PanoramaHolder.dll:10004360  PanoramaHolderRegistry_CloseKey
PanoramaHolder.dll:10004380  PanoramaHolderRegistry_CreateOrReplaceKey
PanoramaHolder.dll:100043f0  PanoramaHolderRegistry_CreateOrReplaceKeyAndWriteStringValue
PanoramaHolder.dll:10004460  PanoramaHolderRegistry_WriteStringValue
```

`10003f50` and `10004490` are the module registration and unregistration
drivers, `10004980` is the shared registry update helper they both use, and
`10004360` / `10004380` / `100043f0` / `10004460` are the close/create/write
registry helpers underneath it. Readback confirmed all seven names, and
`/extracted/GraphBoard 1.00/PanoramaHolder.dll` was saved after the pass. I
left `PanoramaHolder.dll:10001000` unnamed because it still looks like a
separate dialog constructor without caller evidence strong enough for this
core-only sweep.

### AudioHolder.dll follow-up pass

I picked up the remaining COM registration and registry helpers in the audio
holder:

```text
AudioHolder.dll:10002480  AudioHolder_RegisterComServer
AudioHolder.dll:100029f0  AudioHolder_UnregisterComServer
AudioHolder.dll:100028a0  AudioHolderRegistry_CloseKeyQuietly
AudioHolder.dll:100028c0  AudioHolderRegistry_CloseKey
AudioHolder.dll:100028e0  AudioHolderRegistry_CreateOrReplaceKey
AudioHolder.dll:10002950  AudioHolderRegistry_CreateOrReplaceKeyAndWriteStringValue
AudioHolder.dll:100029c0  AudioHolderRegistry_WriteStringValue
AudioHolder.dll:10002ee0  AudioHolderComRegistration_UpdateRegistry
AudioHolder.dll:10003020  AudioHolder_RegisterTypeLibrary
```

`10002480` and `100029f0` are the module registration and unregistration
drivers, `10002ee0` is the shared COM registry update helper, and `10003020`
is the type-library registration routine they delegate to. `100028a0` /
`100028c0` / `100028e0` / `10002950` / `100029c0` are the underlying registry
wrappers. Readback confirmed all nine names, and
`/extracted/GraphBoard 1.00/AudioHolder.dll` was saved after the pass. I left
the remaining `AudioHolder.dll` defaults unnamed because they were still
mostly COM scaffolding, class-factory helpers, or support functions that need
more evidence than this pass was aiming to collect.

### TextHolder.dll follow-up pass

I picked up the shared type-info loader and the small encoding helpers in the
text holder:

```text
TextHolder.dll:10006a10  TextHolder_LoadAndCacheTypeInfo
TextHolder.dll:100068e0  TextHolder_AnsiToWide
TextHolder.dll:10006910  TextHolder_WideToAnsi
```

`10006a10` is the shared loader behind both `TextHolder_GetCachedTypeInfo`
and `TextHolderSecondary_GetCachedTypeInfo`, while `100068e0` and `10006910`
are the ANSI/Wide conversion wrappers used around the type-library loading
path. Readback confirmed all three names, and `/extracted/GraphBoard 1.00/TextHolder.dll`
was saved after the pass. I left the remaining `TextHolder.dll` defaults
unnamed because they are still either generic COM support, dialog plumbing, or
helpers that need more evidence to justify naming.

### SynchroText.dll follow-up pass

I picked up the class-factory COM methods that were still plain `FUN_*`:

```text
SynchroText.dll:100043e7  SynchroTextClassFactory_AddRef
SynchroText.dll:10004423  SynchroTextClassFactory_Release
SynchroText.dll:1000472f  SynchroTextClassFactory_QueryInterface
```

`100043e7` and `10004423` are the class-factory refcount methods, and
`1000472f` is the class-factory `QueryInterface` wrapper that dispatches
through the module's interface map. Readback confirmed all three names, and
`/extracted/GraphBoard 1.00/SynchroText.dll` was saved after the pass. I left
the remaining `SynchroText.dll` defaults unnamed because they are still either
object-construction cleanup shims, table-driven type-info helpers, or other
support code that needs more evidence before naming.

### Recorder.dll serializer/vtable recovery pass

I created the missing vtable-referenced functions in the canonical
`/extracted/GraphBoard 1.00/Recorder.dll` program, then corrected one false
mid-function creation at `10003f00` after re-reading the vtable bytes. The
actual serializer entry is `10003ef0`; it writes schema `1`, serializes
`holder+0x34`, and transfers a 100-byte state block through `CArchive`.

```text
Recorder.dll:10003ef0  Recorder_SerializePrivateState
Recorder.dll:10003eb0  Recorder_ShowPropertiesDialogCommand
Recorder.dll:10003ec0  Recorder_SetOwnerWindowOrHostPointer
Recorder.dll:10003ff0  Recorder_SetCallbackInterfacePointer
Recorder.dll:10004000  Recorder_GetObjectInfoText
Recorder.dll:10004ab0  Recorder_StopCaptureAndPlaybackPreview
Recorder.dll:10004d50  RecorderPropertiesPage_Create
Recorder.dll:10004dd0  RecorderPropertiesPage_Construct
Recorder.dll:10004e30  RecorderPropertiesPage_ScalarDeletingDestructor
Recorder.dll:10004e50  RecorderPropertiesPage_Destroy
```

Readback confirmed all ten names, and `/extracted/GraphBoard 1.00/Recorder.dll`
was saved after the pass. I left the remaining tiny delegate slots unnamed where
the decompile only proved a forwarding call, not a distinct component event
semantics.

### VideoHolder.dll serializer recovery pass

I created the missing vtable-referenced functions in the canonical
`/extracted/GraphBoard 1.00/VideoHolder.dll` program around `10003bd0..100040c0`.
The holder-private serializer was the large archive routine at `10003e20`; it
writes schema `1`, serializes the holder entry count at `holder+0x118`, then
for each video entry writes a `0x6c` record followed by the entry `CString`.

```text
VideoHolder.dll:10003e20  VideoHolder_SerializePrivateState
VideoHolder.dll:10003c90  VideoHolder_ShowPropertiesDialog
VideoHolder.dll:10003bf0  VideoHolder_GetSerializedRecordByteCount
VideoHolder.dll:10003c50  VideoHolder_SetPlaybackRect
```

Readback confirmed all four names, and `/extracted/GraphBoard 1.00/VideoHolder.dll`
was saved after the pass. The remaining default/no-op vtable slots were left
unnamed until their exact interface semantics are needed.

### PanoramaHolder.dll serializer recovery pass

I created the missing vtable-referenced functions in the canonical
`/extracted/GraphBoard 1.00/PanoramaHolder.dll` program around
`10005d70..10005e20`. The holder-private serializer is `10005e20`; it writes
schema `0`, serializes the scene/layer count at `holder+0x40`, each `0x224`
scene record, the backing DIB bytes, variable-size global-memory subimage
blocks at record `+0x88`, and `0x48` animation/region records at record
`+0x154`.

```text
PanoramaHolder.dll:10005e20  PanoramaHolder_SerializePrivateState
PanoramaHolder.dll:10005d70  PanoramaHolder_ShowPropertySheetCommand
PanoramaHolder.dll:10005d80  PanoramaHolder_SetParentWindowPointer
PanoramaHolder.dll:10005d90  PanoramaHolder_SetCallbackInterfacePointer
PanoramaHolder.dll:10005da0  PanoramaHolder_GetObjectInfoText
PanoramaHolder.dll:10005dc0  PanoramaHolder_SetRuntimeEnabled
PanoramaHolder.dll:10005e00  PanoramaHolder_NotifyCurrentSceneChanged
```

Readback confirmed all seven names, and
`/extracted/GraphBoard 1.00/PanoramaHolder.dll` was saved after the pass.
`Panorama.dll` already had the core state serializer named
`PanoramaState_Serialize` at `10004b6a`.

### Tuwim.exe script dispatch table labels

I labeled the script token pointer tables used by
`GraphBrdScript_ExecuteStatementOrBuiltin` and
`GraphBrdScript_IndexBuiltInCalls`. The direct dispatcher and indexer keep
separate copies of the same 24 host builtin pointer table.

```text
Tuwim.exe:0043b62c  GraphBrdScript_HostBuiltinNameTable
Tuwim.exe:0043c470  GraphBrdScript_IndexedHostBuiltinNameTable
Tuwim.exe:0043c444  GraphBrdScript_PageEventNameTable
Tuwim.exe:0043c3a0  GraphBrdScript_ValueTypeNameTable
```

Host builtin order:

```text
0 MessageBox
1 Random
2 FadeScreen
3 LoadPage
4 Exit
5 SetTimer
6 SetCursor
7 CreateDirectSound
8 ReleaseDirectSound
9 EnableScreen
10 DisableScreen
11 ShowBuffer
12 IsProject
13 Debug
14 LoadGroup
15 CloseGroup
16 SetDisplayMode
17 PrintBuffer
18 Execute
19 GetProgramPath
20 GetCommandLine
21 DeleteFile
22 ShellExecute
23 GetResourcePath
```

Page-event lookup order:

```text
0 OnTimer
1 OnKeyDown
2 OnKeyUp
3 OnLButtonDown
4 OnLButtonUp
5 OnRButtonDown
6 OnRButtonUp
7 OnClosePage
8 OnOpenPage
9 OnMouseMoveStart
10 OnMouseMoveStop
```

Value/type keyword order:

```text
0 char
1 int
2 long
3 float
4 unsigned
5 UINT
6 BYTE
7 BOOL
8 void
9 CString
10 CRect
11 CVarStorage
```

Readback through `GraphBrdScript_IndexBuiltInCalls` showed the new labels, and
`/Tuwim.exe` was saved after the pass.

## Tuwim.exe script-engine event offset fields

Confirmed through `GraphBrdScriptEngine_ValidatePageEventHandlers` (00427450),
`GraphBrdScriptEngine_Serialize` (0041aad0), and the individual event runners.
The engine stores page-script text offsets at:

```text
+0x30 OnClosePage
+0x34 OnOpenPage
+0x38 OnTimer
+0x3c OnKeyDown
+0x40 OnKeyUp
+0x44 OnLButtonDown
+0x48 OnLButtonUp
+0x4c OnRButtonDown
+0x50 OnRButtonUp
+0x54 OnMouseMoveStart
+0x58 OnMouseMoveStop
```

Serialization schema 1 writes `+0x38`, `+0x3c`, `+0x30`, `+0x34`; schema 2
adds the button offsets `+0x44..+0x50`; schema 3 adds `+0x54`; schema 4 adds
`+0x58`. Key handlers pass one temporary key-code argument, while button
handlers pass two integer `x`/`y` arguments. No additional Ghidra DB writes
were needed for this pass because the relevant runner functions already had
comments/names from the earlier rename work.
