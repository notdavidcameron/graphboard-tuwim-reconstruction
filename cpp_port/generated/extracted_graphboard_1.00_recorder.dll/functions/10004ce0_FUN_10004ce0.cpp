/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10004ce0 @ 10004ce0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10004ce0 at 10004ce0
// Signature: undefined4 __stdcall FUN_10004ce0(int * param_1)
// Entry: 10004ce0
// Body: 10004ce0 - 10004d05
// ============================================================================
undefined4 FUN_10004ce0(int *param_1)

{
  int *piVar1;
  
  piVar1 = (int *)param_1[4];
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 0x28))(piVar1);
  }
  (**(code **)(*param_1 + 0x2c))(param_1,0);
  return 0;
}

// clang-format on
