/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Notes: raw Ghidra decompiler output with only file-level framing added.
*/
#include "../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10006318 at 10006318
// Signature: AFX_MODULE_STATE * __thiscall FUN_10006318(void * this, byte param_1)
// Entry: 10006318
// Body: 10006318 - 10006332
// ============================================================================
AFX_MODULE_STATE * __thiscall FUN_10006318(void *this,byte param_1)

{
  AFX_MODULE_STATE::~AFX_MODULE_STATE(this);
  if ((param_1 & 1) != 0) {
    CNoTrackObject::operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: ~AFX_MODULE_STATE at 10006333
// Signature: void __thiscall ~AFX_MODULE_STATE(AFX_MODULE_STATE * this)
// Entry: 10006333
// Body: 10006333 - 10006337
// ============================================================================
void __thiscall AFX_MODULE_STATE::~AFX_MODULE_STATE(AFX_MODULE_STATE *this)

{
                    /* WARNING: Could not recover jumptable at 0x100068a6. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~AFX_MODULE_STATE(this);
  return;
}


// ============================================================================
// Function: FUN_10006338 at 10006338
// Signature: undefined __stdcall FUN_10006338(void)
// Entry: 10006338
// Body: 10006338 - 10006343
// ============================================================================
void FUN_10006338(void)

{
  FUN_10006640((_onexit_t)&LAB_10006344);
  return;
}


// ============================================================================
// Function: FUN_1000634e at 1000634e
// Signature: undefined __stdcall FUN_1000634e(HWND__ * param_1, uint param_2, uint param_3, long param_4)
// Entry: 1000634e
// Body: 1000634e - 10006392
// ============================================================================
/* WARNING: Function: _EH_prolog replaced with injection: EH_prolog */

void FUN_1000634e(HWND__ *param_1,uint param_2,uint param_3,long param_4)

{
  undefined4 local_18;
  int local_14;
  void *local_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &DAT_10006e9c;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  AFX_MAINTAIN_STATE2::AFX_MAINTAIN_STATE2
            ((AFX_MAINTAIN_STATE2 *)&local_18,(AFX_MODULE_STATE *)&DAT_1000a4b0);
  local_8 = 0;
  AfxWndProc(param_1,param_2,param_3,param_4);
  *(undefined4 *)(local_14 + 4) = local_18;
  ExceptionList = local_10;
  return;
}


// ============================================================================
// Function: FUN_10006393 at 10006393
// Signature: undefined4 * __stdcall FUN_10006393(void)
// Entry: 10006393
// Body: 10006393 - 10006398
// ============================================================================
undefined4 * FUN_10006393(void)

{
  return &DAT_1000a4b0;
}


// ============================================================================
// Function: FUN_10006399 at 10006399
// Signature: undefined4 __stdcall FUN_10006399(HINSTANCE__ * param_1, int param_2)
// Entry: 10006399
// Body: 10006399 - 100064bf
// ============================================================================
int FUN_10006399(HINSTANCE__ *param_1,int param_2)

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
      pAVar5 = AfxSetModuleState((AFX_MODULE_STATE *)&DAT_1000a4b0);
      p_Var3 = AfxGetThreadState();
      *(AFX_MODULE_STATE **)(p_Var3 + 8) = pAVar5;
      pAVar5 = AfxGetModuleState();
      if (*(int **)(pAVar5 + 4) != (int *)0x0) {
        (**(code **)(**(int **)(pAVar5 + 4) + 0x70))();
      }
      AfxLockTempMaps();
      AfxUnlockTempMaps(-1);
      AfxWinTerm();
      AfxTermExtensionModule((AFX_EXTENSION_MODULE *)&DAT_1000b540,1);
    }
    else if (param_2 == 3) {
      AFX_MAINTAIN_STATE2::AFX_MAINTAIN_STATE2
                ((AFX_MAINTAIN_STATE2 *)&local_c,(AFX_MODULE_STATE *)&DAT_1000a4b0);
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
  iVar4 = AfxWinInit(param_1,(HINSTANCE__ *)0x0,&DAT_1000a490,0);
  if (iVar4 != 0) {
    pAVar5 = AfxGetModuleState();
    piVar2 = *(int **)(pAVar5 + 4);
    if ((piVar2 == (int *)0x0) || (iVar4 = (**(code **)(*piVar2 + 0x58))(), iVar4 != 0)) {
      *(undefined4 *)(p_Var3 + 8) = uVar1;
      AfxInitExtensionModule((AFX_EXTENSION_MODULE *)&DAT_1000b540,param_1);
      this = operator_new(0x40);
      if (this != (CDynLinkLibrary *)0x0) {
        CDynLinkLibrary::CDynLinkLibrary(this,(AFX_EXTENSION_MODULE *)&DAT_1000b540,0);
      }
      param_2 = 1;
      goto LAB_10006425;
    }
    (**(code **)(*piVar2 + 0x70))();
  }
  AfxWinTerm();
LAB_10006425:
  *(undefined4 *)(p_Var3 + 8) = uVar1;
  p_Var3 = AfxGetThreadState();
  AfxSetModuleState(*(AFX_MODULE_STATE **)(p_Var3 + 8));
  return param_2;
}


// ============================================================================
// Function: FUN_100064c0 at 100064c0
// Signature: undefined4 __stdcall FUN_100064c0(undefined4 param_1, int param_2)
// Entry: 100064c0
// Body: 100064c0 - 10006510
// ============================================================================
undefined4 FUN_100064c0(undefined4 param_1,int param_2)

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
    pAVar2 = AfxSetModuleState((AFX_MODULE_STATE *)&DAT_1000a4b0);
    *(AFX_MODULE_STATE **)(p_Var1 + 8) = pAVar2;
  }
  else if (param_2 == 0) {
    p_Var1 = AfxGetThreadState();
    AfxSetModuleState(*(AFX_MODULE_STATE **)(p_Var1 + 8));
  }
  return 1;
}


