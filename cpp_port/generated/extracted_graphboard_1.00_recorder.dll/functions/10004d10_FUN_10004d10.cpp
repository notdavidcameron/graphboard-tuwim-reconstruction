/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10004d10 @ 10004d10
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10004d10 at 10004d10
// Signature: undefined4 __stdcall FUN_10004d10(int param_1, undefined4 param_2)
// Entry: 10004d10
// Body: 10004d10 - 10004d2f
// ============================================================================
undefined4 FUN_10004d10(int param_1,undefined4 param_2)

{
  int *piVar1;
  
  piVar1 = *(int **)(param_1 + 0x10);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 0x24))(piVar1,param_2);
  }
  return 0;
}

// clang-format on
