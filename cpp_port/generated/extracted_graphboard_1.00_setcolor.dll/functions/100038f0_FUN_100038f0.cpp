/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_100038f0 @ 100038f0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100038f0 at 100038f0
// Signature: undefined __fastcall FUN_100038f0(int * param_1)
// Entry: 100038f0
// Body: 100038f0 - 100038fc
// ============================================================================
void __fastcall FUN_100038f0(int *param_1)

{
  int *piVar1;
  
  piVar1 = (int *)*param_1;
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
  }
  return;
}

// clang-format on
