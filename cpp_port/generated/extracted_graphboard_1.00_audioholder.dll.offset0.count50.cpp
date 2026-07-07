/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Notes: raw Ghidra decompiler output with only file-level framing added.
*/
#include "../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001060 at 10001060
// Signature: undefined __stdcall FUN_10001060(void)
// Entry: 10001060
// Body: 10001060 - 10001069
// ============================================================================
void FUN_10001060(void)

{
  FUN_100010a0((CWinApp *)&DAT_1000b3a0);
  return;
}


// ============================================================================
// Function: FUN_10001070 at 10001070
// Signature: undefined __stdcall FUN_10001070(void)
// Entry: 10001070
// Body: 10001070 - 1000107b
// ============================================================================
void FUN_10001070(void)

{
  FUN_10006e0e((_onexit_t)&LAB_10001080);
  return;
}


// ============================================================================
// Function: ~CWinApp at 10001090
// Signature: void __thiscall ~CWinApp(CWinApp * this)
// Entry: 10001090
// Body: 10001090 - 10001094
// ============================================================================
void __thiscall CWinApp::~CWinApp(CWinApp *this)

{
                    /* WARNING: Could not recover jumptable at 0x10006830. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~CWinApp(this);
  return;
}


// ============================================================================
// Function: FUN_100010a0 at 100010a0
// Signature: CWinApp * __fastcall FUN_100010a0(CWinApp * param_1)
// Entry: 100010a0
// Body: 100010a0 - 100010b3
// ============================================================================
CWinApp * __fastcall FUN_100010a0(CWinApp *param_1)

{
  CWinApp::CWinApp(param_1,(char *)0x0);
  *(undefined ***)param_1 = &PTR_LAB_100085b0;
  return param_1;
}


// ============================================================================
// Function: FUN_100010c0 at 100010c0
// Signature: CWinApp * __thiscall FUN_100010c0(void * this, byte param_1)
// Entry: 100010c0
// Body: 100010c0 - 100010dd
// ============================================================================
CWinApp * __thiscall FUN_100010c0(void *this,byte param_1)

{
  CWinApp::~CWinApp(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: DllCanUnloadNow at 100012a0
// Signature: HRESULT __stdcall DllCanUnloadNow(void)
// Entry: 100012a0
// Body: 100012a0 - 100012e8
// ============================================================================
HRESULT DllCanUnloadNow(void)

{
  AFX_MODULE_STATE *pAVar1;
  long lVar2;
  undefined4 local_8;
  int local_4;
  
                    /* 0x12a0  1  DllCanUnloadNow */
  pAVar1 = (AFX_MODULE_STATE *)FUN_10006c5b();
  AFX_MAINTAIN_STATE2::AFX_MAINTAIN_STATE2((AFX_MAINTAIN_STATE2 *)&local_8,pAVar1);
  lVar2 = AfxDllCanUnloadNow();
  if ((lVar2 == 0) && (DAT_1000b47c == 0)) {
    *(undefined4 *)(local_4 + 4) = local_8;
    return 0;
  }
  *(undefined4 *)(local_4 + 4) = local_8;
  return 1;
}


// ============================================================================
// Function: DllGetClassObject at 100012f0
// Signature: HRESULT __stdcall DllGetClassObject(IID * rclsid, IID * riid, LPVOID * ppv)
// Entry: 100012f0
// Body: 100012f0 - 100013d5
// ============================================================================
/* WARNING: Removing unreachable block (ram,0x100012fc) */

HRESULT DllGetClassObject(IID *rclsid,IID *riid,LPVOID *ppv)

{
  int *piVar1;
  ulong *puVar2;
  ulong uVar3;
  int iVar4;
  undefined4 *puVar5;
  
                    /* 0x12f0  2  DllGetClassObject */
  puVar5 = DAT_1000b478;
  iVar4 = 0;
  if (ppv == (LPVOID *)0x0) {
    return -0x7fffbffd;
  }
  *ppv = (LPVOID)0x0;
  puVar2 = (ulong *)*puVar5;
  do {
    if (puVar2 == (ulong *)0x0) {
LAB_100013be:
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
        EnterCriticalSection((LPCRITICAL_SECTION)&DAT_1000b4b4);
        if (*piVar1 == 0) {
          iVar4 = (*(code *)puVar5[2])(puVar5[3],&DAT_10009160,piVar1);
        }
        LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_1000b4b4);
      }
      puVar5 = (undefined4 *)*piVar1;
      if (puVar5 != (undefined4 *)0x0) {
        iVar4 = (**(code **)*puVar5)(puVar5,riid,ppv);
      }
      goto LAB_100013be;
    }
    if (DAT_1000b468 == 100) {
      puVar5 = puVar5 + 7;
    }
    else {
      puVar5 = puVar5 + 9;
    }
    puVar2 = (ulong *)*puVar5;
  } while( true );
}


// ============================================================================
// Function: DllRegisterServer at 100013e0
// Signature: HRESULT __stdcall DllRegisterServer(void)
// Entry: 100013e0
// Body: 100013e0 - 10001462
// ============================================================================
/* WARNING: Removing unreachable block (ram,0x100013e9) */

HRESULT DllRegisterServer(void)

