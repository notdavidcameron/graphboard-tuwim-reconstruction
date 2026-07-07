/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10004f30 @ 10004f30
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10004f30 at 10004f30
// Signature: CPropertyPage * __fastcall FUN_10004f30(CPropertyPage * param_1)
// Entry: 10004f30
// Body: 10004f30 - 10005089
// ============================================================================
CPropertyPage * __fastcall FUN_10004f30(CPropertyPage *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10006cb4;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  CPropertyPage::CPropertyPage(param_1,0xc9,0);
  local_4 = 0;
  CString::CString((CString *)(param_1 + 0x98));
  local_4._0_1_ = 1;
  CWnd::CWnd((CWnd *)(param_1 + 0x9c));
  *(undefined ***)(param_1 + 0x9c) = &PTR_LAB_1000803c;
  local_4._0_1_ = 2;
  CWnd::CWnd((CWnd *)(param_1 + 0xdc));
  *(undefined ***)(param_1 + 0xdc) = &PTR_LAB_10007f78;
  local_4._0_1_ = 3;
  CWnd::CWnd((CWnd *)(param_1 + 0x11c));
  *(undefined ***)(param_1 + 0x11c) = &PTR_LAB_10007eb8;
  local_4._0_1_ = 4;
  CWnd::CWnd((CWnd *)(param_1 + 0x15c));
  *(undefined ***)(param_1 + 0x15c) = &PTR_LAB_10007eb8;
  local_4._0_1_ = 5;
  CWnd::CWnd((CWnd *)(param_1 + 0x19c));
  *(undefined ***)(param_1 + 0x19c) = &PTR_LAB_10007f78;
  local_4._0_1_ = 6;
  CWnd::CWnd((CWnd *)(param_1 + 0x1dc));
  *(undefined ***)(param_1 + 0x1dc) = &PTR_LAB_10007f78;
  local_4._0_1_ = 7;
  CWnd::CWnd((CWnd *)(param_1 + 0x21c));
  *(undefined ***)(param_1 + 0x21c) = &PTR_LAB_10007f78;
  local_4._0_1_ = 8;
  CWnd::CWnd((CWnd *)(param_1 + 0x25c));
  *(undefined ***)(param_1 + 0x25c) = &PTR_LAB_10007f78;
  local_4._0_1_ = 9;
  CWnd::CWnd((CWnd *)(param_1 + 0x29c));
  *(undefined ***)(param_1 + 0x29c) = &PTR_LAB_10007de0;
  local_4 = CONCAT31(local_4._1_3_,10);
  CWnd::CWnd((CWnd *)(param_1 + 0x2dc));
  *(undefined ***)(param_1 + 0x2dc) = &PTR_LAB_10007f78;
  *(undefined ***)param_1 = &PTR_LAB_10007ce8;
  *(undefined4 *)(param_1 + 0x31c) = 0;
  *(undefined4 *)(param_1 + 0x94) = 0;
  ExceptionList = local_c;
  return param_1;
}

// clang-format on
