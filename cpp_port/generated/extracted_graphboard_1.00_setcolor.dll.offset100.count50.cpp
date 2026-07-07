/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Notes: raw Ghidra decompiler output with only file-level framing added.
*/
#include "../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100049db at 100049db
// Signature: undefined4 * __stdcall FUN_100049db(void)
// Entry: 100049db
// Body: 100049db - 100049e0
// ============================================================================
undefined4 * FUN_100049db(void)

{
  return &DAT_10008330;
}


// ============================================================================
// Function: FUN_100049e1 at 100049e1
// Signature: undefined4 __stdcall FUN_100049e1(HINSTANCE__ * param_1, int param_2)
// Entry: 100049e1
// Body: 100049e1 - 10004b07
// ============================================================================
int FUN_100049e1(HINSTANCE__ *param_1,int param_2)

{
  undefined4 uVar1;
  int *piVar2;
  _AFX_THREAD_STATE *p_Var3;
  int iVar4;
  AFX_MODULE_STATE *pAVar5;
  CDynLinkLibrary *this;
  undefined4 local_c;
  int local_8;
  
  if (param_2 != 1) {
    if (param_2 == 0) {
      pAVar5 = AfxSetModuleState((AFX_MODULE_STATE *)&DAT_10008330);
      p_Var3 = AfxGetThreadState();
      *(AFX_MODULE_STATE **)(p_Var3 + 8) = pAVar5;
      pAVar5 = AfxGetModuleState();
      if (*(int **)(pAVar5 + 4) != (int *)0x0) {
        (**(code **)(**(int **)(pAVar5 + 4) + 0x70))();
      }
      AfxLockTempMaps();
      AfxUnlockTempMaps(-1);
      AfxWinTerm();
      AfxTermExtensionModule((AFX_EXTENSION_MODULE *)&DAT_100093c0,1);
    }
    else if (param_2 == 3) {
      AFX_MAINTAIN_STATE2::AFX_MAINTAIN_STATE2
                ((AFX_MAINTAIN_STATE2 *)&local_c,(AFX_MODULE_STATE *)&DAT_10008330);
      AfxLockTempMaps();
      AfxUnlockTempMaps(-1);
      AfxTermThread(param_1);
      *(undefined4 *)(local_8 + 4) = local_c;
    }
    return 1;
  }
  param_2 = 0;
  AfxCoreInitModule();
  p_Var3 = AfxGetThreadState();
  uVar1 = *(undefined4 *)(p_Var3 + 8);
  iVar4 = AfxWinInit(param_1,(HINSTANCE__ *)0x0,&DAT_1000831c,0);
  if (iVar4 != 0) {
    pAVar5 = AfxGetModuleState();
    piVar2 = *(int **)(pAVar5 + 4);
    if ((piVar2 == (int *)0x0) || (iVar4 = (**(code **)(*piVar2 + 0x58))(), iVar4 != 0)) {
      *(undefined4 *)(p_Var3 + 8) = uVar1;
      AfxInitExtensionModule((AFX_EXTENSION_MODULE *)&DAT_100093c0,param_1);
      this = operator_new(0x40);
      if (this != (CDynLinkLibrary *)0x0) {
        CDynLinkLibrary::CDynLinkLibrary(this,(AFX_EXTENSION_MODULE *)&DAT_100093c0,0);
      }
      param_2 = 1;
      goto LAB_10004a6d;
    }
    (**(code **)(*piVar2 + 0x70))();
  }
  AfxWinTerm();
LAB_10004a6d:
  *(undefined4 *)(p_Var3 + 8) = uVar1;
  p_Var3 = AfxGetThreadState();
  AfxSetModuleState(*(AFX_MODULE_STATE **)(p_Var3 + 8));
  return param_2;
}


// ============================================================================
// Function: FUN_10004b08 at 10004b08
// Signature: undefined4 __stdcall FUN_10004b08(undefined4 param_1, int param_2)
// Entry: 10004b08
// Body: 10004b08 - 10004b58
// ============================================================================
undefined4 FUN_10004b08(undefined4 param_1,int param_2)

