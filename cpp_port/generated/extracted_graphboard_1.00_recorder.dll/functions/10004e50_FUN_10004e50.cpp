/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10004e50 @ 10004e50
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10004e50 at 10004e50
// Signature: undefined __fastcall FUN_10004e50(CPropertyPage * param_1)
// Entry: 10004e50
// Body: 10004e50 - 10004ea4
// ============================================================================
void __fastcall FUN_10004e50(CPropertyPage *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_10005bc8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined ***)param_1 = &PTR_LAB_10006c50;
  local_4 = 0;
  CComboBox::~CComboBox((CComboBox *)(param_1 + 0x94));
  local_4 = 0xffffffff;
  CPropertyPage::~CPropertyPage(param_1);
  ExceptionList = local_c;
  return;
}

// clang-format on
