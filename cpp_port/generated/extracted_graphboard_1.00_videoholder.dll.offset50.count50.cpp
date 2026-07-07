/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Notes: raw Ghidra decompiler output with only file-level framing added.
*/
#include "../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100034e0 at 100034e0
// Signature: undefined __fastcall FUN_100034e0(undefined4 * param_1)
// Entry: 100034e0
// Body: 100034e0 - 100034e9
// ============================================================================
void __fastcall FUN_100034e0(undefined4 *param_1)

{
  SysFreeString((BSTR)*param_1);
  return;
}


// ============================================================================
// Function: FUN_100034f0 at 100034f0
// Signature: int __stdcall FUN_100034f0(short * param_1)
// Entry: 100034f0
// Body: 100034f0 - 1000352b
// ============================================================================
int FUN_100034f0(short *param_1)

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
// Function: FUN_100035c0 at 100035c0
// Signature: HRESULT __thiscall FUN_100035c0(void * this, BSTR param_1)
// Entry: 100035c0
// Body: 100035c0 - 10003855
// ============================================================================
/* WARNING: Exceeded maximum restarts with more pending */

HRESULT __thiscall FUN_100035c0(void *this,BSTR param_1)

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
  puStack_8 = &DAT_10006abb;
  local_c = ExceptionList;
  if (*(int *)((int)this + 0xc) == 0) {
    local_1c = -0x7fffbffb;
    ExceptionList = &local_c;
    EnterCriticalSection((LPCRITICAL_SECTION)&DAT_1000a424);
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
        HVar3 = (*pIStack_28->lpVtbl->QueryInterface)(pIStack_28,(IID *)&DAT_1000a1a0,&pIStack_20);
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
              FUN_1000668f(pOVar4,0xc,uVar7,&LAB_10003970);
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
    LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_1000a424);
    FUN_10003870(0x1000a408,ComTypeInfo_CleanupDispatchMap,this);
    ExceptionList = local_c;
    return local_1c;
  }
  return 0;
}


// ============================================================================
// Function: Com_ReleaseInterfacePointer at 10003860
// Signature: undefined __fastcall Com_ReleaseInterfacePointer(int * param_1)
// Entry: 10003860
// Body: 10003860 - 1000386c
// ============================================================================
void __fastcall Com_ReleaseInterfacePointer(int *param_1)

{
  int *piVar1;
  
  piVar1 = (int *)*param_1;
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
  }
  return;
}


// ============================================================================
// Function: FUN_10003870 at 10003870
// Signature: undefined4 __stdcall FUN_10003870(int param_1, undefined4 param_2, undefined4 param_3)
// Entry: 10003870
// Body: 10003870 - 10003906
// ============================================================================
undefined4 FUN_10003870(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_10006ad0;
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
// Function: Catch@100038be at 100038be
// Signature: undefined4 __stdcall Catch@100038be(void)
// Entry: 100038be
// Body: 100038be - 100038c3
// ============================================================================
undefined4 Catch_100038be(void)

{
  return 0x100038a6;
}


// ============================================================================
// Function: ComTypeInfo_CleanupDispatchMap at 10003910
// Signature: undefined __stdcall ComTypeInfo_CleanupDispatchMap(int param_1)
// Entry: 10003910
// Body: 10003910 - 10003964
// ============================================================================
void ComTypeInfo_CleanupDispatchMap(int param_1)

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
    FUN_10006520(iVar2,0xc,*(int *)(iVar2 + -4),FUN_100034e0);
    operator_delete((void *)(iVar2 + -4));
    *(undefined4 *)(param_1 + 0x14) = 0;
    return;
  }
  *(undefined4 *)(param_1 + 0x14) = 0;
  return;
}


// ============================================================================
// Function: VideoHolderContext_Construct at 100039e0
// Signature: undefined4 * __fastcall VideoHolderContext_Construct(undefined4 * param_1)
// Entry: 100039e0
// Body: 100039e0 - 10003ad1
// ============================================================================
undefined4 * __fastcall VideoHolderContext_Construct(undefined4 *param_1)

