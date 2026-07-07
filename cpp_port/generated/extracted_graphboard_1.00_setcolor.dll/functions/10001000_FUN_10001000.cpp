/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_10001000 @ 10001000
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001000 at 10001000
// Signature: CDialog * __thiscall FUN_10001000(void * this, undefined4 param_1)
// Entry: 10001000
// Body: 10001000 - 1000113b
// ============================================================================
CDialog * __thiscall FUN_10001000(void *this,undefined4 param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10004fde;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  CDialog::CDialog(this,0xca,(CWnd *)0x0);
  local_4 = 0;
  CString::CString((CString *)((int)this + 0x78));
  local_4._0_1_ = 1;
  CString::CString((CString *)((int)this + 0x7c));
  local_4._0_1_ = 2;
  CString::CString((CString *)((int)this + 0x80));
  local_4._0_1_ = 3;
  CString::CString((CString *)((int)this + 0x84));
  local_4._0_1_ = 4;
  CWnd::CWnd((CWnd *)((int)this + 0x8c));
  *(undefined ***)((int)this + 0x8c) = &PTR_LAB_10006584;
  local_4._0_1_ = 5;
  CWnd::CWnd((CWnd *)((int)this + 0xcc));
  *(undefined ***)((int)this + 0xcc) = &PTR_LAB_10006584;
  local_4._0_1_ = 6;
  CWnd::CWnd((CWnd *)((int)this + 0x10c));
  *(undefined ***)((int)this + 0x10c) = &PTR_LAB_10006584;
  local_4._0_1_ = 7;
  CWnd::CWnd((CWnd *)((int)this + 0x14c));
  *(undefined ***)((int)this + 0x14c) = &PTR_LAB_100064c0;
  local_4._0_1_ = 8;
  CWnd::CWnd((CWnd *)((int)this + 0x18c));
  *(undefined ***)((int)this + 0x18c) = &PTR_LAB_10006584;
  local_4._0_1_ = 9;
  CWnd::CWnd((CWnd *)((int)this + 0x1cc));
  *(undefined ***)((int)this + 0x1cc) = &PTR_LAB_10006584;
  local_4 = CONCAT31(local_4._1_3_,10);
  CWnd::CWnd((CWnd *)((int)this + 0x20c));
  *(undefined ***)((int)this + 0x20c) = &PTR_LAB_100064c0;
  *(undefined4 *)((int)this + 0x60) = param_1;
  *(undefined ***)this = &PTR_LAB_100063e8;
  *(undefined4 *)((int)this + 0x24c) = 0;
  *(undefined4 *)((int)this + 100) = 0;
  *(undefined4 *)((int)this + 0x74) = 0;
  ExceptionList = local_c;
  return this;
}

// clang-format on
