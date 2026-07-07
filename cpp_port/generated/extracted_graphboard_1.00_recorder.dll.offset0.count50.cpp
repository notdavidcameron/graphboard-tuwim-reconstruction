/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Notes: raw Ghidra decompiler output with only file-level framing added.
*/
#include "../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001020 at 10001020
// Signature: CWnd * __thiscall FUN_10001020(void * this, undefined4 param_1)
// Entry: 10001020
// Body: 10001020 - 100010bf
// ============================================================================
CWnd * __thiscall FUN_10001020(void *this,undefined4 param_1)

{
  char *pcVar1;
  undefined4 *puVar2;
  char *pcVar3;
  ulong uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  HWND__ *pHVar9;
  HMENU__ *pHVar10;
  void *pvVar11;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_100058e8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  CWnd::CWnd(this);
  pvVar11 = (void *)0x0;
  pHVar10 = (HMENU__ *)0x0;
  pHVar9 = (HWND__ *)0x0;
  iVar8 = 0;
  iVar7 = 0;
  iVar6 = 0;
  iVar5 = 0;
  uVar4 = 0x80000000;
  pcVar3 = s_Local_Wnd_10008020;
  local_4 = 0;
  *(undefined ***)this = &PTR_LAB_100063b8;
  pcVar1 = AfxRegisterWndClass(0,(HICON__ *)0x0,(HBRUSH__ *)0x0,(HICON__ *)0x0);
  iVar5 = CWnd::CreateEx(this,0,pcVar1,pcVar3,uVar4,iVar5,iVar6,iVar7,iVar8,pHVar9,pHVar10,pvVar11);
  if (iVar5 != 0) {
    *(undefined4 *)((int)this + 0x44) = param_1;
    puVar2 = (undefined4 *)((int)this + 0x48);
    for (iVar5 = 0x14; iVar5 != 0; iVar5 = iVar5 + -1) {
      *puVar2 = 0;
      puVar2 = puVar2 + 1;
    }
    ExceptionList = local_c;
    return this;
  }
  ExceptionList = local_c;
  return this;
}


// ============================================================================
// Function: FUN_100010c0 at 100010c0
// Signature: CWnd * __thiscall FUN_100010c0(void * this, byte param_1)
// Entry: 100010c0
// Body: 100010c0 - 100010dd
// ============================================================================
CWnd * __thiscall FUN_100010c0(void *this,byte param_1)

