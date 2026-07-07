/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10001f50 @ 10001f50
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001f50 at 10001f50
// Signature: HRESULT __stdcall FUN_10001f50(undefined4 param_1, undefined4 param_2, BSTR param_3, undefined4 * param_4)
// Entry: 10001f50
// Body: 10001f50 - 10001f96
// ============================================================================
HRESULT FUN_10001f50(undefined4 param_1,undefined4 param_2,BSTR param_3,undefined4 *param_4)

{
  HRESULT HVar1;
  
  HVar1 = -0x7fffbffd;
  if (param_4 != (undefined4 *)0x0) {
    HVar1 = 0;
    if (DAT_100080fc == (int *)0x0) {
      HVar1 = FUN_100037e0(&PTR_DAT_100080f0,param_3);
    }
    *param_4 = DAT_100080fc;
    if (DAT_100080fc != (int *)0x0) {
      (**(code **)(*DAT_100080fc + 4))(DAT_100080fc);
      HVar1 = 0;
    }
  }
  return HVar1;
}

// clang-format on
