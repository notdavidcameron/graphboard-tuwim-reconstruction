# Duplicate Ghidra DLL Merge Notes

Date: 2026-07-08

This note records duplicate GraphBoard component DLLs currently open in the
Ghidra project. The binaries were not modified during this inventory pass.

## Merge Status

Merged on 2026-07-08, favoring the higher-annotation copies under
`/extracted/GraphBoard 1.00/`.

Keep these Ghidra programs:

```text
/extracted/GraphBoard 1.00/SpriteHolder.dll
/extracted/GraphBoard 1.00/MultiBmp.dll
```

After confirming the saved project state, these lower-annotation duplicate
programs can be removed from the Ghidra project:

```text
/SpriteHolder.dll
/GraphBoardComponents/MultiBmp.dll
```

The merge copied missing names from the lower-annotation copies into the keepers
and preserved conflicting alternate names as plate comments on the keeper
functions.

## Open Duplicate Pairs

### SpriteHolder.dll

Canonical working program:

```text
/SpriteHolder.dll
C:/Users/Administrator/Documents/GRAPHBOARD/ghidra_import/SpriteHolder.dll
```

Duplicate system-library program:

```text
/extracted/GraphBoard 1.00/SpriteHolder.dll
C:/Program Files (x86)/Common Files/GraphBoard 1.00/SpriteHolder.dll
```

Binary identity:

```text
size:   98304 bytes
sha256: 7C2CFBE9B2F4C3095078C7FEB18F46378D4C0C4EC00E42D621F64020F47FE783
```

Ghidra state:

```text
canonical: function_count 697, symbol_count 1764
duplicate: function_count 704, symbol_count 1787
```

Merge cleanliness:

```text
Not fully clean as a blind merge.
```

The binaries are byte-identical, so address-based transfer is valid. However,
the duplicate has seven additional function definitions:

```text
10005770
10006570
10006590
10006610
1000ac00
1000ce84
1000d05a
```

Most are short runtime/helper fragments or compiler support. They are not
currently named with GraphBoard-specific names, so they are low priority unless
the goal is exact analysis parity.

Useful names present only in the duplicate:

```text
10005be0 Create_SpriteHolderClassFactory
10005d00 SpriteHolderClassFactory_AddRef
10005d40 SpriteHolderClassFactory_Release
10005da0 SpriteHolderClassFactory_Destroy
10005dd0 SpriteHolder_QueryInterfaceFromMap
10005ed0 Create_SpriteHolderComObject
10006550 SpriteHolderRefCountedState_Construct
10007df0 SpriteHolderContext_Construct
10007eb0 SpriteHolderContext_Destroy
1000b2b0 CpuFeatures_DetectMmxSupport
1000b300 BitmapUtil_LoadBitmapInfoFromFileOrResource
1000b600 BitmapUtil_ReadBmpHeaderAndColorTable
1000b830 BitmapUtil_CreateDibFromBitmap
1000bcc0 BitmapUtil_CreateBitmapFromDib
1000bf40 BitmapUtil_ConvertDibBitDepth
1000c070 BitmapUtil_GetIndexedPixelAddress
1000ce8e AfxWndProcDllStatic
```

Name conflicts that need human choice:

```text
10008306 canonical SpriteHolder_DrawSpritesInRect
         duplicate SpriteHolder_DrawSpritesForLayer

10008930 canonical SpriteHolder_SerializePrivateState
         duplicate SpriteHolder_SerializeImagesAndSprites

1000c0f0 canonical SpriteHolder_BlitIndexedSpriteRect
         duplicate SpriteHolder_StretchIndexedFrameToDc

1000c6d0 canonical SpriteHolder_DrawDebugNameLabel
         duplicate SpriteHolder_DrawSpriteLabel
```

Merge guidance: transfer duplicate-only COM scaffold and `BitmapUtil_*` names
by exact address. For the four conflicts, preserve the alternate names as
comments or choose deliberately. Do not use a blind bulk merge unless
overwriting these names is acceptable.

