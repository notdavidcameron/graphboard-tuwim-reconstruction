/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Notes: raw Ghidra decompiler output with only file-level framing added.
*/
#include "../include/ghidra_compat.hpp"

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


// ============================================================================
// Function: FUN_10001140 at 10001140
// Signature: CDialog * __thiscall FUN_10001140(void * this, byte param_1)
// Entry: 10001140
// Body: 10001140 - 1000115d
// ============================================================================
CDialog * __thiscall FUN_10001140(void *this,byte param_1)

{
  FUN_10001160(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


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


// ============================================================================
// Function: FUN_10001280 at 10001280
// Signature: undefined __thiscall FUN_10001280(void * this, CDataExchange * param_1, char * param_2)
// Entry: 10001280
// Body: 10001280 - 1000152d
// ============================================================================
void __thiscall FUN_10001280(void *this,CDataExchange *param_1,char *param_2)

{
  int *piVar1;
  CDataExchange *pCVar2;
  int iVar3;
  CString *pCVar4;
  CString local_18 [4];
  CString local_14 [4];
  CWnd *local_10;
  CWnd *local_c;
  undefined1 *local_8;
  undefined4 local_4;
  
  pCVar2 = param_1;
  local_4 = 0xffffffff;
  local_8 = &LAB_100050b8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  DDX_Control(param_1,0xca,(CWnd *)((int)this + 0x8c));
  local_10 = (CWnd *)((int)this + 0xcc);
  DDX_Control(pCVar2,0xcd,local_10);
  DDX_Control(pCVar2,0xcb,(CWnd *)((int)this + 0x10c));
  DDX_Control(pCVar2,0xd0,(CWnd *)((int)this + 0x14c));
  DDX_Control(pCVar2,0xce,(CWnd *)((int)this + 0x18c));
  DDX_Control(pCVar2,0xc9,(CWnd *)((int)this + 0x1cc));
  DDX_Control(pCVar2,0xcc,(CWnd *)((int)this + 0x20c));
  DDX_Check(pCVar2,0xd0,(int *)((int)this + 0x24c));
  if ((*(int *)((int)this + 0x74) == 0) || (*(int *)((int)this + 0x24c) != 0)) {
    iVar3 = 0;
  }
  else {
    iVar3 = 1;
  }
  CWnd::EnableWindow((CWnd *)((int)this + 0x20c),iVar3);
  CWnd::EnableWindow((CWnd *)((int)this + 0x14c),(uint)(*(int *)((int)this + 0x74) != 0));
  if (*(int *)((int)this + 0x1ac) != 0) {
    if (*(int *)(*(int *)((int)this + 0x78) + -8) == 0) {
      CWnd::GetWindowTextA((CWnd *)((int)this + 0x18c),(CString *)((int)this + 0x78));
    }
    CString::CString((CString *)&param_1);
    local_4 = 0;
    CString::CString(local_18);
    local_4 = CONCAT31(local_4._1_3_,1);
    CString::Format((CString *)&param_1,(char *)&param_1);
    CWnd::SetWindowTextA((CWnd *)((int)this + 0x18c),param_2);
    piVar1 = *(int **)((int)this + 100);
    if (piVar1 != (int *)0x0) {
      CString::Format((CString *)
                      (uint)*(byte *)((int)piVar1 + *(int *)((int)this + 0x6c) * 4 + *piVar1 + 2),
                      (char *)local_14);
      pCVar4 = (CString *)operator+((CString *)&local_c,(CString *)((int)this + 0x7c));
      CString::operator=((CString *)&param_2,pCVar4);
      CString::~CString((CString *)&local_10);
      CWnd::SetWindowTextA((CWnd *)((int)this + 0x8c),param_2);
      CString::Format(*(CString **)((int)this + 0x6c),(char *)local_14);
      pCVar4 = (CString *)operator+((CString *)&local_c,(CString *)((int)this + 0x80));
      CString::operator=((CString *)&param_2,pCVar4);
      CString::~CString((CString *)&local_10);
      CWnd::SetWindowTextA((CWnd *)((int)this + 0x10c),param_2);
      CString::Format(local_14,(char *)local_14);
      pCVar4 = (CString *)operator+((CString *)&local_c,(CString *)((int)this + 0x84));
      CString::operator=((CString *)&param_2,pCVar4);
      CString::~CString((CString *)&local_10);
      CWnd::SetWindowTextA(local_c,param_2);
    }
    CString::~CString(local_14);
    CString::~CString((CString *)&param_2);
  }
  ExceptionList = local_8;
  return;
}


// ============================================================================
// Function: FUN_10001b50 at 10001b50
// Signature: UINT __fastcall FUN_10001b50(CWnd * param_1)
// Entry: 10001b50
// Body: 10001b50 - 10001c16
// ============================================================================
UINT __fastcall FUN_10001b50(CWnd *param_1)

{
  CClientDC *this;
  CDC *this_00;
  CPalette *pCVar1;
  UINT UVar2;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_1000511a;
  local_c = ExceptionList;
  if (*(int *)(param_1 + 0x60) != 0) {
    ExceptionList = &local_c;
    this = operator_new(0x14);
    local_4 = 0;
    if (this == (CClientDC *)0x0) {
      this_00 = (CDC *)0x0;
    }
    else {
      this_00 = (CDC *)CClientDC::CClientDC(this,param_1);
    }
    local_4 = 0xffffffff;
    pCVar1 = CDC::SelectPalette(this_00,*(CPalette **)(param_1 + 0x60),0);
    UVar2 = RealizePalette(*(HDC *)(this_00 + 4));
    CDC::SelectPalette(this_00,pCVar1,0);
    if (this_00 != (CDC *)0x0) {
      (**(code **)(*(int *)this_00 + 4))(1);
    }
    if (UVar2 != 0) {
      InvalidateRect(*(HWND *)(param_1 + 0x20),(RECT *)0x0,1);
    }
    ExceptionList = local_c;
    return UVar2;
  }
  ExceptionList = &local_c;
  UVar2 = CWnd::Default(param_1);
  ExceptionList = local_c;
  return UVar2;
}


// ============================================================================
// Function: FUN_10001c20 at 10001c20
// Signature: undefined __stdcall FUN_10001c20(void)
// Entry: 10001c20
// Body: 10001c20 - 10001c22
// ============================================================================
void FUN_10001c20(void)

{
  return;
}


// ============================================================================
// Function: FUN_10001c30 at 10001c30
// Signature: undefined __stdcall FUN_10001c30(void * param_1)
// Entry: 10001c30
// Body: 10001c30 - 10001c3d
// ============================================================================
void FUN_10001c30(void *param_1)

{
  operator_delete(param_1);
  return;
}


// ============================================================================
// Function: FUN_10001c70 at 10001c70
// Signature: CStatic * __thiscall FUN_10001c70(void * this, byte param_1)
// Entry: 10001c70
// Body: 10001c70 - 10001c8d
// ============================================================================
CStatic * __thiscall FUN_10001c70(void *this,byte param_1)

{
  CStatic::~CStatic(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: FUN_10001c90 at 10001c90
// Signature: CButton * __thiscall FUN_10001c90(void * this, byte param_1)
// Entry: 10001c90
// Body: 10001c90 - 10001cad
// ============================================================================
CButton * __thiscall FUN_10001c90(void *this,byte param_1)

{
  CButton::~CButton(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: FUN_10001d10 at 10001d10
// Signature: undefined __stdcall FUN_10001d10(void)
// Entry: 10001d10
// Body: 10001d10 - 10001d19
// ============================================================================
void FUN_10001d10(void)

{
  FUN_10001d50((CWinApp *)&DAT_100081d0);
  return;
}


// ============================================================================
// Function: FUN_10001d20 at 10001d20
// Signature: undefined __stdcall FUN_10001d20(void)
// Entry: 10001d20
// Body: 10001d20 - 10001d2b
// ============================================================================
void FUN_10001d20(void)

{
  FUN_10004b9c((_onexit_t)&LAB_10001d30);
  return;
}


// ============================================================================
// Function: FUN_10001d50 at 10001d50
// Signature: CWinApp * __fastcall FUN_10001d50(CWinApp * param_1)
// Entry: 10001d50
// Body: 10001d50 - 10001d63
// ============================================================================
CWinApp * __fastcall FUN_10001d50(CWinApp *param_1)

{
  CWinApp::CWinApp(param_1,(char *)0x0);
  *(undefined ***)param_1 = &PTR_LAB_100066a8;
  return param_1;
}


// ============================================================================
// Function: FUN_10001d70 at 10001d70
// Signature: CWinApp * __thiscall FUN_10001d70(void * this, byte param_1)
// Entry: 10001d70
// Body: 10001d70 - 10001d8d
// ============================================================================
CWinApp * __thiscall FUN_10001d70(void *this,byte param_1)

{
  CWinApp::~CWinApp(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: FUN_100021f4 at 100021f4
// Signature: int __stdcall FUN_100021f4(void)
// Entry: 100021f4
// Body: 100021f4 - 1000226a
// ============================================================================
int FUN_100021f4(void)

{
  int iVar1;
  int unaff_EBP;
  undefined4 *unaff_ESI;
  
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  if (unaff_ESI != (undefined4 *)0x0) {
    unaff_ESI[8] = *(undefined4 *)(unaff_EBP + 8);
    iVar1 = (**(code **)*unaff_ESI)();
    if (iVar1 != 0) {
      *unaff_ESI = &PTR_SetColor_QueryInterfaceFromMap_1000674c;
      unaff_ESI[1] = 1;
      DeleteCriticalSection((LPCRITICAL_SECTION)(unaff_ESI + 9));
      DeleteCriticalSection((LPCRITICAL_SECTION)(unaff_ESI + 2));
      operator_delete(unaff_ESI);
    }
    ExceptionList = *(void **)(unaff_EBP + -0xc);
    return iVar1;
  }
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  return *(int *)(unaff_EBP + -0x18);
}


// ============================================================================
// Function: FUN_100024e5 at 100024e5
// Signature: int __stdcall FUN_100024e5(void)
// Entry: 100024e5
// Body: 100024e5 - 1000255f
// ============================================================================
int FUN_100024e5(void)

{
  int iVar1;
  int unaff_EBP;
  undefined4 *unaff_ESI;
  
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  if (unaff_ESI != (undefined4 *)0x0) {
    iVar1 = (**(code **)*unaff_ESI)();
    if (iVar1 != 0) {
      *unaff_ESI = &PTR_LAB_100067a0;
      unaff_ESI[1] = &PTR_LAB_10006790;
      unaff_ESI[2] = 1;
      InterlockedDecrement(&DAT_100082ac);
      DeleteCriticalSection((LPCRITICAL_SECTION)(unaff_ESI + 3));
      operator_delete(unaff_ESI);
    }
    ExceptionList = *(void **)(unaff_EBP + -0xc);
    return iVar1;
  }
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  return *(int *)(unaff_EBP + -0x18);
}


// ============================================================================
// Function: FUN_10002570 at 10002570
// Signature: HRESULT __stdcall FUN_10002570(undefined4 param_1, undefined4 param_2, BSTR param_3, undefined4 * param_4)
// Entry: 10002570
// Body: 10002570 - 100025b6
// ============================================================================
HRESULT FUN_10002570(undefined4 param_1,undefined4 param_2,BSTR param_3,undefined4 *param_4)

{
  HRESULT HVar1;
  
  HVar1 = -0x7fffbffd;
  if (param_4 != (undefined4 *)0x0) {
    HVar1 = 0;
    if (DAT_100080dc == (int *)0x0) {
      HVar1 = FUN_10003c40(&PTR_DAT_100080d0,param_3);
    }
    *param_4 = DAT_100080dc;
    if (DAT_100080dc != (int *)0x0) {
      (**(code **)(*DAT_100080dc + 4))(DAT_100080dc);
      HVar1 = 0;
    }
  }
  return HVar1;
}


// ============================================================================
// Function: FUN_10002740 at 10002740
// Signature: undefined __stdcall FUN_10002740(undefined4 param_1)
// Entry: 10002740
// Body: 10002740 - 100027bd
// ============================================================================
void FUN_10002740(undefined4 param_1)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_1000517b;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x30);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    puVar1[1] = 0;
    SetColorObject_Construct(puVar1 + 3);
    puVar1[3] = &PTR_LAB_100067dc;
    puVar1[4] = &PTR_LAB_100067cc;
    puVar1[5] = param_1;
    *puVar1 = &PTR_LAB_100067c0;
    InterlockedIncrement(&DAT_100082ac);
    FUN_100027c9();
    return;
  }
  FUN_100027c9();
  return;
}


// ============================================================================
// Function: FUN_100027c9 at 100027c9
// Signature: undefined4 * __stdcall FUN_100027c9(void)
// Entry: 100027c9
// Body: 100027c9 - 1000284a
// ============================================================================
undefined4 * FUN_100027c9(void)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *unaff_EBX;
  int unaff_EBP;
  undefined4 *unaff_ESI;
  
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  if (unaff_ESI != unaff_EBX) {
    puVar1 = (undefined4 *)(**(code **)*unaff_ESI)();
    if (puVar1 != unaff_EBX) {
      *unaff_ESI = &PTR_LAB_100067c0;
      unaff_ESI[1] = 1;
      InterlockedDecrement(&DAT_100082ac);
      if (unaff_ESI + 3 == unaff_EBX) {
        puVar2 = (undefined4 *)0x0;
      }
      else {
        puVar2 = unaff_ESI + 5;
      }
      DeleteCriticalSection((LPCRITICAL_SECTION)(puVar2 + 1));
      operator_delete(unaff_ESI);
    }
    ExceptionList = *(void **)(unaff_EBP + -0xc);
    return puVar1;
  }
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  return *(undefined4 **)(unaff_EBP + -0x18);
}


// ============================================================================
// Function: FUN_100028d0 at 100028d0
// Signature: undefined __fastcall FUN_100028d0(undefined4 * param_1)
// Entry: 100028d0
// Body: 100028d0 - 10002907
// ============================================================================
void __fastcall FUN_100028d0(undefined4 *param_1)

{
  *param_1 = &PTR_LAB_100067a0;
  param_1[1] = &PTR_LAB_10006790;
  param_1[2] = 1;
  InterlockedDecrement(&DAT_100082ac);
  DeleteCriticalSection
            ((LPCRITICAL_SECTION)((-(uint)(param_1 != (undefined4 *)0x0) & (uint)(param_1 + 2)) + 4)
            );
  return;
}


// ============================================================================
// Function: FUN_10002a30 at 10002a30
// Signature: LONG __stdcall FUN_10002a30(undefined4 * param_1)
// Entry: 10002a30
// Body: 10002a30 - 10002a60
// ============================================================================
LONG FUN_10002a30(undefined4 *param_1)

{
  LONG LVar1;
  
  LVar1 = InterlockedDecrement(param_1 + 1);
  if ((LVar1 == 0) && (param_1 != (undefined4 *)0x0)) {
    FUN_10002a70(param_1);
    operator_delete(param_1);
  }
  return LVar1;
}


// ============================================================================
// Function: FUN_10002a70 at 10002a70
// Signature: undefined __fastcall FUN_10002a70(undefined4 * param_1)
// Entry: 10002a70
// Body: 10002a70 - 10002aaf
// ============================================================================
void __fastcall FUN_10002a70(undefined4 *param_1)

{
  *param_1 = &PTR_LAB_100067c0;
  param_1[1] = 1;
  InterlockedDecrement(&DAT_100082ac);
  if (param_1 != (undefined4 *)0xfffffff4) {
    DeleteCriticalSection((LPCRITICAL_SECTION)(param_1 + 6));
    return;
  }
  DeleteCriticalSection((LPCRITICAL_SECTION)0x4);
  return;
}


// ============================================================================
// Function: FUN_100031a0 at 100031a0
// Signature: LSTATUS __fastcall FUN_100031a0(undefined4 * param_1)
// Entry: 100031a0
// Body: 100031a0 - 100031b9
// ============================================================================
LSTATUS __fastcall FUN_100031a0(undefined4 *param_1)

{
  LSTATUS LVar1;
  
  LVar1 = 0;
  if ((HKEY)*param_1 != (HKEY)0x0) {
    LVar1 = RegCloseKey((HKEY)*param_1);
    *param_1 = 0;
  }
  return LVar1;
}


// ============================================================================
// Function: FUN_100031c0 at 100031c0
// Signature: undefined __thiscall FUN_100031c0(void * this, HKEY param_1, LPCSTR param_2, LPSTR param_3, DWORD param_4, REGSAM param_5, LPSECURITY_ATTRIBUTES param_6, undefined4 * param_7)
// Entry: 100031c0
// Body: 100031c0 - 1000322b
// ============================================================================
void __thiscall
FUN_100031c0(void *this,HKEY param_1,LPCSTR param_2,LPSTR param_3,DWORD param_4,REGSAM param_5,
            LPSECURITY_ATTRIBUTES param_6,undefined4 *param_7)

{
  LSTATUS LVar1;
  HKEY local_4;
  
  local_4 = (HKEY)0x0;
  LVar1 = RegCreateKeyExA(param_1,param_2,0,param_3,param_4,param_5,param_6,&local_4,
                          (LPDWORD)&param_6);
  if (param_7 != (undefined4 *)0x0) {
    *param_7 = param_6;
  }
  if (LVar1 == 0) {
    if (*(HKEY *)this != (HKEY)0x0) {
      RegCloseKey(*(HKEY *)this);
      *(undefined4 *)this = 0;
    }
    *(HKEY *)this = local_4;
  }
  return;
}


// ============================================================================
// Function: FUN_10003230 at 10003230
// Signature: LSTATUS __thiscall FUN_10003230(void * this, LPCSTR param_1, BYTE * param_2, LPCSTR param_3)
// Entry: 10003230
// Body: 10003230 - 1000329a
// ============================================================================
LSTATUS __thiscall FUN_10003230(void *this,LPCSTR param_1,BYTE *param_2,LPCSTR param_3)

{
  HKEY hKey;
  BYTE *lpData;
  HKEY hKey_00;
  LSTATUS LVar1;
  int iVar2;
  HKEY local_4;
  
  local_4 = (HKEY)0x0;
  LVar1 = RegCreateKeyExA(*(HKEY *)this,param_1,0,(LPSTR)0x0,0,0xf003f,(LPSECURITY_ATTRIBUTES)0x0,
                          &local_4,(LPDWORD)&param_1);
  hKey_00 = local_4;
  lpData = param_2;
  hKey = (HKEY)0x0;
  if (LVar1 == 0) {
    iVar2 = lstrlenA((LPCSTR)param_2);
    LVar1 = RegSetValueExA(hKey_00,param_3,0,1,lpData,iVar2 + 1);
    hKey = hKey_00;
  }
  if (hKey != (HKEY)0x0) {
    RegCloseKey(hKey);
  }
  return LVar1;
}


// ============================================================================
// Function: FUN_100032a0 at 100032a0
// Signature: undefined __thiscall FUN_100032a0(void * this, BYTE * param_1, LPCSTR param_2)
// Entry: 100032a0
// Body: 100032a0 - 100032c8
// ============================================================================
void __thiscall FUN_100032a0(void *this,BYTE *param_1,LPCSTR param_2)

{
  int iVar1;
  
  iVar1 = lstrlenA((LPCSTR)param_1);
  RegSetValueExA(*(HKEY *)this,param_2,0,1,param_1,iVar1 + 1);
  return;
}

// clang-format on
