/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10002520 @ 10002520
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10002520 at 10002520
// Signature: undefined __fastcall FUN_10002520(undefined4 * param_1)
// Entry: 10002520
// Body: 10002520 - 10002544
// ============================================================================
void __fastcall FUN_10002520(undefined4 *param_1)

{
  *param_1 = &PTR_LAB_10006930;
  param_1[1] = 1;
  InterlockedDecrement(&DAT_100083ac);
  RecorderContext_Destroy((char *)(param_1 + 2));
  return;
}

// clang-format on
