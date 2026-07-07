/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10001060 @ 10001060
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001060 at 10001060
// Signature: undefined __fastcall FUN_10001060(CWnd * param_1)
// Entry: 10001060
// Body: 10001060 - 100010b5
// ============================================================================
void __fastcall FUN_10001060(CWnd *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_10006908;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined ***)param_1 = &PTR_LAB_10007438;
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
