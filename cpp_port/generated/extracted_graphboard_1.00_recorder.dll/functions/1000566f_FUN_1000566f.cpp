/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_1000566f @ 1000566f
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_1000566f at 1000566f
// Signature: undefined __stdcall FUN_1000566f(undefined4 param_1, undefined4 param_2, int param_3, undefined * param_4)
// Entry: 1000566f
// Body: 1000566f - 100056d8
// ============================================================================
void FUN_1000566f(undefined4 param_1,undefined4 param_2,int param_3,undefined *param_4)

{
  int local_20;
  void *local_14;
  undefined *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_10006e60;
  puStack_10 = &DAT_1000585c;
  local_14 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_14;
  for (local_20 = 0; local_20 < param_3; local_20 = local_20 + 1) {
    (*(code *)param_4)();
  }
  local_8 = 0xffffffff;
  FUN_100056d9();
  ExceptionList = local_14;
  return;
}

// clang-format on
