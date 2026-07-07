/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: DibUtil_GetIndexedPixelAddress @ 100045b0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: DibUtil_GetIndexedPixelAddress at 100045b0
// Signature: int __stdcall DibUtil_GetIndexedPixelAddress(int param_1, uint param_2, uint param_3)
// Entry: 100045b0
// Body: 100045b0 - 10004607
// ============================================================================
int DibUtil_GetIndexedPixelAddress(int param_1,uint param_2,uint param_3)

{
  int iVar1;
  
  if (param_2 < *(uint *)(param_1 + 4)) {
    if (param_3 < *(uint *)(param_1 + 8)) {
      iVar1 = *(int *)(param_1 + 0x20);
      if ((iVar1 == 0) && (*(ushort *)(param_1 + 0xe) < 9)) {
        iVar1 = 1 << ((byte)*(ushort *)(param_1 + 0xe) & 0x1f);
      }
      return param_1 + ((*(uint *)(param_1 + 8) - param_3) + -1) *
                       (*(uint *)(param_1 + 4) + 3 & 0xfffffffc) + 0x28 + iVar1 * 4 + param_2;
    }
  }
  return 0;
}

// clang-format on