{
  void *this;
  CWnd *pCVar1;
  int iVar2;
  undefined4 *puVar3;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_10006b03;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  *param_1 = &PTR_LAB_10007ba8;
  param_1[1] = &PTR_LAB_10007b8c;
  param_1[2] = &PTR_LAB_10007b70;
  param_1[4] = 0;
  InitializeCriticalSection((LPCRITICAL_SECTION)(param_1 + 5));
  uStack_4 = 0;
  CString::CString((CString *)(param_1 + 0x4e));
  *param_1 = &PTR_LAB_10007af8;
  param_1[1] = &PTR_LAB_10007abc;
  param_1[2] = &PTR_LAB_10007a98;
  param_1[3] = &PTR_LAB_10007a88;
  param_1[0xb] = 0;
  param_1[0x11] = 0;
  param_1[0xc] = 0;
  puVar3 = param_1 + 0x14;
  for (iVar2 = 0x32; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  uStack_4._0_1_ = 1;
  param_1[0x46] = 0;
  this = operator_new(0x44);
  uStack_4 = CONCAT31(uStack_4._1_3_,2);
  if (this == (void *)0x0) {
    pCVar1 = (CWnd *)0x0;
  }
  else {
    pCVar1 = FUN_10001020(this,param_1);
  }
  param_1[0x13] = pCVar1;
  param_1[0x12] = 0;
  param_1[0x47] = 0xffffffff;
  param_1[0x4d] = 0xffffffff;
  param_1[0x4c] = 0xffffffff;
  param_1[0x4f] = 0;
  param_1[0x50] = 0;
  param_1[0x51] = 0;
  ExceptionList = pvStack_c;
  return param_1;
}


// ============================================================================
// Function: VideoHolderCriticalSection_Destroy at 10003ae0
// Signature: undefined __fastcall VideoHolderCriticalSection_Destroy(int param_1)
// Entry: 10003ae0
// Body: 10003ae0 - 10003aea
// ============================================================================
void __fastcall VideoHolderCriticalSection_Destroy(int param_1)

{
  DeleteCriticalSection((LPCRITICAL_SECTION)(param_1 + 4));
  return;
}


// ============================================================================
// Function: FUN_10003af0 at 10003af0
// Signature: undefined __fastcall FUN_10003af0(undefined4 * param_1)
// Entry: 10003af0
// Body: 10003af0 - 10003bc8
// ============================================================================
void __fastcall FUN_10003af0(undefined4 *param_1)

{
  CString *this;
  undefined4 *puVar1;
  int iVar2;
  void *pvStack_c;
  undefined1 *puStack_8;
  uint local_4;
  
  puStack_8 = &LAB_10006b45;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  *param_1 = &PTR_LAB_10007af8;
  param_1[1] = &PTR_LAB_10007abc;
  param_1[2] = &PTR_LAB_10007a98;
  param_1[3] = &PTR_LAB_10007a88;
  local_4 = 1;
  SendMessageA((HWND)param_1[0x12],0x10,0,0);
  iVar2 = 0;
  if (0 < (int)param_1[0x46]) {
    puVar1 = param_1 + 0x14;
    do {
      this = *(CString **)*puVar1;
      if (this != (CString *)0x0) {
        CString::~CString(this);
        operator_delete(this);
      }
      operator_delete((void *)*puVar1);
      iVar2 = iVar2 + 1;
      puVar1 = puVar1 + 1;
    } while (iVar2 < (int)param_1[0x46]);
  }
  if ((int *)param_1[0x13] != (int *)0x0) {
    (**(code **)(*(int *)param_1[0x13] + 4))(1);
  }
  local_4 = local_4 & 0xffffff00;
  CString::~CString((CString *)(param_1 + 0x4e));
  if (param_1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = param_1 + 4;
  }
  DeleteCriticalSection((LPCRITICAL_SECTION)(puVar1 + 1));
  ExceptionList = pvStack_c;
  return;
}


// ============================================================================
// Function: VideoHolder_CloseAllVideos at 10003da0
// Signature: undefined4 __stdcall VideoHolder_CloseAllVideos(int param_1)
// Entry: 10003da0
// Body: 10003da0 - 10003e16
// ============================================================================
undefined4 VideoHolder_CloseAllVideos(int param_1)

{
  int *piVar1;
  int iVar2;
  
  piVar1 = (int *)(param_1 + 4);
  *(undefined4 *)(param_1 + 0x140) = 0;
  (**(code **)(*(int *)(param_1 + 4) + 0x24))(piVar1);
  if (*(int *)(param_1 + 0x13c) != 0) {
    SendMessageA(*(HWND *)(param_1 + 0x48),0x804,0,0);
  }
  if (*(int *)(param_1 + 0x144) != 0) {
    (**(code **)(**(int **)(param_1 + 0x2c) + 0x2c))(*(int **)(param_1 + 0x2c));
  }
  iVar2 = 0;
  *(undefined4 *)(param_1 + 0x144) = 0;
  if (0 < *(int *)(param_1 + 0x118)) {
    do {
      (**(code **)(*piVar1 + 0x38))(piVar1,iVar2);
      iVar2 = iVar2 + 1;
    } while (iVar2 < *(int *)(param_1 + 0x118));
  }
  *(undefined4 *)(param_1 + 0x13c) = 0;
  return 0;
}


// ============================================================================
// Function: VideoHolder_MciWindowSubclassProc at 100040d0
// Signature: undefined __stdcall VideoHolder_MciWindowSubclassProc(HWND param_1, UINT param_2, WPARAM param_3, LPARAM param_4)
// Entry: 100040d0
// Body: 100040d0 - 10004117
// ============================================================================
void VideoHolder_MciWindowSubclassProc(HWND param_1,UINT param_2,WPARAM param_3,LPARAM param_4)

{
  if ((param_2 == 0x201) || (param_2 == 0x204)) {
    (**(code **)(*(int *)(DAT_1000a498 + 8) + 0x20))((int *)(DAT_1000a498 + 8),DAT_1000a4a0);
  }
  CallWindowProcA(DAT_1000a49c,param_1,param_2,param_3,param_4);
  return;
}


// ============================================================================
// Function: VideoHolder_StopAndHideActiveVideo at 10004760
// Signature: undefined4 __stdcall VideoHolder_StopAndHideActiveVideo(int param_1)
// Entry: 10004760
// Body: 10004760 - 100047ea
// ============================================================================
undefined4 VideoHolder_StopAndHideActiveVideo(int param_1)

{
  if ((*(HWND *)(param_1 + 0x44) != (HWND)0x0) && (*(int *)(param_1 + 0x138) != 0)) {
    if (*(int *)(*(int *)(param_1 + 0x4c + *(int *)(param_1 + 0x118) * 4) + 0xc) == 0) {
      ShowWindow(*(HWND *)(param_1 + 0x44),0);
    }
    SendMessageA(*(HWND *)(param_1 + 0x44),0x808,0,0);
    if (*(int *)(param_1 + 0x140) != 0) {
      (**(code **)(**(int **)(param_1 + 0x28) + 0x2c))(*(int **)(param_1 + 0x28));
    }
    *(undefined4 *)(param_1 + 0x140) = 0;
    (**(code **)(**(int **)(param_1 + 0x28) + 0xc))(*(int **)(param_1 + 0x28),param_1 + 0x11c,1);
    *(undefined4 *)(param_1 + 0x13c) = 0;
    return 0;
  }
  return 1;
}


// ============================================================================
// Function: VideoHolder_PlayActiveVideo at 100047f0
// Signature: undefined __fastcall VideoHolder_PlayActiveVideo(int param_1)
// Entry: 100047f0
// Body: 100047f0 - 100048c4
// ============================================================================
void __fastcall VideoHolder_PlayActiveVideo(int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x11c);
  if (((-1 < iVar1) && (*(int *)(param_1 + 0x44) != 0)) && (*(int *)(param_1 + 0x13c) != 0)) {
    if (*(int *)(*(int *)(param_1 + 0x50 + iVar1 * 4) + 8) != 0) {
      if (*(int *)(param_1 + 0x144) == 0) {
        (**(code **)(**(int **)(param_1 + 0x2c) + 0x30))(*(int **)(param_1 + 0x2c));
      }
      *(undefined4 *)(param_1 + 0x144) = 1;
      if ((-1 < *(int *)(param_1 + 0x130)) && (-1 < *(int *)(param_1 + 0x134))) {
        SendMessageA(*(HWND *)(param_1 + 0x48),0x807,0,*(int *)(param_1 + 0x130));
        SendMessageA(*(HWND *)(param_1 + 0x48),0x47b,0,*(LPARAM *)(param_1 + 0x134));
        return;
      }
      SendMessageA(*(HWND *)(param_1 + 0x48),0x807,0,0);
      SendMessageA(*(HWND *)(param_1 + 0x48),0x806,0,0);
      return;
    }
    (**(code **)(*(int *)(param_1 + 8) + 0x1c))(param_1 + 8,iVar1);
    (**(code **)(*(int *)(param_1 + 4) + 0x24))(param_1 + 4);
  }
  return;
}