{
  FUN_100010e0(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: FUN_100010e0 at 100010e0
// Signature: undefined __fastcall FUN_100010e0(CWnd * param_1)
// Entry: 100010e0
// Body: 100010e0 - 10001135
// ============================================================================
void __fastcall FUN_100010e0(CWnd *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_10005908;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined ***)param_1 = &PTR_LAB_100063b8;
  local_4 = 0;
  if (*(int *)(param_1 + 0x20) != 0) {
    CWnd::DestroyWindow(param_1);
  }
  local_4 = 0xffffffff;
  CWnd::~CWnd(param_1);
  ExceptionList = local_c;
  return;
}


// ============================================================================
// Function: FUN_10001140 at 10001140
// Signature: UINT_PTR __thiscall FUN_10001140(void * this, UINT param_1)
// Entry: 10001140
// Body: 10001140 - 1000117b
// ============================================================================
UINT_PTR __thiscall FUN_10001140(void *this,UINT param_1)

{
  int *piVar1;
  UINT_PTR UVar2;
  uint uVar3;
  
  uVar3 = 0;
  piVar1 = (int *)((int)this + 0x48);
  do {
    if (*piVar1 == 0) {
      UVar2 = SetTimer(*(HWND *)((int)this + 0x20),uVar3 + 1,param_1,(TIMERPROC)0x0);
      *(UINT_PTR *)((int)this + uVar3 * 4 + 0x48) = UVar2;
      return UVar2;
    }
    uVar3 = uVar3 + 1;
    piVar1 = piVar1 + 1;
  } while (uVar3 < 0x14);
  return 0;
}


// ============================================================================
// Function: FUN_10001180 at 10001180
// Signature: undefined __thiscall FUN_10001180(void * this, UINT_PTR param_1)
// Entry: 10001180
// Body: 10001180 - 100011bb
// ============================================================================
void __thiscall FUN_10001180(void *this,UINT_PTR param_1)

{
  int iVar1;
  UINT_PTR *pUVar2;
  
  if (param_1 != 0) {
    pUVar2 = (UINT_PTR *)((int)this + 0x48);
    iVar1 = 0x14;
    do {
      if (*pUVar2 == param_1) {
        KillTimer(*(HWND *)((int)this + 0x20),param_1);
        *pUVar2 = 0;
        param_1 = 0;
      }
      pUVar2 = pUVar2 + 1;
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
  }
  return;
}


// ============================================================================
// Function: FUN_10001360 at 10001360
// Signature: undefined __stdcall FUN_10001360(void)
// Entry: 10001360
// Body: 10001360 - 10001362
// ============================================================================
void FUN_10001360(void)

{
  return;
}


// ============================================================================
// Function: FUN_10001390 at 10001390
// Signature: undefined __fastcall FUN_10001390(undefined4 * param_1)
// Entry: 10001390
// Body: 10001390 - 10001399
// ============================================================================
void __fastcall FUN_10001390(undefined4 *param_1)

{
  SysFreeString((BSTR)*param_1);
  return;
}


// ============================================================================
// Function: FUN_100013c0 at 100013c0
// Signature: CPropertySheet * __thiscall FUN_100013c0(void * this, uint param_1, undefined4 param_2, uint param_3)
// Entry: 100013c0
// Body: 100013c0 - 10001437
// ============================================================================
CPropertySheet * __thiscall FUN_100013c0(void *this,uint param_1,undefined4 param_2,uint param_3)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10005936;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  CPropertySheet::CPropertySheet(this,param_1,(CWnd *)0x0,param_3);
  local_4 = 0;
  FUN_10004dd0((CPropertyPage *)((int)this + 0x8c));
  local_4 = CONCAT31(local_4._1_3_,1);
  *(undefined ***)this = &PTR_LAB_100064b8;
  *(undefined4 *)((int)this + 0x11c) = param_2;
  CPropertySheet::AddPage(this,(CPropertyPage *)((int)this + 0x8c));
  ExceptionList = local_c;
  return this;
}


// ============================================================================
// Function: FUN_10001440 at 10001440
// Signature: CPropertySheet * __thiscall FUN_10001440(void * this, byte param_1)
// Entry: 10001440
// Body: 10001440 - 1000145d
// ============================================================================
CPropertySheet * __thiscall FUN_10001440(void *this,byte param_1)

{
  FUN_10001460(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


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


// ============================================================================
// Function: FUN_10001540 at 10001540
// Signature: undefined __stdcall FUN_10001540(void)
// Entry: 10001540
// Body: 10001540 - 10001549
// ============================================================================
void FUN_10001540(void)

{
  FUN_10001580((CWinApp *)&DAT_100082d0);
  return;
}


// ============================================================================
// Function: FUN_10001550 at 10001550
// Signature: undefined __stdcall FUN_10001550(void)
// Entry: 10001550
// Body: 10001550 - 1000155b
// ============================================================================
void FUN_10001550(void)

{
  FUN_10005626((_onexit_t)&LAB_10001560);
  return;
}


// ============================================================================
// Function: ~CWinApp at 10001570
// Signature: void __thiscall ~CWinApp(CWinApp * this)
// Entry: 10001570
// Body: 10001570 - 10001574
// ============================================================================
void __thiscall CWinApp::~CWinApp(CWinApp *this)

{
                    /* WARNING: Could not recover jumptable at 0x10005130. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~CWinApp(this);
  return;
}


// ============================================================================
// Function: FUN_10001580 at 10001580
// Signature: CWinApp * __fastcall FUN_10001580(CWinApp * param_1)
// Entry: 10001580
// Body: 10001580 - 10001593
// ============================================================================
CWinApp * __fastcall FUN_10001580(CWinApp *param_1)

{
  CWinApp::CWinApp(param_1,(char *)0x0);
  *(undefined ***)param_1 = &PTR_LAB_10006758;
  return param_1;
}


// ============================================================================
// Function: FUN_100015a0 at 100015a0
// Signature: CWinApp * __thiscall FUN_100015a0(void * this, byte param_1)
// Entry: 100015a0
// Body: 100015a0 - 100015bd
// ============================================================================
CWinApp * __thiscall FUN_100015a0(void *this,byte param_1)

{
  CWinApp::~CWinApp(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: DllCanUnloadNow at 10001780
// Signature: HRESULT __stdcall DllCanUnloadNow(void)
// Entry: 10001780
// Body: 10001780 - 100017c8
// ============================================================================
HRESULT DllCanUnloadNow(void)

{
  AFX_MODULE_STATE *pAVar1;
  long lVar2;
  undefined4 local_8;
  int local_4;
  
                    /* 0x1780  1  DllCanUnloadNow */
  pAVar1 = (AFX_MODULE_STATE *)FUN_1000536f();
  AFX_MAINTAIN_STATE2::AFX_MAINTAIN_STATE2((AFX_MAINTAIN_STATE2 *)&local_8,pAVar1);
  lVar2 = AfxDllCanUnloadNow();
  if ((lVar2 == 0) && (DAT_100083ac == 0)) {
    *(undefined4 *)(local_4 + 4) = local_8;
    return 0;
  }
  *(undefined4 *)(local_4 + 4) = local_8;
  return 1;
}


// ============================================================================
// Function: DllGetClassObject at 100017d0
// Signature: HRESULT __stdcall DllGetClassObject(IID * rclsid, IID * riid, LPVOID * ppv)
// Entry: 100017d0
// Body: 100017d0 - 100018b5
// ============================================================================
/* WARNING: Removing unreachable block (ram,0x100017dc) */

HRESULT DllGetClassObject(IID *rclsid,IID *riid,LPVOID *ppv)

{
  int *piVar1;
  ulong *puVar2;
  ulong uVar3;
  int iVar4;
  undefined4 *puVar5;
  
                    /* 0x17d0  2  DllGetClassObject */
  puVar5 = DAT_100083a8;
  iVar4 = 0;
  if (ppv == (LPVOID *)0x0) {
    return -0x7fffbffd;
  }
  *ppv = (LPVOID)0x0;
  puVar2 = (ulong *)*puVar5;
  do {
    if (puVar2 == (ulong *)0x0) {
LAB_1000189e:
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
        EnterCriticalSection((LPCRITICAL_SECTION)&DAT_100083e4);
        if (*piVar1 == 0) {
          iVar4 = (*(code *)puVar5[2])(puVar5[3],&DAT_10006e78,piVar1);
        }
        LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_100083e4);
      }
      puVar5 = (undefined4 *)*piVar1;
      if (puVar5 != (undefined4 *)0x0) {
        iVar4 = (**(code **)*puVar5)(puVar5,riid,ppv);
      }
      goto LAB_1000189e;
    }
    if (DAT_10008398 == 100) {
      puVar5 = puVar5 + 7;
    }
    else {
      puVar5 = puVar5 + 9;
    }
    puVar2 = (ulong *)*puVar5;
  } while( true );
}


// ============================================================================
// Function: DllRegisterServer at 100018c0
// Signature: HRESULT __stdcall DllRegisterServer(void)
// Entry: 100018c0
// Body: 100018c0 - 10001942
// ============================================================================
/* WARNING: Removing unreachable block (ram,0x100018c9) */

HRESULT DllRegisterServer(void)

{
  int iVar1;
  int *piVar2;
  HRESULT HVar3;
  int *piVar4;
  int iVar5;
  
                    /* 0x18c0  3  DllRegisterServer */
  iVar5 = 0;
  piVar4 = DAT_100083a8;
  if (*DAT_100083a8 != 0) {
    do {
      if (((code *)piVar4[6] == (code *)0x0) || (iVar1 = (*(code *)piVar4[6])(), iVar1 == 0)) {
        iVar5 = (*(code *)piVar4[1])(1);
        if (iVar5 < 0) {
          return iVar5;
        }
        if (DAT_10008398 == 0x84) {
          iVar5 = 1;
          piVar2 = (int *)(*(code *)piVar4[7])();
          iVar5 = FUN_10003370(*piVar4,piVar2,iVar5);
          if (iVar5 < 0) {
            return iVar5;
          }
        }
      }
      if (DAT_10008398 == 100) {
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
  HVar3 = FUN_100034b0(0x10008398,(LPCWSTR)0x0);
  return HVar3;
}


// ============================================================================
// Function: DllUnregisterServer at 10001950
// Signature: undefined4 __stdcall DllUnregisterServer(void)
// Entry: 10001950
// Body: 10001950 - 100019b0
// ============================================================================
undefined4 DllUnregisterServer(void)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  
                    /* 0x1950  4  DllUnregisterServer */
  iVar1 = *DAT_100083a8;
  piVar3 = DAT_100083a8;
  do {
    if (iVar1 == 0) {
      return 0;
    }
    if ((((code *)piVar3[6] == (code *)0x0) || (iVar1 = (*(code *)piVar3[6])(), iVar1 == 0)) &&
       ((*(code *)piVar3[1])(0), DAT_10008398 == 0x84)) {
      if ((code *)piVar3[7] != (code *)0x0) {
        iVar1 = 0;
        piVar2 = (int *)(*(code *)piVar3[7])();
        FUN_10003370(*piVar3,piVar2,iVar1);
        goto LAB_10001997;
      }
LAB_100019a5:
      piVar3 = piVar3 + 9;
    }
    else {
LAB_10001997:
      if (DAT_10008398 != 100) goto LAB_100019a5;
      piVar3 = piVar3 + 7;
    }
    iVar1 = *piVar3;
  } while( true );
}


// ============================================================================
// Function: Create_RecorderClassFactory at 100019d0
// Signature: undefined __stdcall Create_RecorderClassFactory(void)
// Entry: 100019d0
// Body: 100019d0 - 10001a3a
// ============================================================================
void Create_RecorderClassFactory(void)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_1000596b;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x3c);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    RecorderRefCountedState_Construct(puVar1 + 1);
    *puVar1 = &PTR_LAB_10006810;
    InitializeCriticalSection((LPCRITICAL_SECTION)(puVar1 + 9));
    *puVar1 = &PTR_Recorder_QueryInterfaceFromMap_100067fc;
    FUN_10001a44();
    return;
  }
  FUN_10001a44();
  return;
}


