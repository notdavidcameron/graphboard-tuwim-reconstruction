/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10006f53 @ 10006f53
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10006f53 at 10006f53
// Signature: undefined __stdcall FUN_10006f53(undefined4 param_1, undefined4 param_2, int param_3, undefined * param_4)
// Entry: 10006f53
// Body: 10006f53 - 10006fbc
// ============================================================================
void FUN_10006f53(undefined4 param_1,undefined4 param_2,int param_3,undefined *param_4)

{
  int local_20;
  void *local_14;
  undefined *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_10009148;
  puStack_10 = &DAT_10007146;
  local_14 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_14;
  for (local_20 = 0; local_20 < param_3; local_20 = local_20 + 1) {
    (*(code *)param_4)();
  }
  local_8 = 0xffffffff;
  FUN_10006fbd();
  ExceptionList = local_14;
  return;
}

// clang-format on
