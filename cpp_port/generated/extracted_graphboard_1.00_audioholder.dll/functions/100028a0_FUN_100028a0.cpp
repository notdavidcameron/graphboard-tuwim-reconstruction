/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_100028a0 @ 100028a0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100028a0 at 100028a0
// Signature: undefined __fastcall FUN_100028a0(undefined4 * param_1)
// Entry: 100028a0
// Body: 100028a0 - 100028b7
// ============================================================================
void __fastcall FUN_100028a0(undefined4 *param_1)

{
  if ((HKEY)*param_1 != (HKEY)0x0) {
    RegCloseKey((HKEY)*param_1);
    *param_1 = 0;
  }
  return;
}

// clang-format on