{
  int iVar1;
  int *piVar2;
  HRESULT HVar3;
  int *piVar4;
  int iVar5;
  
                    /* 0x13e0  3  DllRegisterServer */
  iVar5 = 0;
  piVar4 = DAT_1000b478;
  if (*DAT_1000b478 != 0) {
    do {
      if (((code *)piVar4[6] == (code *)0x0) || (iVar1 = (*(code *)piVar4[6])(), iVar1 == 0)) {
        iVar5 = (*(code *)piVar4[1])(1);
        if (iVar5 < 0) {
          return iVar5;
        }
        if (DAT_1000b468 == 0x84) {
          iVar5 = 1;
          piVar2 = (int *)(*(code *)piVar4[7])();
          iVar5 = FUN_10002ee0(*piVar4,piVar2,iVar5);
          if (iVar5 < 0) {
            return iVar5;
          }
        }
      }
      if (DAT_1000b468 == 100) {
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
  HVar3 = FUN_10003020(0x1000b468,(LPCWSTR)0x0);
  return HVar3;
}


// ============================================================================
// Function: DllUnregisterServer at 10001470
// Signature: undefined4 __stdcall DllUnregisterServer(void)
// Entry: 10001470
// Body: 10001470 - 100014d0
// ============================================================================
undefined4 DllUnregisterServer(void)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  
                    /* 0x1470  4  DllUnregisterServer */
  iVar1 = *DAT_1000b478;
  piVar3 = DAT_1000b478;
  do {
    if (iVar1 == 0) {
      return 0;
    }
    if ((((code *)piVar3[6] == (code *)0x0) || (iVar1 = (*(code *)piVar3[6])(), iVar1 == 0)) &&
       ((*(code *)piVar3[1])(0), DAT_1000b468 == 0x84)) {
      if ((code *)piVar3[7] != (code *)0x0) {
        iVar1 = 0;
        piVar2 = (int *)(*(code *)piVar3[7])();
        FUN_10002ee0(*piVar3,piVar2,iVar1);
        goto LAB_100014b7;
      }
LAB_100014c5:
      piVar3 = piVar3 + 9;
    }
    else {
LAB_100014b7:
      if (DAT_1000b468 != 100) goto LAB_100014c5;
      piVar3 = piVar3 + 7;
    }
    iVar1 = *piVar3;
  } while( true );
}


// ============================================================================
// Function: Create_AudioHolderClassFactory at 100014f0
// Signature: undefined __stdcall Create_AudioHolderClassFactory(void)
// Entry: 100014f0
// Body: 100014f0 - 1000155a
// ============================================================================
void Create_AudioHolderClassFactory(void)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_100071cb;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x3c);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    AudioHolderRefCountedState_Construct(puVar1 + 1);
    *puVar1 = &PTR_LAB_10008668;
    InitializeCriticalSection((LPCRITICAL_SECTION)(puVar1 + 9));
    *puVar1 = &PTR_AudioHolder_QueryInterfaceFromMap_10008654;
    FUN_10001564();
    return;
  }
  FUN_10001564();
  return;
}


// ============================================================================
// Function: Catch@1000155b at 1000155b
// Signature: undefined * __stdcall Catch@1000155b(void)
// Entry: 1000155b
// Body: 1000155b - 10001560
// ============================================================================
undefined * Catch_1000155b(void)

{
  return &DAT_10001561;
}


// ============================================================================
// Function: FUN_10001564 at 10001564
// Signature: int __stdcall FUN_10001564(void)
// Entry: 10001564
// Body: 10001564 - 100015da
// ============================================================================
int FUN_10001564(void)

{
  int iVar1;
  int unaff_EBP;
  undefined4 *unaff_ESI;
  
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  if (unaff_ESI != (undefined4 *)0x0) {
    unaff_ESI[8] = *(undefined4 *)(unaff_EBP + 8);
    iVar1 = (**(code **)*unaff_ESI)();
    if (iVar1 != 0) {
      *unaff_ESI = &PTR_AudioHolder_QueryInterfaceFromMap_10008654;
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
// Function: AudioHolderClassFactory_AddRef at 10001610
// Signature: LONG __stdcall AudioHolderClassFactory_AddRef(int param_1)
// Entry: 10001610
// Body: 10001610 - 1000164d
// ============================================================================
LONG AudioHolderClassFactory_AddRef(int param_1)

{
  LONG LVar1;
  
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x24));
  LVar1 = InterlockedIncrement((LONG *)(param_1 + 4));
  if (*(LONG *)(param_1 + 4) == 2) {
    InterlockedIncrement(&DAT_1000b47c);
  }
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x24));
  return LVar1;
}


// ============================================================================
// Function: AudioHolderClassFactory_Release at 10001650
// Signature: int __stdcall AudioHolderClassFactory_Release(undefined4 * param_1)
// Entry: 10001650
// Body: 10001650 - 100016ac
// ============================================================================
int AudioHolderClassFactory_Release(undefined4 *param_1)

{
  int iVar1;
  
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  InterlockedDecrement(param_1 + 1);
  iVar1 = param_1[1];
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  if (iVar1 == 0) {
    if (param_1 != (undefined4 *)0x0) {
      AudioHolderClassFactory_Destroy(param_1);
      operator_delete(param_1);
      return 0;
    }
  }
  else if (iVar1 == 1) {
    InterlockedDecrement(&DAT_1000b47c);
  }
  return iVar1;
}


// ============================================================================
// Function: AudioHolderClassFactory_Destroy at 100016b0
// Signature: undefined __fastcall AudioHolderClassFactory_Destroy(undefined4 * param_1)
// Entry: 100016b0
// Body: 100016b0 - 100016df
// ============================================================================
void __fastcall AudioHolderClassFactory_Destroy(undefined4 *param_1)

{
  *param_1 = &PTR_AudioHolder_QueryInterfaceFromMap_10008654;
  param_1[1] = 1;
  DeleteCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  DeleteCriticalSection
            ((LPCRITICAL_SECTION)((-(uint)(param_1 != (undefined4 *)0x0) & (uint)(param_1 + 1)) + 4)
            );
  return;
}


// ============================================================================
// Function: AudioHolder_QueryInterfaceFromMap at 100016e0
// Signature: int __stdcall AudioHolder_QueryInterfaceFromMap(int * param_1, int * param_2, undefined4 * param_3)
// Entry: 100016e0
// Body: 100016e0 - 100017d2
// ============================================================================
int AudioHolder_QueryInterfaceFromMap(int *param_1,int *param_2,undefined4 *param_3)

{
  undefined **ppuVar1;
  undefined *puVar2;
  int iVar3;
  undefined **ppuVar4;
  int *piVar5;
  
  ppuVar4 = &PTR_DAT_10008680;
  if (param_3 == (undefined4 *)0x0) {
    return -0x7fffbffd;
  }
  *param_3 = 0;
  puVar2 = DAT_10008688;
  if ((((*param_2 == 0) && (param_2[1] == 0)) && (param_2[2] == 0xc0)) && (param_2[3] == 0x46000000)
     ) {
    iVar3 = DAT_10008684 + (int)param_1;
    (**(code **)(*(int *)(DAT_10008684 + (int)param_1) + 4))(iVar3);
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
// Function: Create_AudioHolderComObject at 100017e0
// Signature: undefined __stdcall Create_AudioHolderComObject(void)
// Entry: 100017e0
// Body: 100017e0 - 1000185c
// ============================================================================
void Create_AudioHolderComObject(void)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_100071eb;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x238);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    AudioHolderContext_Construct(puVar1);
    *puVar1 = &PTR_LAB_100086f8;
    puVar1[1] = &PTR_LAB_100086c8;
    puVar1[2] = &PTR_LAB_100086a8;
    puVar1[3] = &PTR_LAB_10008698;
    InterlockedIncrement(&DAT_1000b47c);
    FUN_10001866();
    return;
  }
  FUN_10001866();
  return;
}


// ============================================================================
// Function: Catch@1000185d at 1000185d
// Signature: undefined * __stdcall Catch@1000185d(void)
// Entry: 1000185d
// Body: 1000185d - 10001862
// ============================================================================
undefined * Catch_1000185d(void)

