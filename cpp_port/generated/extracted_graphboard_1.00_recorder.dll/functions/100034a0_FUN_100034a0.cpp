/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_100034a0 @ 100034a0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100034a0 at 100034a0
// Signature: undefined __fastcall FUN_100034a0(int * param_1)
// Entry: 100034a0
// Body: 100034a0 - 100034ac
// ============================================================================
void __fastcall FUN_100034a0(int *param_1)

{
  int *piVar1;
  
  piVar1 = (int *)*param_1;
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
  }
  return;
}

// clang-format on
