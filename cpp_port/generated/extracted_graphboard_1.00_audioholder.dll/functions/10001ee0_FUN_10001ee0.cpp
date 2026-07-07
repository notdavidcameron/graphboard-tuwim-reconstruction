/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10001ee0 @ 10001ee0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001ee0 at 10001ee0
// Signature: undefined __fastcall FUN_10001ee0(undefined4 * param_1)
// Entry: 10001ee0
// Body: 10001ee0 - 10001f18
// ============================================================================
void __fastcall FUN_10001ee0(undefined4 *param_1)

{
  *param_1 = &PTR_LAB_100086f8;
  param_1[1] = &PTR_LAB_100086c8;
  param_1[2] = &PTR_LAB_100086a8;
  param_1[3] = &PTR_LAB_10008698;
  param_1[4] = 1;
  InterlockedDecrement(&DAT_1000b47c);
  FUN_10003850(param_1);
  return;
}

// clang-format on