// ============================================================================
// Function: Catch@10001a3b at 10001a3b
// Signature: undefined * __stdcall Catch@10001a3b(void)
// Entry: 10001a3b
// Body: 10001a3b - 10001a40
// ============================================================================
undefined * Catch_10001a3b(void)

{
  return &DAT_10001a41;
}


// ============================================================================
// Function: FUN_10001a44 at 10001a44
// Signature: int __stdcall FUN_10001a44(void)
// Entry: 10001a44
// Body: 10001a44 - 10001aba
// ============================================================================
int FUN_10001a44(void)

{
  int iVar1;
  int unaff_EBP;
  undefined4 *unaff_ESI;
  
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  if (unaff_ESI != (undefined4 *)0x0) {
    unaff_ESI[8] = *(undefined4 *)(unaff_EBP + 8);
    iVar1 = (**(code **)*unaff_ESI)();
    if (iVar1 != 0) {
      *unaff_ESI = &PTR_Recorder_QueryInterfaceFromMap_100067fc;
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
// Function: RecorderClassFactory_AddRef at 10001af0
// Signature: LONG __stdcall RecorderClassFactory_AddRef(int param_1)
// Entry: 10001af0
// Body: 10001af0 - 10001b2d
// ============================================================================
LONG RecorderClassFactory_AddRef(int param_1)

{
  LONG LVar1;
  
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x24));
  LVar1 = InterlockedIncrement((LONG *)(param_1 + 4));
  if (*(LONG *)(param_1 + 4) == 2) {
    InterlockedIncrement(&DAT_100083ac);
  }
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x24));
  return LVar1;
}


// ============================================================================
// Function: RecorderClassFactory_Release at 10001b30
// Signature: int __stdcall RecorderClassFactory_Release(undefined4 * param_1)
// Entry: 10001b30
// Body: 10001b30 - 10001b8c
// ============================================================================
int RecorderClassFactory_Release(undefined4 *param_1)

{
  int iVar1;
  
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  InterlockedDecrement(param_1 + 1);
  iVar1 = param_1[1];
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  if (iVar1 == 0) {
    if (param_1 != (undefined4 *)0x0) {
      RecorderClassFactory_Destroy(param_1);
      operator_delete(param_1);
      return 0;
    }
  }
  else if (iVar1 == 1) {
    InterlockedDecrement(&DAT_100083ac);
  }
  return iVar1;
}


// ============================================================================
// Function: RecorderClassFactory_Destroy at 10001b90
// Signature: undefined __fastcall RecorderClassFactory_Destroy(undefined4 * param_1)
// Entry: 10001b90
// Body: 10001b90 - 10001bbf
// ============================================================================
void __fastcall RecorderClassFactory_Destroy(undefined4 *param_1)

{
  *param_1 = &PTR_Recorder_QueryInterfaceFromMap_100067fc;
  param_1[1] = 1;
  DeleteCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  DeleteCriticalSection
            ((LPCRITICAL_SECTION)((-(uint)(param_1 != (undefined4 *)0x0) & (uint)(param_1 + 1)) + 4)
            );
  return;
}


