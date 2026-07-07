/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10004880 @ 10004880
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10004880 at 10004880
// Signature: undefined4 __stdcall FUN_10004880(int param_1, undefined4 param_2)
// Entry: 10004880
// Body: 10004880 - 1000489f
// ============================================================================
undefined4 FUN_10004880(int param_1,undefined4 param_2)

{
  int *piVar1;
  
  piVar1 = *(int **)(param_1 + 0x28);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 0x1c))(piVar1,param_2);
  }
  return 0;
}

// clang-format on
