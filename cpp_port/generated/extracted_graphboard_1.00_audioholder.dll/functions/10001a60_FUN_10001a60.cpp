/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10001a60 @ 10001a60
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001a60 at 10001a60
// Signature: HRESULT __stdcall FUN_10001a60(undefined4 param_1, undefined4 param_2, BSTR param_3, undefined4 * param_4)
// Entry: 10001a60
// Body: 10001a60 - 10001aa6
// ============================================================================
HRESULT FUN_10001a60(undefined4 param_1,undefined4 param_2,BSTR param_3,undefined4 *param_4)

{
  HRESULT HVar1;
  
  HVar1 = -0x7fffbffd;
  if (param_4 != (undefined4 *)0x0) {
    HVar1 = 0;
    if (DAT_1000b0ec == (int *)0x0) {
      HVar1 = FUN_10003360(&PTR_DAT_1000b0e0,param_3);
    }
    *param_4 = DAT_1000b0ec;
    if (DAT_1000b0ec != (int *)0x0) {
      (**(code **)(*DAT_1000b0ec + 4))(DAT_1000b0ec);
      HVar1 = 0;
    }
  }
  return HVar1;
}

// clang-format on