// ============================================================================
// Function: FUN_10006511 at 10006511
// Signature: undefined __fastcall FUN_10006511(undefined4 * param_1)
// Entry: 10006511
// Body: 10006511 - 10006519
// ============================================================================
void __fastcall FUN_10006511(undefined4 *param_1)

{
  *(undefined4 *)(param_1[1] + 4) = *param_1;
  return;
}


// ============================================================================
// Function: FUN_10006520 at 10006520
// Signature: undefined __stdcall FUN_10006520(undefined4 param_1, undefined4 param_2, int param_3, undefined * param_4)
// Entry: 10006520
// Body: 10006520 - 10006587
// ============================================================================
void FUN_10006520(undefined4 param_1,undefined4 param_2,int param_3,undefined *param_4)

{
  void *local_14;
  undefined *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_10008108;
  puStack_10 = &DAT_1000687c;
  local_14 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_14;
  while( true ) {
    param_3 = param_3 + -1;
    if (param_3 < 0) break;
    (*(code *)param_4)();
  }
  local_8 = 0xffffffff;
  FUN_10006588();
  ExceptionList = local_14;
  return;
}


// ============================================================================
// Function: FUN_10006588 at 10006588
// Signature: undefined __stdcall FUN_10006588(void)
// Entry: 10006588
// Body: 10006588 - 1000659f
// ============================================================================
void FUN_10006588(void)

{
  int unaff_EBP;
  
  if (*(int *)(unaff_EBP + -0x1c) == 0) {
    FUN_100065a0(*(undefined4 *)(unaff_EBP + 8),*(undefined4 *)(unaff_EBP + 0xc),
                 *(int *)(unaff_EBP + 0x10),*(undefined **)(unaff_EBP + 0x14));
  }
  return;
}


// ============================================================================
// Function: FUN_100065a0 at 100065a0
// Signature: undefined __stdcall FUN_100065a0(undefined4 param_1, undefined4 param_2, int param_3, undefined * param_4)
// Entry: 100065a0
// Body: 100065a0 - 100065fd
// ============================================================================
void FUN_100065a0(undefined4 param_1,undefined4 param_2,int param_3,undefined *param_4)

