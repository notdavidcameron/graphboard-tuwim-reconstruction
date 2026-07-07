/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10001e90 @ 10001e90
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001e90 at 10001e90
// Signature: HRESULT __stdcall FUN_10001e90(undefined4 param_1, undefined4 param_2, BSTR param_3, undefined4 * param_4)
// Entry: 10001e90
// Body: 10001e90 - 10001ed6
// ============================================================================
HRESULT FUN_10001e90(undefined4 param_1,undefined4 param_2,BSTR param_3,undefined4 *param_4)

{
  HRESULT HVar1;
  
  HVar1 = -0x7fffbffd;
  if (param_4 != (undefined4 *)0x0) {
    HVar1 = 0;
    if (DAT_1000a12c == (int *)0x0) {
      HVar1 = FUN_100035c0(&PTR_DAT_1000a120,param_3);
    }
    *param_4 = DAT_1000a12c;
    if (DAT_1000a12c != (int *)0x0) {
      (**(code **)(*DAT_1000a12c + 4))(DAT_1000a12c);
      HVar1 = 0;
    }
  }
  return HVar1;
}

// clang-format on
