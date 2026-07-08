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
