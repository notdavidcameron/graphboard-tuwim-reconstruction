/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: VideoSettingsPage_Destroy @ 100050b0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: VideoSettingsPage_Destroy at 100050b0
// Signature: undefined __fastcall VideoSettingsPage_Destroy(CPropertyPage * param_1)
// Entry: 100050b0
// Body: 100050b0 - 100051a4
// ============================================================================
void __fastcall VideoSettingsPage_Destroy(CPropertyPage *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  puStack_8 = &LAB_10006d54;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined ***)param_1 = &PTR_LAB_10007ce8;
  local_4 = 10;
  CButton::~CButton((CButton *)(param_1 + 0x2dc));
  local_4._0_1_ = 9;
  CListBox::~CListBox((CListBox *)(param_1 + 0x29c));
  local_4._0_1_ = 8;
  CButton::~CButton((CButton *)(param_1 + 0x25c));
  local_4._0_1_ = 7;
  CButton::~CButton((CButton *)(param_1 + 0x21c));
  local_4._0_1_ = 6;
  CButton::~CButton((CButton *)(param_1 + 0x1dc));
  local_4._0_1_ = 5;
  CButton::~CButton((CButton *)(param_1 + 0x19c));
  local_4._0_1_ = 4;
  CEdit::~CEdit((CEdit *)(param_1 + 0x15c));
  local_4._0_1_ = 3;
  CEdit::~CEdit((CEdit *)(param_1 + 0x11c));
  local_4._0_1_ = 2;
  CButton::~CButton((CButton *)(param_1 + 0xdc));
  local_4._0_1_ = 1;
  CStatic::~CStatic((CStatic *)(param_1 + 0x9c));
  local_4 = (uint)local_4._1_3_ << 8;
  CString::~CString((CString *)(param_1 + 0x98));
  local_4 = 0xffffffff;
  CPropertyPage::~CPropertyPage(param_1);
  ExceptionList = local_c;
  return;
}

// clang-format on