{
  HLOCAL hMem;
  _AFX_THREAD_STATE *p_Var1;
  AFX_MODULE_STATE *pAVar2;
  
  if (param_2 == 1) {
    hMem = LocalAlloc(0,0x2000);
    if (hMem == (HLOCAL)0x0) {
      return 0;
    }
    LocalFree(hMem);
    p_Var1 = AfxGetThreadState();
    pAVar2 = AfxSetModuleState((AFX_MODULE_STATE *)&DAT_10008330);
    *(AFX_MODULE_STATE **)(p_Var1 + 8) = pAVar2;
  }
  else if (param_2 == 0) {
    p_Var1 = AfxGetThreadState();
    AfxSetModuleState(*(AFX_MODULE_STATE **)(p_Var1 + 8));
  }
  return 1;
}


// ============================================================================
// Function: FUN_10004b59 at 10004b59
// Signature: undefined __fastcall FUN_10004b59(undefined4 * param_1)
// Entry: 10004b59
// Body: 10004b59 - 10004b61
// ============================================================================
void __fastcall FUN_10004b59(undefined4 *param_1)

{
  *(undefined4 *)(param_1[1] + 4) = *param_1;
  return;
}


// ============================================================================
// Function: FUN_10004b70 at 10004b70
// Signature: undefined __cdecl FUN_10004b70(_onexit_t param_1)
// Entry: 10004b70
// Body: 10004b70 - 10004b9b
// ============================================================================
void __cdecl FUN_10004b70(_onexit_t param_1)

{
  if (DAT_100093e8 == -1) {
    _onexit(param_1);
    return;
  }
  __dllonexit(param_1,&DAT_100093e8,&DAT_100093e4);
  return;
}


// ============================================================================
// Function: FUN_10004b9c at 10004b9c
// Signature: int __cdecl FUN_10004b9c(_onexit_t param_1)
// Entry: 10004b9c
// Body: 10004b9c - 10004bad
// ============================================================================
int __cdecl FUN_10004b9c(_onexit_t param_1)

{
  int iVar1;
  
  iVar1 = FUN_10004b70(param_1);
  return (iVar1 != 0) - 1;
}


// ============================================================================
// Function: FUN_10004bc0 at 10004bc0
// Signature: undefined __stdcall FUN_10004bc0(void)
// Entry: 10004bc0
// Body: 10004bc0 - 10004bee
// ============================================================================
/* WARNING: Unable to track spacebase fully for stack */

void FUN_10004bc0(void)

{
  uint in_EAX;
  undefined1 *puVar1;
  undefined4 unaff_retaddr;
  
  puVar1 = &stack0x00000004;
  for (; 0xfff < in_EAX; in_EAX = in_EAX - 0x1000) {
    puVar1 = puVar1 + -0x1000;
  }
  *(undefined4 *)(puVar1 + (-4 - in_EAX)) = unaff_retaddr;
  return;
}


// ============================================================================
// Function: FUN_10004bef at 10004bef
// Signature: undefined __stdcall FUN_10004bef(undefined4 param_1, undefined4 param_2, int param_3, undefined * param_4)
// Entry: 10004bef
// Body: 10004bef - 10004c56
// ============================================================================
void FUN_10004bef(undefined4 param_1,undefined4 param_2,int param_3,undefined *param_4)

{
  void *local_14;
  undefined *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_10006900;
  puStack_10 = &DAT_10004ed6;
  local_14 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_14;
  while( true ) {
    param_3 = param_3 + -1;
    if (param_3 < 0) break;
    (*(code *)param_4)();
  }
  local_8 = 0xffffffff;
  FUN_10004c57();
  ExceptionList = local_14;
  return;
}


// ============================================================================
// Function: FUN_10004c57 at 10004c57
// Signature: undefined __stdcall FUN_10004c57(void)
// Entry: 10004c57
// Body: 10004c57 - 10004c6e
// ============================================================================
void FUN_10004c57(void)

{
  int unaff_EBP;
  
  if (*(int *)(unaff_EBP + -0x1c) == 0) {
    FUN_10004c6f(*(undefined4 *)(unaff_EBP + 8),*(undefined4 *)(unaff_EBP + 0xc),
                 *(int *)(unaff_EBP + 0x10),*(undefined **)(unaff_EBP + 0x14));
  }
  return;
}


