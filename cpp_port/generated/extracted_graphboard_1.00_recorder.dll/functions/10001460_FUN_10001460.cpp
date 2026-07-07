/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10001460 @ 10001460
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001460 at 10001460
// Signature: undefined __fastcall FUN_10001460(CPropertySheet * param_1)
// Entry: 10001460
// Body: 10001460 - 100014b4
// ============================================================================
void __fastcall FUN_10001460(CPropertySheet *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_10005948;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined ***)param_1 = &PTR_LAB_100064b8;
  local_4 = 0;
  FUN_10004e50((CPropertyPage *)(param_1 + 0x8c));
  local_4 = 0xffffffff;
  CPropertySheet::~CPropertySheet(param_1);
  ExceptionList = local_c;
  return;
}

// clang-format on