// ============================================================================
// Function: Recorder_QueryInterfaceFromMap at 10001bc0
// Signature: int __stdcall Recorder_QueryInterfaceFromMap(int * param_1, int * param_2, undefined4 * param_3)
// Entry: 10001bc0
// Body: 10001bc0 - 10001cb2
// ============================================================================
int Recorder_QueryInterfaceFromMap(int *param_1,int *param_2,undefined4 *param_3)

{
  undefined **ppuVar1;
  undefined *puVar2;
  int iVar3;
  undefined **ppuVar4;
  int *piVar5;
  
  ppuVar4 = &PTR_DAT_10006828;
  if (param_3 == (undefined4 *)0x0) {
    return -0x7fffbffd;
  }
  *param_3 = 0;
  puVar2 = DAT_10006830;
  if ((((*param_2 == 0) && (param_2[1] == 0)) && (param_2[2] == 0xc0)) && (param_2[3] == 0x46000000)
     ) {
    iVar3 = DAT_1000682c + (int)param_1;
    (**(code **)(*(int *)(DAT_1000682c + (int)param_1) + 4))(iVar3);
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
// Function: Create_RecorderComObject at 10001cc0
// Signature: undefined __stdcall Create_RecorderComObject(void)
// Entry: 10001cc0
// Body: 10001cc0 - 10001d3c
// ============================================================================
void Create_RecorderComObject(void)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_1000598b;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x88);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    RecorderContext_Construct(puVar1);
    *puVar1 = &PTR_LAB_100068b8;
    puVar1[1] = &PTR_LAB_10006880;
    puVar1[2] = &PTR_LAB_10006850;
    puVar1[3] = &PTR_LAB_10006840;
    InterlockedIncrement(&DAT_100083ac);
    FUN_10001d46();
    return;
  }
  FUN_10001d46();
  return;
}


// ============================================================================
// Function: Catch@10001d3d at 10001d3d
// Signature: undefined * __stdcall Catch@10001d3d(void)
// Entry: 10001d3d
// Body: 10001d3d - 10001d42
// ============================================================================
undefined * Catch_10001d3d(void)

{
  return &DAT_10001d43;
}


// ============================================================================
// Function: FUN_10001d46 at 10001d46
// Signature: int __stdcall FUN_10001d46(void)
// Entry: 10001d46
// Body: 10001d46 - 10001dcb
// ============================================================================
int FUN_10001d46(void)

{
  int iVar1;
  int unaff_EBP;
  char *unaff_ESI;
  
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  if (unaff_ESI != (char *)0x0) {
    iVar1 = (*(code *)**(undefined4 **)unaff_ESI)();
    if (iVar1 != 0) {
      *(undefined ***)unaff_ESI = &PTR_LAB_100068b8;
      *(undefined ***)(unaff_ESI + 4) = &PTR_LAB_10006880;
      *(undefined ***)(unaff_ESI + 8) = &PTR_LAB_10006850;
      *(undefined ***)(unaff_ESI + 0xc) = &PTR_LAB_10006840;
      unaff_ESI[0x10] = '\x01';
      unaff_ESI[0x11] = '\0';
      unaff_ESI[0x12] = '\0';
      unaff_ESI[0x13] = '\0';
      InterlockedDecrement(&DAT_100083ac);
      RecorderContext_Destroy(unaff_ESI);
      operator_delete(unaff_ESI);
    }
    ExceptionList = *(void **)(unaff_EBP + -0xc);
    return iVar1;
  }
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  return *(int *)(unaff_EBP + -0x18);
}


// ============================================================================
// Function: FUN_10001de0 at 10001de0
// Signature: HRESULT __stdcall FUN_10001de0(undefined4 param_1, undefined4 param_2, BSTR param_3, undefined4 * param_4)
// Entry: 10001de0
// Body: 10001de0 - 10001e26
// ============================================================================
HRESULT FUN_10001de0(undefined4 param_1,undefined4 param_2,BSTR param_3,undefined4 *param_4)

{
  HRESULT HVar1;
  
  HVar1 = -0x7fffbffd;
  if (param_4 != (undefined4 *)0x0) {
    HVar1 = 0;
    if (DAT_100080dc == (int *)0x0) {
      HVar1 = FUN_100037e0(&PTR_DAT_100080d0,param_3);
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
// Function: FUN_10001f50 at 10001f50
// Signature: HRESULT __stdcall FUN_10001f50(undefined4 param_1, undefined4 param_2, BSTR param_3, undefined4 * param_4)
// Entry: 10001f50
// Body: 10001f50 - 10001f96
// ============================================================================
HRESULT FUN_10001f50(undefined4 param_1,undefined4 param_2,BSTR param_3,undefined4 *param_4)

{
  HRESULT HVar1;
  
  HVar1 = -0x7fffbffd;
  if (param_4 != (undefined4 *)0x0) {
    HVar1 = 0;
    if (DAT_100080fc == (int *)0x0) {
      HVar1 = FUN_100037e0(&PTR_DAT_100080f0,param_3);
    }
    *param_4 = DAT_100080fc;
    if (DAT_100080fc != (int *)0x0) {
      (**(code **)(*DAT_100080fc + 4))(DAT_100080fc);
      HVar1 = 0;
    }
  }
  return HVar1;
}


// ============================================================================
// Function: FUN_100020c0 at 100020c0
// Signature: HRESULT __stdcall FUN_100020c0(undefined4 param_1, undefined4 param_2, BSTR param_3, undefined4 * param_4)
// Entry: 100020c0
// Body: 100020c0 - 10002106
// ============================================================================
HRESULT FUN_100020c0(undefined4 param_1,undefined4 param_2,BSTR param_3,undefined4 *param_4)

{
  HRESULT HVar1;
  
  HVar1 = -0x7fffbffd;
  if (param_4 != (undefined4 *)0x0) {
    HVar1 = 0;
    if (DAT_1000811c == (int *)0x0) {
      HVar1 = FUN_100037e0(&PTR_DAT_10008110,param_3);
    }
    *param_4 = DAT_1000811c;
    if (DAT_1000811c != (int *)0x0) {
      (**(code **)(*DAT_1000811c + 4))(DAT_1000811c);
      HVar1 = 0;
    }
  }
  return HVar1;
}


// ============================================================================
// Function: FUN_10002230 at 10002230
// Signature: undefined __stdcall FUN_10002230(undefined4 param_1)
// Entry: 10002230
// Body: 10002230 - 100022c0
// ============================================================================
void FUN_10002230(undefined4 param_1)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_100059ab;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x90);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    puVar1[1] = 0;
    RecorderContext_Construct(puVar1 + 2);
    puVar1[2] = &PTR_LAB_100069b4;
    puVar1[3] = &PTR_LAB_1000697c;
    puVar1[4] = &PTR_LAB_1000694c;
    puVar1[5] = &PTR_LAB_1000693c;
    puVar1[6] = param_1;
    *puVar1 = &PTR_LAB_10006930;
    InterlockedIncrement(&DAT_100083ac);
    FUN_100022cc();
    return;
  }
  FUN_100022cc();
  return;
}


