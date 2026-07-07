/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10001d20 @ 10001d20
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001d20 at 10001d20
// Signature: HRESULT __stdcall FUN_10001d20(undefined4 param_1, undefined4 param_2, BSTR param_3, undefined4 * param_4)
// Entry: 10001d20
// Body: 10001d20 - 10001d66
// ============================================================================
HRESULT FUN_10001d20(undefined4 param_1,undefined4 param_2,BSTR param_3,undefined4 *param_4)

{
  HRESULT HVar1;
  
  HVar1 = -0x7fffbffd;
  if (param_4 != (undefined4 *)0x0) {
    HVar1 = 0;
    if (DAT_1000a10c == (int *)0x0) {
      HVar1 = FUN_100035c0(&PTR_DAT_1000a100,param_3);
    }
    *param_4 = DAT_1000a10c;
    if (DAT_1000a10c != (int *)0x0) {
      (**(code **)(*DAT_1000a10c + 4))(DAT_1000a10c);
      HVar1 = 0;
    }
  }
  return HVar1;
}

// clang-format on