{
  return &DAT_10001863;
}


// ============================================================================
// Function: FUN_10001866 at 10001866
// Signature: int __stdcall FUN_10001866(void)
// Entry: 10001866
// Body: 10001866 - 100018eb
// ============================================================================
int FUN_10001866(void)

{
  int iVar1;
  int unaff_EBP;
  undefined4 *unaff_ESI;
  
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  if (unaff_ESI != (undefined4 *)0x0) {
    iVar1 = (**(code **)*unaff_ESI)();
    if (iVar1 != 0) {
      *unaff_ESI = &PTR_LAB_100086f8;
      unaff_ESI[1] = &PTR_LAB_100086c8;
      unaff_ESI[2] = &PTR_LAB_100086a8;
      unaff_ESI[3] = &PTR_LAB_10008698;
      unaff_ESI[4] = 1;
      InterlockedDecrement(&DAT_1000b47c);
      FUN_10003850(unaff_ESI);
      operator_delete(unaff_ESI);
    }
    ExceptionList = *(void **)(unaff_EBP + -0xc);
    return iVar1;
  }
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  return *(int *)(unaff_EBP + -0x18);
}


// ============================================================================
// Function: FUN_100018f0 at 100018f0
// Signature: HRESULT __stdcall FUN_100018f0(undefined4 param_1, undefined4 param_2, BSTR param_3, undefined4 * param_4)
// Entry: 100018f0
// Body: 100018f0 - 10001936
// ============================================================================
HRESULT FUN_100018f0(undefined4 param_1,undefined4 param_2,BSTR param_3,undefined4 *param_4)

{
  HRESULT HVar1;
  
  HVar1 = -0x7fffbffd;
  if (param_4 != (undefined4 *)0x0) {
    HVar1 = 0;
    if (DAT_1000b0cc == (int *)0x0) {
      HVar1 = FUN_10003360(&PTR_DAT_1000b0c0,param_3);
    }
    *param_4 = DAT_1000b0cc;
    if (DAT_1000b0cc != (int *)0x0) {
      (**(code **)(*DAT_1000b0cc + 4))(DAT_1000b0cc);
      HVar1 = 0;
    }
  }
  return HVar1;
}


// ============================================================================
// Function: FUN_10001a60 at 10001a60
// Signature: HRESULT __stdcall FUN_10001a60(undefined4 param_1, undefined4 param_2, BSTR param_3, undefined4 * param_4)
// Entry: 10001a60
// Body: 10001a60 - 10001aa6
// ============================================================================
HRESULT FUN_10001a60(undefined4 param_1,undefined4 param_2,BSTR param_3,undefined4 *param_4)

{
  HRESULT HVar1;
  
  HVar1 = -0x7fffbffd;
  if (param_4 != (undefined4 *)0x0) {
    HVar1 = 0;
    if (DAT_1000b0ec == (int *)0x0) {
      HVar1 = FUN_10003360(&PTR_DAT_1000b0e0,param_3);
    }
    *param_4 = DAT_1000b0ec;
    if (DAT_1000b0ec != (int *)0x0) {
      (**(code **)(*DAT_1000b0ec + 4))(DAT_1000b0ec);
      HVar1 = 0;
    }
  }
  return HVar1;
}


// ============================================================================
// Function: FUN_10001be0 at 10001be0
// Signature: HRESULT __stdcall FUN_10001be0(undefined4 param_1, undefined4 param_2, BSTR param_3, undefined4 * param_4)
// Entry: 10001be0
// Body: 10001be0 - 10001c26
// ============================================================================
HRESULT FUN_10001be0(undefined4 param_1,undefined4 param_2,BSTR param_3,undefined4 *param_4)

{
  HRESULT HVar1;
  
  HVar1 = -0x7fffbffd;
  if (param_4 != (undefined4 *)0x0) {
    HVar1 = 0;
    if (DAT_1000b10c == (int *)0x0) {
      HVar1 = FUN_10003360(&PTR_DAT_1000b100,param_3);
    }
    *param_4 = DAT_1000b10c;
    if (DAT_1000b10c != (int *)0x0) {
      (**(code **)(*DAT_1000b10c + 4))(DAT_1000b10c);
      HVar1 = 0;
    }
  }
  return HVar1;
}


// ============================================================================
// Function: FUN_10001d50 at 10001d50
// Signature: undefined __stdcall FUN_10001d50(undefined4 param_1)
// Entry: 10001d50
// Body: 10001d50 - 10001de0
// ============================================================================
void FUN_10001d50(undefined4 param_1)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_1000720b;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x244);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    puVar1[1] = 0;
    AudioHolderContext_Construct(puVar1 + 3);
    puVar1[3] = &PTR_LAB_100087dc;
    puVar1[4] = &PTR_LAB_100087ac;
    puVar1[5] = &PTR_LAB_1000878c;
    puVar1[6] = &PTR_LAB_1000877c;
    puVar1[7] = param_1;
    *puVar1 = &PTR_LAB_10008770;
    InterlockedIncrement(&DAT_1000b47c);
    FUN_10001dec();
    return;
  }
  FUN_10001dec();
  return;
}


// ============================================================================
// Function: Catch@10001de1 at 10001de1
// Signature: undefined * __stdcall Catch@10001de1(void)
// Entry: 10001de1
// Body: 10001de1 - 10001de6
// ============================================================================
undefined * Catch_10001de1(void)

{
  return &DAT_10001de7;
}


// ============================================================================
// Function: FUN_10001dec at 10001dec
// Signature: undefined4 * __stdcall FUN_10001dec(void)
// Entry: 10001dec
// Body: 10001dec - 10001e5d
// ============================================================================
undefined4 * FUN_10001dec(void)

{
  undefined4 *puVar1;
  undefined4 *unaff_EBX;
  int unaff_EBP;
  undefined4 *unaff_ESI;
  
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  if (unaff_ESI != unaff_EBX) {
    puVar1 = (undefined4 *)(**(code **)*unaff_ESI)();
    if (puVar1 != unaff_EBX) {
      *unaff_ESI = &PTR_LAB_10008770;
      unaff_ESI[1] = 1;
      InterlockedDecrement(&DAT_1000b47c);
      FUN_10003850(unaff_ESI + 3);
      operator_delete(unaff_ESI);
    }
    ExceptionList = *(void **)(unaff_EBP + -0xc);
    return puVar1;
  }
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  return *(undefined4 **)(unaff_EBP + -0x18);
}