// ============================================================================
// Function: Catch@100022c1 at 100022c1
// Signature: undefined * __stdcall Catch@100022c1(void)
// Entry: 100022c1
// Body: 100022c1 - 100022c6
// ============================================================================
undefined * Catch_100022c1(void)

{
  return &DAT_100022c7;
}


// ============================================================================
// Function: FUN_100022cc at 100022cc
// Signature: undefined4 * __stdcall FUN_100022cc(void)
// Entry: 100022cc
// Body: 100022cc - 1000233d
// ============================================================================
undefined4 * FUN_100022cc(void)

{
  undefined4 *puVar1;
  undefined4 *unaff_EBX;
  int unaff_EBP;
  undefined4 *unaff_ESI;
  
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  if (unaff_ESI != unaff_EBX) {
    puVar1 = (undefined4 *)(**(code **)*unaff_ESI)();
    if (puVar1 != unaff_EBX) {
      *unaff_ESI = &PTR_LAB_10006930;
      unaff_ESI[1] = 1;
      InterlockedDecrement(&DAT_100083ac);
      RecorderContext_Destroy((char *)(unaff_ESI + 2));
      operator_delete(unaff_ESI);
    }
    ExceptionList = *(void **)(unaff_EBP + -0xc);
    return puVar1;
  }
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  return *(undefined4 **)(unaff_EBP + -0x18);
}


// ============================================================================
// Function: RecorderRefCountedState_Construct at 10002340
// Signature: undefined4 * __fastcall RecorderRefCountedState_Construct(undefined4 * param_1)
// Entry: 10002340
// Body: 10002340 - 10002356
// ============================================================================
undefined4 * __fastcall RecorderRefCountedState_Construct(undefined4 *param_1)

{
  *param_1 = 0;
  InitializeCriticalSection((LPCRITICAL_SECTION)(param_1 + 1));
  return param_1;
}


// ============================================================================
// Function: FUN_100023a0 at 100023a0
// Signature: undefined __fastcall FUN_100023a0(char * param_1)
// Entry: 100023a0
// Body: 100023a0 - 100023d8
// ============================================================================
void __fastcall FUN_100023a0(char *param_1)

{
  *(undefined ***)param_1 = &PTR_LAB_100068b8;
  *(undefined ***)(param_1 + 4) = &PTR_LAB_10006880;
  *(undefined ***)(param_1 + 8) = &PTR_LAB_10006850;
  *(undefined ***)(param_1 + 0xc) = &PTR_LAB_10006840;
  param_1[0x10] = '\x01';
  param_1[0x11] = '\0';
  param_1[0x12] = '\0';
  param_1[0x13] = '\0';
  InterlockedDecrement(&DAT_100083ac);
  RecorderContext_Destroy(param_1);
  return;
}


// ============================================================================
// Function: FUN_100024f0 at 100024f0
// Signature: int __stdcall FUN_100024f0(undefined4 * param_1)
// Entry: 100024f0
// Body: 100024f0 - 1000251d
// ============================================================================
int FUN_100024f0(undefined4 *param_1)

{
  int iVar1;
  
  iVar1 = param_1[1] + -1;
  param_1[1] = iVar1;
  if ((iVar1 == 0) && (param_1 != (undefined4 *)0x0)) {
    FUN_10002520(param_1);
    operator_delete(param_1);
  }
  return iVar1;
}


// ============================================================================
// Function: FUN_10002520 at 10002520
// Signature: undefined __fastcall FUN_10002520(undefined4 * param_1)
// Entry: 10002520
// Body: 10002520 - 10002544
// ============================================================================
void __fastcall FUN_10002520(undefined4 *param_1)

{
  *param_1 = &PTR_LAB_10006930;
  param_1[1] = 1;
  InterlockedDecrement(&DAT_100083ac);
  RecorderContext_Destroy((char *)(param_1 + 2));
  return;
}


// ============================================================================
// Function: FUN_10002910 at 10002910
// Signature: IID * __stdcall FUN_10002910(int param_1, IID * param_2, BYTE * param_3, BYTE * param_4, HKEY param_5, uint param_6)
// Entry: 10002910
// Body: 10002910 - 10002d2a
// ============================================================================
IID * FUN_10002910(int param_1,IID *param_2,BYTE *param_3,BYTE *param_4,HKEY param_5,uint param_6)

