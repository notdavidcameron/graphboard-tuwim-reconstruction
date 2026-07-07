/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Notes: raw Ghidra decompiler output with only file-level framing added.
*/
#include "../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001020 at 10001020
// Signature: CWnd * __thiscall FUN_10001020(void * this, undefined4 param_1)
// Entry: 10001020
// Body: 10001020 - 1000103a
// ============================================================================
CWnd * __thiscall FUN_10001020(void *this,undefined4 param_1)

{
  CWnd::CWnd(this);
  *(undefined ***)this = &PTR_LAB_10007438;
  *(undefined4 *)((int)this + 0x40) = param_1;
  return this;
}


// ============================================================================
// Function: FUN_10001040 at 10001040
// Signature: CWnd * __thiscall FUN_10001040(void * this, byte param_1)
// Entry: 10001040
// Body: 10001040 - 1000105d
// ============================================================================
CWnd * __thiscall FUN_10001040(void *this,byte param_1)

{
  FUN_10001060(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: FUN_10001060 at 10001060
// Signature: undefined __fastcall FUN_10001060(CWnd * param_1)
// Entry: 10001060
// Body: 10001060 - 100010b5
// ============================================================================
void __fastcall FUN_10001060(CWnd *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_10006908;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined ***)param_1 = &PTR_LAB_10007438;
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
// Function: FUN_100010c0 at 100010c0
// Signature: bool __fastcall FUN_100010c0(CWnd * param_1)
// Entry: 100010c0
// Body: 100010c0 - 10001110
// ============================================================================
bool __fastcall FUN_100010c0(CWnd *param_1)

{
  HWND__ *pHVar1;
  char *pcVar2;
  char *pcVar3;
  ulong uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  HMENU__ *pHVar9;
  void *pvVar10;
  
  if (*(int *)(param_1 + 0x20) != 0) {
    (**(code **)(*(int *)param_1 + 0x60))();
  }
  pvVar10 = (void *)0x0;
  pHVar9 = *(HMENU__ **)(*(int *)(param_1 + 0x40) + 0x2c);
  pHVar1 = (HWND__ *)(**(code **)(*(int *)pHVar9 + 0x18))(pHVar9,0,0);
  iVar8 = 0;
  iVar7 = 0;
  iVar6 = 0;
  iVar5 = 0;
  uVar4 = 0x80000000;
  pcVar3 = s_Video_parent_Wnd_1000a020;
  pcVar2 = AfxRegisterWndClass(0,(HICON__ *)0x0,(HBRUSH__ *)0x0,(HICON__ *)0x0);
  iVar5 = CWnd::CreateEx(param_1,0,pcVar2,pcVar3,uVar4,iVar5,iVar6,iVar7,iVar8,pHVar1,pHVar9,pvVar10
                        );
  return iVar5 != 0;
}


// ============================================================================
// Function: FUN_10001150 at 10001150
// Signature: undefined __stdcall FUN_10001150(void)
// Entry: 10001150
// Body: 10001150 - 10001152
// ============================================================================
void FUN_10001150(void)

{
  return;
}


// ============================================================================
// Function: FUN_100011b0 at 100011b0
// Signature: CPropertySheet * __thiscall FUN_100011b0(void * this, uint param_1, undefined4 param_2, uint param_3)
// Entry: 100011b0
// Body: 100011b0 - 10001227
// ============================================================================
CPropertySheet * __thiscall FUN_100011b0(void *this,uint param_1,undefined4 param_2,uint param_3)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10006936;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  CPropertySheet::CPropertySheet(this,param_1,(CWnd *)0x0,param_3);
  local_4 = 0;
  FUN_10004f30((CPropertyPage *)((int)this + 0x8c));
  local_4 = CONCAT31(local_4._1_3_,1);
  *(undefined ***)this = &PTR_LAB_10007530;
  *(undefined4 *)((int)this + 0x11c) = param_2;
  CPropertySheet::AddPage(this,(CPropertyPage *)((int)this + 0x8c));
  ExceptionList = local_c;
  return this;
}


// ============================================================================
// Function: FUN_10001230 at 10001230
// Signature: CPropertySheet * __thiscall FUN_10001230(void * this, byte param_1)
// Entry: 10001230
// Body: 10001230 - 1000124d
// ============================================================================
CPropertySheet * __thiscall FUN_10001230(void *this,byte param_1)

{
  FUN_10001250(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: FUN_10001250 at 10001250
// Signature: undefined __fastcall FUN_10001250(CPropertySheet * param_1)
// Entry: 10001250
// Body: 10001250 - 100012a4
// ============================================================================
void __fastcall FUN_10001250(CPropertySheet *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_10006948;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined ***)param_1 = &PTR_LAB_10007530;
  local_4 = 0;
  VideoSettingsPage_Destroy((CPropertyPage *)(param_1 + 0x8c));
  local_4 = 0xffffffff;
  CPropertySheet::~CPropertySheet(param_1);
  ExceptionList = local_c;
  return;
}


// ============================================================================
// Function: FUN_10001330 at 10001330
// Signature: undefined __stdcall FUN_10001330(void)
// Entry: 10001330
// Body: 10001330 - 10001339
// ============================================================================
void FUN_10001330(void)

{
  FUN_10001370((CWinApp *)&DAT_1000a340);
  return;
}


// ============================================================================
// Function: FUN_10001340 at 10001340
// Signature: undefined __stdcall FUN_10001340(void)
// Entry: 10001340
// Body: 10001340 - 1000134b
// ============================================================================
void FUN_10001340(void)

{
  FUN_10006640((_onexit_t)&LAB_10001350);
  return;
}


// ============================================================================
// Function: ~CWinApp at 10001360
// Signature: void __thiscall ~CWinApp(CWinApp * this)
// Entry: 10001360
// Body: 10001360 - 10001364
// ============================================================================
void __thiscall CWinApp::~CWinApp(CWinApp *this)

{
                    /* WARNING: Could not recover jumptable at 0x10006076. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~CWinApp(this);
  return;
}


// ============================================================================
// Function: FUN_10001370 at 10001370
// Signature: CWinApp * __fastcall FUN_10001370(CWinApp * param_1)
// Entry: 10001370
// Body: 10001370 - 10001383
// ============================================================================
CWinApp * __fastcall FUN_10001370(CWinApp *param_1)

{
  CWinApp::CWinApp(param_1,(char *)0x0);
  *(undefined ***)param_1 = &PTR_LAB_10007700;
  return param_1;
}


// ============================================================================
// Function: FUN_10001390 at 10001390
// Signature: CWinApp * __thiscall FUN_10001390(void * this, byte param_1)
// Entry: 10001390
// Body: 10001390 - 100013ad
// ============================================================================
CWinApp * __thiscall FUN_10001390(void *this,byte param_1)

{
  CWinApp::~CWinApp(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: DllCanUnloadNow at 10001570
// Signature: HRESULT __stdcall DllCanUnloadNow(void)
// Entry: 10001570
// Body: 10001570 - 100015b8
// ============================================================================
HRESULT DllCanUnloadNow(void)

{
  AFX_MODULE_STATE *pAVar1;
  long lVar2;
  undefined4 local_8;
  int local_4;
  
                    /* 0x1570  1  DllCanUnloadNow */
  pAVar1 = (AFX_MODULE_STATE *)FUN_10006393();
  AFX_MAINTAIN_STATE2::AFX_MAINTAIN_STATE2((AFX_MAINTAIN_STATE2 *)&local_8,pAVar1);
  lVar2 = AfxDllCanUnloadNow();
  if ((lVar2 == 0) && (DAT_1000a41c == 0)) {
    *(undefined4 *)(local_4 + 4) = local_8;
    return 0;
  }
  *(undefined4 *)(local_4 + 4) = local_8;
  return 1;
}


// ============================================================================
// Function: DllGetClassObject at 100015c0
// Signature: HRESULT __stdcall DllGetClassObject(IID * rclsid, IID * riid, LPVOID * ppv)
// Entry: 100015c0
// Body: 100015c0 - 100016a5
// ============================================================================
/* WARNING: Removing unreachable block (ram,0x100015cc) */

HRESULT DllGetClassObject(IID *rclsid,IID *riid,LPVOID *ppv)

{
  int *piVar1;
  ulong *puVar2;
  ulong uVar3;
  int iVar4;
  undefined4 *puVar5;
  
                    /* 0x15c0  2  DllGetClassObject */
  puVar5 = DAT_1000a418;
  iVar4 = 0;
  if (ppv == (LPVOID *)0x0) {
    return -0x7fffbffd;
  }
  *ppv = (LPVOID)0x0;
  puVar2 = (ulong *)*puVar5;
  do {
    if (puVar2 == (ulong *)0x0) {
LAB_1000168e:
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
        EnterCriticalSection((LPCRITICAL_SECTION)&DAT_1000a454);
        if (*piVar1 == 0) {
          iVar4 = (*(code *)puVar5[2])(puVar5[3],&DAT_10008140,piVar1);
        }
        LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_1000a454);
      }
      puVar5 = (undefined4 *)*piVar1;
      if (puVar5 != (undefined4 *)0x0) {
        iVar4 = (**(code **)*puVar5)(puVar5,riid,ppv);
      }
      goto LAB_1000168e;
    }
    if (DAT_1000a408 == 100) {
      puVar5 = puVar5 + 7;
    }
    else {
      puVar5 = puVar5 + 9;
    }
    puVar2 = (ulong *)*puVar5;
  } while( true );
}


