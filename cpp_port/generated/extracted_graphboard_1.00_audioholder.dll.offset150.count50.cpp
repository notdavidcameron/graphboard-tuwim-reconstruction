/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Notes: raw Ghidra decompiler output with only file-level framing added.
*/
#include "../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10006c5b at 10006c5b
// Signature: undefined4 * __stdcall FUN_10006c5b(void)
// Entry: 10006c5b
// Body: 10006c5b - 10006c60
// ============================================================================
undefined4 * FUN_10006c5b(void)

{
  return &DAT_1000b4f8;
}


// ============================================================================
// Function: FUN_10006c61 at 10006c61
// Signature: undefined4 __stdcall FUN_10006c61(HINSTANCE__ * param_1, int param_2)
// Entry: 10006c61
// Body: 10006c61 - 10006d87
// ============================================================================
int FUN_10006c61(HINSTANCE__ *param_1,int param_2)

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
      pAVar5 = AfxSetModuleState((AFX_MODULE_STATE *)&DAT_1000b4f8);
      p_Var3 = AfxGetThreadState();
      *(AFX_MODULE_STATE **)(p_Var3 + 8) = pAVar5;
      pAVar5 = AfxGetModuleState();
      if (*(int **)(pAVar5 + 4) != (int *)0x0) {
        (**(code **)(**(int **)(pAVar5 + 4) + 0x70))();
      }
      AfxLockTempMaps();
      AfxUnlockTempMaps(-1);
      AfxWinTerm();
      AfxTermExtensionModule((AFX_EXTENSION_MODULE *)&DAT_1000c588,1);
    }
    else if (param_2 == 3) {
      AFX_MAINTAIN_STATE2::AFX_MAINTAIN_STATE2
                ((AFX_MAINTAIN_STATE2 *)&local_c,(AFX_MODULE_STATE *)&DAT_1000b4f8);
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
  iVar4 = AfxWinInit(param_1,(HINSTANCE__ *)0x0,&DAT_1000b4ec,0);
  if (iVar4 != 0) {
    pAVar5 = AfxGetModuleState();
    piVar2 = *(int **)(pAVar5 + 4);
    if ((piVar2 == (int *)0x0) || (iVar4 = (**(code **)(*piVar2 + 0x58))(), iVar4 != 0)) {
      *(undefined4 *)(p_Var3 + 8) = uVar1;
      AfxInitExtensionModule((AFX_EXTENSION_MODULE *)&DAT_1000c588,param_1);
      this = operator_new(0x40);
      if (this != (CDynLinkLibrary *)0x0) {
        CDynLinkLibrary::CDynLinkLibrary(this,(AFX_EXTENSION_MODULE *)&DAT_1000c588,0);
      }
      param_2 = 1;
      goto LAB_10006ced;
    }
    (**(code **)(*piVar2 + 0x70))();
  }
  AfxWinTerm();
LAB_10006ced:
  *(undefined4 *)(p_Var3 + 8) = uVar1;
  p_Var3 = AfxGetThreadState();
  AfxSetModuleState(*(AFX_MODULE_STATE **)(p_Var3 + 8));
  return param_2;
}


// ============================================================================
// Function: FUN_10006d88 at 10006d88
// Signature: undefined4 __stdcall FUN_10006d88(undefined4 param_1, int param_2)
// Entry: 10006d88
// Body: 10006d88 - 10006dd8
// ============================================================================
undefined4 FUN_10006d88(undefined4 param_1,int param_2)

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
    pAVar2 = AfxSetModuleState((AFX_MODULE_STATE *)&DAT_1000b4f8);
    *(AFX_MODULE_STATE **)(p_Var1 + 8) = pAVar2;
  }
  else if (param_2 == 0) {
    p_Var1 = AfxGetThreadState();
    AfxSetModuleState(*(AFX_MODULE_STATE **)(p_Var1 + 8));
  }
  return 1;
}


