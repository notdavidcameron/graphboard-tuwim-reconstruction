/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10004970 @ 10004970
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10004970 at 10004970
// Signature: undefined4 __stdcall FUN_10004970(int * param_1)
// Entry: 10004970
// Body: 10004970 - 10004991
// ============================================================================
undefined4 FUN_10004970(int *param_1)

{
  if (param_1[0xb] != 0) {
    (**(code **)(*param_1 + 0x2c))(param_1);
    (**(code **)(*(int *)param_1[0xb] + 0x34))(0);
  }
  return 0;
}

// clang-format on
