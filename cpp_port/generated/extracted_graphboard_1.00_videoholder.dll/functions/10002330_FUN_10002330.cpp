/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10002330 @ 10002330
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10002330 at 10002330
// Signature: undefined __fastcall FUN_10002330(undefined4 * param_1)
// Entry: 10002330
// Body: 10002330 - 10002354
// ============================================================================
void __fastcall FUN_10002330(undefined4 *param_1)

{
  *param_1 = &PTR_LAB_100078d0;
  param_1[1] = 1;
  InterlockedDecrement(&DAT_1000a41c);
  FUN_10003af0(param_1 + 3);
  return;
}

// clang-format on