{
  uint uVar1;
  LPCWSTR lpWideCharStr;
  uint uVar2;
  int iVar3;
  IID *pIVar4;
  HMODULE pHVar5;
  char *pcVar6;
  DWORD DVar7;
  BYTE *pBVar8;
  BYTE local_324 [260];
  BYTE local_220 [260];
  BYTE local_11c [256];
  LPCWSTR local_1c;
  HKEY local_18;
  BYTE *local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_100059f0;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  LoadStringA(*(HINSTANCE *)(param_1 + 4),(UINT)param_5,(LPSTR)local_11c,0x100);
  GetModuleFileNameA(*(HMODULE *)(param_1 + 4),(LPSTR)local_220,0x104);
  StringFromCLSID(param_2,&local_1c);
  lpWideCharStr = local_1c;
  if (local_1c == (LPCWSTR)0x0) {
    local_14 = (BYTE *)0x0;
  }
  else {
    iVar3 = lstrlenW(local_1c);
    FUN_10005640();
    WideCharToMultiByte(0,0,lpWideCharStr,-1,&stack0xfffffcd0,iVar3 * 2 + 2,(LPCSTR)0x0,(LPBOOL)0x0)
    ;
    local_14 = &stack0xfffffcd0;
  }
  pBVar8 = local_14;
  param_2 = (IID *)0x0;
  local_8 = 0;
  uVar2 = FUN_10002d70(&param_2,(HKEY)0x80000000,(LPCSTR)param_3,(LPSTR)0x0,0,2,
                       (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
  if (uVar2 == 0) {
    iVar3 = lstrlenA((LPCSTR)local_11c);
    RegSetValueExA((HKEY)param_2,(LPCSTR)0x0,0,1,local_11c,iVar3 + 1);
    FUN_10002de0(&param_2,s_CLSID_10008180,pBVar8,(LPCSTR)0x0);
    local_8 = 0xffffffff;
    FUN_10002d50(&param_2);
  }
  else {
    pIVar4 = (IID *)(uVar2 & 0xffff | 0x80070000);
    local_8 = 0xffffffff;
    FUN_10002d50(&param_2);
    param_2 = pIVar4;
    if (pIVar4 != (IID *)0x0) goto LAB_10002ab8;
  }
  param_2 = (IID *)0x0;
  local_8 = 1;
  uVar2 = FUN_10002d70(&param_2,(HKEY)0x80000000,(LPCSTR)param_4,(LPSTR)0x0,0,2,
                       (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
  if (uVar2 == 0) {
    FUN_10002e50(&param_2,local_11c,(LPCSTR)0x0);
    FUN_10002de0(&param_2,s_CLSID_10008180,pBVar8,(LPCSTR)0x0);
    local_8 = 0xffffffff;
    FUN_10002d50(&param_2);
    param_2 = (IID *)0x0;
  }
  else {
    local_8 = 0xffffffff;
    FUN_10002d50(&param_2);
    param_2 = (IID *)(uVar2 & 0xffff | 0x80070000);
  }
LAB_10002ab8:
  uVar2 = 0;
  if (param_2 == (IID *)0x0) {
    param_5 = (HKEY)0x0;
    local_8 = 2;
    local_18 = (HKEY)0x0;
    uVar2 = RegOpenKeyExA((HKEY)0x80000000,s_CLSID_10008180,0,0x2001f,&local_18);
    if (uVar2 == 0) {
      uVar2 = 0;
      if (param_5 != (HKEY)0x0) {
        uVar2 = RegCloseKey(param_5);
      }
      param_5 = local_18;
    }
    if (uVar2 == 0) {
      local_18 = (HKEY)0x0;
      uVar2 = RegCreateKeyExA(param_5,(LPCSTR)local_14,0,(LPSTR)0x0,0,0x2001f,
                              (LPSECURITY_ATTRIBUTES)0x0,&local_18,(LPDWORD)&local_14);
      if (uVar2 == 0) {
        uVar2 = FUN_10002d50(&param_5);
        param_5 = local_18;
      }
      if (uVar2 == 0) {
        iVar3 = lstrlenA((LPCSTR)local_11c);
        RegSetValueExA(param_5,(LPCSTR)0x0,0,1,local_11c,iVar3 + 1);
        local_14 = (BYTE *)0x0;
        local_8 = CONCAT31(local_8._1_3_,3);
        iVar3 = FUN_10002d70(&local_14,param_5,"ProgID",(LPSTR)0x0,0,0xf003f,
                             (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
        if (iVar3 == 0) {
          FUN_10002e50(&local_14,param_3,(LPCSTR)0x0);
        }
        local_8._0_1_ = 2;
        FUN_10002d50(&local_14);
        param_3 = (BYTE *)0x0;
        local_8 = CONCAT31(local_8._1_3_,4);
        iVar3 = FUN_10002d70(&param_3,param_5,"VersionIndependentProgID",(LPSTR)0x0,0,0xf003f,
                             (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
        if (iVar3 == 0) {
          FUN_10002e50(&param_3,param_4,(LPCSTR)0x0);
        }
        local_8 = CONCAT31(local_8._1_3_,2);
        FUN_10002d50(&param_3);
        if ((*(int *)(param_1 + 4) == 0) ||
           (pHVar5 = GetModuleHandleA((LPCSTR)0x0), uVar1 = param_6,
           *(HMODULE *)(param_1 + 4) == pHVar5)) {
          DVar7 = GetShortPathNameA((LPCSTR)local_220,(LPSTR)local_324,0x104);
          if (DVar7 == 0x104) {
            local_8 = 0xffffffff;
            FUN_10002d50(&param_5);
            ExceptionList = local_10;
            return (IID *)0x8007000e;
          }
          if ((DVar7 == 0) || (pBVar8 = local_324, DVar7 == 0x57)) {
            pBVar8 = local_220;
          }
          param_3 = (BYTE *)0x0;
          local_8 = CONCAT31(local_8._1_3_,5);
          iVar3 = FUN_10002d70(&param_3,param_5,"LocalServer32",(LPSTR)0x0,0,0xf003f,
                               (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
          if (iVar3 == 0) {
            FUN_10002e50(&param_3,pBVar8,(LPCSTR)0x0);
          }
          local_8 = CONCAT31(local_8._1_3_,2);
          FUN_10002d50(&param_3);
        }
        else {
          pcVar6 = "AUTPRX32.DLL";
          if ((param_6 & 4) == 0) {
            pcVar6 = (char *)local_220;
          }
          FUN_10002de0(&param_5,"InprocServer32",(BYTE *)pcVar6,(LPCSTR)0x0);
          if ((uVar1 & 2) == 0) {
            pBVar8 = (BYTE *)(-(uint)((uVar1 & 1) != 0) & 0x10006a98);
          }
          else {
            pBVar8 = "both";
          }
          if (pBVar8 != (BYTE *)0x0) {
            FUN_10002de0(&param_5,"InprocServer32",pBVar8,"ThreadingModel");
          }
        }
      }
    }
    if (param_5 != (HKEY)0x0) {
      RegCloseKey(param_5);
    }
  }
  CoTaskMemFree(local_1c);
  if (uVar2 != 0) {
    param_2 = (IID *)(uVar2 & 0xffff | 0x80070000);
  }
  ExceptionList = local_10;
  return param_2;
}


// ============================================================================
// Function: FUN_10002d30 at 10002d30
// Signature: undefined __fastcall FUN_10002d30(undefined4 * param_1)
// Entry: 10002d30
// Body: 10002d30 - 10002d47
// ============================================================================
void __fastcall FUN_10002d30(undefined4 *param_1)

{
  if ((HKEY)*param_1 != (HKEY)0x0) {
    RegCloseKey((HKEY)*param_1);
    *param_1 = 0;
  }
  return;
}


// ============================================================================
// Function: FUN_10002d50 at 10002d50
// Signature: LSTATUS __fastcall FUN_10002d50(undefined4 * param_1)
// Entry: 10002d50
// Body: 10002d50 - 10002d69
// ============================================================================
LSTATUS __fastcall FUN_10002d50(undefined4 *param_1)

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
// Function: FUN_10002d70 at 10002d70
// Signature: undefined __thiscall FUN_10002d70(void * this, HKEY param_1, LPCSTR param_2, LPSTR param_3, DWORD param_4, REGSAM param_5, LPSECURITY_ATTRIBUTES param_6, undefined4 * param_7)
// Entry: 10002d70
// Body: 10002d70 - 10002ddb
// ============================================================================
void __thiscall
FUN_10002d70(void *this,HKEY param_1,LPCSTR param_2,LPSTR param_3,DWORD param_4,REGSAM param_5,
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
// Function: FUN_10002de0 at 10002de0
// Signature: LSTATUS __thiscall FUN_10002de0(void * this, LPCSTR param_1, BYTE * param_2, LPCSTR param_3)
// Entry: 10002de0
// Body: 10002de0 - 10002e4a
// ============================================================================
LSTATUS __thiscall FUN_10002de0(void *this,LPCSTR param_1,BYTE *param_2,LPCSTR param_3)

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
// Function: FUN_10002e50 at 10002e50
// Signature: undefined __thiscall FUN_10002e50(void * this, BYTE * param_1, LPCSTR param_2)
// Entry: 10002e50
// Body: 10002e50 - 10002e78
// ============================================================================
void __thiscall FUN_10002e50(void *this,BYTE *param_1,LPCSTR param_2)

{
  int iVar1;
  
  iVar1 = lstrlenA((LPCSTR)param_1);
  RegSetValueExA(*(HKEY *)this,param_2,0,1,param_1,iVar1 + 1);
  return;
}


// ============================================================================
// Function: FUN_10002e80 at 10002e80
// Signature: undefined4 __stdcall FUN_10002e80(undefined4 param_1, IID * param_2, HKEY param_3, LPCSTR param_4)
// Entry: 10002e80
// Body: 10002e80 - 100031b4
// ============================================================================
undefined4 FUN_10002e80(undefined4 param_1,IID *param_2,HKEY param_3,LPCSTR param_4)

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
  puStack_c = &LAB_10005a20;
  local_10 = ExceptionList;
  local_8 = 0;
  local_28 = (HKEY)0x80000000;
  pcVar4 = RegEnumKeyExA_exref;
  ExceptionList = &local_10;
  if ((param_3 != (HKEY)0x0) &&
     (ExceptionList = &local_10, iVar2 = lstrcmpiA((LPCSTR)param_3,&DAT_1000841c),
     pcVar4 = RegEnumKeyExA_exref, iVar2 != 0)) {
    param_3 = (HKEY)0x0;
    local_8 = CONCAT31(local_8._1_3_,1);
    local_14 = (HKEY)0x0;
    LVar3 = RegOpenKeyExA((HKEY)0x80000000,(LPCSTR)pHVar1,0,0x2001f,&local_14);
    if (LVar3 == 0) {
      LVar3 = FUN_10002d50(&param_3);
      pcVar4 = RegEnumKeyExA_exref;
      param_3 = local_14;
      if (LVar3 == 0) {
        local_18 = 0x100;
        iVar2 = RegEnumKeyExA(local_14,0,local_128,&local_18,(LPDWORD)0x0,(LPSTR)0x0,(LPDWORD)0x0,
                              &local_24);
        while (iVar2 == 0) {
          iVar2 = FUN_10003210(local_128);
          if (iVar2 != 0) goto LAB_10003070;
          local_18 = 0x100;
          iVar2 = RegEnumKeyExA(param_3,0,local_128,&local_18,(LPDWORD)0x0,(LPSTR)0x0,(LPDWORD)0x0,
                                &local_24);
        }
        FUN_10002d50(&param_3);
        RegDeleteKeyA((HKEY)0x80000000,(LPCSTR)pHVar1);
LAB_10003070:
        local_8 = local_8 & 0xffffff00;
        FUN_10002d50(&param_3);
        goto LAB_10002f0f;
      }
    }
    local_8 = local_8 & 0xffffff00;
    FUN_10002d50(&param_3);
    pcVar4 = RegEnumKeyExA_exref;
  }
LAB_10002f0f:
  pCVar5 = param_4;
  if ((param_4 != (LPCSTR)0x0) && (iVar2 = lstrcmpiA(param_4,&DAT_1000841c), iVar2 != 0)) {
    param_3 = (HKEY)0x0;
    local_8 = CONCAT31(local_8._1_3_,2);
    local_14 = (HKEY)0x0;
    LVar3 = RegOpenKeyExA((HKEY)0x80000000,pCVar5,0,0x2001f,&local_14);
    if (LVar3 == 0) {
      LVar3 = FUN_10002d50(&param_3);
      param_3 = local_14;
      if (LVar3 == 0) {
        param_4 = (LPCSTR)0x100;
        iVar2 = (*pcVar4)(local_14,0,local_128,&param_4,0,0,0,&local_24);
        while (iVar2 == 0) {
          iVar2 = FUN_10003210(local_128);
          if (iVar2 != 0) goto LAB_10002fd0;
          param_4 = (LPCSTR)0x100;
          iVar2 = (*pcVar4)(param_3,0,local_128,&param_4,0,0,0,&local_24);
        }
        FUN_10002d50(&param_3);
        RegDeleteKeyA((HKEY)0x80000000,pCVar5);
      }
    }
LAB_10002fd0:
    local_8 = local_8 & 0xffffff00;
    FUN_10002d50(&param_3);
  }
  StringFromCLSID(param_2,(LPOLESTR *)&local_1c);
  param_3 = local_1c;
  if (local_1c == (HKEY)0x0) {
    pCVar5 = (LPCSTR)0x0;
  }
  else {
    pCVar5 = &stack0xfffffecc;
    iVar2 = lstrlenW((LPCWSTR)local_1c);
    FUN_10005640();
    WideCharToMultiByte(0,0,(LPCWSTR)param_3,-1,&stack0xfffffecc,iVar2 * 2 + 2,(LPCSTR)0x0,
                        (LPBOOL)0x0);
  }
  param_3 = (HKEY)0x0;
  LVar3 = RegOpenKeyExA((HKEY)0x80000000,s_CLSID_10008180,0,0x2001f,&param_3);
  if (LVar3 == 0) {
    LVar3 = RegCloseKey((HKEY)0x80000000);
    pHVar1 = param_3;
    local_28 = param_3;
    if (LVar3 == 0) {
      param_3 = (HKEY)0x0;
      local_8 = CONCAT31(local_8._1_3_,3);
      iVar2 = FUN_100031c0(&param_3,pHVar1,pCVar5,0x2001f);
      if (iVar2 == 0) {
        param_4 = (LPCSTR)0x100;
        iVar2 = RegEnumKeyExA(param_3,0,local_128,(LPDWORD)&param_4,(LPDWORD)0x0,(LPSTR)0x0,
                              (LPDWORD)0x0,&local_24);
        while (iVar2 == 0) {
          iVar2 = FUN_10003210(local_128);
          if (iVar2 != 0) goto LAB_1000317a;
          param_4 = (LPCSTR)0x100;
          iVar2 = RegEnumKeyExA(param_3,0,local_128,(LPDWORD)&param_4,(LPDWORD)0x0,(LPSTR)0x0,
                                (LPDWORD)0x0,&local_24);
        }
        FUN_10002d50(&param_3);
        RegDeleteKeyA(pHVar1,pCVar5);
      }
LAB_1000317a:
      FUN_10002d50(&param_3);
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
// Function: FUN_100031c0 at 100031c0
// Signature: undefined __thiscall FUN_100031c0(void * this, HKEY param_1, LPCSTR param_2, REGSAM param_3)
// Entry: 100031c0
// Body: 100031c0 - 10003209
// ============================================================================
void __thiscall FUN_100031c0(void *this,HKEY param_1,LPCSTR param_2,REGSAM param_3)

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
// Function: FUN_10003210 at 10003210
// Signature: undefined FUN_10003210(void)
// Entry: 10003210
// Body: 10003210 - 10003363
// ============================================================================
/* WARNING: Removing unreachable block (ram,0x10003271) */
/* WARNING: Removing unreachable block (ram,0x10003286) */
/* WARNING: Removing unreachable block (ram,0x10003294) */

LSTATUS __thiscall FUN_10003210(undefined4 *param_1,LPCSTR param_2)

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
  
  puStack_8 = &LAB_10005a3b;
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
      iVar2 = FUN_10003210(aCStack_10c);
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
// Function: FUN_10003370 at 10003370
// Signature: int __stdcall FUN_10003370(undefined4 param_1, int * param_2, int param_3)
// Entry: 10003370
// Body: 10003370 - 1000349f
// ============================================================================
int FUN_10003370(undefined4 param_1,int *param_2,int param_3)

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
  
  puStack_8 = &LAB_10005a58;
  local_c = ExceptionList;
  local_20 = (int *)0x0;
  local_4 = 0;
  if (param_2 != (int *)0x0) {
    ExceptionList = &local_c;
    HVar3 = CoCreateInstance((IID *)&DAT_10006e88,(LPUNKNOWN)0x0,1,(IID *)&DAT_10006e98,&local_20);
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
// Function: FUN_100034a0 at 100034a0
// Signature: undefined __fastcall FUN_100034a0(int * param_1)
// Entry: 100034a0
// Body: 100034a0 - 100034ac
// ============================================================================
void __fastcall FUN_100034a0(int *param_1)

{
  int *piVar1;
  
  piVar1 = (int *)*param_1;
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
  }
  return;
}

// clang-format on
