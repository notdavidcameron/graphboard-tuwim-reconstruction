/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_100022f0 @ 100022f0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100022f0 at 100022f0
// Signature: LONG __stdcall FUN_100022f0(undefined4 * param_1)
// Entry: 100022f0
// Body: 100022f0 - 10002320
// ============================================================================
LONG FUN_100022f0(undefined4 *param_1)

{
  LONG LVar1;
  
  LVar1 = InterlockedDecrement(param_1 + 1);
  if ((LVar1 == 0) && (param_1 != (undefined4 *)0x0)) {
    FUN_10002330(param_1);
    operator_delete(param_1);
  }
  return LVar1;
}

// clang-format on
