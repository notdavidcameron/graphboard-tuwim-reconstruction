/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10004c90 @ 10004c90
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10004c90 at 10004c90
// Signature: undefined4 __stdcall FUN_10004c90(int param_1)
// Entry: 10004c90
// Body: 10004c90 - 10004cab
// ============================================================================
undefined4 FUN_10004c90(int param_1)

{
  int *piVar1;
  
  piVar1 = *(int **)(param_1 + 0x10);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 0x1c))(piVar1);
  }
  return 0;
}

// clang-format on
