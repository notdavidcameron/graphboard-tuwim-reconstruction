/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_100056f1 @ 100056f1
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100056f1 at 100056f1
// Signature: undefined4 __stdcall FUN_100056f1(undefined4 param_1, int param_2)
// Entry: 100056f1
// Body: 100056f1 - 1000579b
// ============================================================================
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_100056f1(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  undefined4 *_Memory;
  undefined4 *puVar2;
  
  if (param_2 == 0) {
    if (0 < DAT_100094d4) {
      DAT_100094d4 = DAT_100094d4 + -1;
      goto LAB_10005707;
    }
LAB_1000572f:
    uVar1 = 0;
  }
  else {
LAB_10005707:
    _DAT_100094d8 = *(undefined4 *)_adjust_fdiv_exref;
    if (param_2 == 1) {
      DAT_100094e0 = malloc(0x80);
      if (DAT_100094e0 == (undefined4 *)0x0) goto LAB_1000572f;
      *DAT_100094e0 = 0;
      DAT_100094dc = DAT_100094e0;
      initterm(&DAT_10008000,&DAT_1000800c);
      DAT_100094d4 = DAT_100094d4 + 1;
    }
    else if ((param_2 == 0) &&
            (_Memory = DAT_100094e0, puVar2 = DAT_100094dc, DAT_100094e0 != (undefined4 *)0x0)) {
      while (puVar2 = puVar2 + -1, _Memory <= puVar2) {
        if ((code *)*puVar2 != (code *)0x0) {
          (*(code *)*puVar2)();
          _Memory = DAT_100094e0;
        }
      }
      free(_Memory);
      DAT_100094e0 = (undefined4 *)0x0;
    }
    uVar1 = 1;
  }
  return uVar1;
}

// clang-format on
