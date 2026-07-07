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
// Function: ~CWinApp at 10001d40
// Signature: void __thiscall ~CWinApp(CWinApp * this)
// Entry: 10001d40
// Body: 10001d40 - 10001d44
// ============================================================================
void __thiscall CWinApp::~CWinApp(CWinApp *this)

{
                    /* WARNING: Could not recover jumptable at 0x1000488c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~CWinApp(this);
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
// Function: DllCanUnloadNow at 10001f50
// Signature: HRESULT __stdcall DllCanUnloadNow(void)
// Entry: 10001f50
// Body: 10001f50 - 10001f98
// ============================================================================
HRESULT DllCanUnloadNow(void)

{
  AFX_MODULE_STATE *pAVar1;
  long lVar2;
  undefined4 local_8;
  int local_4;
  
                    /* 0x1f50  1  DllCanUnloadNow */
  pAVar1 = (AFX_MODULE_STATE *)FUN_100049db();
  AFX_MAINTAIN_STATE2::AFX_MAINTAIN_STATE2((AFX_MAINTAIN_STATE2 *)&local_8,pAVar1);
  lVar2 = AfxDllCanUnloadNow();
  if ((lVar2 == 0) && (DAT_100082ac == 0)) {
    *(undefined4 *)(local_4 + 4) = local_8;
    return 0;
  }
  *(undefined4 *)(local_4 + 4) = local_8;
  return 1;
}


// ============================================================================
// Function: DllGetClassObject at 10001fa0
// Signature: HRESULT __stdcall DllGetClassObject(IID * rclsid, IID * riid, LPVOID * ppv)
// Entry: 10001fa0
// Body: 10001fa0 - 10002085
// ============================================================================
/* WARNING: Removing unreachable block (ram,0x10001fac) */

HRESULT DllGetClassObject(IID *rclsid,IID *riid,LPVOID *ppv)

{
  int *piVar1;
  ulong *puVar2;
  ulong uVar3;
  int iVar4;
  undefined4 *puVar5;
  
                    /* 0x1fa0  2  DllGetClassObject */
  puVar5 = DAT_100082a8;
  iVar4 = 0;
  if (ppv == (LPVOID *)0x0) {
    return -0x7fffbffd;
  }
  *ppv = (LPVOID)0x0;
  puVar2 = (ulong *)*puVar5;
  do {
    if (puVar2 == (ulong *)0x0) {
LAB_1000206e:
      if ((*ppv == (LPVOID)0x0) && (iVar4 == 0)) {
        iVar4 = -0x7ffbfeef;
      }
      return iVar4;
    }
    if ((((puVar5[2] != 0) && (rclsid->Data1 == *puVar2)) &&
        (uVar3._0_2_ = rclsid->Data2, uVar3._2_2_ = rclsid->Data3, uVar3 == puVar2[1])) &&
       ((*(ulong *)rclsid->Data4 == puVar2[2] && (*(ulong *)(rclsid->Data4 + 4) == puVar2[3])))) {
      piVar1 = puVar5 + 4;
      if (puVar5[4] == 0) {
        EnterCriticalSection((LPCRITICAL_SECTION)&DAT_100082e4);
        if (*piVar1 == 0) {
          iVar4 = (*(code *)puVar5[2])(puVar5[3],&DAT_10006938,piVar1);
        }
        LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_100082e4);
      }
      puVar5 = (undefined4 *)*piVar1;
      if (puVar5 != (undefined4 *)0x0) {
        iVar4 = (**(code **)*puVar5)(puVar5,riid,ppv);
      }
      goto LAB_1000206e;
    }
    if (DAT_10008298 == 100) {
      puVar5 = puVar5 + 7;
    }
    else {
      puVar5 = puVar5 + 9;
    }
    puVar2 = (ulong *)*puVar5;
  } while( true );
}


// ============================================================================
// Function: DllRegisterServer at 10002090
// Signature: HRESULT __stdcall DllRegisterServer(void)
// Entry: 10002090
// Body: 10002090 - 10002112
// ============================================================================
/* WARNING: Removing unreachable block (ram,0x10002099) */

HRESULT DllRegisterServer(void)

{
  int iVar1;
  int *piVar2;
  HRESULT HVar3;
  int *piVar4;
  int iVar5;
  
                    /* 0x2090  3  DllRegisterServer */
  iVar5 = 0;
  piVar4 = DAT_100082a8;
  if (*DAT_100082a8 != 0) {
    do {
      if (((code *)piVar4[6] == (code *)0x0) || (iVar1 = (*(code *)piVar4[6])(), iVar1 == 0)) {
        iVar5 = (*(code *)piVar4[1])(1);
        if (iVar5 < 0) {
          return iVar5;
        }
        if (DAT_10008298 == 0x84) {
          iVar5 = 1;
          piVar2 = (int *)(*(code *)piVar4[7])();
          iVar5 = FUN_100037c0(*piVar4,piVar2,iVar5);
          if (iVar5 < 0) {
            return iVar5;
          }
        }
      }
      if (DAT_10008298 == 100) {
        piVar4 = piVar4 + 7;
      }
      else {
        piVar4 = piVar4 + 9;
      }
    } while (*piVar4 != 0);
    if (iVar5 < 0) {
      return iVar5;
    }
  }
  HVar3 = FUN_10003900(0x10008298,(LPCWSTR)0x0);
  return HVar3;
}


