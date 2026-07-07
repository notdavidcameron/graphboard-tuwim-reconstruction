/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10003010 @ 10003010
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10003010 at 10003010
// Signature: undefined __fastcall FUN_10003010(int * param_1)
// Entry: 10003010
// Body: 10003010 - 1000301c
// ============================================================================
void __fastcall FUN_10003010(int *param_1)

{
  int *piVar1;
  
  piVar1 = (int *)*param_1;
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
  }
  return;
}

// clang-format on
