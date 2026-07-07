/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10006670 @ 10006670
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10006670 at 10006670
// Signature: undefined __fastcall FUN_10006670(CWnd * param_1)
// Entry: 10006670
// Body: 10006670 - 100066c5
// ============================================================================
void __fastcall FUN_10006670(CWnd *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_10007718;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined ***)param_1 = &PTR_LAB_10008f58;
  local_4 = 0;
  if (*(int *)(param_1 + 0x20) != 0) {
    CWnd::DestroyWindow(param_1);
  }
  local_4 = 0xffffffff;
  CWnd::~CWnd(param_1);
  ExceptionList = local_c;
  return;
}

// clang-format on