// ============================================================================
// Function: DllUnregisterServer at 10002120
// Signature: undefined4 __stdcall DllUnregisterServer(void)
// Entry: 10002120
// Body: 10002120 - 1000216e
// ============================================================================
undefined4 DllUnregisterServer(void)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  
                    /* 0x2120  4  DllUnregisterServer */
  iVar1 = *DAT_100082a8;
  piVar3 = DAT_100082a8;
  while (iVar1 != 0) {
    if (((code *)piVar3[6] == (code *)0x0) || (iVar1 = (*(code *)piVar3[6])(), iVar1 == 0)) {
      iVar1 = 0;
      (*(code *)piVar3[1])();
      piVar2 = (int *)(*(code *)piVar3[7])(0);
      FUN_100037c0(*piVar3,piVar2,iVar1);
    }
    if (DAT_10008298 == 100) {
      piVar3 = piVar3 + 7;
    }
    else {
      piVar3 = piVar3 + 9;
    }
    iVar1 = *piVar3;
  }
  return 0;
}


// ============================================================================
// Function: Create_SetColorClassFactory at 10002180
// Signature: undefined __stdcall Create_SetColorClassFactory(void)
// Entry: 10002180
// Body: 10002180 - 100021ea
// ============================================================================
void Create_SetColorClassFactory(void)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_1000513b;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x3c);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    SetColorRefCountedState_Construct(puVar1 + 1);
    *puVar1 = &PTR_LAB_10006760;
    InitializeCriticalSection((LPCRITICAL_SECTION)(puVar1 + 9));
    *puVar1 = &PTR_SetColor_QueryInterfaceFromMap_1000674c;
    FUN_100021f4();
    return;
  }
  FUN_100021f4();
  return;
}


// ============================================================================
// Function: Catch@100021eb at 100021eb
// Signature: undefined * __stdcall Catch@100021eb(void)
// Entry: 100021eb
// Body: 100021eb - 100021f0
// ============================================================================
undefined * Catch_100021eb(void)

{
  return &DAT_100021f1;
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
// Function: SetColorClassFactory_AddRef at 100022a0
// Signature: LONG __stdcall SetColorClassFactory_AddRef(int param_1)
// Entry: 100022a0
// Body: 100022a0 - 100022dd
// ============================================================================
LONG SetColorClassFactory_AddRef(int param_1)

{
  LONG LVar1;
  
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x24));
  LVar1 = InterlockedIncrement((LONG *)(param_1 + 4));
  if (*(LONG *)(param_1 + 4) == 2) {
    InterlockedIncrement(&DAT_100082ac);
  }
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x24));
  return LVar1;
}


// ============================================================================
// Function: SetColorClassFactory_Release at 100022e0
// Signature: int __stdcall SetColorClassFactory_Release(undefined4 * param_1)
// Entry: 100022e0
// Body: 100022e0 - 1000233c
// ============================================================================
int SetColorClassFactory_Release(undefined4 *param_1)

{
  int iVar1;
  
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  InterlockedDecrement(param_1 + 1);
  iVar1 = param_1[1];
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  if (iVar1 == 0) {
    if (param_1 != (undefined4 *)0x0) {
      SetColorClassFactory_Destroy(param_1);
      operator_delete(param_1);
      return 0;
    }
  }
  else if (iVar1 == 1) {
    InterlockedDecrement(&DAT_100082ac);
  }
  return iVar1;
}


// ============================================================================
// Function: SetColorClassFactory_Destroy at 10002340
// Signature: undefined __fastcall SetColorClassFactory_Destroy(undefined4 * param_1)
// Entry: 10002340
// Body: 10002340 - 1000236f
// ============================================================================
void __fastcall SetColorClassFactory_Destroy(undefined4 *param_1)

{
  *param_1 = &PTR_SetColor_QueryInterfaceFromMap_1000674c;
  param_1[1] = 1;
  DeleteCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  DeleteCriticalSection
            ((LPCRITICAL_SECTION)((-(uint)(param_1 != (undefined4 *)0x0) & (uint)(param_1 + 1)) + 4)
            );
  return;
}


// ============================================================================
// Function: SetColor_QueryInterfaceFromMap at 10002370
// Signature: int __stdcall SetColor_QueryInterfaceFromMap(int * param_1, int * param_2, undefined4 * param_3)
// Entry: 10002370
// Body: 10002370 - 10002462
// ============================================================================
int SetColor_QueryInterfaceFromMap(int *param_1,int *param_2,undefined4 *param_3)

