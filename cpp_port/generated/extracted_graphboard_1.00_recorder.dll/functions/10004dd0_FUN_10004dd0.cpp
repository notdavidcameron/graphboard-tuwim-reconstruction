/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10004dd0 @ 10004dd0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10004dd0 at 10004dd0
// Signature: CPropertyPage * __fastcall FUN_10004dd0(CPropertyPage * param_1)
// Entry: 10004dd0
// Body: 10004dd0 - 10004e2d
// ============================================================================
CPropertyPage * __fastcall FUN_10004dd0(CPropertyPage *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10005ba8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  CPropertyPage::CPropertyPage(param_1,0xca,0);
  local_4 = 0;
  CWnd::CWnd((CWnd *)(param_1 + 0x94));
  *(undefined ***)(param_1 + 0x94) = &PTR_LAB_10006d48;
  *(undefined ***)param_1 = &PTR_LAB_10006c50;
  ExceptionList = local_c;
  return param_1;
}

// clang-format on