// ============================================================================
// Function: VideoHolder_DeletePropertiesDialog at 10004aa0
// Signature: undefined __stdcall VideoHolder_DeletePropertiesDialog(void * param_1)
// Entry: 10004aa0
// Body: 10004aa0 - 10004aad
// ============================================================================
void VideoHolder_DeletePropertiesDialog(void *param_1)

{
  operator_delete(param_1);
  return;
}


// ============================================================================
// Function: VideoHolderCallback_ClearPlaybackState at 10004e50
// Signature: undefined4 __stdcall VideoHolderCallback_ClearPlaybackState(int param_1, undefined4 param_2)
// Entry: 10004e50
// Body: 10004e50 - 10004e85
// ============================================================================
undefined4 VideoHolderCallback_ClearPlaybackState(int param_1,undefined4 param_2)

{
  int *piVar1;
  
  piVar1 = *(int **)(param_1 + 0x28);
  if ((piVar1 != (int *)0x0) && (*(int *)(param_1 + 0x138) != 0)) {
    (**(code **)(*piVar1 + 0x1c))(piVar1,param_2);
    *(undefined4 *)(param_1 + 0x138) = 0;
  }
  return 0;
}


// ============================================================================
// Function: FUN_10004e90 at 10004e90
// Signature: undefined4 __stdcall FUN_10004e90(int param_1, undefined4 param_2)
// Entry: 10004e90
// Body: 10004e90 - 10004eaf
// ============================================================================
undefined4 FUN_10004e90(int param_1,undefined4 param_2)

{
  int *piVar1;
  
  piVar1 = *(int **)(param_1 + 0x28);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 0x20))(piVar1,param_2);
  }
  return 0;
}


// ============================================================================
// Function: VideoSettingsPage_Create at 10004eb0
// Signature: CPropertyPage * __stdcall VideoSettingsPage_Create(void)
// Entry: 10004eb0
// Body: 10004eb0 - 10004f09
// ============================================================================
CPropertyPage * VideoSettingsPage_Create(void)

{
  CPropertyPage *pCVar1;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10006c0a;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  pCVar1 = operator_new(800);
  local_4 = 0;
  if (pCVar1 != (CPropertyPage *)0x0) {
    pCVar1 = FUN_10004f30(pCVar1);
    ExceptionList = local_c;
    return pCVar1;
  }
  ExceptionList = local_c;
  return (CPropertyPage *)0x0;
}


// ============================================================================
// Function: FUN_10004f30 at 10004f30
// Signature: CPropertyPage * __fastcall FUN_10004f30(CPropertyPage * param_1)
// Entry: 10004f30
// Body: 10004f30 - 10005089
// ============================================================================
CPropertyPage * __fastcall FUN_10004f30(CPropertyPage *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10006cb4;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  CPropertyPage::CPropertyPage(param_1,0xc9,0);
  local_4 = 0;
  CString::CString((CString *)(param_1 + 0x98));
  local_4._0_1_ = 1;
  CWnd::CWnd((CWnd *)(param_1 + 0x9c));
  *(undefined ***)(param_1 + 0x9c) = &PTR_LAB_1000803c;
  local_4._0_1_ = 2;
  CWnd::CWnd((CWnd *)(param_1 + 0xdc));
  *(undefined ***)(param_1 + 0xdc) = &PTR_LAB_10007f78;
  local_4._0_1_ = 3;
  CWnd::CWnd((CWnd *)(param_1 + 0x11c));
  *(undefined ***)(param_1 + 0x11c) = &PTR_LAB_10007eb8;
  local_4._0_1_ = 4;
  CWnd::CWnd((CWnd *)(param_1 + 0x15c));
  *(undefined ***)(param_1 + 0x15c) = &PTR_LAB_10007eb8;
  local_4._0_1_ = 5;
  CWnd::CWnd((CWnd *)(param_1 + 0x19c));
  *(undefined ***)(param_1 + 0x19c) = &PTR_LAB_10007f78;
  local_4._0_1_ = 6;
  CWnd::CWnd((CWnd *)(param_1 + 0x1dc));
  *(undefined ***)(param_1 + 0x1dc) = &PTR_LAB_10007f78;
  local_4._0_1_ = 7;
  CWnd::CWnd((CWnd *)(param_1 + 0x21c));
  *(undefined ***)(param_1 + 0x21c) = &PTR_LAB_10007f78;
  local_4._0_1_ = 8;
  CWnd::CWnd((CWnd *)(param_1 + 0x25c));
  *(undefined ***)(param_1 + 0x25c) = &PTR_LAB_10007f78;
  local_4._0_1_ = 9;
  CWnd::CWnd((CWnd *)(param_1 + 0x29c));
  *(undefined ***)(param_1 + 0x29c) = &PTR_LAB_10007de0;
  local_4 = CONCAT31(local_4._1_3_,10);
  CWnd::CWnd((CWnd *)(param_1 + 0x2dc));
  *(undefined ***)(param_1 + 0x2dc) = &PTR_LAB_10007f78;
  *(undefined ***)param_1 = &PTR_LAB_10007ce8;
  *(undefined4 *)(param_1 + 0x31c) = 0;
  *(undefined4 *)(param_1 + 0x94) = 0;
  ExceptionList = local_c;
  return param_1;
}


// ============================================================================
// Function: VideoSettingsPage_ScalarDeletingDestructor at 10005090
// Signature: CPropertyPage * __thiscall VideoSettingsPage_ScalarDeletingDestructor(void * this, byte param_1)
// Entry: 10005090
// Body: 10005090 - 100050ad
// ============================================================================
CPropertyPage * __thiscall VideoSettingsPage_ScalarDeletingDestructor(void *this,byte param_1)