// ============================================================================
// Function: DllRegisterServer at 100016b0
// Signature: HRESULT __stdcall DllRegisterServer(void)
// Entry: 100016b0
// Body: 100016b0 - 10001732
// ============================================================================
/* WARNING: Removing unreachable block (ram,0x100016b9) */

HRESULT DllRegisterServer(void)

{
  int iVar1;
  int *piVar2;
  HRESULT HVar3;
  int *piVar4;
  int iVar5;
  
                    /* 0x16b0  3  DllRegisterServer */
  iVar5 = 0;
  piVar4 = DAT_1000a418;
  if (*DAT_1000a418 != 0) {
    do {
      if (((code *)piVar4[6] == (code *)0x0) || (iVar1 = (*(code *)piVar4[6])(), iVar1 == 0)) {
        iVar5 = (*(code *)piVar4[1])(1);
        if (iVar5 < 0) {
          return iVar5;
        }
        if (DAT_1000a408 == 0x84) {
          iVar5 = 1;
          piVar2 = (int *)(*(code *)piVar4[7])();
          iVar5 = FUN_10003150(*piVar4,piVar2,iVar5);
          if (iVar5 < 0) {
            return iVar5;
          }
        }
      }
      if (DAT_1000a408 == 100) {
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
  HVar3 = FUN_10003280(0x1000a408,(LPCWSTR)0x0);
  return HVar3;
}


// ============================================================================
// Function: DllUnregisterServer at 10001740
// Signature: undefined4 __stdcall DllUnregisterServer(void)
// Entry: 10001740
// Body: 10001740 - 1000178e
// ============================================================================
undefined4 DllUnregisterServer(void)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  
                    /* 0x1740  4  DllUnregisterServer */
  iVar1 = *DAT_1000a418;
  piVar3 = DAT_1000a418;
  while (iVar1 != 0) {
    if (((code *)piVar3[6] == (code *)0x0) || (iVar1 = (*(code *)piVar3[6])(), iVar1 == 0)) {
      iVar1 = 0;
      (*(code *)piVar3[1])();
      piVar2 = (int *)(*(code *)piVar3[7])(0);
      FUN_10003150(*piVar3,piVar2,iVar1);
    }
    if (DAT_1000a408 == 100) {
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
// Function: Create_VideoHolderClassFactory at 100017a0
// Signature: undefined __stdcall Create_VideoHolderClassFactory(void)
// Entry: 100017a0
// Body: 100017a0 - 1000180a
// ============================================================================
void Create_VideoHolderClassFactory(void)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_1000696b;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x3c);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    VideoHolderRefCountedState_Construct(puVar1 + 1);
    *puVar1 = &PTR_LAB_100077b8;
    InitializeCriticalSection((LPCRITICAL_SECTION)(puVar1 + 9));
    *puVar1 = &PTR_VideoHolder_QueryInterfaceFromMap_100077a4;
    FUN_10001814();
    return;
  }
  FUN_10001814();
  return;
}


// ============================================================================
// Function: Catch@1000180b at 1000180b
// Signature: undefined * __stdcall Catch@1000180b(void)
// Entry: 1000180b
// Body: 1000180b - 10001810
// ============================================================================
undefined * Catch_1000180b(void)

{
  return &DAT_10001811;
}


// ============================================================================
// Function: FUN_10001814 at 10001814
// Signature: int __stdcall FUN_10001814(void)
// Entry: 10001814
// Body: 10001814 - 1000188a
// ============================================================================
int FUN_10001814(void)

{
  int iVar1;
  int unaff_EBP;
  undefined4 *unaff_ESI;
  
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  if (unaff_ESI != (undefined4 *)0x0) {
    unaff_ESI[8] = *(undefined4 *)(unaff_EBP + 8);
    iVar1 = (**(code **)*unaff_ESI)();
    if (iVar1 != 0) {
      *unaff_ESI = &PTR_VideoHolder_QueryInterfaceFromMap_100077a4;
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
// Function: VideoHolderClassFactory_AddRef at 100018c0
// Signature: LONG __stdcall VideoHolderClassFactory_AddRef(int param_1)
// Entry: 100018c0
// Body: 100018c0 - 100018fd
// ============================================================================
LONG VideoHolderClassFactory_AddRef(int param_1)

{
  LONG LVar1;
  
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x24));
  LVar1 = InterlockedIncrement((LONG *)(param_1 + 4));
  if (*(LONG *)(param_1 + 4) == 2) {
    InterlockedIncrement(&DAT_1000a41c);
  }
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x24));
  return LVar1;
}


// ============================================================================
// Function: VideoHolderClassFactory_Release at 10001900
// Signature: int __stdcall VideoHolderClassFactory_Release(undefined4 * param_1)
// Entry: 10001900
// Body: 10001900 - 1000195c
// ============================================================================
int VideoHolderClassFactory_Release(undefined4 *param_1)

{
  int iVar1;
  
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  InterlockedDecrement(param_1 + 1);
  iVar1 = param_1[1];
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  if (iVar1 == 0) {
    if (param_1 != (undefined4 *)0x0) {
      VideoHolderClassFactory_Destroy(param_1);
      operator_delete(param_1);
      return 0;
    }
  }
  else if (iVar1 == 1) {
    InterlockedDecrement(&DAT_1000a41c);
  }
  return iVar1;
}


// ============================================================================
// Function: VideoHolderClassFactory_Destroy at 10001960
// Signature: undefined __fastcall VideoHolderClassFactory_Destroy(undefined4 * param_1)
// Entry: 10001960
// Body: 10001960 - 1000198f
// ============================================================================
void __fastcall VideoHolderClassFactory_Destroy(undefined4 *param_1)

{
  *param_1 = &PTR_VideoHolder_QueryInterfaceFromMap_100077a4;
  param_1[1] = 1;
  DeleteCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  DeleteCriticalSection
            ((LPCRITICAL_SECTION)((-(uint)(param_1 != (undefined4 *)0x0) & (uint)(param_1 + 1)) + 4)
            );
  return;
}


// ============================================================================
// Function: VideoHolder_QueryInterfaceFromMap at 10001990
// Signature: int __stdcall VideoHolder_QueryInterfaceFromMap(int * param_1, int * param_2, undefined4 * param_3)
// Entry: 10001990
// Body: 10001990 - 10001a82
// ============================================================================
int VideoHolder_QueryInterfaceFromMap(int *param_1,int *param_2,undefined4 *param_3)

{
  undefined **ppuVar1;
  undefined *puVar2;
  int iVar3;
  undefined **ppuVar4;
  int *piVar5;
  
  ppuVar4 = &PTR_DAT_100077d0;
  if (param_3 == (undefined4 *)0x0) {
    return -0x7fffbffd;
  }
  *param_3 = 0;
  puVar2 = DAT_100077d8;
  if ((((*param_2 == 0) && (param_2[1] == 0)) && (param_2[2] == 0xc0)) && (param_2[3] == 0x46000000)
     ) {
    iVar3 = DAT_100077d4 + (int)param_1;
    (**(code **)(*(int *)(DAT_100077d4 + (int)param_1) + 4))(iVar3);
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
// Function: Create_VideoHolderComObject at 10001a90
// Signature: undefined __stdcall Create_VideoHolderComObject(void)
// Entry: 10001a90
// Body: 10001a90 - 10001b0c
// ============================================================================
void Create_VideoHolderComObject(void)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_1000698b;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x148);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    VideoHolderContext_Construct(puVar1);
    *puVar1 = &PTR_LAB_10007858;
    puVar1[1] = &PTR_LAB_1000781c;
    puVar1[2] = &PTR_LAB_100077f8;
    puVar1[3] = &PTR_LAB_100077e8;
    InterlockedIncrement(&DAT_1000a41c);
    FUN_10001b16();
    return;
  }
  FUN_10001b16();
  return;
}


