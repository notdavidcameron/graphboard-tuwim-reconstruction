/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10002b30 @ 10002b30
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10002b30 at 10002b30
// Signature: LSTATUS __fastcall FUN_10002b30(undefined4 * param_1)
// Entry: 10002b30
// Body: 10002b30 - 10002b49
// ============================================================================
LSTATUS __fastcall FUN_10002b30(undefined4 *param_1)

{
  LSTATUS LVar1;
  
  LVar1 = 0;
  if ((HKEY)*param_1 != (HKEY)0x0) {
    LVar1 = RegCloseKey((HKEY)*param_1);
    *param_1 = 0;
  }
  return LVar1;
}

// clang-format on
