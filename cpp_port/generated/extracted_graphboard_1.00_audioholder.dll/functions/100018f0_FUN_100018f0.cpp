/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_100018f0 @ 100018f0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100018f0 at 100018f0
// Signature: HRESULT __stdcall FUN_100018f0(undefined4 param_1, undefined4 param_2, BSTR param_3, undefined4 * param_4)
// Entry: 100018f0
// Body: 100018f0 - 10001936
// ============================================================================
HRESULT FUN_100018f0(undefined4 param_1,undefined4 param_2,BSTR param_3,undefined4 *param_4)

{
  HRESULT HVar1;
  
  HVar1 = -0x7fffbffd;
  if (param_4 != (undefined4 *)0x0) {
    HVar1 = 0;
    if (DAT_1000b0cc == (int *)0x0) {
      HVar1 = FUN_10003360(&PTR_DAT_1000b0c0,param_3);
    }
    *param_4 = DAT_1000b0cc;
    if (DAT_1000b0cc != (int *)0x0) {
      (**(code **)(*DAT_1000b0cc + 4))(DAT_1000b0cc);
      HVar1 = 0;
    }
  }
  return HVar1;
}

// clang-format on
