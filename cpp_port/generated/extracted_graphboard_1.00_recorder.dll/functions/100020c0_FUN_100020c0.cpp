/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_100020c0 @ 100020c0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100020c0 at 100020c0
// Signature: HRESULT __stdcall FUN_100020c0(undefined4 param_1, undefined4 param_2, BSTR param_3, undefined4 * param_4)
// Entry: 100020c0
// Body: 100020c0 - 10002106
// ============================================================================
HRESULT FUN_100020c0(undefined4 param_1,undefined4 param_2,BSTR param_3,undefined4 *param_4)

{
  HRESULT HVar1;
  
  HVar1 = -0x7fffbffd;
  if (param_4 != (undefined4 *)0x0) {
    HVar1 = 0;
    if (DAT_1000811c == (int *)0x0) {
      HVar1 = FUN_100037e0(&PTR_DAT_10008110,param_3);
    }
    *param_4 = DAT_1000811c;
    if (DAT_1000811c != (int *)0x0) {
      (**(code **)(*DAT_1000811c + 4))(DAT_1000811c);
      HVar1 = 0;
    }
  }
  return HVar1;
}

// clang-format on
