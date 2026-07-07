/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10002b10 @ 10002b10
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10002b10 at 10002b10
// Signature: undefined __fastcall FUN_10002b10(undefined4 * param_1)
// Entry: 10002b10
// Body: 10002b10 - 10002b27
// ============================================================================
void __fastcall FUN_10002b10(undefined4 *param_1)

{
  if ((HKEY)*param_1 != (HKEY)0x0) {
    RegCloseKey((HKEY)*param_1);
    *param_1 = 0;
  }
  return;
}

// clang-format on
