/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_10004c6f @ 10004c6f
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10004c6f at 10004c6f
// Signature: undefined __stdcall FUN_10004c6f(undefined4 param_1, undefined4 param_2, int param_3, undefined * param_4)
// Entry: 10004c6f
// Body: 10004c6f - 10004ccc
// ============================================================================
void FUN_10004c6f(undefined4 param_1,undefined4 param_2,int param_3,undefined *param_4)

{
  void *local_14;
  undefined *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_10006910;
  puStack_10 = &DAT_10004ed6;
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