Actual merge result: the keeper is now
`/extracted/GraphBoard 1.00/SpriteHolder.dll`, because it had the richer
annotation set. The lower-annotation `/SpriteHolder.dll` names at `100021f0`
and `100024d0` were copied directly. The four conflicting names were retained
on the keeper as plate-comment alternate names.

### MultiBmp.dll

Canonical working program:

```text
/GraphBoardComponents/MultiBmp.dll
Tuwim/extracted/_GraphBoardComponents/MultiBmp.dll
```

Duplicate system-library program:

```text
/extracted/GraphBoard 1.00/MultiBmp.dll
C:/Program Files (x86)/Common Files/GraphBoard 1.00/MultiBmp.dll
```

Binary identity:

```text
size:   57344 bytes
sha256: 0DCAB7015022A8527169760E42BC0F45491B4AFBBD304579DC7083890E8908B1
```

Ghidra state:

```text
canonical: function_count 532, symbol_count 1531
duplicate: function_count 532, symbol_count 1531
```

Merge cleanliness:

```text
Mostly clean with address-based selective merge.
```

The binaries and function-address sets match. The duplicate has useful COM and
component scaffolding names not present in the canonical program:

```text
1000248a Create_MultiBmpClassFactory
10002530 MultiBmpClassFactory_AddRef
10002554 MultiBmpClassFactory_Release
10002584 MultiBmpClassFactory_Destroy
100025c0 MultiBmp_QueryInterfaceFromMap
1000262d Create_MultiBmpComObject
10002743 MultiBmpRefCountedState_Construct
10002757 MultiBmpOuterObject_Construct
100027fc ComTypeInfo_GetIDsOfNamesCached
10002896 MultiBmp_AddRefCounter
10002eb7 Com_UnregisterProgIdAndClsid
1000315d ComClassProviderTable_Construct
100031e0 ComClassProviderTable_CreateMatchingInstance
10003d67 MultiBmpContext_Construct
10003de1 MultiBmpContext_Destroy
10003ee1 MultiBmp_HitTestPoint
10003fb6 MultiBmp_GetLayerRange
10004039 MultiBmp_DrawRecordsForLayer
100040e2 MultiBmp_ShowPropertiesDialog
```

Name conflicts that need human choice:

```text
10003ea9 canonical MultiBmp_ClearDecodedBitmapBuffers
         duplicate MultiBmp_FreeLoadedPayloads

1000423b canonical MultiBmp_CopyTempFileBytesToArchive
         duplicate MultiBmp_WriteBackingFileToArchive
```

Merge guidance: transfer source-only names by address. For `1000423b`, the
duplicate name is probably cleaner because the routine streams the backing/temp
file into the archive. For `10003ea9`, both names describe the same cleanup
behavior; keep the name that best matches future extractor notes.

Actual merge result: the keeper is now
`/extracted/GraphBoard 1.00/MultiBmp.dll`, because it had the richer annotation
set. Missing lower-annotation names were copied directly for `10001b6a`,
`100042c3`, `10004353`, `10004743`, `10004769`, and `100058d0`. The two
conflicting lower-annotation names were retained on the keeper as plate-comment
alternate names.

## Safe Merge Procedure

Use this GhidraMCP install carefully: write endpoints follow the active
CodeBrowser more reliably than the `program` field. Before any write, focus the
intended target with `tool/launch_codebrowser` and verify `check_connection`.

Suggested workflow:

```text
1. Pick one canonical target per DLL:
   - /SpriteHolder.dll
   - /GraphBoardComponents/MultiBmp.dll

2. Export/read function tables from source and target with exact project paths.

3. Build an address-keyed merge list:
   - copy source custom names only when target still has a default FUN_* name
   - handle the listed conflicts manually
   - do not copy names from duplicate-only SpriteHolder functions unless those
     functions are created in the canonical target first

4. Apply renames to the focused target CodeBrowser only.

5. Verify with search_functions/readback from exact paths.

6. Save only the target program after verification.
```

Avoid a blind `merge_program_documentation` write for these pairs unless the
overwrite behavior is acceptable. It is better suited after a dry-run and after
resolving the conflict list above.