{
  undefined **ppuVar1;
  undefined *puVar2;
  int iVar3;
  undefined **ppuVar4;
  int *piVar5;
  
  ppuVar4 = &PTR_DAT_10006778;
  if (param_3 == (undefined4 *)0x0) {
    return -0x7fffbffd;
  }
  *param_3 = 0;
  puVar2 = DAT_10006780;
  if ((((*param_2 == 0) && (param_2[1] == 0)) && (param_2[2] == 0xc0)) && (param_2[3] == 0x46000000)
     ) {
    iVar3 = DAT_1000677c + (int)param_1;
    (**(code **)(*(int *)(DAT_1000677c + (int)param_1) + 4))(iVar3);
    *param_3 = iVar3;
    return 0;
  }
  do {
    if (puVar2 == (undefined *)0x0) {
      return -0x7fffbffe;
    }
    piVar5 = (int *)*ppuVar4;
    if ((piVar5 == (int *)0x0) ||
       (((*piVar5 == *param_2 && (piVar5[1] == param_2[1])) &&
        ((piVar5[2] == param_2[2] && (piVar5[3] == param_2[3])))))) {
      if ((code *)ppuVar4[2] == (code *)0x1) {
        piVar5 = (int *)(ppuVar4[1] + (int)param_1);
        (**(code **)(*piVar5 + 4))(piVar5);
        *param_3 = piVar5;
        return 0;
      }
      iVar3 = (*(code *)ppuVar4[2])(param_1,param_2,param_3,ppuVar4[1]);
      if (iVar3 == 0) {
        return 0;
      }
      if ((piVar5 != (int *)0x0) && (iVar3 < 0)) {
        return iVar3;
      }
    }
    ppuVar1 = ppuVar4 + 5;
    ppuVar4 = ppuVar4 + 3;
    puVar2 = *ppuVar1;
  } while( true );
}


// ============================================================================
// Function: Create_SetColorComObject at 10002470
// Signature: undefined __stdcall Create_SetColorComObject(void)
// Entry: 10002470
// Body: 10002470 - 100024db
// ============================================================================
void Create_SetColorComObject(void)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_1000515b;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x24);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    SetColorObject_Construct(puVar1);
    *puVar1 = &PTR_LAB_100067a0;
    puVar1[1] = &PTR_LAB_10006790;
    InterlockedIncrement(&DAT_100082ac);
    FUN_100024e5();
    return;
  }
  FUN_100024e5();
  return;
}


// ============================================================================
// Function: Catch@100024dc at 100024dc
// Signature: undefined * __stdcall Catch@100024dc(void)
// Entry: 100024dc
// Body: 100024dc - 100024e1
// ============================================================================
undefined * Catch_100024dc(void)