// ============================================================================
// Function: Catch@10001b0d at 10001b0d
// Signature: undefined * __stdcall Catch@10001b0d(void)
// Entry: 10001b0d
// Body: 10001b0d - 10001b12
// ============================================================================
undefined * Catch_10001b0d(void)

{
  return &DAT_10001b13;
}


// ============================================================================
// Function: FUN_10001b16 at 10001b16
// Signature: int __stdcall FUN_10001b16(void)
// Entry: 10001b16
// Body: 10001b16 - 10001b9b
// ============================================================================
int FUN_10001b16(void)

{
  int iVar1;
  int unaff_EBP;
  undefined4 *unaff_ESI;
  
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  if (unaff_ESI != (undefined4 *)0x0) {
    iVar1 = (**(code **)*unaff_ESI)();
    if (iVar1 != 0) {
      *unaff_ESI = &PTR_LAB_10007858;
      unaff_ESI[1] = &PTR_LAB_1000781c;
      unaff_ESI[2] = &PTR_LAB_100077f8;
      unaff_ESI[3] = &PTR_LAB_100077e8;
      unaff_ESI[4] = 1;
      InterlockedDecrement(&DAT_1000a41c);
      FUN_10003af0(unaff_ESI);
      operator_delete(unaff_ESI);
    }
    ExceptionList = *(void **)(unaff_EBP + -0xc);
    return iVar1;
  }
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  return *(int *)(unaff_EBP + -0x18);
}


