/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_10001160 @ 10001160
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001160 at 10001160
// Signature: undefined __fastcall FUN_10001160(CDialog * param_1)
// Entry: 10001160
// Body: 10001160 - 10001274
// ============================================================================
void __fastcall FUN_10001160(CDialog *param_1)

{
  HGLOBAL pvVar1;
  void *pvStack_c;
  undefined1 *puStack_8;
  int local_4;
  
  puStack_8 = &LAB_1000507e;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  *(undefined ***)param_1 = &PTR_LAB_100063e8;
  local_4 = 10;
  if (*(LPCVOID *)(param_1 + 100) != (LPCVOID)0x0) {
    pvVar1 = GlobalHandle(*(LPCVOID *)(param_1 + 100));
    GlobalUnlock(pvVar1);
    pvVar1 = GlobalHandle(*(LPCVOID *)(param_1 + 100));
    GlobalFree(pvVar1);
  }
  CButton::~CButton((CButton *)(param_1 + 0x20c));
  local_4._0_1_ = 9;
  CStatic::~CStatic((CStatic *)(param_1 + 0x1cc));
  local_4._0_1_ = 8;
  CStatic::~CStatic((CStatic *)(param_1 + 0x18c));
  local_4._0_1_ = 7;
  CButton::~CButton((CButton *)(param_1 + 0x14c));
  local_4._0_1_ = 6;
  CStatic::~CStatic((CStatic *)(param_1 + 0x10c));
  local_4._0_1_ = 5;
  CStatic::~CStatic((CStatic *)(param_1 + 0xcc));
  local_4._0_1_ = 4;
  CStatic::~CStatic((CStatic *)(param_1 + 0x8c));
  local_4._0_1_ = 3;
  CString::~CString((CString *)(param_1 + 0x84));
  local_4._0_1_ = 2;
  CString::~CString((CString *)(param_1 + 0x80));
  local_4._0_1_ = 1;
  CString::~CString((CString *)(param_1 + 0x7c));
  local_4 = (uint)local_4._1_3_ << 8;
  CString::~CString((CString *)(param_1 + 0x78));
  local_4 = 0xffffffff;
  CDialog::~CDialog(param_1);
  ExceptionList = pvStack_c;
  return;
}

// clang-format on
