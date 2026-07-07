/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10002d30 @ 10002d30
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10002d30 at 10002d30
// Signature: undefined __fastcall FUN_10002d30(undefined4 * param_1)
// Entry: 10002d30
// Body: 10002d30 - 10002d47
// ============================================================================
void __fastcall FUN_10002d30(undefined4 *param_1)

{
  if ((HKEY)*param_1 != (HKEY)0x0) {
    RegCloseKey((HKEY)*param_1);
    *param_1 = 0;
  }
  return;
}

// clang-format on