// ============================================================================
// Function: FUN_10006dd9 at 10006dd9
// Signature: undefined __fastcall FUN_10006dd9(undefined4 * param_1)
// Entry: 10006dd9
// Body: 10006dd9 - 10006de1
// ============================================================================
void __fastcall FUN_10006dd9(undefined4 *param_1)

{
  *(undefined4 *)(param_1[1] + 4) = *param_1;
  return;
}


// ============================================================================
// Function: FUN_10006de2 at 10006de2
// Signature: undefined __cdecl FUN_10006de2(_onexit_t param_1)
// Entry: 10006de2
// Body: 10006de2 - 10006e0d
// ============================================================================
void __cdecl FUN_10006de2(_onexit_t param_1)

{
  if (DAT_1000c5b0 == -1) {
    _onexit(param_1);
    return;
  }
  __dllonexit(param_1,&DAT_1000c5b0,&DAT_1000c5ac);
  return;
}


// ============================================================================
// Function: FUN_10006e0e at 10006e0e
// Signature: int __cdecl FUN_10006e0e(_onexit_t param_1)
// Entry: 10006e0e
// Body: 10006e0e - 10006e1f
// ============================================================================
int __cdecl FUN_10006e0e(_onexit_t param_1)

{
  int iVar1;
  
  iVar1 = FUN_10006de2(param_1);
  return (iVar1 != 0) - 1;
}


// ============================================================================
// Function: FUN_10006e30 at 10006e30
// Signature: undefined __stdcall FUN_10006e30(void)
// Entry: 10006e30
// Body: 10006e30 - 10006e5e
// ============================================================================
/* WARNING: Unable to track spacebase fully for stack */

void FUN_10006e30(void)

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
// Function: FUN_10006e5f at 10006e5f
// Signature: undefined __stdcall FUN_10006e5f(undefined4 param_1, undefined4 param_2, int param_3, undefined * param_4)
// Entry: 10006e5f
// Body: 10006e5f - 10006ec6
// ============================================================================
void FUN_10006e5f(undefined4 param_1,undefined4 param_2,int param_3,undefined *param_4)

{
  void *local_14;
  undefined *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_10009128;
  puStack_10 = &DAT_10007146;
  local_14 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_14;
  while( true ) {
    param_3 = param_3 + -1;
    if (param_3 < 0) break;
    (*(code *)param_4)();
  }
  local_8 = 0xffffffff;
  FUN_10006ec7();
  ExceptionList = local_14;
  return;
}


// ============================================================================
// Function: FUN_10006ec7 at 10006ec7
// Signature: undefined __stdcall FUN_10006ec7(void)
// Entry: 10006ec7
// Body: 10006ec7 - 10006ede
// ============================================================================
void FUN_10006ec7(void)

{
  int unaff_EBP;
  
  if (*(int *)(unaff_EBP + -0x1c) == 0) {
    FUN_10006edf(*(undefined4 *)(unaff_EBP + 8),*(undefined4 *)(unaff_EBP + 0xc),
                 *(int *)(unaff_EBP + 0x10),*(undefined **)(unaff_EBP + 0x14));
  }
  return;
}


// ============================================================================
// Function: FUN_10006edf at 10006edf
// Signature: undefined __stdcall FUN_10006edf(undefined4 param_1, undefined4 param_2, int param_3, undefined * param_4)
// Entry: 10006edf
// Body: 10006edf - 10006f3c
// ============================================================================
void FUN_10006edf(undefined4 param_1,undefined4 param_2,int param_3,undefined *param_4)

