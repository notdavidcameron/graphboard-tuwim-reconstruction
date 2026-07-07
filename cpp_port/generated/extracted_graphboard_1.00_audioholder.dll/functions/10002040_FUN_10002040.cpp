/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10002040 @ 10002040
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10002040 at 10002040
// Signature: LONG __stdcall FUN_10002040(undefined4 * param_1)
// Entry: 10002040
// Body: 10002040 - 10002070
// ============================================================================
LONG FUN_10002040(undefined4 *param_1)

{
  LONG LVar1;
  
  LVar1 = InterlockedDecrement(param_1 + 1);
  if ((LVar1 == 0) && (param_1 != (undefined4 *)0x0)) {
    FUN_10002080(param_1);
    operator_delete(param_1);
  }
  return LVar1;
}

// clang-format on