{
  VideoSettingsPage_Destroy(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: VideoSettingsPage_Destroy at 100050b0
// Signature: undefined __fastcall VideoSettingsPage_Destroy(CPropertyPage * param_1)
// Entry: 100050b0
// Body: 100050b0 - 100051a4
// ============================================================================
void __fastcall VideoSettingsPage_Destroy(CPropertyPage *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  puStack_8 = &LAB_10006d54;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined ***)param_1 = &PTR_LAB_10007ce8;
  local_4 = 10;
  CButton::~CButton((CButton *)(param_1 + 0x2dc));
  local_4._0_1_ = 9;
  CListBox::~CListBox((CListBox *)(param_1 + 0x29c));
  local_4._0_1_ = 8;
  CButton::~CButton((CButton *)(param_1 + 0x25c));
  local_4._0_1_ = 7;
  CButton::~CButton((CButton *)(param_1 + 0x21c));
  local_4._0_1_ = 6;
  CButton::~CButton((CButton *)(param_1 + 0x1dc));
  local_4._0_1_ = 5;
  CButton::~CButton((CButton *)(param_1 + 0x19c));
  local_4._0_1_ = 4;
  CEdit::~CEdit((CEdit *)(param_1 + 0x15c));
  local_4._0_1_ = 3;
  CEdit::~CEdit((CEdit *)(param_1 + 0x11c));
  local_4._0_1_ = 2;
  CButton::~CButton((CButton *)(param_1 + 0xdc));
  local_4._0_1_ = 1;
  CStatic::~CStatic((CStatic *)(param_1 + 0x9c));
  local_4 = (uint)local_4._1_3_ << 8;
  CString::~CString((CString *)(param_1 + 0x98));
  local_4 = 0xffffffff;
  CPropertyPage::~CPropertyPage(param_1);
  ExceptionList = local_c;
  return;
}


// ============================================================================
// Function: VideoSettingsPage_UpdateControls at 100053b0
// Signature: undefined __fastcall VideoSettingsPage_UpdateControls(int param_1)
// Entry: 100053b0
// Body: 100053b0 - 100054c3
// ============================================================================
void __fastcall VideoSettingsPage_UpdateControls(int param_1)

{
  int iVar1;
  
  CWnd::EnableWindow((CWnd *)(param_1 + 0x11c),*(int *)(*(int *)(param_1 + 0x90) + 0x118));
  CWnd::EnableWindow((CWnd *)(param_1 + 0x15c),*(int *)(*(int *)(param_1 + 0x90) + 0x118));
  CWnd::EnableWindow((CWnd *)(param_1 + 0x19c),
                     (uint)(*(int *)(*(int *)(param_1 + 0x90) + 0x118) < 0x31));
  iVar1 = *(int *)(*(int *)(param_1 + 0x90) + 0x118);
  if ((iVar1 == 0) || (iVar1 + -1 <= *(int *)(param_1 + 0x94))) {
    iVar1 = 0;
  }
  else {
    iVar1 = 1;
  }
  CWnd::EnableWindow((CWnd *)(param_1 + 0x1dc),iVar1);
  if ((*(int *)(*(int *)(param_1 + 0x90) + 0x118) == 0) || (*(int *)(param_1 + 0x94) == 0)) {
    iVar1 = 0;
  }
  else {
    iVar1 = 1;
  }
  CWnd::EnableWindow((CWnd *)(param_1 + 0x21c),iVar1);
  CWnd::EnableWindow((CWnd *)(param_1 + 0x25c),*(int *)(*(int *)(param_1 + 0x90) + 0x118));
  CWnd::EnableWindow((CWnd *)(param_1 + 0x29c),*(int *)(*(int *)(param_1 + 0x90) + 0x118));
  CWnd::EnableWindow((CWnd *)(param_1 + 0x2dc),*(int *)(*(int *)(param_1 + 0x90) + 0x118));
  CWnd::EnableWindow((CWnd *)(param_1 + 0xdc),*(int *)(*(int *)(param_1 + 0x90) + 0x118));
  return;
}


// ============================================================================
// Function: FUN_100054d0 at 100054d0
// Signature: undefined __fastcall FUN_100054d0(CWnd * param_1)
// Entry: 100054d0
// Body: 100054d0 - 10005640
// ============================================================================
void __fastcall FUN_100054d0(CWnd *param_1)

{
  CString *this;
  int iVar1;
  int iVar2;
  int iVar3;
  CWnd *local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10006d88;
  local_c = ExceptionList;
  if (*(int *)(*(int *)(param_1 + 0x90) + 0x118) != 0) {
    ExceptionList = &local_c;
    local_10 = param_1;
    CString::CString((CString *)&local_10,(CString *)(*(int *)(param_1 + 0x90) + 0x138));
    local_4 = 0;
    CString::operator+=((CString *)&local_10,
                        (CString *)
                        **(undefined4 **)
                          (*(int *)(param_1 + 0x90) + 0x50 + *(int *)(param_1 + 0x94) * 4));
    CFile::Remove((char *)local_10);
    this = (CString *)
           **(undefined4 **)(*(int *)(param_1 + 0x90) + 0x50 + *(int *)(param_1 + 0x94) * 4);
    if (this != (CString *)0x0) {
      CString::~CString(this);
      operator_delete(this);
    }
    operator_delete(*(void **)(*(int *)(param_1 + 0x90) + 0x50 + *(int *)(param_1 + 0x94) * 4));
    iVar1 = *(int *)(param_1 + 0x90);
    iVar3 = *(int *)(param_1 + 0x94);
    if (iVar3 < *(int *)(iVar1 + 0x118) + -1) {
      iVar2 = iVar3 * 4 + 0x50;
      do {
        iVar3 = iVar3 + 1;
        *(undefined4 *)(iVar2 + iVar1) = *(undefined4 *)(iVar2 + 4 + iVar1);
        iVar1 = *(int *)(param_1 + 0x90);
        iVar2 = iVar2 + 4;
      } while (iVar3 < *(int *)(iVar1 + 0x118) + -1);
    }
    *(int *)(*(int *)(param_1 + 0x90) + 0x118) = *(int *)(*(int *)(param_1 + 0x90) + 0x118) + -1;
    iVar1 = *(int *)(*(int *)(param_1 + 0x90) + 0x118);
    if (iVar1 <= *(int *)(param_1 + 0x94)) {
      if (iVar1 == 0) {
        *(undefined4 *)(param_1 + 0x94) = 0;
      }
      else {
        *(int *)(param_1 + 0x94) = iVar1 + -1;
      }
    }
    CWnd::UpdateData(param_1,0);
    FUN_10005d90((int)param_1);
    SendMessageA(*(HWND *)(param_1 + 700),0x186,*(WPARAM *)(param_1 + 0x94),0);
    local_4 = 0xffffffff;
    CString::~CString((CString *)&local_10);
  }
  ExceptionList = local_c;
  return;
}


// ============================================================================
// Function: FUN_10005650 at 10005650
// Signature: undefined __fastcall FUN_10005650(CWnd * param_1)
// Entry: 10005650
// Body: 10005650 - 100056a0
// ============================================================================
void __fastcall FUN_10005650(CWnd *param_1)

{
  LRESULT LVar1;
  
  LVar1 = SendMessageA(*(HWND *)(param_1 + 700),0x188,0,0);
  if (*(int *)(param_1 + 0x94) != LVar1) {
    CWnd::UpdateData(param_1,1);
    LVar1 = SendMessageA(*(HWND *)(param_1 + 700),0x188,0,0);
    *(LRESULT *)(param_1 + 0x94) = LVar1;
    CWnd::UpdateData(param_1,0);
  }
  return;
}


// ============================================================================
// Function: FUN_100056c0 at 100056c0
// Signature: undefined __fastcall FUN_100056c0(CWnd * param_1)
// Entry: 100056c0
// Body: 100056c0 - 1000573d
// ============================================================================
void __fastcall FUN_100056c0(CWnd *param_1)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  
  CWnd::UpdateData(param_1,1);
  iVar3 = *(int *)(param_1 + 0x94);
  uVar1 = *(undefined4 *)(*(int *)(param_1 + 0x90) + 0x50 + iVar3 * 4);
  if (0 < iVar3) {
    iVar2 = iVar3 * 4 + 0x50;
    do {
      iVar3 = iVar3 + -1;
      *(undefined4 *)(iVar2 + *(int *)(param_1 + 0x90)) =
           *(undefined4 *)(iVar2 + -4 + *(int *)(param_1 + 0x90));
      iVar2 = iVar2 + -4;
    } while (iVar3 != 0);
  }
  *(undefined4 *)(*(int *)(param_1 + 0x90) + 0x50) = uVar1;
  *(undefined4 *)(param_1 + 0x94) = 0;
  CWnd::UpdateData(param_1,0);
  FUN_10005d90((int)param_1);
  SendMessageA(*(HWND *)(param_1 + 700),0x186,*(WPARAM *)(param_1 + 0x94),0);
  return;
}