// ============================================================================
// Function: FUN_10004c6f at 10004c6f
// Signature: undefined __stdcall FUN_10004c6f(undefined4 param_1, undefined4 param_2, int param_3, undefined * param_4)
// Entry: 10004c6f
// Body: 10004c6f - 10004ccc
// ============================================================================
void FUN_10004c6f(undefined4 param_1,undefined4 param_2,int param_3,undefined *param_4)

{
  void *local_14;
  undefined *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_10006910;
  puStack_10 = &DAT_10004ed6;
  local_14 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_14;
  while( true ) {
    param_3 = param_3 + -1;
    if (param_3 < 0) break;
    (*(code *)param_4)();
  }
  ExceptionList = local_14;
  return;
}


// ============================================================================
// Function: FUN_10004ccd at 10004ccd
// Signature: undefined4 __cdecl FUN_10004ccd(undefined4 * param_1)
// Entry: 10004ccd
// Body: 10004ccd - 10004ce2
// ============================================================================
undefined4 __cdecl FUN_10004ccd(undefined4 *param_1)

{
  if (*(int *)*param_1 != -0x1f928c9d) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  terminate();
}


// ============================================================================
// Function: FUN_10004ce3 at 10004ce3
// Signature: undefined __stdcall FUN_10004ce3(undefined4 param_1, undefined4 param_2, int param_3, undefined * param_4)
// Entry: 10004ce3
// Body: 10004ce3 - 10004d4c
// ============================================================================
void FUN_10004ce3(undefined4 param_1,undefined4 param_2,int param_3,undefined *param_4)

{
  int local_20;
  void *local_14;
  undefined *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_10006920;
  puStack_10 = &DAT_10004ed6;
  local_14 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_14;
  for (local_20 = 0; local_20 < param_3; local_20 = local_20 + 1) {
    (*(code *)param_4)();
  }
  local_8 = 0xffffffff;
  FUN_10004d4d();
  ExceptionList = local_14;
  return;
}


// ============================================================================
// Function: FUN_10004d4d at 10004d4d
// Signature: undefined __stdcall FUN_10004d4d(void)
// Entry: 10004d4d
// Body: 10004d4d - 10004d64
// ============================================================================
void FUN_10004d4d(void)

{
  int unaff_EBP;
  
  if (*(int *)(unaff_EBP + -0x20) == 0) {
    FUN_10004c6f(*(undefined4 *)(unaff_EBP + 8),*(undefined4 *)(unaff_EBP + 0xc),
                 *(int *)(unaff_EBP + -0x1c),*(undefined **)(unaff_EBP + 0x18));
  }
  return;
}


// ============================================================================
// Function: FUN_10004d65 at 10004d65
// Signature: undefined4 __stdcall FUN_10004d65(undefined4 param_1, int param_2)
// Entry: 10004d65
// Body: 10004d65 - 10004e0f
// ============================================================================
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_10004d65(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  undefined4 *_Memory;
  undefined4 *puVar2;
  
  if (param_2 == 0) {
    if (0 < DAT_100093dc) {
      DAT_100093dc = DAT_100093dc + -1;
      goto LAB_10004d7b;
    }
LAB_10004da3:
    uVar1 = 0;
  }
  else {
LAB_10004d7b:
    _DAT_100093e0 = *(undefined4 *)_adjust_fdiv_exref;
    if (param_2 == 1) {
      DAT_100093e8 = malloc(0x80);
      if (DAT_100093e8 == (undefined4 *)0x0) goto LAB_10004da3;
      *DAT_100093e8 = 0;
      DAT_100093e4 = DAT_100093e8;
      initterm(&DAT_10008000,&DAT_1000800c);
      DAT_100093dc = DAT_100093dc + 1;
    }
    else if ((param_2 == 0) &&
            (_Memory = DAT_100093e8, puVar2 = DAT_100093e4, DAT_100093e8 != (undefined4 *)0x0)) {
      while (puVar2 = puVar2 + -1, _Memory <= puVar2) {
        if ((code *)*puVar2 != (code *)0x0) {
          (*(code *)*puVar2)();
          _Memory = DAT_100093e8;
        }
      }
      free(_Memory);
      DAT_100093e8 = (undefined4 *)0x0;
    }
    uVar1 = 1;
  }
  return uVar1;
}


