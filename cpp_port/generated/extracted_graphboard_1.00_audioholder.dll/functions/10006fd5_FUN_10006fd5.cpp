/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10006fd5 @ 10006fd5
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10006fd5 at 10006fd5
// Signature: undefined4 __stdcall FUN_10006fd5(undefined4 param_1, int param_2)
// Entry: 10006fd5
// Body: 10006fd5 - 1000707f
// ============================================================================
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_10006fd5(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  undefined4 *_Memory;
  undefined4 *puVar2;
  
  if (param_2 == 0) {
    if (0 < DAT_1000c5a4) {
      DAT_1000c5a4 = DAT_1000c5a4 + -1;
      goto LAB_10006feb;
    }
LAB_10007013:
    uVar1 = 0;
  }
  else {
LAB_10006feb:
    _DAT_1000c5a8 = *(undefined4 *)_adjust_fdiv_exref;
    if (param_2 == 1) {
      DAT_1000c5b0 = malloc(0x80);
      if (DAT_1000c5b0 == (undefined4 *)0x0) goto LAB_10007013;
      *DAT_1000c5b0 = 0;
      DAT_1000c5ac = DAT_1000c5b0;
      initterm(&DAT_1000b000,&DAT_1000b00c);
      DAT_1000c5a4 = DAT_1000c5a4 + 1;
    }
    else if ((param_2 == 0) &&
            (_Memory = DAT_1000c5b0, puVar2 = DAT_1000c5ac, DAT_1000c5b0 != (undefined4 *)0x0)) {
      while (puVar2 = puVar2 + -1, _Memory <= puVar2) {
        if ((code *)*puVar2 != (code *)0x0) {
          (*(code *)*puVar2)();
          _Memory = DAT_1000c5b0;
        }
      }
      free(_Memory);
      DAT_1000c5b0 = (undefined4 *)0x0;
    }
    uVar1 = 1;
  }
  return uVar1;
}

// clang-format on
