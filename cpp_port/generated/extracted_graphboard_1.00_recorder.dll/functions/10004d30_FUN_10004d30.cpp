/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10004d30 @ 10004d30
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10004d30 at 10004d30
// Signature: undefined4 __stdcall FUN_10004d30(int param_1, undefined4 param_2)
// Entry: 10004d30
// Body: 10004d30 - 10004d4f
// ============================================================================
undefined4 FUN_10004d30(int param_1,undefined4 param_2)

{
  int *piVar1;
  
  piVar1 = *(int **)(param_1 + 0x10);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 0x2c))(piVar1,param_2);
  }
  return 0;
}

// clang-format on