{
  return &DAT_100024e2;
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
// Function: Catch@100027be at 100027be
// Signature: undefined * __stdcall Catch@100027be(void)
// Entry: 100027be
// Body: 100027be - 100027c3
// ============================================================================
undefined * Catch_100027be(void)

{
  return &DAT_100027c4;
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
// Function: SetColorRefCountedState_Construct at 10002850
// Signature: undefined4 * __fastcall SetColorRefCountedState_Construct(undefined4 * param_1)
// Entry: 10002850
// Body: 10002850 - 10002866
// ============================================================================
undefined4 * __fastcall SetColorRefCountedState_Construct(undefined4 *param_1)

{
  *param_1 = 0;
  InitializeCriticalSection((LPCRITICAL_SECTION)(param_1 + 1));
  return param_1;
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
// Function: Com_RegisterProgIdAndClsid at 10002d90
// Signature: IID * __stdcall Com_RegisterProgIdAndClsid(int param_1, IID * param_2, BYTE * param_3, BYTE * param_4, HKEY param_5, uint param_6)
// Entry: 10002d90
// Body: 10002d90 - 10003171
// ============================================================================
IID * Com_RegisterProgIdAndClsid
                (int param_1,IID *param_2,BYTE *param_3,BYTE *param_4,HKEY param_5,uint param_6)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  IID *pIVar4;
  HMODULE pHVar5;
  char *pcVar6;
  BYTE *pBVar7;
  BYTE local_324 [260];
  BYTE local_220 [260];
  BYTE local_11c [256];
  HKEY local_1c;
  HKEY local_18;
  LPCSTR local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_100051c0;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  LoadStringA(*(HINSTANCE *)(param_1 + 4),(UINT)param_5,(LPSTR)local_11c,0x100);
  GetModuleFileNameA(*(HMODULE *)(param_1 + 4),(LPSTR)local_220,0x104);
  StringFromCLSID(param_2,(LPOLESTR *)&local_1c);
  param_5 = local_1c;
  if (local_1c == (HKEY)0x0) {
    pBVar7 = (BYTE *)0x0;
    local_14 = (LPCSTR)0x0;
  }
  else {
    pBVar7 = &stack0xfffffcd0;
    iVar3 = lstrlenW((LPCWSTR)local_1c);
    FUN_10004bc0();
    WideCharToMultiByte(0,0,(LPCWSTR)param_5,-1,&stack0xfffffcd0,iVar3 * 2 + 2,(LPCSTR)0x0,
                        (LPBOOL)0x0);
    local_14 = &stack0xfffffcd0;
  }
  param_2 = (IID *)0x0;
  local_8 = 0;
  uVar2 = FUN_100031c0(&param_2,(HKEY)0x80000000,(LPCSTR)param_3,(LPSTR)0x0,0,0x20019,
                       (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
  if (uVar2 == 0) {
    iVar3 = lstrlenA((LPCSTR)local_11c);
    RegSetValueExA((HKEY)param_2,(LPCSTR)0x0,0,1,local_11c,iVar3 + 1);
    FUN_10003230(&param_2,s_CLSID_10008120,pBVar7,(LPCSTR)0x0);
    local_8 = 0xffffffff;
    FUN_100031a0(&param_2);
  }
  else {
    pIVar4 = (IID *)(uVar2 & 0xffff | 0x80070000);
    local_8 = 0xffffffff;
    FUN_100031a0(&param_2);
    param_2 = pIVar4;
    if (pIVar4 != (IID *)0x0) goto LAB_10002f39;
  }
  param_2 = (IID *)0x0;
  local_8 = 1;
  uVar2 = FUN_100031c0(&param_2,(HKEY)0x80000000,(LPCSTR)param_4,(LPSTR)0x0,0,0x20019,
                       (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
  if (uVar2 == 0) {
    FUN_100032a0(&param_2,local_11c,(LPCSTR)0x0);
    FUN_10003230(&param_2,s_CLSID_10008120,pBVar7,(LPCSTR)0x0);
    local_8 = 0xffffffff;
    FUN_100031a0(&param_2);
    param_2 = (IID *)0x0;
  }
  else {
    local_8 = 0xffffffff;
    FUN_100031a0(&param_2);
    param_2 = (IID *)(uVar2 & 0xffff | 0x80070000);
  }
LAB_10002f39:
  uVar2 = 0;
  if (param_2 == (IID *)0x0) {
    param_5 = (HKEY)0x0;
    local_8 = 2;
    local_18 = (HKEY)0x0;
    uVar2 = RegOpenKeyExA((HKEY)0x80000000,s_CLSID_10008120,0,0x20019,&local_18);
    if (uVar2 == 0) {
      uVar2 = FUN_100031a0(&param_5);
      param_5 = local_18;
    }
    if (uVar2 == 0) {
      local_18 = (HKEY)0x0;
      uVar2 = RegCreateKeyExA(param_5,local_14,0,(LPSTR)0x0,0,0x20019,(LPSECURITY_ATTRIBUTES)0x0,
                              &local_18,(LPDWORD)&local_14);
      if (uVar2 == 0) {
        uVar2 = FUN_100031a0(&param_5);
        param_5 = local_18;
      }
      if (uVar2 == 0) {
        iVar3 = lstrlenA((LPCSTR)local_11c);
        RegSetValueExA(param_5,(LPCSTR)0x0,0,1,local_11c,iVar3 + 1);
        local_14 = (LPCSTR)0x0;
        local_8 = CONCAT31(local_8._1_3_,3);
        iVar3 = FUN_100031c0(&local_14,param_5,"ProgID",(LPSTR)0x0,0,0xf003f,
                             (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
        if (iVar3 == 0) {
          FUN_100032a0(&local_14,param_3,(LPCSTR)0x0);
        }
        local_8._0_1_ = 2;
        FUN_100031a0(&local_14);
        param_3 = (BYTE *)0x0;
        local_8 = CONCAT31(local_8._1_3_,4);
        iVar3 = FUN_100031c0(&param_3,param_5,"VersionIndependentProgID",(LPSTR)0x0,0,0xf003f,
                             (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
        if (iVar3 == 0) {
          FUN_100032a0(&param_3,param_4,(LPCSTR)0x0);
        }
        local_8 = CONCAT31(local_8._1_3_,2);
        FUN_100031a0(&param_3);
        if ((*(int *)(param_1 + 4) == 0) ||
           (pHVar5 = GetModuleHandleA((LPCSTR)0x0), uVar1 = param_6,
           *(HMODULE *)(param_1 + 4) == pHVar5)) {
          GetShortPathNameA((LPCSTR)local_220,(LPSTR)local_324,0x104);
          param_3 = (BYTE *)0x0;
          local_8 = CONCAT31(local_8._1_3_,5);
          iVar3 = FUN_100031c0(&param_3,param_5,"LocalServer32",(LPSTR)0x0,0,0xf003f,
                               (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
          if (iVar3 == 0) {
            FUN_100032a0(&param_3,local_324,(LPCSTR)0x0);
          }
          local_8 = CONCAT31(local_8._1_3_,2);
          FUN_100031a0(&param_3);
        }
        else {
          pcVar6 = "AUTPRX32.DLL";
          if ((param_6 & 4) == 0) {
            pcVar6 = (char *)local_220;
          }
          FUN_10003230(&param_5,"InprocServer32",(BYTE *)pcVar6,(LPCSTR)0x0);
          if ((uVar1 & 2) == 0) {
            pBVar7 = (BYTE *)(-(uint)((uVar1 & 1) != 0) & 0x10006850);
          }
          else {
            pBVar7 = "both";
          }
          if (pBVar7 != (BYTE *)0x0) {
            FUN_10003230(&param_5,"InprocServer32",pBVar7,"ThreadingModel");
          }
        }
      }
    }
    local_8 = 0xffffffff;
    FUN_100031a0(&param_5);
  }
  CoTaskMemFree(local_1c);
  if (uVar2 != 0) {
    param_2 = (IID *)(uVar2 & 0xffff | 0x80070000);
  }
  ExceptionList = local_10;
  return param_2;
}


// ============================================================================
// Function: RegistryKey_Close at 10003180
// Signature: undefined __fastcall RegistryKey_Close(undefined4 * param_1)
// Entry: 10003180
// Body: 10003180 - 10003197
// ============================================================================
void __fastcall RegistryKey_Close(undefined4 *param_1)

{
  if ((HKEY)*param_1 != (HKEY)0x0) {
    RegCloseKey((HKEY)*param_1);
    *param_1 = 0;
  }
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


// ============================================================================
// Function: FUN_100032d0 at 100032d0
// Signature: undefined4 __stdcall FUN_100032d0(undefined4 param_1, IID * param_2, HKEY param_3, LPCSTR param_4)
// Entry: 100032d0
// Body: 100032d0 - 10003604
// ============================================================================
undefined4 FUN_100032d0(undefined4 param_1,IID *param_2,HKEY param_3,LPCSTR param_4)

{
  HKEY pHVar1;
  int iVar2;
  LSTATUS LVar3;
  code *pcVar4;
  LPCSTR pCVar5;
  CHAR local_128 [256];
  HKEY local_28;
  _FILETIME local_24;
  HKEY local_1c;
  DWORD local_18;
  HKEY local_14;
  void *local_10;
  undefined1 *puStack_c;
  uint local_8;
  
  pHVar1 = param_3;
  puStack_c = &LAB_100051f0;
  local_10 = ExceptionList;
  local_8 = 0;
  local_28 = (HKEY)0x80000000;
  pcVar4 = RegEnumKeyExA_exref;
  ExceptionList = &local_10;
  if ((param_3 != (HKEY)0x0) &&
     (ExceptionList = &local_10, iVar2 = lstrcmpiA((LPCSTR)param_3,&DAT_1000831c),
     pcVar4 = RegEnumKeyExA_exref, iVar2 != 0)) {
    param_3 = (HKEY)0x0;
    local_8 = CONCAT31(local_8._1_3_,1);
    local_14 = (HKEY)0x0;
    LVar3 = RegOpenKeyExA((HKEY)0x80000000,(LPCSTR)pHVar1,0,0x2001f,&local_14);
    if (LVar3 == 0) {
      LVar3 = FUN_100031a0(&param_3);
      pcVar4 = RegEnumKeyExA_exref;
      param_3 = local_14;
      if (LVar3 == 0) {
        local_18 = 0x100;
        iVar2 = RegEnumKeyExA(local_14,0,local_128,&local_18,(LPDWORD)0x0,(LPSTR)0x0,(LPDWORD)0x0,
                              &local_24);
        while (iVar2 == 0) {
          iVar2 = FUN_10003660(local_128);
          if (iVar2 != 0) goto LAB_100034c0;
          local_18 = 0x100;
          iVar2 = RegEnumKeyExA(param_3,0,local_128,&local_18,(LPDWORD)0x0,(LPSTR)0x0,(LPDWORD)0x0,
                                &local_24);
        }
        FUN_100031a0(&param_3);
        RegDeleteKeyA((HKEY)0x80000000,(LPCSTR)pHVar1);
LAB_100034c0:
        local_8 = local_8 & 0xffffff00;
        FUN_100031a0(&param_3);
        goto LAB_1000335f;
      }
    }
    local_8 = local_8 & 0xffffff00;
    FUN_100031a0(&param_3);
    pcVar4 = RegEnumKeyExA_exref;
  }
LAB_1000335f:
  pCVar5 = param_4;
  if ((param_4 != (LPCSTR)0x0) && (iVar2 = lstrcmpiA(param_4,&DAT_1000831c), iVar2 != 0)) {
    param_3 = (HKEY)0x0;
    local_8 = CONCAT31(local_8._1_3_,2);
    local_14 = (HKEY)0x0;
    LVar3 = RegOpenKeyExA((HKEY)0x80000000,pCVar5,0,0x2001f,&local_14);
    if (LVar3 == 0) {
      LVar3 = FUN_100031a0(&param_3);
      param_3 = local_14;
      if (LVar3 == 0) {
        param_4 = (LPCSTR)0x100;
        iVar2 = (*pcVar4)(local_14,0,local_128,&param_4,0,0,0,&local_24);
        while (iVar2 == 0) {
          iVar2 = FUN_10003660(local_128);
          if (iVar2 != 0) goto LAB_10003420;
          param_4 = (LPCSTR)0x100;
          iVar2 = (*pcVar4)(param_3,0,local_128,&param_4,0,0,0,&local_24);
        }
        FUN_100031a0(&param_3);
        RegDeleteKeyA((HKEY)0x80000000,pCVar5);
      }
    }
LAB_10003420:
    local_8 = local_8 & 0xffffff00;
    FUN_100031a0(&param_3);
  }
  StringFromCLSID(param_2,(LPOLESTR *)&local_1c);
  param_3 = local_1c;
  if (local_1c == (HKEY)0x0) {
    pCVar5 = (LPCSTR)0x0;
  }
  else {
    pCVar5 = &stack0xfffffecc;
    iVar2 = lstrlenW((LPCWSTR)local_1c);
    FUN_10004bc0();
    WideCharToMultiByte(0,0,(LPCWSTR)param_3,-1,&stack0xfffffecc,iVar2 * 2 + 2,(LPCSTR)0x0,
                        (LPBOOL)0x0);
  }
  param_3 = (HKEY)0x0;
  LVar3 = RegOpenKeyExA((HKEY)0x80000000,s_CLSID_10008120,0,0x20019,&param_3);
  if (LVar3 == 0) {
    LVar3 = RegCloseKey((HKEY)0x80000000);
    pHVar1 = param_3;
    local_28 = param_3;
    if (LVar3 == 0) {
      param_3 = (HKEY)0x0;
      local_8 = CONCAT31(local_8._1_3_,3);
      iVar2 = FUN_10003610(&param_3,pHVar1,pCVar5,0x2001f);
      if (iVar2 == 0) {
        param_4 = (LPCSTR)0x100;
        iVar2 = RegEnumKeyExA(param_3,0,local_128,(LPDWORD)&param_4,(LPDWORD)0x0,(LPSTR)0x0,
                              (LPDWORD)0x0,&local_24);
        while (iVar2 == 0) {
          iVar2 = FUN_10003660(local_128);
          if (iVar2 != 0) goto LAB_100035ca;
          param_4 = (LPCSTR)0x100;
          iVar2 = RegEnumKeyExA(param_3,0,local_128,(LPDWORD)&param_4,(LPDWORD)0x0,(LPSTR)0x0,
                                (LPDWORD)0x0,&local_24);
        }
        FUN_100031a0(&param_3);
        RegDeleteKeyA(pHVar1,pCVar5);
      }
LAB_100035ca:
      FUN_100031a0(&param_3);
    }
  }
  CoTaskMemFree(local_1c);
  if (local_28 != (HKEY)0x0) {
    RegCloseKey(local_28);
  }
  ExceptionList = local_10;
  return 0;
}


// ============================================================================
// Function: FUN_10003610 at 10003610
// Signature: undefined __thiscall FUN_10003610(void * this, HKEY param_1, LPCSTR param_2, REGSAM param_3)
// Entry: 10003610
// Body: 10003610 - 10003659
// ============================================================================
void __thiscall FUN_10003610(void *this,HKEY param_1,LPCSTR param_2,REGSAM param_3)

{
  LSTATUS LVar1;
  HKEY local_4;
  
  local_4 = (HKEY)0x0;
  LVar1 = RegOpenKeyExA(param_1,param_2,0,param_3,&local_4);
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
// Function: FUN_10003660 at 10003660
// Signature: undefined FUN_10003660(void)
// Entry: 10003660
// Body: 10003660 - 100037b3
// ============================================================================
/* WARNING: Removing unreachable block (ram,0x100036c1) */
/* WARNING: Removing unreachable block (ram,0x100036d6) */
/* WARNING: Removing unreachable block (ram,0x100036e4) */

LSTATUS __thiscall FUN_10003660(undefined4 *param_1,LPCSTR param_2)

{
  HKEY hKey;
  LSTATUS LVar1;
  int iVar2;
  HKEY local_124;
  DWORD DStack_120;
  HKEY local_11c;
  undefined4 *local_118;
  _FILETIME _Stack_114;
  CHAR aCStack_10c [256];
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_1000520b;
  pvStack_c = ExceptionList;
  local_4 = 0;
  local_11c = (HKEY)0x0;
  ExceptionList = &pvStack_c;
  local_118 = param_1;
  LVar1 = RegOpenKeyExA((HKEY)*param_1,param_2,0,0x2001f,&local_11c);
  hKey = local_11c;
  if (LVar1 == 0) {
    local_124 = local_11c;
    DStack_120 = 0x100;
    iVar2 = RegEnumKeyExA(local_11c,0,aCStack_10c,&DStack_120,(LPDWORD)0x0,(LPSTR)0x0,(LPDWORD)0x0,
                          &_Stack_114);
    while (iVar2 == 0) {
      iVar2 = FUN_10003660(aCStack_10c);
      if (iVar2 != 0) {
        if (hKey == (HKEY)0x0) {
          ExceptionList = pvStack_c;
          return iVar2;
        }
        RegCloseKey(hKey);
        ExceptionList = pvStack_c;
        return iVar2;
      }
      DStack_120 = 0x100;
      iVar2 = RegEnumKeyExA(hKey,0,aCStack_10c,&DStack_120,(LPDWORD)0x0,(LPSTR)0x0,(LPDWORD)0x0,
                            &_Stack_114);
      param_1 = local_118;
    }
    if (hKey != (HKEY)0x0) {
      RegCloseKey(hKey);
      local_124 = (HKEY)0x0;
    }
    LVar1 = RegDeleteKeyA((HKEY)*param_1,param_2);
    if (local_124 != (HKEY)0x0) {
      RegCloseKey(local_124);
    }
  }
  ExceptionList = pvStack_c;
  return LVar1;
}


// ============================================================================
// Function: FUN_100037c0 at 100037c0
// Signature: int __stdcall FUN_100037c0(undefined4 param_1, int * param_2, int param_3)
// Entry: 100037c0
// Body: 100037c0 - 100038ef
// ============================================================================
int FUN_100037c0(undefined4 param_1,int *param_2,int param_3)

{
  int *piVar1;
  undefined4 *puVar2;
  HRESULT HVar3;
  int iVar4;
  int *local_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_10005228;
  local_c = ExceptionList;
  local_20 = (int *)0x0;
  local_4 = 0;
  if (param_2 != (int *)0x0) {
    ExceptionList = &local_c;
    HVar3 = CoCreateInstance((IID *)&DAT_10006948,(LPUNKNOWN)0x0,1,(IID *)&DAT_10006958,&local_20);
    if (-1 < HVar3) {
      iVar4 = *param_2;
      while (iVar4 != 0) {
        puVar2 = (undefined4 *)param_2[1];
        uStack_1c = *puVar2;
        uStack_18 = puVar2[1];
        uStack_14 = puVar2[2];
        uStack_10 = puVar2[3];
        if (param_3 == 0) {
          if (*param_2 == 1) {
            (**(code **)(*local_20 + 0x18))();
          }
          else {
            (**(code **)(*local_20 + 0x20))(local_20,param_1,1,&uStack_1c);
          }
        }
        else {
          if (*param_2 == 1) {
            iVar4 = (**(code **)(*local_20 + 0x14))();
          }
          else {
            iVar4 = (**(code **)(*local_20 + 0x1c))(local_20,param_1,1,&uStack_1c);
          }
          if (iVar4 < 0) {
            local_4 = 0xffffffff;
            if (local_20 == (int *)0x0) {
              ExceptionList = local_c;
              return iVar4;
            }
            (**(code **)(*local_20 + 8))(local_20);
            ExceptionList = local_c;
            return iVar4;
          }
        }
        piVar1 = param_2 + 2;
        param_2 = param_2 + 2;
        iVar4 = *piVar1;
      }
    }
    if (local_20 != (int *)0x0) {
      local_4 = 0xffffffff;
      (**(code **)(*local_20 + 8))(local_20);
    }
  }
  ExceptionList = local_c;
  return 0;
}


// ============================================================================
// Function: FUN_100038f0 at 100038f0
// Signature: undefined __fastcall FUN_100038f0(int * param_1)
// Entry: 100038f0
// Body: 100038f0 - 100038fc
// ============================================================================
void __fastcall FUN_100038f0(int *param_1)

{
  int *piVar1;
  
  piVar1 = (int *)*param_1;
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
  }
  return;
}


// ============================================================================
// Function: FUN_10003900 at 10003900
// Signature: HRESULT __stdcall FUN_10003900(int param_1, LPCWSTR param_2)
// Entry: 10003900
// Body: 10003900 - 10003af0
// ============================================================================
HRESULT FUN_10003900(int param_1,LPCWSTR param_2)

{
  int iVar1;
  LPSTR lpString2;
  HRESULT HVar2;
  LPCSTR lpsz;
  uint uVar3;
  uint uVar4;
  LPWSTR szFile;
  LPCSTR lpString1;
  LPCWSTR pWVar5;
  OLECHAR *pOVar6;
  OLECHAR local_220 [124];
  char local_128 [272];
  ITypeLib *local_18;
  LPCWSTR local_14;
  void *local_10;
  undefined1 *puStack_c;
  uint local_8;
  
  puStack_c = &LAB_10005250;
  local_10 = ExceptionList;
  local_8 = 1;
  local_14 = (LPCWSTR)0x0;
  local_18 = (ITypeLib *)0x0;
  ExceptionList = &local_10;
  GetModuleFileNameA(*(HMODULE *)(param_1 + 0xc),local_128,0x104);
  if (param_2 != (LPCWSTR)0x0) {
    iVar1 = lstrlenW(param_2);
    FUN_10004bc0();
    lpString2 = FUN_10003b30(&stack0xfffffdd4,param_2,iVar1 * 2 + 2,0);
    lstrcatA(local_128,lpString2);
  }
  if (&stack0x00000000 == (undefined1 *)0x128) {
    szFile = (LPCOLESTR)0x0;
  }
  else {
    iVar1 = lstrlenA(local_128);
    FUN_10004bc0();
    szFile = String_MultiByteToWideChar((LPWSTR)&stack0xfffffdd4,local_128,iVar1 + 1,0);
  }
  HVar2 = LoadTypeLib(szFile,&local_18);
  if (HVar2 < 0) {
    lpString1 = (LPCSTR)0x0;
    lpsz = local_128;
    if (local_128[0] == '\0') {
LAB_100039ff:
      lpString1 = lpsz;
    }
    else {
      do {
        if (local_128[0] == '.') {
          lpString1 = lpsz;
        }
        lpsz = CharNextA(lpsz);
        local_128[0] = *lpsz;
      } while (local_128[0] != '\0');
      if (lpString1 == (LPCSTR)0x0) goto LAB_100039ff;
    }
    lstrcpyA(lpString1,&DAT_10008128);
    if (&stack0x00000000 == (undefined1 *)0x128) {
      szFile = (LPCOLESTR)0x0;
    }
    else {
      iVar1 = lstrlenA(local_128);
      FUN_10004bc0();
      szFile = String_MultiByteToWideChar((LPWSTR)&stack0xfffffdd4,local_128,iVar1 + 1,0);
    }
    HVar2 = LoadTypeLib(szFile,&local_18);
    if (HVar2 < 0) goto LAB_10003a66;
  }
  local_14 = SysAllocString(szFile);
LAB_10003a66:
  if (-1 < HVar2) {
    iVar1 = lstrlenW(local_14);
    uVar4 = iVar1 * 2 + 2;
    pWVar5 = local_14;
    pOVar6 = local_220;
    for (uVar3 = uVar4 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
      *(undefined4 *)pOVar6 = *(undefined4 *)pWVar5;
      pWVar5 = pWVar5 + 2;
      pOVar6 = pOVar6 + 2;
    }
    for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
      *(char *)pOVar6 = (char)*pWVar5;
      pWVar5 = (LPCWSTR)((int)pWVar5 + 1);
      pOVar6 = (OLECHAR *)((int)pOVar6 + 1);
    }
    iVar1 = FUN_10003b70(local_220);
    pWVar5 = local_14;
    local_220[iVar1] = L'\0';
    HVar2 = RegisterTypeLib(local_18,pWVar5,local_220);
  }
  local_8 = local_8 & 0xffffff00;
  if (local_18 != (ITypeLib *)0x0) {
    (*local_18->lpVtbl->Release)(local_18);
  }
  SysFreeString(local_14);
  ExceptionList = local_10;
  return HVar2;
}


// ============================================================================
// Function: String_MultiByteToWideChar at 10003b00
// Signature: LPWSTR __stdcall String_MultiByteToWideChar(LPWSTR param_1, LPCSTR param_2, int param_3, UINT param_4)
// Entry: 10003b00
// Body: 10003b00 - 10003b29
// ============================================================================
LPWSTR String_MultiByteToWideChar(LPWSTR param_1,LPCSTR param_2,int param_3,UINT param_4)

{
  *param_1 = L'\0';
  MultiByteToWideChar(param_4,0,param_2,-1,param_1,param_3);
  return param_1;
}


// ============================================================================
// Function: FUN_10003b30 at 10003b30
// Signature: LPSTR __stdcall FUN_10003b30(LPSTR param_1, LPCWSTR param_2, int param_3, UINT param_4)
// Entry: 10003b30
// Body: 10003b30 - 10003b5b
// ============================================================================
LPSTR FUN_10003b30(LPSTR param_1,LPCWSTR param_2,int param_3,UINT param_4)

{
  *param_1 = '\0';
  WideCharToMultiByte(param_4,0,param_2,-1,param_1,param_3,(LPCSTR)0x0,(LPBOOL)0x0);
  return param_1;
}

// clang-format on
