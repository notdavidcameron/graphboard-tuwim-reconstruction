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