{
  void *local_14;
  undefined *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_10008118;
  puStack_10 = &DAT_1000687c;
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
// Function: FUN_100065fe at 100065fe
// Signature: undefined4 __cdecl FUN_100065fe(undefined4 * param_1)
// Entry: 100065fe
// Body: 100065fe - 10006613
// ============================================================================
undefined4 __cdecl FUN_100065fe(undefined4 *param_1)

{
  if (*(int *)*param_1 != -0x1f928c9d) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  terminate();
}


// ============================================================================
// Function: FUN_10006614 at 10006614
// Signature: undefined __cdecl FUN_10006614(_onexit_t param_1)
// Entry: 10006614
// Body: 10006614 - 1000663f
// ============================================================================
void __cdecl FUN_10006614(_onexit_t param_1)

{
  if (DAT_1000b568 == -1) {
    _onexit(param_1);
    return;
  }
  __dllonexit(param_1,&DAT_1000b568,&DAT_1000b564);
  return;
}


// ============================================================================
// Function: FUN_10006640 at 10006640
// Signature: int __cdecl FUN_10006640(_onexit_t param_1)
// Entry: 10006640
// Body: 10006640 - 10006651
// ============================================================================
int __cdecl FUN_10006640(_onexit_t param_1)

{
  int iVar1;
  
  iVar1 = FUN_10006614(param_1);
  return (iVar1 != 0) - 1;
}


// ============================================================================
// Function: FUN_10006660 at 10006660
// Signature: undefined __stdcall FUN_10006660(void)
// Entry: 10006660
// Body: 10006660 - 1000668e
// ============================================================================
/* WARNING: Unable to track spacebase fully for stack */

void FUN_10006660(void)

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
// Function: FUN_1000668f at 1000668f
// Signature: undefined __stdcall FUN_1000668f(undefined4 param_1, undefined4 param_2, int param_3, undefined * param_4)
// Entry: 1000668f
// Body: 1000668f - 100066f8
// ============================================================================
void FUN_1000668f(undefined4 param_1,undefined4 param_2,int param_3,undefined *param_4)

{
  int local_20;
  void *local_14;
  undefined *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_10008128;
  puStack_10 = &DAT_1000687c;
  local_14 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_14;
  for (local_20 = 0; local_20 < param_3; local_20 = local_20 + 1) {
    (*(code *)param_4)();
  }
  local_8 = 0xffffffff;
  FUN_100066f9();
  ExceptionList = local_14;
  return;
}


// ============================================================================
// Function: FUN_100066f9 at 100066f9
// Signature: undefined __stdcall FUN_100066f9(void)
// Entry: 100066f9
// Body: 100066f9 - 10006710
// ============================================================================
void FUN_100066f9(void)

{
  int unaff_EBP;
  
  if (*(int *)(unaff_EBP + -0x20) == 0) {
    FUN_100065a0(*(undefined4 *)(unaff_EBP + 8),*(undefined4 *)(unaff_EBP + 0xc),
                 *(int *)(unaff_EBP + -0x1c),*(undefined **)(unaff_EBP + 0x18));
  }
  return;
}


// ============================================================================
// Function: FUN_10006711 at 10006711
// Signature: undefined4 __stdcall FUN_10006711(undefined4 param_1, int param_2)
// Entry: 10006711
// Body: 10006711 - 100067bb
// ============================================================================
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_10006711(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  undefined4 *_Memory;
  undefined4 *puVar2;
  
  if (param_2 == 0) {
    if (0 < DAT_1000b55c) {
      DAT_1000b55c = DAT_1000b55c + -1;
      goto LAB_10006727;
    }
LAB_1000674f:
    uVar1 = 0;
  }
  else {
LAB_10006727:
    _DAT_1000b560 = *(undefined4 *)_adjust_fdiv_exref;
    if (param_2 == 1) {
      DAT_1000b568 = malloc(0x80);
      if (DAT_1000b568 == (undefined4 *)0x0) goto LAB_1000674f;
      *DAT_1000b568 = 0;
      DAT_1000b564 = DAT_1000b568;
      initterm(&DAT_1000a000,&DAT_1000a00c);
      DAT_1000b55c = DAT_1000b55c + 1;
    }
    else if ((param_2 == 0) &&
            (_Memory = DAT_1000b568, puVar2 = DAT_1000b564, DAT_1000b568 != (undefined4 *)0x0)) {
      while (puVar2 = puVar2 + -1, _Memory <= puVar2) {
        if ((code *)*puVar2 != (code *)0x0) {
          (*(code *)*puVar2)();
          _Memory = DAT_1000b568;
        }
      }
      free(_Memory);
      DAT_1000b568 = (undefined4 *)0x0;
    }
    uVar1 = 1;
  }
  return uVar1;
}


// ============================================================================
// Function: entry at 100067bc
// Signature: int __stdcall entry(HINSTANCE__ * param_1, int param_2, undefined4 param_3)
// Entry: 100067bc
// Body: 100067bc - 10006858
// ============================================================================
int entry(HINSTANCE__ *param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = param_2;
  iVar2 = DAT_1000b55c;
  if (param_2 != 0) {
    if ((param_2 != 1) && (param_2 != 2)) goto LAB_10006804;
    if ((PTR_FUN_1000a2ac != (undefined *)0x0) &&
       (iVar2 = (*(code *)PTR_FUN_1000a2ac)(param_1,param_2,param_3), iVar2 == 0)) {
      return 0;
    }
    iVar2 = FUN_10006711(param_1,param_2);
  }
  if (iVar2 == 0) {
    return 0;
  }
LAB_10006804:
  iVar2 = FUN_10006399(param_1,param_2);
  if (param_2 == 1) {
    if (iVar2 != 0) {
      return iVar2;
    }
    FUN_10006711(param_1,0);
  }
  if ((param_2 != 0) && (param_2 != 3)) {
    return iVar2;
  }
  iVar3 = FUN_10006711(param_1,param_2);
  param_2 = iVar2;
  if (iVar3 == 0) {
    param_2 = 0;
  }
  if (param_2 != 0) {
    if (PTR_FUN_1000a2ac != (undefined *)0x0) {
      iVar2 = (*(code *)PTR_FUN_1000a2ac)(param_1,iVar1,param_3);
      return iVar2;
    }
    return param_2;
  }
  return 0;
}


// ============================================================================
// Function: FUN_10006859 at 10006859
// Signature: type_info * __thiscall FUN_10006859(void * this, byte param_1)
// Entry: 10006859
// Body: 10006859 - 10006874
// ============================================================================
type_info * __thiscall FUN_10006859(void *this,byte param_1)

{
  type_info::~type_info(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: _EH_prolog at 10006876
// Signature: undefined _EH_prolog(void)
// Entry: 10006876
// Body: 10006876 - 1000687b
// ============================================================================
/* WARNING: This is an inlined function */
/* WARNING: Function: _EH_prolog replaced with injection: EH_prolog */

void _EH_prolog(void)

{
  undefined1 auStack_c [12];
  
  ExceptionList = auStack_c;
                    /* WARNING: Could not recover jumptable at 0x10006876. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  return;
}


// ============================================================================
// Function: terminate at 10006882
// Signature: noreturn void __cdecl terminate(void)
// Entry: 10006882
// Body: 10006882 - 10006887
// ============================================================================
void __cdecl terminate(void)

{
                    /* WARNING: Could not recover jumptable at 0x10006882. Too many branches */
                    /* WARNING: Subroutine does not return */
                    /* WARNING: Treating indirect jump as call */
  terminate();
  return;
}


// ============================================================================
// Function: __dllonexit at 10006888
// Signature: undefined __dllonexit(void)
// Entry: 10006888
// Body: 10006888 - 1000688d
// ============================================================================
void __dllonexit(void)

{
                    /* WARNING: Could not recover jumptable at 0x10006888. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  __dllonexit();
  return;
}


// ============================================================================
// Function: initterm at 1000688e
// Signature: undefined __cdecl initterm(void)
// Entry: 1000688e
// Body: 1000688e - 10006893
// ============================================================================
void __cdecl initterm(void)

{
                    /* WARNING: Could not recover jumptable at 0x1000688e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  initterm();
  return;
}


// ============================================================================
// Function: ~type_info at 10006894
// Signature: void __thiscall ~type_info(type_info * this)
// Entry: 10006894
// Body: 10006894 - 10006899
// ============================================================================
void __thiscall type_info::~type_info(type_info *this)

{
                    /* WARNING: Could not recover jumptable at 0x10006894. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~type_info(this);
  return;
}


// ============================================================================
// Function: AFX_MODULE_STATE at 1000689a
// Signature: undefined __thiscall AFX_MODULE_STATE(AFX_MODULE_STATE * this, int param_1, _func_long_HWND___ptr_uint_uint_long * param_2, ulong param_3)
// Entry: 1000689a
// Body: 1000689a - 1000689f
// ============================================================================
void __thiscall
AFX_MODULE_STATE::AFX_MODULE_STATE
          (AFX_MODULE_STATE *this,int param_1,_func_long_HWND___ptr_uint_uint_long *param_2,
          ulong param_3)

{
                    /* WARNING: Could not recover jumptable at 0x1000689a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AFX_MODULE_STATE(this,param_1,param_2,param_3);
  return;
}


// ============================================================================
// Function: operator_delete at 100068a0
// Signature: void __stdcall operator_delete(void * param_1)
// Entry: 100068a0
// Body: 100068a0 - 100068a5
// ============================================================================
void CNoTrackObject::operator_delete(void *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x100068a0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  operator_delete(param_1);
  return;
}


// ============================================================================
// Function: ~AFX_MODULE_STATE at 100068a6
// Signature: void __thiscall ~AFX_MODULE_STATE(AFX_MODULE_STATE * this)
// Entry: 100068a6
// Body: 100068a6 - 100068ab
// ============================================================================
void __thiscall AFX_MODULE_STATE::~AFX_MODULE_STATE(AFX_MODULE_STATE *this)

{
                    /* WARNING: Could not recover jumptable at 0x100068a6. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~AFX_MODULE_STATE(this);
  return;
}


// ============================================================================
// Function: AfxWndProc at 100068ac
// Signature: long __stdcall AfxWndProc(HWND__ * param_1, uint param_2, uint param_3, long param_4)
// Entry: 100068ac
// Body: 100068ac - 100068b1
// ============================================================================
long AfxWndProc(HWND__ *param_1,uint param_2,uint param_3,long param_4)

{
  long lVar1;
  
                    /* WARNING: Could not recover jumptable at 0x100068ac. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  lVar1 = AfxWndProc(param_1,param_2,param_3,param_4);
  return lVar1;
}


// ============================================================================
// Function: AfxTermThread at 100068b2
// Signature: void __stdcall AfxTermThread(HINSTANCE__ * param_1)
// Entry: 100068b2
// Body: 100068b2 - 100068b7
// ============================================================================
void AfxTermThread(HINSTANCE__ *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x100068b2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AfxTermThread(param_1);
  return;
}


// ============================================================================
// Function: AfxTermExtensionModule at 100068b8
// Signature: void __stdcall AfxTermExtensionModule(AFX_EXTENSION_MODULE * param_1, int param_2)
// Entry: 100068b8
// Body: 100068b8 - 100068bd
// ============================================================================
void AfxTermExtensionModule(AFX_EXTENSION_MODULE *param_1,int param_2)

{
                    /* WARNING: Could not recover jumptable at 0x100068b8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AfxTermExtensionModule(param_1,param_2);
  return;
}


// ============================================================================
// Function: AfxUnlockTempMaps at 100068be
// Signature: int __stdcall AfxUnlockTempMaps(int param_1)
// Entry: 100068be
// Body: 100068be - 100068c3
// ============================================================================
int AfxUnlockTempMaps(int param_1)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x100068be. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = AfxUnlockTempMaps(param_1);
  return iVar1;
}


// ============================================================================
// Function: AfxLockTempMaps at 100068c4
// Signature: void __stdcall AfxLockTempMaps(void)
// Entry: 100068c4
// Body: 100068c4 - 100068c9
// ============================================================================
void AfxLockTempMaps(void)

{
                    /* WARNING: Could not recover jumptable at 0x100068c4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AfxLockTempMaps();
  return;
}


// ============================================================================
// Function: AfxSetModuleState at 100068ca
// Signature: AFX_MODULE_STATE * __stdcall AfxSetModuleState(AFX_MODULE_STATE * param_1)
// Entry: 100068ca
// Body: 100068ca - 100068cf
// ============================================================================
AFX_MODULE_STATE * AfxSetModuleState(AFX_MODULE_STATE *param_1)

{
  AFX_MODULE_STATE *pAVar1;
  
                    /* WARNING: Could not recover jumptable at 0x100068ca. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pAVar1 = AfxSetModuleState(param_1);
  return pAVar1;
}


// ============================================================================
// Function: CDynLinkLibrary at 100068d0
// Signature: undefined __thiscall CDynLinkLibrary(CDynLinkLibrary * this, AFX_EXTENSION_MODULE * param_1, int param_2)
// Entry: 100068d0
// Body: 100068d0 - 100068d5
// ============================================================================
void __thiscall
CDynLinkLibrary::CDynLinkLibrary(CDynLinkLibrary *this,AFX_EXTENSION_MODULE *param_1,int param_2)

{
                    /* WARNING: Could not recover jumptable at 0x100068d0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  CDynLinkLibrary(this,param_1,param_2);
  return;
}


// ============================================================================
// Function: AfxInitExtensionModule at 100068d6
// Signature: int __stdcall AfxInitExtensionModule(AFX_EXTENSION_MODULE * param_1, HINSTANCE__ * param_2)
// Entry: 100068d6
// Body: 100068d6 - 100068db
// ============================================================================
int AfxInitExtensionModule(AFX_EXTENSION_MODULE *param_1,HINSTANCE__ *param_2)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x100068d6. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = AfxInitExtensionModule(param_1,param_2);
  return iVar1;
}


// ============================================================================
// Function: AfxWinTerm at 100068dc
// Signature: void __stdcall AfxWinTerm(void)
// Entry: 100068dc
// Body: 100068dc - 100068e1
// ============================================================================
void AfxWinTerm(void)

{
                    /* WARNING: Could not recover jumptable at 0x100068dc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AfxWinTerm();
  return;
}


// ============================================================================
// Function: AfxWinInit at 100068e2
// Signature: int __stdcall AfxWinInit(HINSTANCE__ * param_1, HINSTANCE__ * param_2, char * param_3, int param_4)
// Entry: 100068e2
// Body: 100068e2 - 100068e7
// ============================================================================
int AfxWinInit(HINSTANCE__ *param_1,HINSTANCE__ *param_2,char *param_3,int param_4)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x100068e2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = AfxWinInit(param_1,param_2,param_3,param_4);
  return iVar1;
}


// ============================================================================
// Function: AfxGetThreadState at 100068e8
// Signature: _AFX_THREAD_STATE * __stdcall AfxGetThreadState(void)
// Entry: 100068e8
// Body: 100068e8 - 100068ed
// ============================================================================
_AFX_THREAD_STATE * AfxGetThreadState(void)

{
  _AFX_THREAD_STATE *p_Var1;
  
                    /* WARNING: Could not recover jumptable at 0x100068e8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  p_Var1 = AfxGetThreadState();
  return p_Var1;
}


// ============================================================================
// Function: AfxCoreInitModule at 100068ee
// Signature: void __stdcall AfxCoreInitModule(void)
// Entry: 100068ee
// Body: 100068ee - 100068f3
// ============================================================================
void AfxCoreInitModule(void)

{
                    /* WARNING: Could not recover jumptable at 0x100068ee. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AfxCoreInitModule();
  return;
}


// ============================================================================
// Function: Unwind@10006900 at 10006900
// Signature: undefined __stdcall Unwind@10006900(void)
// Entry: 10006900
// Body: 10006900 - 10006907
// ============================================================================
void Unwind_10006900(void)

{
  int unaff_EBP;
  
  CWnd::~CWnd(*(CWnd **)(unaff_EBP + -0x10));
  return;
}


// ============================================================================
// Function: Unwind@10006920 at 10006920
// Signature: undefined __stdcall Unwind@10006920(void)
// Entry: 10006920
// Body: 10006920 - 10006927
// ============================================================================
void Unwind_10006920(void)

{
  int unaff_EBP;
  
  CPropertySheet::~CPropertySheet(*(CPropertySheet **)(unaff_EBP + -0x10));
  return;
}


// ============================================================================
// Function: Unwind@10006928 at 10006928
// Signature: undefined __stdcall Unwind@10006928(void)
// Entry: 10006928
// Body: 10006928 - 10006935
// ============================================================================
void Unwind_10006928(void)

{
  int unaff_EBP;
  
  VideoSettingsPage_Destroy((CPropertyPage *)(*(int *)(unaff_EBP + -0x10) + 0x8c));
  return;
}


// ============================================================================
// Function: Unwind@10006940 at 10006940
// Signature: undefined __stdcall Unwind@10006940(void)
// Entry: 10006940
// Body: 10006940 - 10006947
// ============================================================================
void Unwind_10006940(void)

{
  int unaff_EBP;
  
  CPropertySheet::~CPropertySheet(*(CPropertySheet **)(unaff_EBP + -0x10));
  return;
}


// ============================================================================
// Function: Unwind@10006960 at 10006960
// Signature: undefined __stdcall Unwind@10006960(void)
// Entry: 10006960
// Body: 10006960 - 1000696a
// ============================================================================
void Unwind_10006960(void)

{
  int unaff_EBP;
  
  operator_delete(*(void **)(unaff_EBP + -0x1c));
  return;
}


// ============================================================================
// Function: Unwind@10006980 at 10006980
// Signature: undefined __stdcall Unwind@10006980(void)
// Entry: 10006980
// Body: 10006980 - 1000698a
// ============================================================================
void Unwind_10006980(void)

{
  int unaff_EBP;
  
  operator_delete(*(void **)(unaff_EBP + -0x1c));
  return;
}


// ============================================================================
// Function: Unwind@100069a0 at 100069a0
// Signature: undefined __stdcall Unwind@100069a0(void)
// Entry: 100069a0
// Body: 100069a0 - 100069aa
// ============================================================================
void Unwind_100069a0(void)

{
  int unaff_EBP;
  
  operator_delete(*(void **)(unaff_EBP + -0x1c));
  return;
}


// ============================================================================
// Function: Unwind@100069c0 at 100069c0
// Signature: undefined __stdcall Unwind@100069c0(void)
// Entry: 100069c0
// Body: 100069c0 - 100069c7
// ============================================================================
void Unwind_100069c0(void)

{
  int unaff_EBP;
  
  FUN_10002b10((undefined4 *)(unaff_EBP + 0xc));
  return;
}


// ============================================================================
// Function: Unwind@100069c8 at 100069c8
// Signature: undefined __stdcall Unwind@100069c8(void)
// Entry: 100069c8
// Body: 100069c8 - 100069cf
// ============================================================================
void Unwind_100069c8(void)

{
  int unaff_EBP;
  
  FUN_10002b10((undefined4 *)(unaff_EBP + 0xc));
  return;
}


// ============================================================================
// Function: Unwind@100069d0 at 100069d0
// Signature: undefined __stdcall Unwind@100069d0(void)
// Entry: 100069d0
// Body: 100069d0 - 100069d7
// ============================================================================
void Unwind_100069d0(void)

{
  int unaff_EBP;
  
  FUN_10002b10((undefined4 *)(unaff_EBP + 0x18));
  return;
}

// clang-format on
