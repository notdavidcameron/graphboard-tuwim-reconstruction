/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10002190 @ 10002190
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10002190 at 10002190
// Signature: undefined __fastcall FUN_10002190(undefined4 * param_1)
// Entry: 10002190
// Body: 10002190 - 100021c8
// ============================================================================
void __fastcall FUN_10002190(undefined4 *param_1)

{
  *param_1 = &PTR_LAB_10007858;
  param_1[1] = &PTR_LAB_1000781c;
  param_1[2] = &PTR_LAB_100077f8;
  param_1[3] = &PTR_LAB_100077e8;
  param_1[4] = 1;
  InterlockedDecrement(&DAT_1000a41c);
  FUN_10003af0(param_1);
  return;
}

// clang-format on
