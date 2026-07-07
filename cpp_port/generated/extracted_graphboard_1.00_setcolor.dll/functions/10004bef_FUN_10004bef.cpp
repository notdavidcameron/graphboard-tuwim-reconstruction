/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_10004bef @ 10004bef
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10004bef at 10004bef
// Signature: undefined __stdcall FUN_10004bef(undefined4 param_1, undefined4 param_2, int param_3, undefined * param_4)
// Entry: 10004bef
// Body: 10004bef - 10004c56
// ============================================================================
void FUN_10004bef(undefined4 param_1,undefined4 param_2,int param_3,undefined *param_4)

{
  void *local_14;
  undefined *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_10006900;
  puStack_10 = &DAT_10004ed6;
  local_14 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_14;
  while( true ) {
    param_3 = param_3 + -1;
    if (param_3 < 0) break;
    (*(code *)param_4)();
  }
  local_8 = 0xffffffff;
  FUN_10004c57();
  ExceptionList = local_14;
  return;
}

// clang-format on
