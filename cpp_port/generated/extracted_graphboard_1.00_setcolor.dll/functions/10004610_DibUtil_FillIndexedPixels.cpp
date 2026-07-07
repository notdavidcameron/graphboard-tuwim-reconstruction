/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: DibUtil_FillIndexedPixels @ 10004610
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: DibUtil_FillIndexedPixels at 10004610
// Signature: undefined4 __stdcall DibUtil_FillIndexedPixels(uint param_1, undefined4 * param_2, undefined1 param_3)
// Entry: 10004610
// Body: 10004610 - 10004691
// ============================================================================
undefined4 DibUtil_FillIndexedPixels(uint param_1,undefined4 *param_2,undefined1 param_3)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  
  if (((param_1 != 0) && (param_2 != (undefined4 *)0x0)) && (((uint)param_2 & 3) == 0)) {
    uVar2 = *(uint *)(param_1 + 0x14);
    if (*(uint *)(param_1 + 0x14) == 0) {
      iVar1 = *(int *)(param_1 + 8);
      if (iVar1 < 0) {
        iVar1 = -iVar1;
      }
      uVar2 = ((uint)*(ushort *)(param_1 + 0xe) * *(int *)(param_1 + 4) + 0x1f >> 3 & 0x1ffffffc) *
              iVar1;
    }
    param_1 = uVar2;
    uVar3 = param_1 & 3;
    for (uVar2 = param_1 >> 2; uVar2 != 0; uVar2 = uVar2 - 1) {
      *param_2 = CONCAT22(CONCAT11(param_3,param_3),CONCAT11(param_3,param_3));
      param_2 = param_2 + 1;
    }
    for (; uVar3 != 0; uVar3 = uVar3 - 1) {
      *(undefined1 *)param_2 = param_3;
      param_2 = (undefined4 *)((int)param_2 + 1);
    }
    return 1;
  }
  return 0;
}

// clang-format on