// ============================================================================
// Function: entry at 10004e10
// Signature: int __stdcall entry(HINSTANCE__ * param_1, int param_2, undefined4 param_3)
// Entry: 10004e10
// Body: 10004e10 - 10004eac
// ============================================================================
int entry(HINSTANCE__ *param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = param_2;
  iVar2 = DAT_100093dc;
  if (param_2 != 0) {
    if ((param_2 != 1) && (param_2 != 2)) goto LAB_10004e58;
    if ((PTR_FUN_10008144 != (undefined *)0x0) &&
       (iVar2 = (*(code *)PTR_FUN_10008144)(param_1,param_2,param_3), iVar2 == 0)) {
      return 0;
    }
    iVar2 = FUN_10004d65(param_1,param_2);
  }
  if (iVar2 == 0) {
    return 0;
  }
LAB_10004e58:
  iVar2 = FUN_100049e1(param_1,param_2);
  if (param_2 == 1) {
    if (iVar2 != 0) {
      return iVar2;
    }
    FUN_10004d65(param_1,0);
  }
  if ((param_2 != 0) && (param_2 != 3)) {
    return iVar2;
  }
  iVar3 = FUN_10004d65(param_1,param_2);
  param_2 = iVar2;
  if (iVar3 == 0) {
    param_2 = 0;
  }
  if (param_2 != 0) {
    if (PTR_FUN_10008144 != (undefined *)0x0) {
      iVar2 = (*(code *)PTR_FUN_10008144)(param_1,iVar1,param_3);
      return iVar2;
    }
    return param_2;
  }
  return 0;
}


// ============================================================================
// Function: FUN_10004ead at 10004ead
// Signature: type_info * __thiscall FUN_10004ead(void * this, byte param_1)
// Entry: 10004ead
// Body: 10004ead - 10004ec8
// ============================================================================
type_info * __thiscall FUN_10004ead(void *this,byte param_1)

