/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10001de0 @ 10001de0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001de0 at 10001de0
// Signature: HRESULT __stdcall FUN_10001de0(undefined4 param_1, undefined4 param_2, BSTR param_3, undefined4 * param_4)
// Entry: 10001de0
// Body: 10001de0 - 10001e26
// ============================================================================
HRESULT FUN_10001de0(undefined4 param_1,undefined4 param_2,BSTR param_3,undefined4 *param_4)

{
  HRESULT HVar1;
  
  HVar1 = -0x7fffbffd;
  if (param_4 != (undefined4 *)0x0) {
    HVar1 = 0;
    if (DAT_100080dc == (int *)0x0) {
      HVar1 = FUN_100037e0(&PTR_DAT_100080d0,param_3);
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