// ============================================================================
// Function: FUN_10001bb0 at 10001bb0
// Signature: HRESULT __stdcall FUN_10001bb0(undefined4 param_1, undefined4 param_2, BSTR param_3, undefined4 * param_4)
// Entry: 10001bb0
// Body: 10001bb0 - 10001bf6
// ============================================================================
HRESULT FUN_10001bb0(undefined4 param_1,undefined4 param_2,BSTR param_3,undefined4 *param_4)

{
  HRESULT HVar1;
  
  HVar1 = -0x7fffbffd;
  if (param_4 != (undefined4 *)0x0) {
    HVar1 = 0;
    if (DAT_1000a0ec == (int *)0x0) {
      HVar1 = FUN_100035c0(&PTR_DAT_1000a0e0,param_3);
    }
    *param_4 = DAT_1000a0ec;
    if (DAT_1000a0ec != (int *)0x0) {
      (**(code **)(*DAT_1000a0ec + 4))(DAT_1000a0ec);
      HVar1 = 0;
    }
  }
  return HVar1;
}


// ============================================================================
// Function: FUN_10001d20 at 10001d20
// Signature: HRESULT __stdcall FUN_10001d20(undefined4 param_1, undefined4 param_2, BSTR param_3, undefined4 * param_4)
// Entry: 10001d20
// Body: 10001d20 - 10001d66
// ============================================================================
HRESULT FUN_10001d20(undefined4 param_1,undefined4 param_2,BSTR param_3,undefined4 *param_4)

{
  HRESULT HVar1;
  
  HVar1 = -0x7fffbffd;
  if (param_4 != (undefined4 *)0x0) {
    HVar1 = 0;
    if (DAT_1000a10c == (int *)0x0) {
      HVar1 = FUN_100035c0(&PTR_DAT_1000a100,param_3);
    }
    *param_4 = DAT_1000a10c;
    if (DAT_1000a10c != (int *)0x0) {
      (**(code **)(*DAT_1000a10c + 4))(DAT_1000a10c);
      HVar1 = 0;
    }
  }
  return HVar1;
}


// ============================================================================
// Function: FUN_10001e90 at 10001e90
// Signature: HRESULT __stdcall FUN_10001e90(undefined4 param_1, undefined4 param_2, BSTR param_3, undefined4 * param_4)
// Entry: 10001e90
// Body: 10001e90 - 10001ed6
// ============================================================================
HRESULT FUN_10001e90(undefined4 param_1,undefined4 param_2,BSTR param_3,undefined4 *param_4)

{
  HRESULT HVar1;
  
  HVar1 = -0x7fffbffd;
  if (param_4 != (undefined4 *)0x0) {
    HVar1 = 0;
    if (DAT_1000a12c == (int *)0x0) {
      HVar1 = FUN_100035c0(&PTR_DAT_1000a120,param_3);
    }
    *param_4 = DAT_1000a12c;
    if (DAT_1000a12c != (int *)0x0) {
      (**(code **)(*DAT_1000a12c + 4))(DAT_1000a12c);
      HVar1 = 0;
    }
  }
  return HVar1;
}


// ============================================================================
// Function: FUN_10002000 at 10002000
// Signature: undefined __stdcall FUN_10002000(undefined4 param_1)
// Entry: 10002000
// Body: 10002000 - 10002090
// ============================================================================
void FUN_10002000(undefined4 param_1)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_100069ab;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x154);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    puVar1[1] = 0;
    VideoHolderContext_Construct(puVar1 + 3);
    puVar1[3] = &PTR_LAB_1000794c;
    puVar1[4] = &PTR_LAB_10007910;
    puVar1[5] = &PTR_LAB_100078ec;
    puVar1[6] = &PTR_LAB_100078dc;
    puVar1[7] = param_1;
    *puVar1 = &PTR_LAB_100078d0;
    InterlockedIncrement(&DAT_1000a41c);
    FUN_1000209c();
    return;
  }
  FUN_1000209c();
  return;
}


// ============================================================================
// Function: Catch@10002091 at 10002091
// Signature: undefined * __stdcall Catch@10002091(void)
// Entry: 10002091
// Body: 10002091 - 10002096
// ============================================================================
undefined * Catch_10002091(void)

{
  return &DAT_10002097;
}