// ============================================================================
// Function: FUN_10005740 at 10005740
// Signature: undefined __fastcall FUN_10005740(CWnd * param_1)
// Entry: 10005740
// Body: 10005740 - 100057db
// ============================================================================
void __fastcall FUN_10005740(CWnd *param_1)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  CWnd::UpdateData(param_1,1);
  iVar2 = *(int *)(param_1 + 0x90);
  iVar3 = *(int *)(param_1 + 0x94);
  uVar1 = *(undefined4 *)(iVar2 + 0x50 + iVar3 * 4);
  if (iVar3 < *(int *)(iVar2 + 0x118) + -1) {
    iVar4 = iVar3 * 4 + 0x50;
    do {
      iVar3 = iVar3 + 1;
      *(undefined4 *)(iVar4 + iVar2) = *(undefined4 *)(iVar4 + 4 + iVar2);
      iVar2 = *(int *)(param_1 + 0x90);
      iVar4 = iVar4 + 4;
    } while (iVar3 < *(int *)(iVar2 + 0x118) + -1);
  }
  *(undefined4 *)(*(int *)(param_1 + 0x90) + 0x4c + *(int *)(*(int *)(param_1 + 0x90) + 0x118) * 4)
       = uVar1;
  *(int *)(param_1 + 0x94) = *(int *)(*(int *)(param_1 + 0x90) + 0x118) + -1;
  CWnd::UpdateData(param_1,0);
  FUN_10005d90((int)param_1);
  SendMessageA(*(HWND *)(param_1 + 700),0x186,*(WPARAM *)(param_1 + 0x94),0);
  return;
}


// ============================================================================
// Function: FUN_100057e0 at 100057e0
// Signature: undefined4 __stdcall FUN_100057e0(char * param_1, char * param_2, char * param_3)
// Entry: 100057e0
// Body: 100057e0 - 100059d5
// ============================================================================
undefined4 FUN_100057e0(char *param_1,char *param_2,char *param_3)

{
  char *pcVar1;
  int iVar2;
  HGLOBAL pvVar3;
  LPVOID pMem;
  uint uVar4;
  CString *this;
  CString *this_00;
  CFile local_2c [16];
  CFile local_1c [4];
  CFile local_18 [12];
  void *pvStack_c;
  undefined1 *local_8;
  int local_4;
  
  local_4 = 0xffffffff;
  local_8 = &LAB_10006dc0;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  CFile::CFile(local_1c);
  local_4 = 0;
  CFile::CFile(local_2c);
  pcVar1 = param_1;
  local_4._0_1_ = 1;
  iVar2 = CFile::Open(local_1c,param_1,0x1001,(CFileException *)0x0);
  if (iVar2 == 0) {
    CString::CString((CString *)&param_1);
    local_4._0_1_ = 2;
    CString::Format(this,(char *)&param_1);
    AfxMessageBox(param_2,0,0);
    CString::~CString((CString *)&param_2);
  }
  else {
    iVar2 = CFile::Open(local_2c,param_2,0,(CFileException *)0x0);
    if (iVar2 == 0) {
      CString::CString((CString *)&param_2);
      local_4._0_1_ = 3;
      CString::Format(this_00,(char *)&param_2);
      AfxMessageBox(param_3,0,0);
      CFile::Close(local_18);
      CFile::Remove(pcVar1);
      CString::~CString((CString *)&param_3);
    }
    else {
      pvVar3 = GlobalAlloc(2,0xffff);
      pMem = GlobalLock(pvVar3);
      if (pMem != (LPVOID)0x0) {
        uVar4 = CFile::Read(local_2c,pMem,0xffff);
        while (uVar4 != 0) {
          CFile::Write(local_1c,pMem,uVar4);
          uVar4 = CFile::Read(local_2c,pMem,0xffff);
        }
        pvVar3 = GlobalHandle(pMem);
        GlobalUnlock(pvVar3);
        pvVar3 = GlobalHandle(pMem);
        GlobalFree(pvVar3);
        CFile::Close(local_1c);
        CFile::Close(local_2c);
        local_4 = (uint)local_4._1_3_ << 8;
        CFile::~CFile(local_2c);
        local_4 = 0xffffffff;
        CFile::~CFile(local_1c);
        ExceptionList = pvStack_c;
        return 1;
      }
      CFile::Close(local_1c);
      CFile::Close(local_2c);
      CFile::Remove(pcVar1);
      AfxMessageBox(s_End_of_memory_1000a218,0,0);
    }
  }
  local_4 = (uint)local_4._1_3_ << 8;
  CFile::~CFile(local_2c);
  local_4 = 0xffffffff;
  CFile::~CFile(local_1c);
  ExceptionList = pvStack_c;
  return 0;
}


