/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_10004d65 @ 10004d65
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10004d65 at 10004d65
// Signature: undefined4 __stdcall FUN_10004d65(undefined4 param_1, int param_2)
// Entry: 10004d65
// Body: 10004d65 - 10004e0f
// ============================================================================
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_10004d65(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  undefined4 *_Memory;
  undefined4 *puVar2;
  
  if (param_2 == 0) {
    if (0 < DAT_100093dc) {
      DAT_100093dc = DAT_100093dc + -1;
      goto LAB_10004d7b;
    }
LAB_10004da3:
    uVar1 = 0;
  }
  else {
LAB_10004d7b:
    _DAT_100093e0 = *(undefined4 *)_adjust_fdiv_exref;
    if (param_2 == 1) {
      DAT_100093e8 = malloc(0x80);
      if (DAT_100093e8 == (undefined4 *)0x0) goto LAB_10004da3;
      *DAT_100093e8 = 0;
      DAT_100093e4 = DAT_100093e8;
      initterm(&DAT_10008000,&DAT_1000800c);
      DAT_100093dc = DAT_100093dc + 1;
    }
    else if ((param_2 == 0) &&
            (_Memory = DAT_100093e8, puVar2 = DAT_100093e4, DAT_100093e8 != (undefined4 *)0x0)) {
      while (puVar2 = puVar2 + -1, _Memory <= puVar2) {
        if ((code *)*puVar2 != (code *)0x0) {
          (*(code *)*puVar2)();
          _Memory = DAT_100093e8;
        }
      }
      free(_Memory);
      DAT_100093e8 = (undefined4 *)0x0;
    }
    uVar1 = 1;
  }
  return uVar1;
}

// clang-format on