// ============================================================================
// Function: FUN_1000209c at 1000209c
// Signature: undefined4 * __stdcall FUN_1000209c(void)
// Entry: 1000209c
// Body: 1000209c - 1000210d
// ============================================================================
undefined4 * FUN_1000209c(void)

{
  undefined4 *puVar1;
  undefined4 *unaff_EBX;
  int unaff_EBP;
  undefined4 *unaff_ESI;
  
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  if (unaff_ESI != unaff_EBX) {
    puVar1 = (undefined4 *)(**(code **)*unaff_ESI)();
    if (puVar1 != unaff_EBX) {
      *unaff_ESI = &PTR_LAB_100078d0;
      unaff_ESI[1] = 1;
      InterlockedDecrement(&DAT_1000a41c);
      FUN_10003af0(unaff_ESI + 3);
      operator_delete(unaff_ESI);
    }
    ExceptionList = *(void **)(unaff_EBP + -0xc);
    return puVar1;
  }
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  return *(undefined4 **)(unaff_EBP + -0x18);
}


// ============================================================================
// Function: VideoHolderRefCountedState_Construct at 10002110
// Signature: undefined4 * __fastcall VideoHolderRefCountedState_Construct(undefined4 * param_1)
// Entry: 10002110
// Body: 10002110 - 10002126
// ============================================================================
undefined4 * __fastcall VideoHolderRefCountedState_Construct(undefined4 *param_1)

{
  *param_1 = 0;
  InitializeCriticalSection((LPCRITICAL_SECTION)(param_1 + 1));
  return param_1;
}


// ============================================================================
// Function: FUN_10002190 at 10002190
// Signature: undefined __fastcall FUN_10002190(undefined4 * param_1)
// Entry: 10002190
// Body: 10002190 - 100021c8
// ============================================================================
void __fastcall FUN_10002190(undefined4 *param_1)

{
  *param_1 = &PTR_LAB_10007858;
  param_1[1] = &PTR_LAB_1000781c;
  param_1[2] = &PTR_LAB_100077f8;
  param_1[3] = &PTR_LAB_100077e8;
  param_1[4] = 1;
  InterlockedDecrement(&DAT_1000a41c);
  FUN_10003af0(param_1);
  return;
}


// ============================================================================
// Function: FUN_100022f0 at 100022f0
// Signature: LONG __stdcall FUN_100022f0(undefined4 * param_1)
// Entry: 100022f0
// Body: 100022f0 - 10002320
// ============================================================================
LONG FUN_100022f0(undefined4 *param_1)

{
  LONG LVar1;
  
  LVar1 = InterlockedDecrement(param_1 + 1);
  if ((LVar1 == 0) && (param_1 != (undefined4 *)0x0)) {
    FUN_10002330(param_1);
    operator_delete(param_1);
  }
  return LVar1;
}


// ============================================================================
// Function: FUN_10002330 at 10002330
// Signature: undefined __fastcall FUN_10002330(undefined4 * param_1)
// Entry: 10002330
// Body: 10002330 - 10002354
// ============================================================================
void __fastcall FUN_10002330(undefined4 *param_1)

{
  *param_1 = &PTR_LAB_100078d0;
  param_1[1] = 1;
  InterlockedDecrement(&DAT_1000a41c);
  FUN_10003af0(param_1 + 3);
  return;
}


