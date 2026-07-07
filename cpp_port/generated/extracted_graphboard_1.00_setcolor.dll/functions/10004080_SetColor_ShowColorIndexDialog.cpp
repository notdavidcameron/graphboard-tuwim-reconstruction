/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: SetColor_ShowColorIndexDialog @ 10004080
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: SetColor_ShowColorIndexDialog at 10004080
// Signature: undefined4 __stdcall SetColor_ShowColorIndexDialog(undefined4 param_1, byte * param_2, undefined4 param_3, int * param_4)
// Entry: 10004080
// Body: 10004080 - 10004276
// ============================================================================
undefined4
SetColor_ShowColorIndexDialog(undefined4 param_1,byte *param_2,undefined4 param_3,int *param_4)

{
  byte bVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  AFX_MODULE_STATE *pAVar4;
  int iVar5;
  HGLOBAL pvVar6;
  undefined **local_25c [25];
  LPCVOID local_1f8;
  uint local_1f0;
  int *local_1e8;
  CString aCStack_1e4 [4];
  CString aCStack_1e0 [4];
  CString aCStack_1dc [4];
  CString aCStack_1d8 [4];
  byte *local_1d4;
  CStatic aCStack_1d0 [64];
  CStatic aCStack_190 [64];
  CStatic aCStack_150 [64];
  CButton aCStack_110 [64];
  CStatic aCStack_d0 [64];
  CStatic aCStack_90 [64];
  CButton local_50 [64];
  undefined4 local_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10005334;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  pAVar4 = AfxGetModuleState();
  uVar3 = DAT_100082a0;
  uVar2 = *(undefined4 *)(pAVar4 + 0xc);
  pAVar4 = AfxGetModuleState();
  *(undefined4 *)(pAVar4 + 0xc) = uVar3;
  FUN_10001000(local_25c,param_3);
  local_4 = 0;
  local_1e8 = param_4;
  bVar1 = *param_2;
  local_1f0 = (uint)bVar1;
  if (param_4 != (int *)0x0) {
    local_10 = 1;
  }
  local_1d4 = param_2;
  iVar5 = CDialog::DoModal((CDialog *)local_25c);
  if (iVar5 == 1) {
    *param_2 = (byte)local_1f0;
  }
  else {
    *param_2 = bVar1;
    if (local_1e8 != (int *)0x0) {
      (**(code **)(*local_1e8 + 0xc))(local_1e8);
    }
  }
  pAVar4 = AfxGetModuleState();
  *(undefined4 *)(pAVar4 + 0xc) = uVar2;
  local_25c[0] = &PTR_LAB_100063e8;
  local_4 = 0xb;
  if (local_1f8 != (LPCVOID)0x0) {
    pvVar6 = GlobalHandle(local_1f8);
    GlobalUnlock(pvVar6);
    pvVar6 = GlobalHandle(local_1f8);
    GlobalFree(pvVar6);
  }
  CButton::~CButton(local_50);
  local_4._0_1_ = 10;
  CStatic::~CStatic(aCStack_90);
  local_4._0_1_ = 9;
  CStatic::~CStatic(aCStack_d0);
  local_4._0_1_ = 8;
  CButton::~CButton(aCStack_110);
  local_4._0_1_ = 7;
  CStatic::~CStatic(aCStack_150);
  local_4._0_1_ = 6;
  CStatic::~CStatic(aCStack_190);
  local_4._0_1_ = 5;
  CStatic::~CStatic(aCStack_1d0);
  local_4._0_1_ = 4;
  CString::~CString(aCStack_1d8);
  local_4._0_1_ = 3;
  CString::~CString(aCStack_1dc);
  local_4._0_1_ = 2;
  CString::~CString(aCStack_1e0);
  local_4 = CONCAT31(local_4._1_3_,1);
  CString::~CString(aCStack_1e4);
  local_4 = 0xffffffff;
  CDialog::~CDialog((CDialog *)local_25c);
  ExceptionList = pvStack_c;
  return 0;
}

// clang-format on
