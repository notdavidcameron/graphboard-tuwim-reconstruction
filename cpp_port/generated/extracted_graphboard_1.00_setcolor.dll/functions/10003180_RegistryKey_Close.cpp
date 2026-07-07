/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: RegistryKey_Close @ 10003180
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: RegistryKey_Close at 10003180
// Signature: undefined __fastcall RegistryKey_Close(undefined4 * param_1)
// Entry: 10003180
// Body: 10003180 - 10003197
// ============================================================================
void __fastcall RegistryKey_Close(undefined4 *param_1)

{
  if ((HKEY)*param_1 != (HKEY)0x0) {
    RegCloseKey((HKEY)*param_1);
    *param_1 = 0;
  }
  return;
}

// clang-format on
