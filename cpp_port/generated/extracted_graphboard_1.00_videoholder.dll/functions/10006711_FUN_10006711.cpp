/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10006711 @ 10006711
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10006711 at 10006711
// Signature: undefined4 __stdcall FUN_10006711(undefined4 param_1, int param_2)
// Entry: 10006711
// Body: 10006711 - 100067bb
// ============================================================================
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_10006711(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  undefined4 *_Memory;
  undefined4 *puVar2;
  
  if (param_2 == 0) {
    if (0 < DAT_1000b55c) {
      DAT_1000b55c = DAT_1000b55c + -1;
      goto LAB_10006727;
    }
LAB_1000674f:
    uVar1 = 0;
  }
  else {
LAB_10006727:
    _DAT_1000b560 = *(undefined4 *)_adjust_fdiv_exref;
    if (param_2 == 1) {
      DAT_1000b568 = malloc(0x80);
      if (DAT_1000b568 == (undefined4 *)0x0) goto LAB_1000674f;
      *DAT_1000b568 = 0;
      DAT_1000b564 = DAT_1000b568;
      initterm(&DAT_1000a000,&DAT_1000a00c);
      DAT_1000b55c = DAT_1000b55c + 1;
    }
    else if ((param_2 == 0) &&
            (_Memory = DAT_1000b568, puVar2 = DAT_1000b564, DAT_1000b568 != (undefined4 *)0x0)) {
      while (puVar2 = puVar2 + -1, _Memory <= puVar2) {
        if ((code *)*puVar2 != (code *)0x0) {
          (*(code *)*puVar2)();
          _Memory = DAT_1000b568;
        }
      }
      free(_Memory);
      DAT_1000b568 = (undefined4 *)0x0;
    }
    uVar1 = 1;
  }
  return uVar1;
}

// clang-format on
