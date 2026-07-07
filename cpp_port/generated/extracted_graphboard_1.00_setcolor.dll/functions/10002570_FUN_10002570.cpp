/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_10002570 @ 10002570
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10002570 at 10002570
// Signature: HRESULT __stdcall FUN_10002570(undefined4 param_1, undefined4 param_2, BSTR param_3, undefined4 * param_4)
// Entry: 10002570
// Body: 10002570 - 100025b6
// ============================================================================
HRESULT FUN_10002570(undefined4 param_1,undefined4 param_2,BSTR param_3,undefined4 *param_4)

{
  HRESULT HVar1;
  
  HVar1 = -0x7fffbffd;
  if (param_4 != (undefined4 *)0x0) {
    HVar1 = 0;
    if (DAT_100080dc == (int *)0x0) {
      HVar1 = FUN_10003c40(&PTR_DAT_100080d0,param_3);
    }
    *param_4 = DAT_100080dc;
    if (DAT_100080dc != (int *)0x0) {
      (**(code **)(*DAT_100080dc + 4))(DAT_100080dc);
      HVar1 = 0;
    }
  }
  return HVar1;
}

// clang-format on
