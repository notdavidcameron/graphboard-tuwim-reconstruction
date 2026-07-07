/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10006edf @ 10006edf
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10006edf at 10006edf
// Signature: undefined __stdcall FUN_10006edf(undefined4 param_1, undefined4 param_2, int param_3, undefined * param_4)
// Entry: 10006edf
// Body: 10006edf - 10006f3c
// ============================================================================
void FUN_10006edf(undefined4 param_1,undefined4 param_2,int param_3,undefined *param_4)

{
  void *local_14;
  undefined *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_10009138;
  puStack_10 = &DAT_10007146;
  local_14 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_14;
  while( true ) {
    param_3 = param_3 + -1;
    if (param_3 < 0) break;
    (*(code *)param_4)();
  }
  ExceptionList = local_14;
  return;
}

// clang-format on