// ============================================================================
// Function: FUN_100059e0 at 100059e0
// Signature: undefined __fastcall FUN_100059e0(CWnd * param_1)
// Entry: 100059e0
// Body: 100059e0 - 10005d29
// ============================================================================
void __fastcall FUN_100059e0(CWnd *param_1)

{
  char cVar1;
  int iVar2;
  CString *pCVar3;
  undefined4 *puVar4;
  void *pvVar5;
  undefined4 uVar6;
  uint uVar7;
  uint uVar8;
  byte unaff_BP;
  char *pcVar9;
  undefined4 *puVar10;
  char *local_220;
  CString *local_21c;
  CString local_218 [4];
  CFileDialog local_214 [4];
  undefined4 local_210;
  CFileDialog local_20c [4];
  CFileDialog local_208 [152];
  CString aCStack_170 [332];
  void *pvStack_24;
  int local_1c;
  undefined1 local_18;
  undefined1 local_14;
  undefined1 local_10;
  void *local_c;
  undefined1 *local_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  local_8 = &LAB_10006e53;
  local_c = ExceptionList;
  local_210 = 0;
  ExceptionList = &local_c;
  CFileDialog::CFileDialog
            (local_208,1,&DAT_1000a270,(char *)0x0,0x1004,
             s_Video_Files____avi____avi_All_Fi_1000a274,(CWnd *)0x0);
  local_4 = 0;
  iVar2 = CFileDialog::DoModal(local_208);
  if (iVar2 == 1) {
    CString::CString(local_218,(CString *)(*(int *)(param_1 + 0x90) + 0x138));
    local_4 = CONCAT31(local_4._1_3_,1);
    pCVar3 = (CString *)CFileDialog::GetFileName(local_208);
    local_8._0_1_ = 2;
    CString::operator+=((CString *)&local_21c,pCVar3);
    local_8 = (undefined1 *)CONCAT31(local_8._1_3_,1);
    CString::~CString((CString *)&stack0xfffffddc);
    pcVar9 = &stack0xfffffddc;
    puVar4 = (undefined4 *)CFileDialog::GetPathName(local_20c);
    local_c._0_1_ = 3;
    iVar2 = FUN_100057e0(local_220,(char *)*puVar4,pcVar9);
    local_c._0_1_ = 1;
    CString::~CString((CString *)&stack0xfffffdd8);
    if (iVar2 == 0) {
      local_c = (void *)((uint)local_c._1_3_ << 8);
      CString::~CString((CString *)&local_220);
      local_c = (void *)0x4;
      goto LAB_10005cf2;
    }
    pvVar5 = operator_new(0x6c);
    *(void **)(*(int *)(param_1 + 0x90) + 0x50 + *(int *)(*(int *)(param_1 + 0x90) + 0x118) * 4) =
         pvVar5;
    puVar4 = *(undefined4 **)
              (*(int *)(param_1 + 0x90) + 0x50 + *(int *)(*(int *)(param_1 + 0x90) + 0x118) * 4);
    for (iVar2 = 0x1b; iVar2 != 0; iVar2 = iVar2 + -1) {
      *puVar4 = 0;
      puVar4 = puVar4 + 1;
    }
    local_21c = operator_new(4);
    local_c._0_1_ = 5;
    if (local_21c == (CString *)0x0) {
      uVar6 = 0;
    }
    else {
      uVar6 = CString::CString(local_21c);
    }
    local_c = (void *)CONCAT31(local_c._1_3_,1);
    **(undefined4 **)
      (*(int *)(param_1 + 0x90) + 0x50 + *(int *)(*(int *)(param_1 + 0x90) + 0x118) * 4) = uVar6;
    pCVar3 = (CString *)CFileDialog::GetFileName((CFileDialog *)&local_210);
    local_10 = 6;
    CString::operator=((CString *)
                       **(undefined4 **)
                         (*(int *)(param_1 + 0x90) + 0x50 +
                         *(int *)(*(int *)(param_1 + 0x90) + 0x118) * 4),pCVar3);
    local_10 = 1;
    CString::~CString((CString *)&stack0xfffffdd4);
    puVar4 = (undefined4 *)CFileDialog::GetFileTitle(local_214);
    uVar7 = 0xffffffff;
    local_14 = 7;
    pcVar9 = (char *)*puVar4;
    do {
      if (uVar7 == 0) break;
      uVar7 = uVar7 - 1;
      cVar1 = *pcVar9;
      pcVar9 = pcVar9 + 1;
    } while (cVar1 != '\0');
    if (~uVar7 - 1 < 9) {
      puVar4 = (undefined4 *)CFileDialog::GetFileTitle((CFileDialog *)local_218);
      uVar7 = 0xffffffff;
      local_18 = 8;
      pcVar9 = (char *)*puVar4;
      do {
        if (uVar7 == 0) break;
        uVar7 = uVar7 - 1;
        cVar1 = *pcVar9;
        pcVar9 = pcVar9 + 1;
      } while (cVar1 != '\0');
      uVar7 = ~uVar7 - 1;
    }
    else {
      uVar7 = 9;
    }
    puVar4 = (undefined4 *)CFileDialog::GetFileTitle((CFileDialog *)&local_21c);
    puVar4 = (undefined4 *)*puVar4;
    puVar10 = (undefined4 *)
              (*(int *)(*(int *)(param_1 + 0x90) + 0x50 +
                       *(int *)(*(int *)(param_1 + 0x90) + 0x118) * 4) + 0x20);
    for (uVar8 = uVar7 >> 2; uVar8 != 0; uVar8 = uVar8 - 1) {
      *puVar10 = *puVar4;
      puVar4 = puVar4 + 1;
      puVar10 = puVar10 + 1;
    }
    for (uVar7 = uVar7 & 3; uVar7 != 0; uVar7 = uVar7 - 1) {
      *(undefined1 *)puVar10 = *(undefined1 *)puVar4;
      puVar4 = (undefined4 *)((int)puVar4 + 1);
      puVar10 = (undefined4 *)((int)puVar10 + 1);
    }
    CString::~CString((CString *)&stack0xfffffdc8);
    local_1c = 7;
    if ((unaff_BP & 1) != 0) {
      CString::~CString((CString *)&stack0xfffffddc);
    }
    local_1c._0_1_ = 1;
    CString::~CString((CString *)&stack0xfffffdd4);
    iVar2 = *(int *)(param_1 + 0x90);
    uVar6 = *(undefined4 *)(iVar2 + 0x118);
    *(undefined4 *)(param_1 + 0x94) = uVar6;
    *(undefined4 *)(*(int *)(iVar2 + 0x50 + *(int *)(iVar2 + 0x118) * 4) + 0x1c) = uVar6;
    iVar2 = *(int *)(*(int *)(param_1 + 0x90) + 0x50 +
                    *(int *)(*(int *)(param_1 + 0x90) + 0x118) * 4);
    *(undefined4 *)(iVar2 + 0x18) = *(undefined4 *)(iVar2 + 0x1c);
    *(int *)(*(int *)(param_1 + 0x90) + 0x118) = *(int *)(*(int *)(param_1 + 0x90) + 0x118) + 1;
    CWnd::UpdateData(param_1,0);
    CPropertyPage::SetModified((CPropertyPage *)param_1,1);
    FUN_10005d90((int)param_1);
    SendMessageA(*(HWND *)(param_1 + 700),0x186,*(WPARAM *)(param_1 + 0x94),0);
    local_1c = (uint)local_1c._1_3_ << 8;
    CString::~CString((CString *)&stack0xfffffdd0);
  }
  local_1c = 9;
LAB_10005cf2:
  CString::~CString(aCStack_170);
  local_1c = 0xffffffff;
  CDialog::~CDialog((CDialog *)&local_220);
  ExceptionList = pvStack_24;
  return;
}


