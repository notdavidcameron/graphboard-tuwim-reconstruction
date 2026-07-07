/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_100010e0 @ 100010e0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100010e0 at 100010e0
// Signature: undefined __fastcall FUN_100010e0(CWnd * param_1)
// Entry: 100010e0
// Body: 100010e0 - 10001135
// ============================================================================
void __fastcall FUN_100010e0(CWnd *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_10005908;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined ***)param_1 = &PTR_LAB_100063b8;
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