{
  void *local_14;
  undefined *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_10009138;
  puStack_10 = &DAT_10007146;
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
// Function: FUN_10006f3d at 10006f3d
// Signature: undefined4 __cdecl FUN_10006f3d(undefined4 * param_1)
// Entry: 10006f3d
// Body: 10006f3d - 10006f52
// ============================================================================
undefined4 __cdecl FUN_10006f3d(undefined4 *param_1)

{
  if (*(int *)*param_1 != -0x1f928c9d) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  terminate();
}


// ============================================================================
// Function: FUN_10006f53 at 10006f53
// Signature: undefined __stdcall FUN_10006f53(undefined4 param_1, undefined4 param_2, int param_3, undefined * param_4)
// Entry: 10006f53
// Body: 10006f53 - 10006fbc
// ============================================================================
void FUN_10006f53(undefined4 param_1,undefined4 param_2,int param_3,undefined *param_4)

{
  int local_20;
  void *local_14;
  undefined *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_10009148;
  puStack_10 = &DAT_10007146;
  local_14 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_14;
  for (local_20 = 0; local_20 < param_3; local_20 = local_20 + 1) {
    (*(code *)param_4)();
  }
  local_8 = 0xffffffff;
  FUN_10006fbd();
  ExceptionList = local_14;
  return;
}


// ============================================================================
// Function: FUN_10006fbd at 10006fbd
// Signature: undefined __stdcall FUN_10006fbd(void)
// Entry: 10006fbd
// Body: 10006fbd - 10006fd4
// ============================================================================
void FUN_10006fbd(void)

{
  int unaff_EBP;
  
  if (*(int *)(unaff_EBP + -0x20) == 0) {
    FUN_10006edf(*(undefined4 *)(unaff_EBP + 8),*(undefined4 *)(unaff_EBP + 0xc),
                 *(int *)(unaff_EBP + -0x1c),*(undefined **)(unaff_EBP + 0x18));
  }
  return;
}


// ============================================================================
// Function: FUN_10006fd5 at 10006fd5
// Signature: undefined4 __stdcall FUN_10006fd5(undefined4 param_1, int param_2)
// Entry: 10006fd5
// Body: 10006fd5 - 1000707f
// ============================================================================
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_10006fd5(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  undefined4 *_Memory;
  undefined4 *puVar2;
  
  if (param_2 == 0) {
    if (0 < DAT_1000c5a4) {
      DAT_1000c5a4 = DAT_1000c5a4 + -1;
      goto LAB_10006feb;
    }
LAB_10007013:
    uVar1 = 0;
  }
  else {
LAB_10006feb:
    _DAT_1000c5a8 = *(undefined4 *)_adjust_fdiv_exref;
    if (param_2 == 1) {
      DAT_1000c5b0 = malloc(0x80);
      if (DAT_1000c5b0 == (undefined4 *)0x0) goto LAB_10007013;
      *DAT_1000c5b0 = 0;
      DAT_1000c5ac = DAT_1000c5b0;
      initterm(&DAT_1000b000,&DAT_1000b00c);
      DAT_1000c5a4 = DAT_1000c5a4 + 1;
    }
    else if ((param_2 == 0) &&
            (_Memory = DAT_1000c5b0, puVar2 = DAT_1000c5ac, DAT_1000c5b0 != (undefined4 *)0x0)) {
      while (puVar2 = puVar2 + -1, _Memory <= puVar2) {
        if ((code *)*puVar2 != (code *)0x0) {
          (*(code *)*puVar2)();
          _Memory = DAT_1000c5b0;
        }
      }
      free(_Memory);
      DAT_1000c5b0 = (undefined4 *)0x0;
    }
    uVar1 = 1;
  }
  return uVar1;
}


// ============================================================================
// Function: entry at 10007080
// Signature: int __stdcall entry(HINSTANCE__ * param_1, int param_2, undefined4 param_3)
// Entry: 10007080
// Body: 10007080 - 1000711c
// ============================================================================
int entry(HINSTANCE__ *param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = param_2;
  iVar2 = DAT_1000c5a4;
  if (param_2 != 0) {
    if ((param_2 != 1) && (param_2 != 2)) goto LAB_100070c8;
    if ((PTR_FUN_1000b314 != (undefined *)0x0) &&
       (iVar2 = (*(code *)PTR_FUN_1000b314)(param_1,param_2,param_3), iVar2 == 0)) {
      return 0;
    }
    iVar2 = FUN_10006fd5(param_1,param_2);
  }
  if (iVar2 == 0) {
    return 0;
  }
LAB_100070c8:
  iVar2 = FUN_10006c61(param_1,param_2);
  if (param_2 == 1) {
    if (iVar2 != 0) {
      return iVar2;
    }
    FUN_10006fd5(param_1,0);
  }
  if ((param_2 != 0) && (param_2 != 3)) {
    return iVar2;
  }
  iVar3 = FUN_10006fd5(param_1,param_2);
  param_2 = iVar2;
  if (iVar3 == 0) {
    param_2 = 0;
  }
  if (param_2 != 0) {
    if (PTR_FUN_1000b314 != (undefined *)0x0) {
      iVar2 = (*(code *)PTR_FUN_1000b314)(param_1,iVar1,param_3);
      return iVar2;
    }
    return param_2;
  }
  return 0;
}


// ============================================================================
// Function: FUN_1000711d at 1000711d
// Signature: type_info * __thiscall FUN_1000711d(void * this, byte param_1)
// Entry: 1000711d
// Body: 1000711d - 10007138
// ============================================================================
type_info * __thiscall FUN_1000711d(void *this,byte param_1)

{
  type_info::~type_info(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: _EH_prolog at 1000713a
// Signature: undefined _EH_prolog(void)
// Entry: 1000713a
// Body: 1000713a - 1000713f
// ============================================================================
/* WARNING: This is an inlined function */
/* WARNING: Function: _EH_prolog replaced with injection: EH_prolog */

void _EH_prolog(void)

{
  undefined1 auStack_c [12];
  
  ExceptionList = auStack_c;
                    /* WARNING: Could not recover jumptable at 0x1000713a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  return;
}


// ============================================================================
// Function: __dllonexit at 10007140
// Signature: undefined __dllonexit(void)
// Entry: 10007140
// Body: 10007140 - 10007145
// ============================================================================
void __dllonexit(void)

{
                    /* WARNING: Could not recover jumptable at 0x10007140. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  __dllonexit();
  return;
}


// ============================================================================
// Function: terminate at 1000714c
// Signature: noreturn void __cdecl terminate(void)
// Entry: 1000714c
// Body: 1000714c - 10007151
// ============================================================================
void __cdecl terminate(void)

{
                    /* WARNING: Could not recover jumptable at 0x1000714c. Too many branches */
                    /* WARNING: Subroutine does not return */
                    /* WARNING: Treating indirect jump as call */
  terminate();
  return;
}


// ============================================================================
// Function: initterm at 10007152
// Signature: undefined __cdecl initterm(void)
// Entry: 10007152
// Body: 10007152 - 10007157
// ============================================================================
void __cdecl initterm(void)

{
                    /* WARNING: Could not recover jumptable at 0x10007152. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  initterm();
  return;
}


// ============================================================================
// Function: ~type_info at 10007158
// Signature: void __thiscall ~type_info(type_info * this)
// Entry: 10007158
// Body: 10007158 - 1000715d
// ============================================================================
void __thiscall type_info::~type_info(type_info *this)

{
                    /* WARNING: Could not recover jumptable at 0x10007158. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~type_info(this);
  return;
}


// ============================================================================
// Function: AFX_MODULE_STATE at 1000715e
// Signature: undefined __thiscall AFX_MODULE_STATE(AFX_MODULE_STATE * this, int param_1, _func_long_HWND___ptr_uint_uint_long * param_2, ulong param_3)
// Entry: 1000715e
// Body: 1000715e - 10007163
// ============================================================================
void __thiscall
AFX_MODULE_STATE::AFX_MODULE_STATE
          (AFX_MODULE_STATE *this,int param_1,_func_long_HWND___ptr_uint_uint_long *param_2,
          ulong param_3)

{
                    /* WARNING: Could not recover jumptable at 0x1000715e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AFX_MODULE_STATE(this,param_1,param_2,param_3);
  return;
}


// ============================================================================
// Function: operator_delete at 10007164
// Signature: void __stdcall operator_delete(void * param_1)
// Entry: 10007164
// Body: 10007164 - 10007169
// ============================================================================
void CNoTrackObject::operator_delete(void *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x10007164. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  operator_delete(param_1);
  return;
}


// ============================================================================
// Function: ~AFX_MODULE_STATE at 1000716a
// Signature: void __thiscall ~AFX_MODULE_STATE(AFX_MODULE_STATE * this)
// Entry: 1000716a
// Body: 1000716a - 1000716f
// ============================================================================
void __thiscall AFX_MODULE_STATE::~AFX_MODULE_STATE(AFX_MODULE_STATE *this)

{
                    /* WARNING: Could not recover jumptable at 0x1000716a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~AFX_MODULE_STATE(this);
  return;
}


// ============================================================================
// Function: AfxWndProc at 10007170
// Signature: long __stdcall AfxWndProc(HWND__ * param_1, uint param_2, uint param_3, long param_4)
// Entry: 10007170
// Body: 10007170 - 10007175
// ============================================================================
long AfxWndProc(HWND__ *param_1,uint param_2,uint param_3,long param_4)

{
  long lVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10007170. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  lVar1 = AfxWndProc(param_1,param_2,param_3,param_4);
  return lVar1;
}


// ============================================================================
// Function: AfxTermThread at 10007176
// Signature: void __stdcall AfxTermThread(HINSTANCE__ * param_1)
// Entry: 10007176
// Body: 10007176 - 1000717b
// ============================================================================
void AfxTermThread(HINSTANCE__ *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x10007176. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AfxTermThread(param_1);
  return;
}


// ============================================================================
// Function: AfxTermExtensionModule at 1000717c
// Signature: void __stdcall AfxTermExtensionModule(AFX_EXTENSION_MODULE * param_1, int param_2)
// Entry: 1000717c
// Body: 1000717c - 10007181
// ============================================================================
void AfxTermExtensionModule(AFX_EXTENSION_MODULE *param_1,int param_2)

{
                    /* WARNING: Could not recover jumptable at 0x1000717c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AfxTermExtensionModule(param_1,param_2);
  return;
}


// ============================================================================
// Function: AfxUnlockTempMaps at 10007182
// Signature: int __stdcall AfxUnlockTempMaps(int param_1)
// Entry: 10007182
// Body: 10007182 - 10007187
// ============================================================================
int AfxUnlockTempMaps(int param_1)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10007182. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = AfxUnlockTempMaps(param_1);
  return iVar1;
}


// ============================================================================
// Function: AfxLockTempMaps at 10007188
// Signature: void __stdcall AfxLockTempMaps(void)
// Entry: 10007188
// Body: 10007188 - 1000718d
// ============================================================================
void AfxLockTempMaps(void)

{
                    /* WARNING: Could not recover jumptable at 0x10007188. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AfxLockTempMaps();
  return;
}


// ============================================================================
// Function: AfxSetModuleState at 1000718e
// Signature: AFX_MODULE_STATE * __stdcall AfxSetModuleState(AFX_MODULE_STATE * param_1)
// Entry: 1000718e
// Body: 1000718e - 10007193
// ============================================================================
AFX_MODULE_STATE * AfxSetModuleState(AFX_MODULE_STATE *param_1)

{
  AFX_MODULE_STATE *pAVar1;
  
                    /* WARNING: Could not recover jumptable at 0x1000718e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pAVar1 = AfxSetModuleState(param_1);
  return pAVar1;
}


// ============================================================================
// Function: CDynLinkLibrary at 10007194
// Signature: undefined __thiscall CDynLinkLibrary(CDynLinkLibrary * this, AFX_EXTENSION_MODULE * param_1, int param_2)
// Entry: 10007194
// Body: 10007194 - 10007199
// ============================================================================
void __thiscall
CDynLinkLibrary::CDynLinkLibrary(CDynLinkLibrary *this,AFX_EXTENSION_MODULE *param_1,int param_2)

{
                    /* WARNING: Could not recover jumptable at 0x10007194. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  CDynLinkLibrary(this,param_1,param_2);
  return;
}


// ============================================================================
// Function: AfxInitExtensionModule at 1000719a
// Signature: int __stdcall AfxInitExtensionModule(AFX_EXTENSION_MODULE * param_1, HINSTANCE__ * param_2)
// Entry: 1000719a
// Body: 1000719a - 1000719f
// ============================================================================
int AfxInitExtensionModule(AFX_EXTENSION_MODULE *param_1,HINSTANCE__ *param_2)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x1000719a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = AfxInitExtensionModule(param_1,param_2);
  return iVar1;
}


// ============================================================================
// Function: AfxWinTerm at 100071a0
// Signature: void __stdcall AfxWinTerm(void)
// Entry: 100071a0
// Body: 100071a0 - 100071a5
// ============================================================================
void AfxWinTerm(void)

{
                    /* WARNING: Could not recover jumptable at 0x100071a0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AfxWinTerm();
  return;
}


// ============================================================================
// Function: AfxWinInit at 100071a6
// Signature: int __stdcall AfxWinInit(HINSTANCE__ * param_1, HINSTANCE__ * param_2, char * param_3, int param_4)
// Entry: 100071a6
// Body: 100071a6 - 100071ab
// ============================================================================
int AfxWinInit(HINSTANCE__ *param_1,HINSTANCE__ *param_2,char *param_3,int param_4)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x100071a6. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = AfxWinInit(param_1,param_2,param_3,param_4);
  return iVar1;
}


// ============================================================================
// Function: AfxGetThreadState at 100071ac
// Signature: _AFX_THREAD_STATE * __stdcall AfxGetThreadState(void)
// Entry: 100071ac
// Body: 100071ac - 100071b1
// ============================================================================
_AFX_THREAD_STATE * AfxGetThreadState(void)

{
  _AFX_THREAD_STATE *p_Var1;
  
                    /* WARNING: Could not recover jumptable at 0x100071ac. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  p_Var1 = AfxGetThreadState();
  return p_Var1;
}


// ============================================================================
// Function: AfxCoreInitModule at 100071b2
// Signature: void __stdcall AfxCoreInitModule(void)
// Entry: 100071b2
// Body: 100071b2 - 100071b7
// ============================================================================
void AfxCoreInitModule(void)

{
                    /* WARNING: Could not recover jumptable at 0x100071b2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AfxCoreInitModule();
  return;
}


// ============================================================================
// Function: Unwind@100071c0 at 100071c0
// Signature: undefined __stdcall Unwind@100071c0(void)
// Entry: 100071c0
// Body: 100071c0 - 100071ca
// ============================================================================
void Unwind_100071c0(void)

{
  int unaff_EBP;
  
  operator_delete(*(void **)(unaff_EBP + -0x1c));
  return;
}


// ============================================================================
// Function: Unwind@100071e0 at 100071e0
// Signature: undefined __stdcall Unwind@100071e0(void)
// Entry: 100071e0
// Body: 100071e0 - 100071ea
// ============================================================================
void Unwind_100071e0(void)

{
  int unaff_EBP;
  
  operator_delete(*(void **)(unaff_EBP + -0x1c));
  return;
}


// ============================================================================
// Function: Unwind@10007200 at 10007200
// Signature: undefined __stdcall Unwind@10007200(void)
// Entry: 10007200
// Body: 10007200 - 1000720a
// ============================================================================
void Unwind_10007200(void)

{
  int unaff_EBP;
  
  operator_delete(*(void **)(unaff_EBP + -0x1c));
  return;
}


// ============================================================================
// Function: Unwind@10007220 at 10007220
// Signature: undefined __stdcall Unwind@10007220(void)
// Entry: 10007220
// Body: 10007220 - 10007227
// ============================================================================
void Unwind_10007220(void)

{
  int unaff_EBP;
  
  FUN_100028a0((undefined4 *)(unaff_EBP + 0xc));
  return;
}


// ============================================================================
// Function: Unwind@10007228 at 10007228
// Signature: undefined __stdcall Unwind@10007228(void)
// Entry: 10007228
// Body: 10007228 - 1000722f
// ============================================================================
void Unwind_10007228(void)

{
  int unaff_EBP;
  
  FUN_100028a0((undefined4 *)(unaff_EBP + 0xc));
  return;
}


// ============================================================================
// Function: Unwind@10007230 at 10007230
// Signature: undefined __stdcall Unwind@10007230(void)
// Entry: 10007230
// Body: 10007230 - 10007237
// ============================================================================
void Unwind_10007230(void)

{
  int unaff_EBP;
  
  FUN_100028a0((undefined4 *)(unaff_EBP + 0x18));
  return;
}


// ============================================================================
// Function: Unwind@10007238 at 10007238
// Signature: undefined __stdcall Unwind@10007238(void)
// Entry: 10007238
// Body: 10007238 - 1000723f
// ============================================================================
void Unwind_10007238(void)

{
  int unaff_EBP;
  
  FUN_100028a0((undefined4 *)(unaff_EBP + -0x10));
  return;
}


// ============================================================================
// Function: Unwind@10007240 at 10007240
// Signature: undefined __stdcall Unwind@10007240(void)
// Entry: 10007240
// Body: 10007240 - 10007247
// ============================================================================
void Unwind_10007240(void)

{
  int unaff_EBP;
  
  FUN_100028a0((undefined4 *)(unaff_EBP + 0x10));
  return;
}


// ============================================================================
// Function: Unwind@10007248 at 10007248
// Signature: undefined __stdcall Unwind@10007248(void)
// Entry: 10007248
// Body: 10007248 - 1000724f
// ============================================================================
void Unwind_10007248(void)

{
  int unaff_EBP;
  
  FUN_100028a0((undefined4 *)(unaff_EBP + 0x10));
  return;
}


// ============================================================================
// Function: Unwind@10007260 at 10007260
// Signature: undefined __stdcall Unwind@10007260(void)
// Entry: 10007260
// Body: 10007260 - 10007267
// ============================================================================
void Unwind_10007260(void)

{
  int unaff_EBP;
  
  FUN_100028a0((undefined4 *)(unaff_EBP + -0x24));
  return;
}


// ============================================================================
// Function: Unwind@10007268 at 10007268
// Signature: undefined __stdcall Unwind@10007268(void)
// Entry: 10007268
// Body: 10007268 - 1000726f
// ============================================================================
void Unwind_10007268(void)

{
  int unaff_EBP;
  
  FUN_100028a0((undefined4 *)(unaff_EBP + 0x10));
  return;
}


// ============================================================================
// Function: Unwind@10007270 at 10007270
// Signature: undefined __stdcall Unwind@10007270(void)
// Entry: 10007270
// Body: 10007270 - 10007277
// ============================================================================
void Unwind_10007270(void)

{
  int unaff_EBP;
  
  FUN_100028a0((undefined4 *)(unaff_EBP + 0x10));
  return;
}


// ============================================================================
// Function: Unwind@10007278 at 10007278
// Signature: undefined __stdcall Unwind@10007278(void)
// Entry: 10007278
// Body: 10007278 - 1000727f
// ============================================================================
void Unwind_10007278(void)

{
  int unaff_EBP;
  
  FUN_100028a0((undefined4 *)(unaff_EBP + 0x10));
  return;
}


// ============================================================================
// Function: Unwind@10007290 at 10007290
// Signature: undefined __stdcall Unwind@10007290(void)
// Entry: 10007290
// Body: 10007290 - 1000729a
// ============================================================================
void Unwind_10007290(void)

{
  int unaff_EBP;
  
  FUN_100028a0((undefined4 *)(unaff_EBP + -0x124));
  return;
}

// clang-format on
