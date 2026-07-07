/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10002080 @ 10002080
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10002080 at 10002080
// Signature: undefined __fastcall FUN_10002080(undefined4 * param_1)
// Entry: 10002080
// Body: 10002080 - 100020a4
// ============================================================================
void __fastcall FUN_10002080(undefined4 *param_1)

{
  *param_1 = &PTR_LAB_10008770;
  param_1[1] = 1;
  InterlockedDecrement(&DAT_1000b47c);
  FUN_10003850(param_1 + 3);
  return;
}

// clang-format on