{
  type_info::~type_info(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: _EH_prolog at 10004eca
// Signature: undefined _EH_prolog(void)
// Entry: 10004eca
// Body: 10004eca - 10004ecf
// ============================================================================
/* WARNING: This is an inlined function */
/* WARNING: Function: _EH_prolog replaced with injection: EH_prolog */

void _EH_prolog(void)

{
  undefined1 auStack_c [12];
  
  ExceptionList = auStack_c;
                    /* WARNING: Could not recover jumptable at 0x10004eca. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  return;
}


// ============================================================================
// Function: __dllonexit at 10004ed0
// Signature: undefined __dllonexit(void)
// Entry: 10004ed0
// Body: 10004ed0 - 10004ed5
// ============================================================================
void __dllonexit(void)

{
                    /* WARNING: Could not recover jumptable at 0x10004ed0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  __dllonexit();
  return;
}


// ============================================================================
// Function: terminate at 10004edc
// Signature: noreturn void __cdecl terminate(void)
// Entry: 10004edc
// Body: 10004edc - 10004ee1
// ============================================================================
void __cdecl terminate(void)

{
                    /* WARNING: Could not recover jumptable at 0x10004edc. Too many branches */
                    /* WARNING: Subroutine does not return */
                    /* WARNING: Treating indirect jump as call */
  terminate();
  return;
}


// ============================================================================
// Function: initterm at 10004ee2
// Signature: undefined __cdecl initterm(void)
// Entry: 10004ee2
// Body: 10004ee2 - 10004ee7
// ============================================================================
void __cdecl initterm(void)

{
                    /* WARNING: Could not recover jumptable at 0x10004ee2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  initterm();
  return;
}


// ============================================================================
// Function: ~type_info at 10004ee8
// Signature: void __thiscall ~type_info(type_info * this)
// Entry: 10004ee8
// Body: 10004ee8 - 10004eed
// ============================================================================
void __thiscall type_info::~type_info(type_info *this)

{
                    /* WARNING: Could not recover jumptable at 0x10004ee8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~type_info(this);
  return;
}


// ============================================================================
// Function: AFX_MODULE_STATE at 10004eee
// Signature: undefined __thiscall AFX_MODULE_STATE(AFX_MODULE_STATE * this, int param_1, _func_long_HWND___ptr_uint_uint_long * param_2, ulong param_3)
// Entry: 10004eee
// Body: 10004eee - 10004ef3
// ============================================================================
void __thiscall
AFX_MODULE_STATE::AFX_MODULE_STATE
          (AFX_MODULE_STATE *this,int param_1,_func_long_HWND___ptr_uint_uint_long *param_2,
          ulong param_3)

{
                    /* WARNING: Could not recover jumptable at 0x10004eee. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AFX_MODULE_STATE(this,param_1,param_2,param_3);
  return;
}


// ============================================================================
// Function: operator_delete at 10004ef4
// Signature: void __stdcall operator_delete(void * param_1)
// Entry: 10004ef4
// Body: 10004ef4 - 10004ef9
// ============================================================================
void CNoTrackObject::operator_delete(void *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x10004ef4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  operator_delete(param_1);
  return;
}


// ============================================================================
// Function: ~AFX_MODULE_STATE at 10004efa
// Signature: void __thiscall ~AFX_MODULE_STATE(AFX_MODULE_STATE * this)
// Entry: 10004efa
// Body: 10004efa - 10004eff
// ============================================================================
void __thiscall AFX_MODULE_STATE::~AFX_MODULE_STATE(AFX_MODULE_STATE *this)

{
                    /* WARNING: Could not recover jumptable at 0x10004efa. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~AFX_MODULE_STATE(this);
  return;
}


// ============================================================================
// Function: AfxWndProc at 10004f00
// Signature: long __stdcall AfxWndProc(HWND__ * param_1, uint param_2, uint param_3, long param_4)
// Entry: 10004f00
// Body: 10004f00 - 10004f05
// ============================================================================
long AfxWndProc(HWND__ *param_1,uint param_2,uint param_3,long param_4)

{
  long lVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10004f00. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  lVar1 = AfxWndProc(param_1,param_2,param_3,param_4);
  return lVar1;
}


// ============================================================================
// Function: AfxTermThread at 10004f06
// Signature: void __stdcall AfxTermThread(HINSTANCE__ * param_1)
// Entry: 10004f06
// Body: 10004f06 - 10004f0b
// ============================================================================
void AfxTermThread(HINSTANCE__ *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x10004f06. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AfxTermThread(param_1);
  return;
}


// ============================================================================
// Function: AfxTermExtensionModule at 10004f0c
// Signature: void __stdcall AfxTermExtensionModule(AFX_EXTENSION_MODULE * param_1, int param_2)
// Entry: 10004f0c
// Body: 10004f0c - 10004f11
// ============================================================================
void AfxTermExtensionModule(AFX_EXTENSION_MODULE *param_1,int param_2)

{
                    /* WARNING: Could not recover jumptable at 0x10004f0c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AfxTermExtensionModule(param_1,param_2);
  return;
}


// ============================================================================
// Function: AfxUnlockTempMaps at 10004f12
// Signature: int __stdcall AfxUnlockTempMaps(int param_1)
// Entry: 10004f12
// Body: 10004f12 - 10004f17
// ============================================================================
int AfxUnlockTempMaps(int param_1)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10004f12. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = AfxUnlockTempMaps(param_1);
  return iVar1;
}


// ============================================================================
// Function: AfxLockTempMaps at 10004f18
// Signature: void __stdcall AfxLockTempMaps(void)
// Entry: 10004f18
// Body: 10004f18 - 10004f1d
// ============================================================================
void AfxLockTempMaps(void)

{
                    /* WARNING: Could not recover jumptable at 0x10004f18. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AfxLockTempMaps();
  return;
}


// ============================================================================
// Function: AfxSetModuleState at 10004f1e
// Signature: AFX_MODULE_STATE * __stdcall AfxSetModuleState(AFX_MODULE_STATE * param_1)
// Entry: 10004f1e
// Body: 10004f1e - 10004f23
// ============================================================================
AFX_MODULE_STATE * AfxSetModuleState(AFX_MODULE_STATE *param_1)

{
  AFX_MODULE_STATE *pAVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10004f1e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pAVar1 = AfxSetModuleState(param_1);
  return pAVar1;
}


// ============================================================================
// Function: CDynLinkLibrary at 10004f24
// Signature: undefined __thiscall CDynLinkLibrary(CDynLinkLibrary * this, AFX_EXTENSION_MODULE * param_1, int param_2)
// Entry: 10004f24
// Body: 10004f24 - 10004f29
// ============================================================================
void __thiscall
CDynLinkLibrary::CDynLinkLibrary(CDynLinkLibrary *this,AFX_EXTENSION_MODULE *param_1,int param_2)

{
                    /* WARNING: Could not recover jumptable at 0x10004f24. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  CDynLinkLibrary(this,param_1,param_2);
  return;
}


// ============================================================================
// Function: AfxInitExtensionModule at 10004f2a
// Signature: int __stdcall AfxInitExtensionModule(AFX_EXTENSION_MODULE * param_1, HINSTANCE__ * param_2)
// Entry: 10004f2a
// Body: 10004f2a - 10004f2f
// ============================================================================
int AfxInitExtensionModule(AFX_EXTENSION_MODULE *param_1,HINSTANCE__ *param_2)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10004f2a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = AfxInitExtensionModule(param_1,param_2);
  return iVar1;
}


// ============================================================================
// Function: AfxWinTerm at 10004f30
// Signature: void __stdcall AfxWinTerm(void)
// Entry: 10004f30
// Body: 10004f30 - 10004f35
// ============================================================================
void AfxWinTerm(void)

{
                    /* WARNING: Could not recover jumptable at 0x10004f30. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AfxWinTerm();
  return;
}


// ============================================================================
// Function: AfxWinInit at 10004f36
// Signature: int __stdcall AfxWinInit(HINSTANCE__ * param_1, HINSTANCE__ * param_2, char * param_3, int param_4)
// Entry: 10004f36
// Body: 10004f36 - 10004f3b
// ============================================================================
int AfxWinInit(HINSTANCE__ *param_1,HINSTANCE__ *param_2,char *param_3,int param_4)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10004f36. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = AfxWinInit(param_1,param_2,param_3,param_4);
  return iVar1;
}


// ============================================================================
// Function: AfxGetThreadState at 10004f3c
// Signature: _AFX_THREAD_STATE * __stdcall AfxGetThreadState(void)
// Entry: 10004f3c
// Body: 10004f3c - 10004f41
// ============================================================================
_AFX_THREAD_STATE * AfxGetThreadState(void)

{
  _AFX_THREAD_STATE *p_Var1;
  
                    /* WARNING: Could not recover jumptable at 0x10004f3c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  p_Var1 = AfxGetThreadState();
  return p_Var1;
}


// ============================================================================
// Function: AfxCoreInitModule at 10004f42
// Signature: void __stdcall AfxCoreInitModule(void)
// Entry: 10004f42
// Body: 10004f42 - 10004f47
// ============================================================================
void AfxCoreInitModule(void)

{
                    /* WARNING: Could not recover jumptable at 0x10004f42. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AfxCoreInitModule();
  return;
}


// ============================================================================
// Function: Unwind@10004f50 at 10004f50
// Signature: undefined __stdcall Unwind@10004f50(void)
// Entry: 10004f50
// Body: 10004f50 - 10004f57
// ============================================================================
void Unwind_10004f50(void)

{
  int unaff_EBP;
  
  CDialog::~CDialog(*(CDialog **)(unaff_EBP + -0x10));
  return;
}


// ============================================================================
// Function: Unwind@10004f58 at 10004f58
// Signature: undefined __stdcall Unwind@10004f58(void)
// Entry: 10004f58
// Body: 10004f58 - 10004f62
// ============================================================================
void Unwind_10004f58(void)

{
  int unaff_EBP;
  
  CString::~CString((CString *)(*(int *)(unaff_EBP + -0x10) + 0x78));
  return;
}


// ============================================================================
// Function: Unwind@10004f63 at 10004f63
// Signature: undefined __stdcall Unwind@10004f63(void)
// Entry: 10004f63
// Body: 10004f63 - 10004f6d
// ============================================================================
void Unwind_10004f63(void)

{
  int unaff_EBP;
  
  CString::~CString((CString *)(*(int *)(unaff_EBP + -0x10) + 0x7c));
  return;
}


// ============================================================================
// Function: Unwind@10004f6e at 10004f6e
// Signature: undefined __stdcall Unwind@10004f6e(void)
// Entry: 10004f6e
// Body: 10004f6e - 10004f7b
// ============================================================================
void Unwind_10004f6e(void)

{
  int unaff_EBP;
  
  CString::~CString((CString *)(*(int *)(unaff_EBP + -0x10) + 0x80));
  return;
}


// ============================================================================
// Function: Unwind@10004f7c at 10004f7c
// Signature: undefined __stdcall Unwind@10004f7c(void)
// Entry: 10004f7c
// Body: 10004f7c - 10004f89
// ============================================================================
void Unwind_10004f7c(void)

{
  int unaff_EBP;
  
  CString::~CString((CString *)(*(int *)(unaff_EBP + -0x10) + 0x84));
  return;
}


// ============================================================================
// Function: Unwind@10004f8a at 10004f8a
// Signature: undefined __stdcall Unwind@10004f8a(void)
// Entry: 10004f8a
// Body: 10004f8a - 10004f97
// ============================================================================
void Unwind_10004f8a(void)

{
  int unaff_EBP;
  
  CStatic::~CStatic((CStatic *)(*(int *)(unaff_EBP + -0x10) + 0x8c));
  return;
}


// ============================================================================
// Function: Unwind@10004f98 at 10004f98
// Signature: undefined __stdcall Unwind@10004f98(void)
// Entry: 10004f98
// Body: 10004f98 - 10004fa5
// ============================================================================
void Unwind_10004f98(void)

{
  int unaff_EBP;
  
  CStatic::~CStatic((CStatic *)(*(int *)(unaff_EBP + -0x10) + 0xcc));
  return;
}


// ============================================================================
// Function: Unwind@10004fa6 at 10004fa6
// Signature: undefined __stdcall Unwind@10004fa6(void)
// Entry: 10004fa6
// Body: 10004fa6 - 10004fb3
// ============================================================================
void Unwind_10004fa6(void)

{
  int unaff_EBP;
  
  CStatic::~CStatic((CStatic *)(*(int *)(unaff_EBP + -0x10) + 0x10c));
  return;
}


// ============================================================================
// Function: Unwind@10004fb4 at 10004fb4
// Signature: undefined __stdcall Unwind@10004fb4(void)
// Entry: 10004fb4
// Body: 10004fb4 - 10004fc1
// ============================================================================
void Unwind_10004fb4(void)

{
  int unaff_EBP;
  
  CButton::~CButton((CButton *)(*(int *)(unaff_EBP + -0x10) + 0x14c));
  return;
}


// ============================================================================
// Function: Unwind@10004fc2 at 10004fc2
// Signature: undefined __stdcall Unwind@10004fc2(void)
// Entry: 10004fc2
// Body: 10004fc2 - 10004fcf
// ============================================================================
void Unwind_10004fc2(void)

{
  int unaff_EBP;
  
  CStatic::~CStatic((CStatic *)(*(int *)(unaff_EBP + -0x10) + 0x18c));
  return;
}


// ============================================================================
// Function: Unwind@10004fd0 at 10004fd0
// Signature: undefined __stdcall Unwind@10004fd0(void)
// Entry: 10004fd0
// Body: 10004fd0 - 10004fdd
// ============================================================================
void Unwind_10004fd0(void)

{
  int unaff_EBP;
  
  CStatic::~CStatic((CStatic *)(*(int *)(unaff_EBP + -0x10) + 0x1cc));
  return;
}


// ============================================================================
// Function: Unwind@10004ff0 at 10004ff0
// Signature: undefined __stdcall Unwind@10004ff0(void)
// Entry: 10004ff0
// Body: 10004ff0 - 10004ff7
// ============================================================================
void Unwind_10004ff0(void)

{
  int unaff_EBP;
  
  CDialog::~CDialog(*(CDialog **)(unaff_EBP + -0x10));
  return;
}


// ============================================================================
// Function: Unwind@10004ff8 at 10004ff8
// Signature: undefined __stdcall Unwind@10004ff8(void)
// Entry: 10004ff8
// Body: 10004ff8 - 10005002
// ============================================================================
void Unwind_10004ff8(void)

{
  int unaff_EBP;
  
  CString::~CString((CString *)(*(int *)(unaff_EBP + -0x10) + 0x78));
  return;
}


// ============================================================================
// Function: Unwind@10005003 at 10005003
// Signature: undefined __stdcall Unwind@10005003(void)
// Entry: 10005003
// Body: 10005003 - 1000500d
// ============================================================================
void Unwind_10005003(void)

{
  int unaff_EBP;
  
  CString::~CString((CString *)(*(int *)(unaff_EBP + -0x10) + 0x7c));
  return;
}

// clang-format on