// ============================================================================
// Function: AudioHolderRefCountedState_Construct at 10001e60
// Signature: undefined4 * __fastcall AudioHolderRefCountedState_Construct(undefined4 * param_1)
// Entry: 10001e60
// Body: 10001e60 - 10001e76
// ============================================================================
undefined4 * __fastcall AudioHolderRefCountedState_Construct(undefined4 *param_1)

{
  *param_1 = 0;
  InitializeCriticalSection((LPCRITICAL_SECTION)(param_1 + 1));
  return param_1;
}


// ============================================================================
// Function: FUN_10001ee0 at 10001ee0
// Signature: undefined __fastcall FUN_10001ee0(undefined4 * param_1)
// Entry: 10001ee0
// Body: 10001ee0 - 10001f18
// ============================================================================
void __fastcall FUN_10001ee0(undefined4 *param_1)

{
  *param_1 = &PTR_LAB_100086f8;
  param_1[1] = &PTR_LAB_100086c8;
  param_1[2] = &PTR_LAB_100086a8;
  param_1[3] = &PTR_LAB_10008698;
  param_1[4] = 1;
  InterlockedDecrement(&DAT_1000b47c);
  FUN_10003850(param_1);
  return;
}


// ============================================================================
// Function: FUN_10002040 at 10002040
// Signature: LONG __stdcall FUN_10002040(undefined4 * param_1)
// Entry: 10002040
// Body: 10002040 - 10002070
// ============================================================================
LONG FUN_10002040(undefined4 *param_1)

{
  LONG LVar1;
  
  LVar1 = InterlockedDecrement(param_1 + 1);
  if ((LVar1 == 0) && (param_1 != (undefined4 *)0x0)) {
    FUN_10002080(param_1);
    operator_delete(param_1);
  }
  return LVar1;
}


// ============================================================================
// Function: FUN_10002080 at 10002080
// Signature: undefined __fastcall FUN_10002080(undefined4 * param_1)
// Entry: 10002080
// Body: 10002080 - 100020a4
// ============================================================================
void __fastcall FUN_10002080(undefined4 *param_1)

{
  *param_1 = &PTR_LAB_10008770;
  param_1[1] = 1;
  InterlockedDecrement(&DAT_1000b47c);
  FUN_10003850(param_1 + 3);
  return;
}