// ============================================================================
// Function: ~CDialog at 10005d30
// Signature: void __thiscall ~CDialog(CDialog * this)
// Entry: 10005d30
// Body: 10005d30 - 10005d34
// ============================================================================
void __thiscall CDialog::~CDialog(CDialog *this)

{
  ~CDialog(this);
  return;
}


// ============================================================================
// Function: FUN_10005d40 at 10005d40
// Signature: undefined __fastcall FUN_10005d40(CDialog * param_1)
// Entry: 10005d40
// Body: 10005d40 - 10005d8e
// ============================================================================
void __fastcall FUN_10005d40(CDialog *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_10006e68;
  local_c = ExceptionList;
  local_4 = 0;
  ExceptionList = &local_c;
  CString::~CString((CString *)(param_1 + 0xb0));
  local_4 = 0xffffffff;
  CDialog::~CDialog(param_1);
  ExceptionList = local_c;
  return;
}


// ============================================================================
// Function: FUN_10005d90 at 10005d90
// Signature: undefined __fastcall FUN_10005d90(int param_1)
// Entry: 10005d90
// Body: 10005d90 - 10005e48
// ============================================================================
void __fastcall FUN_10005d90(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  HWND__ *hWnd;
  HWND__ local_10;
  void *local_c;
  undefined1 *local_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  local_8 = &LAB_10006e88;
  local_c = ExceptionList;
  iVar3 = 0;
  ExceptionList = &local_c;
  local_10.unused = param_1;
  SendMessageA(*(HWND *)(param_1 + 700),0x184,0,0);
  CString::CString((CString *)&local_10);
  iVar1 = *(int *)(param_1 + 0x90);
  local_4 = 0;
  if (0 < *(int *)(iVar1 + 0x118)) {
    iVar2 = 0x50;
    do {
      hWnd = &local_10;
      CString::Format((CString *)(*(int *)(iVar2 + iVar1) + 0x20),(char *)hWnd);
      SendMessageA(hWnd,*(UINT *)(param_1 + 700),0x180,0);
      iVar1 = *(int *)(param_1 + 0x90);
      iVar3 = iVar3 + 1;
      iVar2 = iVar2 + 4;
    } while (iVar3 < *(int *)(iVar1 + 0x118));
  }
  local_4 = 0xffffffff;
  CString::~CString((CString *)&local_10);
  ExceptionList = local_c;
  return;
}


// ============================================================================
// Function: FUN_10005e50 at 10005e50
// Signature: undefined4 __fastcall FUN_10005e50(CDialog * param_1)
// Entry: 10005e50
// Body: 10005e50 - 10005e9b
// ============================================================================
undefined4 __fastcall FUN_10005e50(CDialog *param_1)

{
  CDialog::OnInitDialog(param_1);
  FUN_10005d90((int)param_1);
  SendMessageA(*(HWND *)(param_1 + 700),0x186,*(WPARAM *)(param_1 + 0x94),0);
  CWnd::GetWindowTextA((CWnd *)(param_1 + 0x9c),(CString *)(param_1 + 0x98));
  CWnd::UpdateData((CWnd *)param_1,0);
  return 1;
}


// ============================================================================
// Function: FUN_10005ea0 at 10005ea0
// Signature: CStatic * __thiscall FUN_10005ea0(void * this, byte param_1)
// Entry: 10005ea0
// Body: 10005ea0 - 10005ebd
// ============================================================================
CStatic * __thiscall FUN_10005ea0(void *this,byte param_1)

