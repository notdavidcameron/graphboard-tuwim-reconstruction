/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_100024f0 @ 100024f0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100024f0 at 100024f0
// Signature: int __stdcall FUN_100024f0(undefined4 * param_1)
// Entry: 100024f0
// Body: 100024f0 - 1000251d
// ============================================================================
int FUN_100024f0(undefined4 *param_1)

{
  int iVar1;
  
  iVar1 = param_1[1] + -1;
  param_1[1] = iVar1;
  if ((iVar1 == 0) && (param_1 != (undefined4 *)0x0)) {
    FUN_10002520(param_1);
    operator_delete(param_1);
  }
  return iVar1;
}

// clang-format on