// ============================================================================
// Function: FUN_10002480 at 10002480
// Signature: IID * __stdcall FUN_10002480(int param_1, IID * param_2, BYTE * param_3, BYTE * param_4, HKEY param_5, uint param_6)
// Entry: 10002480
// Body: 10002480 - 1000289a
// ============================================================================
IID * FUN_10002480(int param_1,IID *param_2,BYTE *param_3,BYTE *param_4,HKEY param_5,uint param_6)

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
  puStack_c = &LAB_10007250;
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
    FUN_10006e30();
    WideCharToMultiByte(0,0,lpWideCharStr,-1,&stack0xfffffcd0,iVar3 * 2 + 2,(LPCSTR)0x0,(LPBOOL)0x0)
    ;
    local_14 = &stack0xfffffcd0;
  }
  pBVar8 = local_14;
  param_2 = (IID *)0x0;
  local_8 = 0;
  uVar2 = FUN_100028e0(&param_2,(HKEY)0x80000000,(LPCSTR)param_3,(LPSTR)0x0,0,2,
                       (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
  if (uVar2 == 0) {
    iVar3 = lstrlenA((LPCSTR)local_11c);
    RegSetValueExA((HKEY)param_2,(LPCSTR)0x0,0,1,local_11c,iVar3 + 1);
    FUN_10002950(&param_2,s_CLSID_1000b170,pBVar8,(LPCSTR)0x0);
    local_8 = 0xffffffff;
    FUN_100028c0(&param_2);
  }
  else {
    pIVar4 = (IID *)(uVar2 & 0xffff | 0x80070000);
    local_8 = 0xffffffff;
    FUN_100028c0(&param_2);
    param_2 = pIVar4;
    if (pIVar4 != (IID *)0x0) goto LAB_10002628;
  }
  param_2 = (IID *)0x0;
  local_8 = 1;
  uVar2 = FUN_100028e0(&param_2,(HKEY)0x80000000,(LPCSTR)param_4,(LPSTR)0x0,0,2,
                       (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
  if (uVar2 == 0) {
    FUN_100029c0(&param_2,local_11c,(LPCSTR)0x0);
    FUN_10002950(&param_2,s_CLSID_1000b170,pBVar8,(LPCSTR)0x0);
    local_8 = 0xffffffff;
    FUN_100028c0(&param_2);
    param_2 = (IID *)0x0;
  }
  else {
    local_8 = 0xffffffff;
    FUN_100028c0(&param_2);
    param_2 = (IID *)(uVar2 & 0xffff | 0x80070000);
  }
LAB_10002628:
  uVar2 = 0;
  if (param_2 == (IID *)0x0) {
    param_5 = (HKEY)0x0;
    local_8 = 2;
    local_18 = (HKEY)0x0;
    uVar2 = RegOpenKeyExA((HKEY)0x80000000,s_CLSID_1000b170,0,0x2001f,&local_18);
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
        uVar2 = FUN_100028c0(&param_5);
        param_5 = local_18;
      }
      if (uVar2 == 0) {
        iVar3 = lstrlenA((LPCSTR)local_11c);
        RegSetValueExA(param_5,(LPCSTR)0x0,0,1,local_11c,iVar3 + 1);
        local_14 = (BYTE *)0x0;
        local_8 = CONCAT31(local_8._1_3_,3);
        iVar3 = FUN_100028e0(&local_14,param_5,"ProgID",(LPSTR)0x0,0,0xf003f,
                             (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
        if (iVar3 == 0) {
          FUN_100029c0(&local_14,param_3,(LPCSTR)0x0);
        }
        local_8._0_1_ = 2;
        FUN_100028c0(&local_14);
        param_3 = (BYTE *)0x0;
        local_8 = CONCAT31(local_8._1_3_,4);
        iVar3 = FUN_100028e0(&param_3,param_5,"VersionIndependentProgID",(LPSTR)0x0,0,0xf003f,
                             (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
        if (iVar3 == 0) {
          FUN_100029c0(&param_3,param_4,(LPCSTR)0x0);
        }
        local_8 = CONCAT31(local_8._1_3_,2);
        FUN_100028c0(&param_3);
        if ((*(int *)(param_1 + 4) == 0) ||
           (pHVar5 = GetModuleHandleA((LPCSTR)0x0), uVar1 = param_6,
           *(HMODULE *)(param_1 + 4) == pHVar5)) {
          DVar7 = GetShortPathNameA((LPCSTR)local_220,(LPSTR)local_324,0x104);
          if (DVar7 == 0x104) {
            local_8 = 0xffffffff;
            FUN_100028c0(&param_5);
            ExceptionList = local_10;
            return (IID *)0x8007000e;
          }
          if ((DVar7 == 0) || (pBVar8 = local_324, DVar7 == 0x57)) {
            pBVar8 = local_220;
          }
          param_3 = (BYTE *)0x0;
          local_8 = CONCAT31(local_8._1_3_,5);
          iVar3 = FUN_100028e0(&param_3,param_5,"LocalServer32",(LPSTR)0x0,0,0xf003f,
                               (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
          if (iVar3 == 0) {
            FUN_100029c0(&param_3,pBVar8,(LPCSTR)0x0);
          }
          local_8 = CONCAT31(local_8._1_3_,2);
          FUN_100028c0(&param_3);
        }
        else {
          pcVar6 = "AUTPRX32.DLL";
          if ((param_6 & 4) == 0) {
            pcVar6 = (char *)local_220;
          }
          FUN_10002950(&param_5,"InprocServer32",(BYTE *)pcVar6,(LPCSTR)0x0);
          if ((uVar1 & 2) == 0) {
            pBVar8 = (BYTE *)(-(uint)((uVar1 & 1) != 0) & 0x100088c0);
          }
          else {
            pBVar8 = "both";
          }
          if (pBVar8 != (BYTE *)0x0) {
            FUN_10002950(&param_5,"InprocServer32",pBVar8,"ThreadingModel");
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
// Function: FUN_100028a0 at 100028a0
// Signature: undefined __fastcall FUN_100028a0(undefined4 * param_1)
// Entry: 100028a0
// Body: 100028a0 - 100028b7
// ============================================================================
void __fastcall FUN_100028a0(undefined4 *param_1)

{
  if ((HKEY)*param_1 != (HKEY)0x0) {
    RegCloseKey((HKEY)*param_1);
    *param_1 = 0;
  }
  return;
}


// ============================================================================
// Function: FUN_100028c0 at 100028c0
// Signature: LSTATUS __fastcall FUN_100028c0(undefined4 * param_1)
// Entry: 100028c0
// Body: 100028c0 - 100028d9
// ============================================================================
LSTATUS __fastcall FUN_100028c0(undefined4 *param_1)

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
// Function: FUN_100028e0 at 100028e0
// Signature: undefined __thiscall FUN_100028e0(void * this, HKEY param_1, LPCSTR param_2, LPSTR param_3, DWORD param_4, REGSAM param_5, LPSECURITY_ATTRIBUTES param_6, undefined4 * param_7)
// Entry: 100028e0
// Body: 100028e0 - 1000294b
// ============================================================================
void __thiscall
FUN_100028e0(void *this,HKEY param_1,LPCSTR param_2,LPSTR param_3,DWORD param_4,REGSAM param_5,
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
// Function: FUN_10002950 at 10002950
// Signature: LSTATUS __thiscall FUN_10002950(void * this, LPCSTR param_1, BYTE * param_2, LPCSTR param_3)
// Entry: 10002950
// Body: 10002950 - 100029ba
// ============================================================================
LSTATUS __thiscall FUN_10002950(void *this,LPCSTR param_1,BYTE *param_2,LPCSTR param_3)

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
// Function: FUN_100029c0 at 100029c0
// Signature: undefined __thiscall FUN_100029c0(void * this, BYTE * param_1, LPCSTR param_2)
// Entry: 100029c0
// Body: 100029c0 - 100029e8
// ============================================================================
void __thiscall FUN_100029c0(void *this,BYTE *param_1,LPCSTR param_2)

{
  int iVar1;
  
  iVar1 = lstrlenA((LPCSTR)param_1);
  RegSetValueExA(*(HKEY *)this,param_2,0,1,param_1,iVar1 + 1);
  return;
}


// ============================================================================
// Function: FUN_100029f0 at 100029f0
// Signature: undefined4 __stdcall FUN_100029f0(undefined4 param_1, IID * param_2, HKEY param_3, LPCSTR param_4)
// Entry: 100029f0
// Body: 100029f0 - 10002d24
// ============================================================================
undefined4 FUN_100029f0(undefined4 param_1,IID *param_2,HKEY param_3,LPCSTR param_4)

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
  puStack_c = &LAB_10007280;
  local_10 = ExceptionList;
  local_8 = 0;
  local_28 = (HKEY)0x80000000;
  pcVar4 = RegEnumKeyExA_exref;
  ExceptionList = &local_10;
  if ((param_3 != (HKEY)0x0) &&
     (ExceptionList = &local_10, iVar2 = lstrcmpiA((LPCSTR)param_3,&DAT_1000b4ec),
     pcVar4 = RegEnumKeyExA_exref, iVar2 != 0)) {
    param_3 = (HKEY)0x0;
    local_8 = CONCAT31(local_8._1_3_,1);
    local_14 = (HKEY)0x0;
    LVar3 = RegOpenKeyExA((HKEY)0x80000000,(LPCSTR)pHVar1,0,0x2001f,&local_14);
    if (LVar3 == 0) {
      LVar3 = FUN_100028c0(&param_3);
      pcVar4 = RegEnumKeyExA_exref;
      param_3 = local_14;
      if (LVar3 == 0) {
        local_18 = 0x100;
        iVar2 = RegEnumKeyExA(local_14,0,local_128,&local_18,(LPDWORD)0x0,(LPSTR)0x0,(LPDWORD)0x0,
                              &local_24);
        while (iVar2 == 0) {
          iVar2 = FUN_10002d80(local_128);
          if (iVar2 != 0) goto LAB_10002be0;
          local_18 = 0x100;
          iVar2 = RegEnumKeyExA(param_3,0,local_128,&local_18,(LPDWORD)0x0,(LPSTR)0x0,(LPDWORD)0x0,
                                &local_24);
        }
        FUN_100028c0(&param_3);
        RegDeleteKeyA((HKEY)0x80000000,(LPCSTR)pHVar1);
LAB_10002be0:
        local_8 = local_8 & 0xffffff00;
        FUN_100028c0(&param_3);
        goto LAB_10002a7f;
      }
    }
    local_8 = local_8 & 0xffffff00;
    FUN_100028c0(&param_3);
    pcVar4 = RegEnumKeyExA_exref;
  }
LAB_10002a7f:
  pCVar5 = param_4;
  if ((param_4 != (LPCSTR)0x0) && (iVar2 = lstrcmpiA(param_4,&DAT_1000b4ec), iVar2 != 0)) {
    param_3 = (HKEY)0x0;
    local_8 = CONCAT31(local_8._1_3_,2);
    local_14 = (HKEY)0x0;
    LVar3 = RegOpenKeyExA((HKEY)0x80000000,pCVar5,0,0x2001f,&local_14);
    if (LVar3 == 0) {
      LVar3 = FUN_100028c0(&param_3);
      param_3 = local_14;
      if (LVar3 == 0) {
        param_4 = (LPCSTR)0x100;
        iVar2 = (*pcVar4)(local_14,0,local_128,&param_4,0,0,0,&local_24);
        while (iVar2 == 0) {
          iVar2 = FUN_10002d80(local_128);
          if (iVar2 != 0) goto LAB_10002b40;
          param_4 = (LPCSTR)0x100;
          iVar2 = (*pcVar4)(param_3,0,local_128,&param_4,0,0,0,&local_24);
        }
        FUN_100028c0(&param_3);
        RegDeleteKeyA((HKEY)0x80000000,pCVar5);
      }
    }
LAB_10002b40:
    local_8 = local_8 & 0xffffff00;
    FUN_100028c0(&param_3);
  }
  StringFromCLSID(param_2,(LPOLESTR *)&local_1c);
  param_3 = local_1c;
  if (local_1c == (HKEY)0x0) {
    pCVar5 = (LPCSTR)0x0;
  }
  else {
    pCVar5 = &stack0xfffffecc;
    iVar2 = lstrlenW((LPCWSTR)local_1c);
    FUN_10006e30();
    WideCharToMultiByte(0,0,(LPCWSTR)param_3,-1,&stack0xfffffecc,iVar2 * 2 + 2,(LPCSTR)0x0,
                        (LPBOOL)0x0);
  }
  param_3 = (HKEY)0x0;
  LVar3 = RegOpenKeyExA((HKEY)0x80000000,s_CLSID_1000b170,0,0x2001f,&param_3);
  if (LVar3 == 0) {
    LVar3 = RegCloseKey((HKEY)0x80000000);
    pHVar1 = param_3;
    local_28 = param_3;
    if (LVar3 == 0) {
      param_3 = (HKEY)0x0;
      local_8 = CONCAT31(local_8._1_3_,3);
      iVar2 = FUN_10002d30(&param_3,pHVar1,pCVar5,0x2001f);
      if (iVar2 == 0) {
        param_4 = (LPCSTR)0x100;
        iVar2 = RegEnumKeyExA(param_3,0,local_128,(LPDWORD)&param_4,(LPDWORD)0x0,(LPSTR)0x0,
                              (LPDWORD)0x0,&local_24);
        while (iVar2 == 0) {
          iVar2 = FUN_10002d80(local_128);
          if (iVar2 != 0) goto LAB_10002cea;
          param_4 = (LPCSTR)0x100;
          iVar2 = RegEnumKeyExA(param_3,0,local_128,(LPDWORD)&param_4,(LPDWORD)0x0,(LPSTR)0x0,
                                (LPDWORD)0x0,&local_24);
        }
        FUN_100028c0(&param_3);
        RegDeleteKeyA(pHVar1,pCVar5);
      }
LAB_10002cea:
      FUN_100028c0(&param_3);
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
// Function: FUN_10002d30 at 10002d30
// Signature: undefined __thiscall FUN_10002d30(void * this, HKEY param_1, LPCSTR param_2, REGSAM param_3)
// Entry: 10002d30
// Body: 10002d30 - 10002d79
// ============================================================================
void __thiscall FUN_10002d30(void *this,HKEY param_1,LPCSTR param_2,REGSAM param_3)

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
// Function: FUN_10002d80 at 10002d80
// Signature: undefined FUN_10002d80(void)
// Entry: 10002d80
// Body: 10002d80 - 10002ed3
// ============================================================================
/* WARNING: Removing unreachable block (ram,0x10002de1) */
/* WARNING: Removing unreachable block (ram,0x10002df6) */
/* WARNING: Removing unreachable block (ram,0x10002e04) */

LSTATUS __thiscall FUN_10002d80(undefined4 *param_1,LPCSTR param_2)

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
  
  puStack_8 = &LAB_1000729b;
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
      iVar2 = FUN_10002d80(aCStack_10c);
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
// Function: FUN_10002ee0 at 10002ee0
// Signature: int __stdcall FUN_10002ee0(undefined4 param_1, int * param_2, int param_3)
// Entry: 10002ee0
// Body: 10002ee0 - 1000300f
// ============================================================================
int FUN_10002ee0(undefined4 param_1,int *param_2,int param_3)

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
  
  puStack_8 = &LAB_100072b8;
  local_c = ExceptionList;
  local_20 = (int *)0x0;
  local_4 = 0;
  if (param_2 != (int *)0x0) {
    ExceptionList = &local_c;
    HVar3 = CoCreateInstance((IID *)&DAT_10009170,(LPUNKNOWN)0x0,1,(IID *)&DAT_10009180,&local_20);
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
// Function: FUN_10003010 at 10003010
// Signature: undefined __fastcall FUN_10003010(int * param_1)
// Entry: 10003010
// Body: 10003010 - 1000301c
// ============================================================================
void __fastcall FUN_10003010(int *param_1)

{
  int *piVar1;
  
  piVar1 = (int *)*param_1;
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
  }
  return;
}


// ============================================================================
// Function: FUN_10003020 at 10003020
// Signature: HRESULT __stdcall FUN_10003020(int param_1, LPCWSTR param_2)
// Entry: 10003020
// Body: 10003020 - 10003210
// ============================================================================
HRESULT FUN_10003020(int param_1,LPCWSTR param_2)

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
  
  puStack_c = &LAB_100072e0;
  local_10 = ExceptionList;
  local_8 = 1;
  local_14 = (LPCWSTR)0x0;
  local_18 = (ITypeLib *)0x0;
  ExceptionList = &local_10;
  GetModuleFileNameA(*(HMODULE *)(param_1 + 0xc),local_128,0x104);
  if (param_2 != (LPCWSTR)0x0) {
    iVar1 = lstrlenW(param_2);
    FUN_10006e30();
    lpString2 = FUN_10003250(&stack0xfffffdd4,param_2,iVar1 * 2 + 2,0);
    lstrcatA(local_128,lpString2);
  }
  if (&stack0x00000000 == (undefined1 *)0x128) {
    szFile = (LPCOLESTR)0x0;
  }
  else {
    iVar1 = lstrlenA(local_128);
    FUN_10006e30();
    szFile = FUN_10003220((LPWSTR)&stack0xfffffdd4,local_128,iVar1 + 1,0);
  }
  HVar2 = LoadTypeLib(szFile,&local_18);
  if (HVar2 < 0) {
    lpString1 = (LPCSTR)0x0;
    lpsz = local_128;
    if (local_128[0] == '\0') {
LAB_1000311f:
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
      if (lpString1 == (LPCSTR)0x0) goto LAB_1000311f;
    }
    lstrcpyA(lpString1,&DAT_1000b178);
    if (&stack0x00000000 == (undefined1 *)0x128) {
      szFile = (LPCOLESTR)0x0;
    }
    else {
      iVar1 = lstrlenA(local_128);
      FUN_10006e30();
      szFile = FUN_10003220((LPWSTR)&stack0xfffffdd4,local_128,iVar1 + 1,0);
    }
    HVar2 = LoadTypeLib(szFile,&local_18);
    if (HVar2 < 0) goto LAB_10003186;
  }
  local_14 = SysAllocString(szFile);
LAB_10003186:
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
    iVar1 = FUN_10003280(local_220);
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
// Function: FUN_10003220 at 10003220
// Signature: LPWSTR __stdcall FUN_10003220(LPWSTR param_1, LPCSTR param_2, int param_3, UINT param_4)
// Entry: 10003220
// Body: 10003220 - 10003249
// ============================================================================
LPWSTR FUN_10003220(LPWSTR param_1,LPCSTR param_2,int param_3,UINT param_4)

{
  *param_1 = L'\0';
  MultiByteToWideChar(param_4,0,param_2,-1,param_1,param_3);
  return param_1;
}


// ============================================================================
// Function: FUN_10003250 at 10003250
// Signature: LPSTR __stdcall FUN_10003250(LPSTR param_1, LPCWSTR param_2, int param_3, UINT param_4)
// Entry: 10003250
// Body: 10003250 - 1000327b
// ============================================================================
LPSTR FUN_10003250(LPSTR param_1,LPCWSTR param_2,int param_3,UINT param_4)

{
  *param_1 = '\0';
  WideCharToMultiByte(param_4,0,param_2,-1,param_1,param_3,(LPCSTR)0x0,(LPBOOL)0x0);
  return param_1;
}


// ============================================================================
// Function: FUN_10003280 at 10003280
// Signature: int __stdcall FUN_10003280(short * param_1)
// Entry: 10003280
// Body: 10003280 - 100032bb
// ============================================================================
int FUN_10003280(short *param_1)

{
  short sVar1;
  short *psVar2;
  short *psVar3;
  
  sVar1 = *param_1;
  psVar2 = param_1;
  psVar3 = param_1;
  while (sVar1 != 0) {
    sVar1 = *psVar3;
    if (sVar1 != 0) {
      psVar3 = psVar3 + 1;
    }
    if (((sVar1 == 0x5c) || (sVar1 == 0x2f)) || (sVar1 == 0x3a)) {
      psVar2 = psVar3;
    }
    sVar1 = *psVar3;
  }
  return (int)psVar2 - (int)param_1 >> 1;
}


// ============================================================================
// Function: FUN_10003350 at 10003350
// Signature: undefined __fastcall FUN_10003350(undefined4 * param_1)
// Entry: 10003350
// Body: 10003350 - 10003359
// ============================================================================
void __fastcall FUN_10003350(undefined4 *param_1)

{
  SysFreeString((BSTR)*param_1);
  return;
}


// ============================================================================
// Function: FUN_10003360 at 10003360
// Signature: HRESULT __thiscall FUN_10003360(void * this, BSTR param_1)
// Entry: 10003360
// Body: 10003360 - 100035f5
// ============================================================================
/* WARNING: Exceeded maximum restarts with more pending */

HRESULT __thiscall FUN_10003360(void *this,BSTR param_1)

{
  ITypeInfo *pIVar1;
  ITypeInfo *pIVar2;
  HRESULT HVar3;
  BSTR pOVar4;
  int iVar5;
  UINT UVar6;
  ITypeInfo *unaff_EBX;
  UINT index;
  ITypeInfo *This;
  uint uVar7;
  int iVar8;
  FUNCDESC *pFuncDesc;
  ITypeInfo *pIStack_28;
  TYPEATTR *pTStack_24;
  ITypeInfo *pIStack_20;
  HRESULT local_1c;
  TYPEATTR *pTStack_18;
  ITypeInfo *pIStack_14;
  ITypeLib *pIStack_10;
  void *local_c;
  undefined *puStack_8;
  int iStack_4;
  
  iStack_4 = 0xffffffff;
  puStack_8 = &DAT_1000731b;
  local_c = ExceptionList;
  if (*(int *)((int)this + 0xc) == 0) {
    local_1c = -0x7fffbffb;
    ExceptionList = &local_c;
    EnterCriticalSection((LPCRITICAL_SECTION)&DAT_1000b484);
    if ((*(int *)((int)this + 0xc) == 0) &&
       (local_1c = LoadRegTypeLib(*(GUID **)((int)this + 4),*(WORD *)((int)this + 8),
                                  *(WORD *)((int)this + 10),(LCID)param_1,&pIStack_10),
       -1 < local_1c)) {
      pIStack_28 = (ITypeInfo *)0x0;
      iStack_4 = 0;
      local_1c = (*pIStack_10->lpVtbl->GetTypeInfoOfGuid)(pIStack_10,*(GUID **)this,&pIStack_28);
      pIVar1 = pIStack_28;
      if (-1 < local_1c) {
        pIStack_14 = pIStack_28;
        if (pIStack_28 != (ITypeInfo *)0x0) {
          (*pIStack_28->lpVtbl->AddRef)(pIStack_28);
        }
        pIStack_20 = (ITypeInfo *)0x0;
        iStack_4 = CONCAT31(iStack_4._1_3_,2);
        HVar3 = (*pIStack_28->lpVtbl->QueryInterface)(pIStack_28,(IID *)&DAT_1000b180,&pIStack_20);
        This = pIVar1;
        pIVar2 = pIStack_20;
        if (-1 < HVar3) {
          if (unaff_EBX != (ITypeInfo *)0x0) {
            (*unaff_EBX->lpVtbl->AddRef)(unaff_EBX);
          }
          This = unaff_EBX;
          pIVar2 = unaff_EBX;
          if (pIVar1 != (ITypeInfo *)0x0) {
            (*pIVar1->lpVtbl->Release)(pIVar1);
            pIVar2 = unaff_EBX;
          }
        }
        pIStack_20 = pIVar2;
        HVar3 = (*This->lpVtbl->GetTypeAttr)(This,&pTStack_24);
        if (-1 < HVar3) {
          uVar7 = (uint)pTStack_18->cFuncs;
          *(uint *)((int)this + 0x18) = uVar7;
          if (uVar7 == 0) {
            pOVar4 = (BSTR)0x0;
          }
          else {
            param_1 = operator_new(uVar7 * 0xc + 4);
            iStack_4._0_1_ = 3;
            if (param_1 == (BSTR)0x0) {
              pOVar4 = (BSTR)0x0;
            }
            else {
              pOVar4 = param_1 + 2;
              *(uint *)param_1 = uVar7;
              FUN_10006f53(pOVar4,0xc,uVar7,&LAB_10003700);
            }
            iStack_4 = CONCAT31(iStack_4._1_3_,2);
          }
          index = 0;
          *(BSTR *)((int)this + 0x14) = pOVar4;
          if (0 < *(int *)((int)this + 0x18)) {
            iVar8 = 0;
            do {
              HVar3 = (*This->lpVtbl->GetFuncDesc)(This,index,(FUNCDESC **)&pTStack_24);
              if (-1 < HVar3) {
                param_1 = (BSTR)0x0;
                pFuncDesc = (FUNCDESC *)0x0;
                iStack_4 = CONCAT31(iStack_4._1_3_,4);
                iVar5 = (*This->lpVtbl->GetDocumentation)
                                  (This,(pTStack_24->guid).Data1,&param_1,0,0);
                pIVar1 = pIStack_14;
                if (-1 < iVar5) {
                  pIStack_14 = (ITypeInfo *)0x0;
                  *(ITypeInfo **)(iVar8 + *(int *)((int)this + 0x14)) = pIVar1;
                  UVar6 = SysStringLen(*(BSTR *)(iVar8 + *(int *)((int)this + 0x14)));
                  *(UINT *)(iVar8 + 4 + *(int *)((int)this + 0x14)) = UVar6;
                  *(MEMBERID *)(iVar8 + 8 + *(int *)((int)this + 0x14)) = pFuncDesc->memid;
                }
                (*This->lpVtbl->ReleaseFuncDesc)(This,pFuncDesc);
                iStack_4 = CONCAT31(iStack_4._1_3_,2);
                SysFreeString(param_1);
              }
              index = index + 1;
              iVar8 = iVar8 + 0xc;
            } while ((int)index < *(int *)((int)this + 0x18));
          }
          (*This->lpVtbl->ReleaseTypeAttr)(This,pTStack_18);
        }
        *(ITypeInfo **)((int)this + 0xc) = This;
        pIStack_14 = (ITypeInfo *)0x0;
        iStack_4._0_1_ = 1;
        if (pIStack_20 != (ITypeInfo *)0x0) {
          (*pIStack_20->lpVtbl->Release)(pIStack_20);
        }
        iStack_4 = (uint)iStack_4._1_3_ << 8;
      }
      (*pIStack_10->lpVtbl->Release)(pIStack_10);
      iStack_4 = 0xffffffff;
      if (pIStack_28 != (ITypeInfo *)0x0) {
        (*pIStack_28->lpVtbl->Release)(pIStack_28);
      }
    }
    LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_1000b484);
    FUN_10003600(0x1000b468,FUN_100036a0,this);
    ExceptionList = local_c;
    return local_1c;
  }
  return 0;
}


// ============================================================================
// Function: FUN_10003600 at 10003600
// Signature: undefined4 __stdcall FUN_10003600(int param_1, undefined4 param_2, undefined4 param_3)
// Entry: 10003600
// Body: 10003600 - 10003696
// ============================================================================
undefined4 FUN_10003600(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_10007330;
  local_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_10;
  puVar1 = operator_new(0xc);
  if (puVar1 == (undefined4 *)0x0) {
    ExceptionList = local_10;
    return 0x8007000e;
  }
  *puVar1 = param_2;
  puVar1[1] = param_3;
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x1c));
  puVar1[2] = *(undefined4 *)(param_1 + 0x80);
  *(undefined4 **)(param_1 + 0x80) = puVar1;
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x1c));
  ExceptionList = local_10;
  return 0;
}


// ============================================================================
// Function: Catch@1000364e at 1000364e
// Signature: undefined4 __stdcall Catch@1000364e(void)
// Entry: 1000364e
// Body: 1000364e - 10003653
// ============================================================================
undefined4 Catch_1000364e(void)

{
  return 0x10003636;
}


// ============================================================================
// Function: FUN_100036a0 at 100036a0
// Signature: undefined __stdcall FUN_100036a0(int param_1)
// Entry: 100036a0
// Body: 100036a0 - 100036f4
// ============================================================================
void FUN_100036a0(int param_1)

{
  int *piVar1;
  int iVar2;
  
  piVar1 = *(int **)(param_1 + 0xc);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
  }
  iVar2 = *(int *)(param_1 + 0x14);
  *(undefined4 *)(param_1 + 0xc) = 0;
  if (iVar2 != 0) {
    FUN_10006e5f(iVar2,0xc,*(int *)(iVar2 + -4),FUN_10003350);
    operator_delete((void *)(iVar2 + -4));
    *(undefined4 *)(param_1 + 0x14) = 0;
    return;
  }
  *(undefined4 *)(param_1 + 0x14) = 0;
  return;
}


// ============================================================================
// Function: AudioHolderContext_Construct at 10003770
// Signature: undefined4 * __fastcall AudioHolderContext_Construct(undefined4 * param_1)
// Entry: 10003770
// Body: 10003770 - 1000383c
// ============================================================================
undefined4 * __fastcall AudioHolderContext_Construct(undefined4 *param_1)

{
  void *this;
  CWnd *pCVar1;
  int iVar2;
  undefined4 *puVar3;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_10007355;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  *param_1 = &PTR_LAB_10008a28;
  param_1[1] = &PTR_LAB_10008a0c;
  param_1[2] = &PTR_LAB_100089f0;
  param_1[4] = 0;
  InitializeCriticalSection((LPCRITICAL_SECTION)(param_1 + 5));
  *param_1 = &PTR_LAB_10008978;
  param_1[1] = &PTR_LAB_10008948;
  param_1[2] = &PTR_LAB_10008928;
  param_1[3] = &PTR_LAB_10008918;
  param_1[0xb] = 0;
  param_1[0xd] = 0;
  param_1[0xc] = 0;
  param_1[0xe] = 0;
  puVar3 = param_1 + 0xf;
  for (iVar2 = 0x78; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  uStack_4 = 0;
  param_1[0x87] = 0;
  param_1[0x88] = 0;
  this = operator_new(0x94);
  uStack_4 = CONCAT31(uStack_4._1_3_,1);
  if (this == (void *)0x0) {
    pCVar1 = (CWnd *)0x0;
  }
  else {
    pCVar1 = FUN_10006530(this,param_1);
  }
  param_1[0x8a] = pCVar1;
  param_1[0x8c] = 0;
  ExceptionList = pvStack_c;
  return param_1;
}

// clang-format on