// ============================================================================
// Function: FUN_10002720 at 10002720
// Signature: IID * __stdcall FUN_10002720(int param_1, IID * param_2, BYTE * param_3, BYTE * param_4, HKEY param_5, uint param_6)
// Entry: 10002720
// Body: 10002720 - 10002b01
// ============================================================================
IID * FUN_10002720(int param_1,IID *param_2,BYTE *param_3,BYTE *param_4,HKEY param_5,uint param_6)

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
  puStack_c = &LAB_100069f0;
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
    FUN_10006660();
    WideCharToMultiByte(0,0,(LPCWSTR)param_5,-1,&stack0xfffffcd0,iVar3 * 2 + 2,(LPCSTR)0x0,
                        (LPBOOL)0x0);
    local_14 = &stack0xfffffcd0;
  }
  param_2 = (IID *)0x0;
  local_8 = 0;
  uVar2 = FUN_10002b50(&param_2,(HKEY)0x80000000,(LPCSTR)param_3,(LPSTR)0x0,0,0x20019,
                       (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
  if (uVar2 == 0) {
    iVar3 = lstrlenA((LPCSTR)local_11c);
    RegSetValueExA((HKEY)param_2,(LPCSTR)0x0,0,1,local_11c,iVar3 + 1);
    FUN_10002bc0(&param_2,s_CLSID_1000a190,pBVar7,(LPCSTR)0x0);
    local_8 = 0xffffffff;
    FUN_10002b30(&param_2);
  }
  else {
    pIVar4 = (IID *)(uVar2 & 0xffff | 0x80070000);
    local_8 = 0xffffffff;
    FUN_10002b30(&param_2);
    param_2 = pIVar4;
    if (pIVar4 != (IID *)0x0) goto LAB_100028c9;
  }
  param_2 = (IID *)0x0;
  local_8 = 1;
  uVar2 = FUN_10002b50(&param_2,(HKEY)0x80000000,(LPCSTR)param_4,(LPSTR)0x0,0,0x20019,
                       (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
  if (uVar2 == 0) {
    FUN_10002c30(&param_2,local_11c,(LPCSTR)0x0);
    FUN_10002bc0(&param_2,s_CLSID_1000a190,pBVar7,(LPCSTR)0x0);
    local_8 = 0xffffffff;
    FUN_10002b30(&param_2);
    param_2 = (IID *)0x0;
  }
  else {
    local_8 = 0xffffffff;
    FUN_10002b30(&param_2);
    param_2 = (IID *)(uVar2 & 0xffff | 0x80070000);
  }
LAB_100028c9:
  uVar2 = 0;
  if (param_2 == (IID *)0x0) {
    param_5 = (HKEY)0x0;
    local_8 = 2;
    local_18 = (HKEY)0x0;
    uVar2 = RegOpenKeyExA((HKEY)0x80000000,s_CLSID_1000a190,0,0x20019,&local_18);
    if (uVar2 == 0) {
      uVar2 = FUN_10002b30(&param_5);
      param_5 = local_18;
    }
    if (uVar2 == 0) {
      local_18 = (HKEY)0x0;
      uVar2 = RegCreateKeyExA(param_5,local_14,0,(LPSTR)0x0,0,0x20019,(LPSECURITY_ATTRIBUTES)0x0,
                              &local_18,(LPDWORD)&local_14);
      if (uVar2 == 0) {
        uVar2 = FUN_10002b30(&param_5);
        param_5 = local_18;
      }
      if (uVar2 == 0) {
        iVar3 = lstrlenA((LPCSTR)local_11c);
        RegSetValueExA(param_5,(LPCSTR)0x0,0,1,local_11c,iVar3 + 1);
        local_14 = (LPCSTR)0x0;
        local_8 = CONCAT31(local_8._1_3_,3);
        iVar3 = FUN_10002b50(&local_14,param_5,"ProgID",(LPSTR)0x0,0,0xf003f,
                             (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
        if (iVar3 == 0) {
          FUN_10002c30(&local_14,param_3,(LPCSTR)0x0);
        }
        local_8._0_1_ = 2;
        FUN_10002b30(&local_14);
        param_3 = (BYTE *)0x0;
        local_8 = CONCAT31(local_8._1_3_,4);
        iVar3 = FUN_10002b50(&param_3,param_5,"VersionIndependentProgID",(LPSTR)0x0,0,0xf003f,
                             (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
        if (iVar3 == 0) {
          FUN_10002c30(&param_3,param_4,(LPCSTR)0x0);
        }
        local_8 = CONCAT31(local_8._1_3_,2);
        FUN_10002b30(&param_3);
        if ((*(int *)(param_1 + 4) == 0) ||
           (pHVar5 = GetModuleHandleA((LPCSTR)0x0), uVar1 = param_6,
           *(HMODULE *)(param_1 + 4) == pHVar5)) {
          GetShortPathNameA((LPCSTR)local_220,(LPSTR)local_324,0x104);
          param_3 = (BYTE *)0x0;
          local_8 = CONCAT31(local_8._1_3_,5);
          iVar3 = FUN_10002b50(&param_3,param_5,"LocalServer32",(LPSTR)0x0,0,0xf003f,
                               (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
          if (iVar3 == 0) {
            FUN_10002c30(&param_3,local_324,(LPCSTR)0x0);
          }
          local_8 = CONCAT31(local_8._1_3_,2);
          FUN_10002b30(&param_3);
        }
        else {
          pcVar6 = "AUTPRX32.DLL";
          if ((param_6 & 4) == 0) {
            pcVar6 = (char *)local_220;
          }
          FUN_10002bc0(&param_5,"InprocServer32",(BYTE *)pcVar6,(LPCSTR)0x0);
          if ((uVar1 & 2) == 0) {
            pBVar7 = (BYTE *)(-(uint)((uVar1 & 1) != 0) & 0x10007a30);
          }
          else {
            pBVar7 = "both";
          }
          if (pBVar7 != (BYTE *)0x0) {
            FUN_10002bc0(&param_5,"InprocServer32",pBVar7,"ThreadingModel");
          }
        }
      }
    }
    local_8 = 0xffffffff;
    FUN_10002b30(&param_5);
  }
  CoTaskMemFree(local_1c);
  if (uVar2 != 0) {
    param_2 = (IID *)(uVar2 & 0xffff | 0x80070000);
  }
  ExceptionList = local_10;
  return param_2;
}


// ============================================================================
// Function: FUN_10002b10 at 10002b10
// Signature: undefined __fastcall FUN_10002b10(undefined4 * param_1)
// Entry: 10002b10
// Body: 10002b10 - 10002b27
// ============================================================================
void __fastcall FUN_10002b10(undefined4 *param_1)

{
  if ((HKEY)*param_1 != (HKEY)0x0) {
    RegCloseKey((HKEY)*param_1);
    *param_1 = 0;
  }
  return;
}


// ============================================================================
// Function: FUN_10002b30 at 10002b30
// Signature: LSTATUS __fastcall FUN_10002b30(undefined4 * param_1)
// Entry: 10002b30
// Body: 10002b30 - 10002b49
// ============================================================================
LSTATUS __fastcall FUN_10002b30(undefined4 *param_1)

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
// Function: FUN_10002b50 at 10002b50
// Signature: undefined __thiscall FUN_10002b50(void * this, HKEY param_1, LPCSTR param_2, LPSTR param_3, DWORD param_4, REGSAM param_5, LPSECURITY_ATTRIBUTES param_6, undefined4 * param_7)
// Entry: 10002b50
// Body: 10002b50 - 10002bbb
// ============================================================================
void __thiscall
FUN_10002b50(void *this,HKEY param_1,LPCSTR param_2,LPSTR param_3,DWORD param_4,REGSAM param_5,
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
// Function: FUN_10002bc0 at 10002bc0
// Signature: LSTATUS __thiscall FUN_10002bc0(void * this, LPCSTR param_1, BYTE * param_2, LPCSTR param_3)
// Entry: 10002bc0
// Body: 10002bc0 - 10002c2a
// ============================================================================
LSTATUS __thiscall FUN_10002bc0(void *this,LPCSTR param_1,BYTE *param_2,LPCSTR param_3)

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
// Function: FUN_10002c30 at 10002c30
// Signature: undefined __thiscall FUN_10002c30(void * this, BYTE * param_1, LPCSTR param_2)
// Entry: 10002c30
// Body: 10002c30 - 10002c58
// ============================================================================
void __thiscall FUN_10002c30(void *this,BYTE *param_1,LPCSTR param_2)

{
  int iVar1;
  
  iVar1 = lstrlenA((LPCSTR)param_1);
  RegSetValueExA(*(HKEY *)this,param_2,0,1,param_1,iVar1 + 1);
  return;
}


// ============================================================================
// Function: FUN_10002c60 at 10002c60
// Signature: undefined4 __stdcall FUN_10002c60(undefined4 param_1, IID * param_2, HKEY param_3, LPCSTR param_4)
// Entry: 10002c60
// Body: 10002c60 - 10002f94
// ============================================================================
undefined4 FUN_10002c60(undefined4 param_1,IID *param_2,HKEY param_3,LPCSTR param_4)

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
  puStack_c = &LAB_10006a20;
  local_10 = ExceptionList;
  local_8 = 0;
  local_28 = (HKEY)0x80000000;
  pcVar4 = RegEnumKeyExA_exref;
  ExceptionList = &local_10;
  if ((param_3 != (HKEY)0x0) &&
     (ExceptionList = &local_10, iVar2 = lstrcmpiA((LPCSTR)param_3,&DAT_1000a490),
     pcVar4 = RegEnumKeyExA_exref, iVar2 != 0)) {
    param_3 = (HKEY)0x0;
    local_8 = CONCAT31(local_8._1_3_,1);
    local_14 = (HKEY)0x0;
    LVar3 = RegOpenKeyExA((HKEY)0x80000000,(LPCSTR)pHVar1,0,0x2001f,&local_14);
    if (LVar3 == 0) {
      LVar3 = FUN_10002b30(&param_3);
      pcVar4 = RegEnumKeyExA_exref;
      param_3 = local_14;
      if (LVar3 == 0) {
        local_18 = 0x100;
        iVar2 = RegEnumKeyExA(local_14,0,local_128,&local_18,(LPDWORD)0x0,(LPSTR)0x0,(LPDWORD)0x0,
                              &local_24);
        while (iVar2 == 0) {
          iVar2 = FUN_10002ff0(local_128);
          if (iVar2 != 0) goto LAB_10002e50;
          local_18 = 0x100;
          iVar2 = RegEnumKeyExA(param_3,0,local_128,&local_18,(LPDWORD)0x0,(LPSTR)0x0,(LPDWORD)0x0,
                                &local_24);
        }
        FUN_10002b30(&param_3);
        RegDeleteKeyA((HKEY)0x80000000,(LPCSTR)pHVar1);
LAB_10002e50:
        local_8 = local_8 & 0xffffff00;
        FUN_10002b30(&param_3);
        goto LAB_10002cef;
      }
    }
    local_8 = local_8 & 0xffffff00;
    FUN_10002b30(&param_3);
    pcVar4 = RegEnumKeyExA_exref;
  }
LAB_10002cef:
  pCVar5 = param_4;
  if ((param_4 != (LPCSTR)0x0) && (iVar2 = lstrcmpiA(param_4,&DAT_1000a490), iVar2 != 0)) {
    param_3 = (HKEY)0x0;
    local_8 = CONCAT31(local_8._1_3_,2);
    local_14 = (HKEY)0x0;
    LVar3 = RegOpenKeyExA((HKEY)0x80000000,pCVar5,0,0x2001f,&local_14);
    if (LVar3 == 0) {
      LVar3 = FUN_10002b30(&param_3);
      param_3 = local_14;
      if (LVar3 == 0) {
        param_4 = (LPCSTR)0x100;
        iVar2 = (*pcVar4)(local_14,0,local_128,&param_4,0,0,0,&local_24);
        while (iVar2 == 0) {
          iVar2 = FUN_10002ff0(local_128);
          if (iVar2 != 0) goto LAB_10002db0;
          param_4 = (LPCSTR)0x100;
          iVar2 = (*pcVar4)(param_3,0,local_128,&param_4,0,0,0,&local_24);
        }
        FUN_10002b30(&param_3);
        RegDeleteKeyA((HKEY)0x80000000,pCVar5);
      }
    }
LAB_10002db0:
    local_8 = local_8 & 0xffffff00;
    FUN_10002b30(&param_3);
  }
  StringFromCLSID(param_2,(LPOLESTR *)&local_1c);
  param_3 = local_1c;
  if (local_1c == (HKEY)0x0) {
    pCVar5 = (LPCSTR)0x0;
  }
  else {
    pCVar5 = &stack0xfffffecc;
    iVar2 = lstrlenW((LPCWSTR)local_1c);
    FUN_10006660();
    WideCharToMultiByte(0,0,(LPCWSTR)param_3,-1,&stack0xfffffecc,iVar2 * 2 + 2,(LPCSTR)0x0,
                        (LPBOOL)0x0);
  }
  param_3 = (HKEY)0x0;
  LVar3 = RegOpenKeyExA((HKEY)0x80000000,s_CLSID_1000a190,0,0x20019,&param_3);
  if (LVar3 == 0) {
    LVar3 = RegCloseKey((HKEY)0x80000000);
    pHVar1 = param_3;
    local_28 = param_3;
    if (LVar3 == 0) {
      param_3 = (HKEY)0x0;
      local_8 = CONCAT31(local_8._1_3_,3);
      iVar2 = FUN_10002fa0(&param_3,pHVar1,pCVar5,0x2001f);
      if (iVar2 == 0) {
        param_4 = (LPCSTR)0x100;
        iVar2 = RegEnumKeyExA(param_3,0,local_128,(LPDWORD)&param_4,(LPDWORD)0x0,(LPSTR)0x0,
                              (LPDWORD)0x0,&local_24);
        while (iVar2 == 0) {
          iVar2 = FUN_10002ff0(local_128);
          if (iVar2 != 0) goto LAB_10002f5a;
          param_4 = (LPCSTR)0x100;
          iVar2 = RegEnumKeyExA(param_3,0,local_128,(LPDWORD)&param_4,(LPDWORD)0x0,(LPSTR)0x0,
                                (LPDWORD)0x0,&local_24);
        }
        FUN_10002b30(&param_3);
        RegDeleteKeyA(pHVar1,pCVar5);
      }
LAB_10002f5a:
      FUN_10002b30(&param_3);
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
// Function: FUN_10002fa0 at 10002fa0
// Signature: undefined __thiscall FUN_10002fa0(void * this, HKEY param_1, LPCSTR param_2, REGSAM param_3)
// Entry: 10002fa0
// Body: 10002fa0 - 10002fe9
// ============================================================================
void __thiscall FUN_10002fa0(void *this,HKEY param_1,LPCSTR param_2,REGSAM param_3)

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
// Function: FUN_10002ff0 at 10002ff0
// Signature: undefined FUN_10002ff0(void)
// Entry: 10002ff0
// Body: 10002ff0 - 10003143
// ============================================================================
/* WARNING: Removing unreachable block (ram,0x10003051) */
/* WARNING: Removing unreachable block (ram,0x10003066) */
/* WARNING: Removing unreachable block (ram,0x10003074) */

LSTATUS __thiscall FUN_10002ff0(undefined4 *param_1,LPCSTR param_2)

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
  
  puStack_8 = &LAB_10006a3b;
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
      iVar2 = FUN_10002ff0(aCStack_10c);
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
// Function: FUN_10003150 at 10003150
// Signature: int __stdcall FUN_10003150(undefined4 param_1, int * param_2, int param_3)
// Entry: 10003150
// Body: 10003150 - 1000327f
// ============================================================================
int FUN_10003150(undefined4 param_1,int *param_2,int param_3)

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
  
  puStack_8 = &LAB_10006a58;
  local_c = ExceptionList;
  local_20 = (int *)0x0;
  local_4 = 0;
  if (param_2 != (int *)0x0) {
    ExceptionList = &local_c;
    HVar3 = CoCreateInstance((IID *)&DAT_10008150,(LPUNKNOWN)0x0,1,(IID *)&DAT_10008160,&local_20);
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
// Function: FUN_10003280 at 10003280
// Signature: HRESULT __stdcall FUN_10003280(int param_1, LPCWSTR param_2)
// Entry: 10003280
// Body: 10003280 - 10003470
// ============================================================================
HRESULT FUN_10003280(int param_1,LPCWSTR param_2)

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
  
  puStack_c = &LAB_10006a80;
  local_10 = ExceptionList;
  local_8 = 1;
  local_14 = (LPCWSTR)0x0;
  local_18 = (ITypeLib *)0x0;
  ExceptionList = &local_10;
  GetModuleFileNameA(*(HMODULE *)(param_1 + 0xc),local_128,0x104);
  if (param_2 != (LPCWSTR)0x0) {
    iVar1 = lstrlenW(param_2);
    FUN_10006660();
    lpString2 = FUN_100034b0(&stack0xfffffdd4,param_2,iVar1 * 2 + 2,0);
    lstrcatA(local_128,lpString2);
  }
  if (&stack0x00000000 == (undefined1 *)0x128) {
    szFile = (LPCOLESTR)0x0;
  }
  else {
    iVar1 = lstrlenA(local_128);
    FUN_10006660();
    szFile = FUN_10003480((LPWSTR)&stack0xfffffdd4,local_128,iVar1 + 1,0);
  }
  HVar2 = LoadTypeLib(szFile,&local_18);
  if (HVar2 < 0) {
    lpString1 = (LPCSTR)0x0;
    lpsz = local_128;
    if (local_128[0] == '\0') {
LAB_1000337f:
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
      if (lpString1 == (LPCSTR)0x0) goto LAB_1000337f;
    }
    lstrcpyA(lpString1,&DAT_1000a198);
    if (&stack0x00000000 == (undefined1 *)0x128) {
      szFile = (LPCOLESTR)0x0;
    }
    else {
      iVar1 = lstrlenA(local_128);
      FUN_10006660();
      szFile = FUN_10003480((LPWSTR)&stack0xfffffdd4,local_128,iVar1 + 1,0);
    }
    HVar2 = LoadTypeLib(szFile,&local_18);
    if (HVar2 < 0) goto LAB_100033e6;
  }
  local_14 = SysAllocString(szFile);
LAB_100033e6:
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
    iVar1 = FUN_100034f0(local_220);
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
// Function: FUN_10003480 at 10003480
// Signature: LPWSTR __stdcall FUN_10003480(LPWSTR param_1, LPCSTR param_2, int param_3, UINT param_4)
// Entry: 10003480
// Body: 10003480 - 100034a9
// ============================================================================
LPWSTR FUN_10003480(LPWSTR param_1,LPCSTR param_2,int param_3,UINT param_4)

{
  *param_1 = L'\0';
  MultiByteToWideChar(param_4,0,param_2,-1,param_1,param_3);
  return param_1;
}


// ============================================================================
// Function: FUN_100034b0 at 100034b0
// Signature: LPSTR __stdcall FUN_100034b0(LPSTR param_1, LPCWSTR param_2, int param_3, UINT param_4)
// Entry: 100034b0
// Body: 100034b0 - 100034db
// ============================================================================
LPSTR FUN_100034b0(LPSTR param_1,LPCWSTR param_2,int param_3,UINT param_4)

{
  *param_1 = '\0';
  WideCharToMultiByte(param_4,0,param_2,-1,param_1,param_3,(LPCSTR)0x0,(LPBOOL)0x0);
  return param_1;
}

// clang-format on
