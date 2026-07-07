/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_100031a0 @ 100031a0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100031a0 at 100031a0
// Signature: LSTATUS __fastcall FUN_100031a0(undefined4 * param_1)
// Entry: 100031a0
// Body: 100031a0 - 100031b9
// ============================================================================
LSTATUS __fastcall FUN_100031a0(undefined4 *param_1)

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