{
  CStatic::~CStatic(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: FUN_10005ec0 at 10005ec0
// Signature: CButton * __thiscall FUN_10005ec0(void * this, byte param_1)
// Entry: 10005ec0
// Body: 10005ec0 - 10005edd
// ============================================================================
CButton * __thiscall FUN_10005ec0(void *this,byte param_1)

{
  CButton::~CButton(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: FUN_10005ee0 at 10005ee0
// Signature: CListBox * __thiscall FUN_10005ee0(void * this, byte param_1)
// Entry: 10005ee0
// Body: 10005ee0 - 10005efd
// ============================================================================
CListBox * __thiscall FUN_10005ee0(void *this,byte param_1)

{
  CListBox::~CListBox(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: FUN_10005f00 at 10005f00
// Signature: CEdit * __thiscall FUN_10005f00(void * this, byte param_1)
// Entry: 10005f00
// Body: 10005f00 - 10005f1d
// ============================================================================
CEdit * __thiscall FUN_10005f00(void *this,byte param_1)

{
  CEdit::~CEdit(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: MCIWndCreateA at 10005f20
// Signature: undefined MCIWndCreateA(void)
// Entry: 10005f20
// Body: 10005f20 - 10005f25
// ============================================================================
void MCIWndCreateA(void)

{
                    /* WARNING: Could not recover jumptable at 0x10005f20. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  MCIWndCreateA();
  return;
}


// ============================================================================
// Function: DestroyWindow at 10005f9e
// Signature: int __thiscall DestroyWindow(CWnd * this)
// Entry: 10005f9e
// Body: 10005f9e - 10005fa3
// ============================================================================
int __thiscall CWnd::DestroyWindow(CWnd *this)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10005f9e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = DestroyWindow(this);
  return iVar1;
}


// ============================================================================
// Function: CWnd at 10006016
// Signature: undefined __thiscall CWnd(CWnd * this)
// Entry: 10006016
// Body: 10006016 - 1000601b
// ============================================================================
void __thiscall CWnd::CWnd(CWnd *this)

{
                    /* WARNING: Could not recover jumptable at 0x10006016. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  CWnd(this);
  return;
}


// ============================================================================
// Function: operator_delete at 1000601c
// Signature: void __cdecl operator_delete(void * param_1)
// Entry: 1000601c
// Body: 1000601c - 10006021
// ============================================================================
void __cdecl operator_delete(void *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x1000601c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  operator_delete(param_1);
  return;
}


// ============================================================================
// Function: ~CWnd at 10006022
// Signature: void __thiscall ~CWnd(CWnd * this)
// Entry: 10006022
// Body: 10006022 - 10006027
// ============================================================================
void __thiscall CWnd::~CWnd(CWnd *this)

{
                    /* WARNING: Could not recover jumptable at 0x10006022. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~CWnd(this);
  return;
}


// ============================================================================
// Function: CreateEx at 10006028
// Signature: int __thiscall CreateEx(CWnd * this, ulong param_1, char * param_2, char * param_3, ulong param_4, int param_5, int param_6, int param_7, int param_8, HWND__ * param_9, HMENU__ * param_10, void * param_11)
// Entry: 10006028
// Body: 10006028 - 1000602d
// ============================================================================
int __thiscall
CWnd::CreateEx(CWnd *this,ulong param_1,char *param_2,char *param_3,ulong param_4,int param_5,
              int param_6,int param_7,int param_8,HWND__ *param_9,HMENU__ *param_10,void *param_11)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10006028. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = CreateEx(this,param_1,param_2,param_3,param_4,param_5,param_6,param_7,param_8,param_9,
                   param_10,param_11);
  return iVar1;
}


// ============================================================================
// Function: AfxRegisterWndClass at 1000602e
// Signature: char * __stdcall AfxRegisterWndClass(uint param_1, HICON__ * param_2, HBRUSH__ * param_3, HICON__ * param_4)
// Entry: 1000602e
// Body: 1000602e - 10006033
// ============================================================================
char * AfxRegisterWndClass(uint param_1,HICON__ *param_2,HBRUSH__ *param_3,HICON__ *param_4)

{
  char *pcVar1;
  
                    /* WARNING: Could not recover jumptable at 0x1000602e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pcVar1 = AfxRegisterWndClass(param_1,param_2,param_3,param_4);
  return pcVar1;
}


// ============================================================================
// Function: DoModal at 10006046
// Signature: int __thiscall DoModal(CPropertySheet * this)
// Entry: 10006046
// Body: 10006046 - 1000604b
// ============================================================================
int __thiscall CPropertySheet::DoModal(CPropertySheet *this)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10006046. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = DoModal(this);
  return iVar1;
}


// ============================================================================
// Function: ~CPropertySheet at 10006064
// Signature: void __thiscall ~CPropertySheet(CPropertySheet * this)
// Entry: 10006064
// Body: 10006064 - 10006069
// ============================================================================
void __thiscall CPropertySheet::~CPropertySheet(CPropertySheet *this)

{
                    /* WARNING: Could not recover jumptable at 0x10006064. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~CPropertySheet(this);
  return;
}


// ============================================================================
// Function: AddPage at 1000606a
// Signature: void __thiscall AddPage(CPropertySheet * this, CPropertyPage * param_1)
// Entry: 1000606a
// Body: 1000606a - 1000606f
// ============================================================================
void __thiscall CPropertySheet::AddPage(CPropertySheet *this,CPropertyPage *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x1000606a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AddPage(this,param_1);
  return;
}


// ============================================================================
// Function: CPropertySheet at 10006070
// Signature: undefined __thiscall CPropertySheet(CPropertySheet * this, uint param_1, CWnd * param_2, uint param_3)
// Entry: 10006070
// Body: 10006070 - 10006075
// ============================================================================
void __thiscall
CPropertySheet::CPropertySheet(CPropertySheet *this,uint param_1,CWnd *param_2,uint param_3)

{
                    /* WARNING: Could not recover jumptable at 0x10006070. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  CPropertySheet(this,param_1,param_2,param_3);
  return;
}


// ============================================================================
// Function: ~CWinApp at 10006076
// Signature: void __thiscall ~CWinApp(CWinApp * this)
// Entry: 10006076
// Body: 10006076 - 1000607b
// ============================================================================
void __thiscall CWinApp::~CWinApp(CWinApp *this)

{
                    /* WARNING: Could not recover jumptable at 0x10006076. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~CWinApp(this);
  return;
}


// ============================================================================
// Function: CWinApp at 100060f4
// Signature: undefined __thiscall CWinApp(CWinApp * this, char * param_1)
// Entry: 100060f4
// Body: 100060f4 - 100060f9
// ============================================================================
void __thiscall CWinApp::CWinApp(CWinApp *this,char *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x100060f4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  CWinApp(this,param_1);
  return;
}


// ============================================================================
// Function: InitInstance at 100060fa
// Signature: int __thiscall InitInstance(CWinApp * this)
// Entry: 100060fa
// Body: 100060fa - 100060ff
// ============================================================================
int __thiscall CWinApp::InitInstance(CWinApp *this)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x100060fa. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = InitInstance(this);
  return iVar1;
}

// clang-format on
