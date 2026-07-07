/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Notes: raw Ghidra decompiler output with only file-level framing added.
*/
#include "../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100034b0 at 100034b0
// Signature: HRESULT __stdcall FUN_100034b0(int param_1, LPCWSTR param_2)
// Entry: 100034b0
// Body: 100034b0 - 100036a0
// ============================================================================
HRESULT FUN_100034b0(int param_1,LPCWSTR param_2)

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
  
  puStack_c = &LAB_10005a80;
  local_10 = ExceptionList;
  local_8 = 1;
  local_14 = (LPCWSTR)0x0;
  local_18 = (ITypeLib *)0x0;
  ExceptionList = &local_10;
  GetModuleFileNameA(*(HMODULE *)(param_1 + 0xc),local_128,0x104);
  if (param_2 != (LPCWSTR)0x0) {
    iVar1 = lstrlenW(param_2);
    FUN_10005640();
    lpString2 = FUN_100036e0(&stack0xfffffdd4,param_2,iVar1 * 2 + 2,0);
    lstrcatA(local_128,lpString2);
  }
  if (&stack0x00000000 == (undefined1 *)0x128) {
    szFile = (LPCOLESTR)0x0;
  }
  else {
    iVar1 = lstrlenA(local_128);
    FUN_10005640();
    szFile = FUN_100036b0((LPWSTR)&stack0xfffffdd4,local_128,iVar1 + 1,0);
  }
  HVar2 = LoadTypeLib(szFile,&local_18);
  if (HVar2 < 0) {
    lpString1 = (LPCSTR)0x0;
    lpsz = local_128;
    if (local_128[0] == '\0') {
LAB_100035af:
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
      if (lpString1 == (LPCSTR)0x0) goto LAB_100035af;
    }
    lstrcpyA(lpString1,&DAT_10008188);
    if (&stack0x00000000 == (undefined1 *)0x128) {
      szFile = (LPCOLESTR)0x0;
    }
    else {
      iVar1 = lstrlenA(local_128);
      FUN_10005640();
      szFile = FUN_100036b0((LPWSTR)&stack0xfffffdd4,local_128,iVar1 + 1,0);
    }
    HVar2 = LoadTypeLib(szFile,&local_18);
    if (HVar2 < 0) goto LAB_10003616;
  }
  local_14 = SysAllocString(szFile);
LAB_10003616:
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
    iVar1 = FUN_10003710(local_220);
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
// Function: FUN_100036b0 at 100036b0
// Signature: LPWSTR __stdcall FUN_100036b0(LPWSTR param_1, LPCSTR param_2, int param_3, UINT param_4)
// Entry: 100036b0
// Body: 100036b0 - 100036d9
// ============================================================================
LPWSTR FUN_100036b0(LPWSTR param_1,LPCSTR param_2,int param_3,UINT param_4)

{
  *param_1 = L'\0';
  MultiByteToWideChar(param_4,0,param_2,-1,param_1,param_3);
  return param_1;
}


// ============================================================================
// Function: FUN_100036e0 at 100036e0
// Signature: LPSTR __stdcall FUN_100036e0(LPSTR param_1, LPCWSTR param_2, int param_3, UINT param_4)
// Entry: 100036e0
// Body: 100036e0 - 1000370b
// ============================================================================
LPSTR FUN_100036e0(LPSTR param_1,LPCWSTR param_2,int param_3,UINT param_4)

{
  *param_1 = '\0';
  WideCharToMultiByte(param_4,0,param_2,-1,param_1,param_3,(LPCSTR)0x0,(LPBOOL)0x0);
  return param_1;
}


// ============================================================================
// Function: FUN_10003710 at 10003710
// Signature: int __stdcall FUN_10003710(short * param_1)
// Entry: 10003710
// Body: 10003710 - 1000374b
// ============================================================================
int FUN_10003710(short *param_1)

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
// Function: FUN_100037e0 at 100037e0
// Signature: HRESULT __thiscall FUN_100037e0(void * this, BSTR param_1)
// Entry: 100037e0
// Body: 100037e0 - 10003a75
// ============================================================================
/* WARNING: Exceeded maximum restarts with more pending */

HRESULT __thiscall FUN_100037e0(void *this,BSTR param_1)

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
  puStack_8 = &DAT_10005abb;
  local_c = ExceptionList;
  if (*(int *)((int)this + 0xc) == 0) {
    local_1c = -0x7fffbffb;
    ExceptionList = &local_c;
    EnterCriticalSection((LPCRITICAL_SECTION)&DAT_100083b4);
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
        HVar3 = (*pIStack_28->lpVtbl->QueryInterface)(pIStack_28,(IID *)&DAT_10008190,&pIStack_20);
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
              FUN_1000566f(pOVar4,0xc,uVar7,&LAB_10003b80);
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
    LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_100083b4);
    FUN_10003a80(0x10008398,ComTypeInfo_CleanupDispatchMap,this);
    ExceptionList = local_c;
    return local_1c;
  }
  return 0;
}


// ============================================================================
// Function: FUN_10003a80 at 10003a80
// Signature: undefined4 __stdcall FUN_10003a80(int param_1, undefined4 param_2, undefined4 param_3)
// Entry: 10003a80
// Body: 10003a80 - 10003b16
// ============================================================================
undefined4 FUN_10003a80(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_10005ad0;
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
// Function: Catch@10003ace at 10003ace
// Signature: undefined4 __stdcall Catch@10003ace(void)
// Entry: 10003ace
// Body: 10003ace - 10003ad3
// ============================================================================
undefined4 Catch_10003ace(void)

{
  return 0x10003ab6;
}


// ============================================================================
// Function: ComTypeInfo_CleanupDispatchMap at 10003b20
// Signature: undefined __stdcall ComTypeInfo_CleanupDispatchMap(int param_1)
// Entry: 10003b20
// Body: 10003b20 - 10003b74
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
    FUN_10005506(iVar2,0xc,*(int *)(iVar2 + -4),FUN_10001390);
    operator_delete((void *)(iVar2 + -4));
    *(undefined4 *)(param_1 + 0x14) = 0;
    return;
  }
  *(undefined4 *)(param_1 + 0x14) = 0;
  return;
}


// ============================================================================
// Function: RecorderContext_Construct at 10003bf0
// Signature: undefined4 * __fastcall RecorderContext_Construct(undefined4 * param_1)
// Entry: 10003bf0
// Body: 10003bf0 - 10003c91
// ============================================================================
undefined4 * __fastcall RecorderContext_Construct(undefined4 *param_1)

{
  void *this;
  CWnd *pCVar1;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10005aea;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  param_1[1] = &PTR_LAB_10006bfc;
  param_1[2] = &PTR_LAB_10006be0;
  param_1[4] = 0;
  *param_1 = &PTR_LAB_10006b68;
  param_1[1] = &PTR_LAB_10006b30;
  param_1[2] = &PTR_LAB_10006b00;
  param_1[3] = &PTR_LAB_10006af0;
  param_1[5] = 0;
  param_1[0xb] = 0;
  param_1[6] = 0;
  param_1[0xd] = 8;
  this = operator_new(0x98);
  local_4 = 0;
  if (this == (void *)0x0) {
    pCVar1 = (CWnd *)0x0;
  }
  else {
    pCVar1 = FUN_10001020(this,param_1);
  }
  param_1[0xe] = pCVar1;
  param_1[0xf] = 0;
  param_1[0xc] = 0;
  param_1[0x17] = 0;
  param_1[0x18] = 0;
  param_1[0x16] = 0;
  ExceptionList = local_c;
  return param_1;
}


// ============================================================================
// Function: RecorderContext_Destroy at 10003ca0
// Signature: undefined __fastcall RecorderContext_Destroy(char * param_1)
// Entry: 10003ca0
// Body: 10003ca0 - 10003d6a
// ============================================================================
void __fastcall RecorderContext_Destroy(char *param_1)

{
  int iVar1;
  char *local_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_10005b08;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  *(undefined ***)param_1 = &PTR_LAB_10006b68;
  *(undefined ***)(param_1 + 8) = &PTR_LAB_10006b00;
  *(undefined ***)(param_1 + 4) = &PTR_LAB_10006b30;
  *(undefined ***)(param_1 + 0xc) = &PTR_LAB_10006af0;
  local_10 = param_1;
  FUN_10004ab0((int)(param_1 + 4));
  if (*(int **)(param_1 + 0x38) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0x38) + 4))(1);
  }
  if (*(int **)(param_1 + 0x30) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0x30) + 0x20))(&local_10);
    puStack_8 = (undefined1 *)0x0;
    iVar1 = (**(code **)(**(int **)(param_1 + 0x30) + 0x38))();
    (**(code **)(**(int **)(param_1 + 0x30) + 0x54))();
    if (*(int **)(param_1 + 0x30) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0x30) + 4))(1);
    }
    param_1[0x30] = '\0';
    param_1[0x31] = '\0';
    param_1[0x32] = '\0';
    param_1[0x33] = '\0';
    if (iVar1 == 0) {
      CFile::Remove(local_10);
    }
    uStack_4 = 0xffffffff;
    CString::~CString((CString *)&local_10);
  }
  if (*(void **)(param_1 + 0x3c) != (void *)0x0) {
    operator_delete(*(void **)(param_1 + 0x3c));
  }
  ExceptionList = pvStack_c;
  return;
}


// ============================================================================
// Function: Recorder_ShowPropertiesDialog at 10003e00
// Signature: undefined __fastcall Recorder_ShowPropertiesDialog(int param_1)
// Entry: 10003e00
// Body: 10003e00 - 10003ea9
// ============================================================================
void __fastcall Recorder_ShowPropertiesDialog(int param_1)

{
  undefined4 uVar1;
  int *piVar2;
  undefined4 uVar3;
  AFX_MODULE_STATE *pAVar4;
  void *this;
  CPropertySheet *pCVar5;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10005b2a;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  pAVar4 = AfxGetModuleState();
  uVar3 = DAT_100083a0;
  uVar1 = *(undefined4 *)(pAVar4 + 0xc);
  pAVar4 = AfxGetModuleState();
  *(undefined4 *)(pAVar4 + 0xc) = uVar3;
  this = operator_new(0x160);
  local_4 = 0;
  if (this == (void *)0x0) {
    pCVar5 = (CPropertySheet *)0x0;
  }
  else {
    pCVar5 = FUN_100013c0(this,0x65,param_1,0);
  }
  local_4 = 0xffffffff;
  (**(code **)(*(int *)pCVar5 + 0xc0))();
  if (pCVar5 != (CPropertySheet *)0x0) {
    (**(code **)(*(int *)pCVar5 + 4))(1);
  }
  pAVar4 = AfxGetModuleState();
  *(undefined4 *)(pAVar4 + 0xc) = uVar1;
  piVar2 = *(int **)(param_1 + 0x14);
  if (piVar2 != (int *)0x0) {
    (**(code **)(*piVar2 + 0x1c))(piVar2,0,this);
  }
  ExceptionList = pvStack_c;
  return;
}


// ============================================================================
// Function: Recorder_BuildWaveFormatFromPreset at 10004030
// Signature: undefined __stdcall Recorder_BuildWaveFormatFromPreset(undefined2 * param_1, uint param_2)
// Entry: 10004030
// Body: 10004030 - 100040cf
// ============================================================================
void Recorder_BuildWaveFormatFromPreset(undefined2 *param_1,uint param_2)

{
  uint uVar1;
  bool bVar2;
  
  uVar1 = param_2 & 0x80000001;
  bVar2 = uVar1 == 0;
  *param_1 = 1;
  param_1[1] = 2;
  if ((int)uVar1 < 0) {
    bVar2 = (uVar1 - 1 | 0xfffffffe) == 0xffffffff;
  }
  if (bVar2) {
    param_1[1] = 1;
  }
  param_1[7] = 0x10;
  param_1[6] = param_1[1] * 2;
  uVar1 = param_2 & 0x80000003;
  if ((int)uVar1 < 0) {
    uVar1 = (uVar1 - 1 | 0xfffffffc) + 1;
  }
  if ((int)uVar1 < 2) {
    param_1[6] = param_1[1];
    param_1[7] = 8;
  }
  *(undefined4 *)(param_1 + 2) = 0xac44;
  if ((int)param_2 < 4) {
    *(undefined4 *)(param_1 + 2) = 8000;
  }
  else if ((int)param_2 < 8) {
    *(undefined4 *)(param_1 + 2) = 0x2b11;
  }
  else if ((int)param_2 < 0xc) {
    *(undefined4 *)(param_1 + 2) = 0x5622;
  }
  param_1[8] = 0;
  *(uint *)(param_1 + 4) = (uint)(ushort)param_1[6] * *(int *)(param_1 + 2);
  return;
}


// ============================================================================
// Function: Recorder_StartCapture at 100040d0
// Signature: bool __fastcall Recorder_StartCapture(int param_1)
// Entry: 100040d0
// Body: 100040d0 - 10004233
// ============================================================================
bool __fastcall Recorder_StartCapture(int param_1)

{
  int *piVar1;
  void *pvVar2;
  MMRESULT MVar3;
  int iVar4;
  HGLOBAL hMem;
  LPVOID pvVar5;
  int iVar6;
  
  piVar1 = *(int **)(param_1 + 0x14);
  if ((piVar1 != (int *)0x0) && (*(int *)(param_1 + 0x3c) == 0)) {
    (**(code **)(*piVar1 + 0x30))(piVar1);
    pvVar2 = operator_new(0x2c);
    *(void **)(param_1 + 0x3c) = pvVar2;
    Recorder_BuildWaveFormatFromPreset((undefined2 *)((int)pvVar2 + 0x10),*(uint *)(param_1 + 0x34))
    ;
    *(uint *)(*(int *)(param_1 + 0x3c) + 0x28) = *(uint *)(*(int *)(param_1 + 0x3c) + 0x18) / 10;
    *(undefined4 *)(*(int *)(param_1 + 0x3c) + 0x24) = 0;
    MVar3 = waveInOpen((LPHWAVEIN)(*(int *)(param_1 + 0x3c) + 0x24),0xffffffff,
                       (LPCWAVEFORMATEX)(*(int *)(param_1 + 0x3c) + 0x10),
                       *(DWORD_PTR *)(*(int *)(param_1 + 0x38) + 0x20),0,0x10000);
    iVar6 = 0;
    if (MVar3 == 0) {
      do {
        iVar4 = iVar6 + 4;
        *(undefined4 *)(iVar6 + *(int *)(param_1 + 0x3c)) = 0;
        iVar6 = iVar4;
      } while (iVar4 < 0x10);
      iVar6 = 0;
      while( true ) {
        hMem = GlobalAlloc(0x2002,*(int *)(*(int *)(param_1 + 0x3c) + 0x28) + 0x20);
        pvVar5 = GlobalLock(hMem);
        *(LPVOID *)(iVar6 + *(int *)(param_1 + 0x3c)) = pvVar5;
        piVar1 = *(int **)(iVar6 + *(int *)(param_1 + 0x3c));
        if (piVar1 == (int *)0x0) break;
        *piVar1 = (int)(piVar1 + 8);
        *(undefined4 *)(*(int *)(iVar6 + *(int *)(param_1 + 0x3c)) + 4) =
             *(undefined4 *)(*(int *)(param_1 + 0x3c) + 0x28);
        *(undefined4 *)(*(int *)(iVar6 + *(int *)(param_1 + 0x3c)) + 8) = 0;
        *(undefined4 *)(*(int *)(iVar6 + *(int *)(param_1 + 0x3c)) + 0xc) = 0;
        *(undefined4 *)(*(int *)(iVar6 + *(int *)(param_1 + 0x3c)) + 0x10) = 0;
        *(undefined4 *)(*(int *)(iVar6 + *(int *)(param_1 + 0x3c)) + 0x14) = 0;
        MVar3 = waveInPrepareHeader(*(HWAVEIN *)(*(int *)(param_1 + 0x3c) + 0x24),
                                    *(LPWAVEHDR *)(iVar6 + *(int *)(param_1 + 0x3c)),0x20);
        if (MVar3 != 0) {
          return false;
        }
        MVar3 = waveInAddBuffer(*(HWAVEIN *)(*(int *)(param_1 + 0x3c) + 0x24),
                                *(LPWAVEHDR *)(iVar6 + *(int *)(param_1 + 0x3c)),0x20);
        if (MVar3 != 0) {
          return false;
        }
        iVar6 = iVar6 + 4;
        if (0xf < iVar6) {
          if (*(int **)(param_1 + 0x30) != (int *)0x0) {
            (**(code **)(**(int **)(param_1 + 0x30) + 0x34))(0);
            (**(code **)(**(int **)(param_1 + 0x30) + 0x30))(0,0);
          }
          MVar3 = waveInStart(*(HWAVEIN *)(*(int *)(param_1 + 0x3c) + 0x24));
          return (bool)('\x01' - (MVar3 != 0));
        }
      }
    }
  }
  return false;
}


// ============================================================================
// Function: Recorder_StopCapture at 10004240
// Signature: undefined4 __fastcall Recorder_StopCapture(int param_1)
// Entry: 10004240
// Body: 10004240 - 100042f3
// ============================================================================
undefined4 __fastcall Recorder_StopCapture(int param_1)

{
  HWAVEIN hwi;
  LPWAVEHDR pwh;
  HGLOBAL hMem;
  int iVar1;
  
  if ((*(int *)(param_1 + 0x3c) != 0) &&
     (hwi = *(HWAVEIN *)(*(int *)(param_1 + 0x3c) + 0x24), hwi != (HWAVEIN)0x0)) {
    waveInStop(hwi);
    waveInReset(*(HWAVEIN *)(*(int *)(param_1 + 0x3c) + 0x24));
    iVar1 = 0;
    do {
      pwh = *(LPWAVEHDR *)(iVar1 + *(int *)(param_1 + 0x3c));
      if (pwh != (LPWAVEHDR)0x0) {
        waveInUnprepareHeader(*(HWAVEIN *)(*(int *)(param_1 + 0x3c) + 0x24),pwh,0x20);
        hMem = GlobalHandle(*(LPCVOID *)(iVar1 + *(int *)(param_1 + 0x3c)));
        GlobalFree(hMem);
        *(undefined4 *)(iVar1 + *(int *)(param_1 + 0x3c)) = 0;
      }
      iVar1 = iVar1 + 4;
    } while (iVar1 < 0x10);
    waveInClose(*(HWAVEIN *)(*(int *)(param_1 + 0x3c) + 0x24));
    operator_delete(*(void **)(param_1 + 0x3c));
    *(undefined4 *)(param_1 + 0x3c) = 0;
    (**(code **)(*(int *)(param_1 + 8) + 0x20))(param_1 + 8);
    (**(code **)(**(int **)(param_1 + 0x14) + 0x2c))(*(int **)(param_1 + 0x14));
    return 1;
  }
  return 0;
}


// ============================================================================
// Function: Recorder_StartPlaybackPreview at 100045e0
// Signature: undefined4 __fastcall Recorder_StartPlaybackPreview(int param_1)
// Entry: 100045e0
// Body: 100045e0 - 100047e9
// ============================================================================
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __fastcall Recorder_StartPlaybackPreview(int param_1)

{
  undefined4 *puVar1;
  uint uVar2;
  undefined4 uVar3;
  int iVar4;
  int **ppiVar5;
  UINT_PTR UVar6;
  undefined1 *puVar7;
  int **ppiVar8;
  int *piStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_28;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  uint uStack_18;
  undefined4 uStack_14;
  undefined2 *puStack_10;
  
  if (*(int **)(param_1 + 0x30) == (int *)0x0) {
    return 0;
  }
  if (*(int *)(param_1 + 0x14) == 0) {
    return 0;
  }
  uStack_3c = 0x10004603;
  (**(code **)(**(int **)(param_1 + 0x30) + 0x50))();
  uStack_3c = 0;
  uStack_40 = 0;
  piStack_44 = (int *)0x1000460d;
  (**(code **)(**(int **)(param_1 + 0x30) + 0x30))();
  piStack_44 = (int *)0x10004615;
  uVar2 = (**(code **)(**(int **)(param_1 + 0x30) + 0x38))();
  uStack_20 = 0;
  *(uint *)(param_1 + 0x74) = uVar2;
  piStack_44 = *(int **)(param_1 + 0x14);
  *(float *)(param_1 + 0x80) = (float)uVar2 * _DAT_10006478;
  uVar3 = (**(code **)(*piStack_44 + 0x28))();
  *(undefined4 *)(param_1 + 0x5c) = uVar3;
  uStack_18 = 0;
  uStack_14 = 0;
  *(undefined4 *)(param_1 + 0x78) = 0;
  *(undefined4 *)(param_1 + 0x7c) = 0;
  *(undefined4 *)(param_1 + 0x84) = 0;
  puStack_10 = (undefined2 *)0x0;
  uStack_20 = 0x14;
  uStack_1c = 0xe2;
  Recorder_BuildWaveFormatFromPreset((undefined2 *)(param_1 + 0x44),*(uint *)(param_1 + 0x34));
  uStack_18 = (uint)(*(ushort *)(param_1 + 0x52) >> 3) * (uint)*(ushort *)(param_1 + 0x46) *
              *(int *)(param_1 + 0x48) * 4;
  if (*(uint *)(param_1 + 0x74) <= uStack_18) {
    uStack_18 = *(uint *)(param_1 + 0x74);
  }
  puVar1 = (undefined4 *)(param_1 + 0x60);
  uVar2 = 0;
  puStack_10 = (undefined2 *)(param_1 + 0x44);
  iVar4 = (**(code **)(**(int **)(param_1 + 0x5c) + 0xc))
                    (*(int **)(param_1 + 0x5c),&uStack_20,puVar1);
  if (iVar4 != 0) {
    return 0;
  }
  ppiVar8 = &piStack_44;
  puVar7 = &stack0xffffffb8;
  iVar4 = (**(code **)(*(int *)*puVar1 + 0x2c))
                    ((int *)*puVar1,0,uStack_28,puVar7,ppiVar8,&uStack_40,&uStack_3c,0);
  if ((iVar4 != 0) || (ppiVar8 == (int **)0x0)) goto LAB_100047c9;
  if (uVar2 == *(uint *)(param_1 + 0x74)) {
    *(undefined4 *)(param_1 + 100) = 0;
    ppiVar5 = (int **)(**(code **)(**(int **)(param_1 + 0x30) + 0x3c))(puVar7,ppiVar8);
    if ((ppiVar5 != ppiVar8) ||
       (iVar4 = (**(code **)(*(int *)*puVar1 + 0x4c))((int *)*puVar1,puVar7,ppiVar8,0,0), iVar4 != 0
       )) goto LAB_100047c9;
    iVar4 = (**(code **)(*(int *)*puVar1 + 0x30))((int *)*puVar1,0,0,0);
  }
  else {
    *(uint *)(param_1 + 0x68) = uVar2 >> 1;
    iVar4 = (**(code **)(**(int **)(param_1 + 0x30) + 0x3c))(puVar7,uVar2 >> 1);
    if (iVar4 != *(int *)(param_1 + 0x68)) goto LAB_100047c9;
    uVar3 = (**(code **)(**(int **)(param_1 + 0x30) + 0x14))();
    *(undefined4 *)(param_1 + 100) = uVar3;
    iVar4 = (**(code **)(*(int *)*puVar1 + 0x4c))((int *)*puVar1,puVar7,ppiVar8,0,0);
    if (iVar4 != 0) goto LAB_100047c9;
    *(undefined4 *)(param_1 + 0x70) = 1;
    *(undefined4 *)(param_1 + 0x6c) = 0;
    iVar4 = (**(code **)(*(int *)*puVar1 + 0x30))((int *)*puVar1,0,0,1);
  }
  if (iVar4 == 0) {
    if (*(int *)(param_1 + 0x58) == 0) {
      UVar6 = FUN_10001140(*(void **)(param_1 + 0x38),100);
      *(UINT_PTR *)(param_1 + 0x58) = UVar6;
    }
    return 1;
  }
LAB_100047c9:
  iVar4 = (**(code **)(*(int *)*puVar1 + 8))((int *)*puVar1);
  while (iVar4 != 0) {
    iVar4 = (**(code **)(*(int *)*puVar1 + 8))((int *)*puVar1);
  }
  return 0;
}


// ============================================================================
// Function: Recorder_StopPlaybackPreview at 100047f0
// Signature: undefined __fastcall Recorder_StopPlaybackPreview(int param_1)
// Entry: 100047f0
// Body: 100047f0 - 10004846
// ============================================================================
void __fastcall Recorder_StopPlaybackPreview(int param_1)

{
  int *piVar1;
  int iVar2;
  undefined4 uVar3;
  
  if ((*(int *)(param_1 + 0x5c) != 0) && (piVar1 = *(int **)(param_1 + 0x60), piVar1 != (int *)0x0))
  {
    (**(code **)(*piVar1 + 0x48))(piVar1);
    iVar2 = (**(code **)(**(int **)(param_1 + 0x60) + 8))(*(int **)(param_1 + 0x60));
    while (iVar2 != 0) {
      iVar2 = (**(code **)(**(int **)(param_1 + 0x60) + 8))(*(int **)(param_1 + 0x60));
    }
    if (*(UINT_PTR *)(param_1 + 0x58) != 0) {
      uVar3 = FUN_10001180(*(void **)(param_1 + 0x38),*(UINT_PTR *)(param_1 + 0x58));
      *(undefined4 *)(param_1 + 0x58) = uVar3;
    }
    *(undefined4 *)(param_1 + 0x60) = 0;
    (**(code **)(*(int *)(param_1 + 8) + 0x28))(param_1 + 8);
  }
  return;
}


// ============================================================================
// Function: FUN_10004970 at 10004970
// Signature: undefined4 __stdcall FUN_10004970(int * param_1)
// Entry: 10004970
// Body: 10004970 - 10004991
// ============================================================================
undefined4 FUN_10004970(int *param_1)

{
  if (param_1[0xb] != 0) {
    (**(code **)(*param_1 + 0x2c))(param_1);
    (**(code **)(*(int *)param_1[0xb] + 0x34))(0);
  }
  return 0;
}


// ============================================================================
// Function: FUN_100049d0 at 100049d0
// Signature: undefined __stdcall FUN_100049d0(void * param_1)
// Entry: 100049d0
// Body: 100049d0 - 100049dd
// ============================================================================
void FUN_100049d0(void *param_1)

{
  operator_delete(param_1);
  return;
}


// ============================================================================
// Function: Recorder_StartCaptureFromInnerObject at 10004a80
// Signature: bool __stdcall Recorder_StartCaptureFromInnerObject(int * param_1)
// Entry: 10004a80
// Body: 10004a80 - 10004aa2
// ============================================================================
bool Recorder_StartCaptureFromInnerObject(int *param_1)

{
  bool bVar1;
  undefined3 extraout_var;
  
  (**(code **)(*param_1 + 0x2c))(param_1);
  param_1[0xf] = (int)param_1;
  bVar1 = Recorder_StartCapture((int)(param_1 + -1));
  return (bool)('\x01' - (CONCAT31(extraout_var,bVar1) != 0));
}


// ============================================================================
// Function: FUN_10004ab0 at 10004ab0
// Signature: undefined4 __stdcall FUN_10004ab0(int param_1)
// Entry: 10004ab0
// Body: 10004ab0 - 10004ad8
// ============================================================================
undefined4 FUN_10004ab0(int param_1)

{
  int iVar1;
  
  iVar1 = Recorder_StopCapture(param_1 + -4);
  if (iVar1 == 0) {
    Recorder_StopPlaybackPreview(param_1 + -4);
    return 0;
  }
  return 1;
}


// ============================================================================
// Function: FUN_10004c90 at 10004c90
// Signature: undefined4 __stdcall FUN_10004c90(int param_1)
// Entry: 10004c90
// Body: 10004c90 - 10004cab
// ============================================================================
undefined4 FUN_10004c90(int param_1)

{
  int *piVar1;
  
  piVar1 = *(int **)(param_1 + 0x10);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 0x1c))(piVar1);
  }
  return 0;
}


// ============================================================================
// Function: FUN_10004cb0 at 10004cb0
// Signature: undefined4 __stdcall FUN_10004cb0(int * param_1)
// Entry: 10004cb0
// Body: 10004cb0 - 10004cd5
// ============================================================================
undefined4 FUN_10004cb0(int *param_1)

{
  int *piVar1;
  
  piVar1 = (int *)param_1[4];
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 0x20))(piVar1);
  }
  (**(code **)(*param_1 + 0x2c))(param_1,0);
  return 0;
}


// ============================================================================
// Function: FUN_10004ce0 at 10004ce0
// Signature: undefined4 __stdcall FUN_10004ce0(int * param_1)
// Entry: 10004ce0
// Body: 10004ce0 - 10004d05
// ============================================================================
undefined4 FUN_10004ce0(int *param_1)

{
  int *piVar1;
  
  piVar1 = (int *)param_1[4];
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 0x28))(piVar1);
  }
  (**(code **)(*param_1 + 0x2c))(param_1,0);
  return 0;
}


// ============================================================================
// Function: FUN_10004d10 at 10004d10
// Signature: undefined4 __stdcall FUN_10004d10(int param_1, undefined4 param_2)
// Entry: 10004d10
// Body: 10004d10 - 10004d2f
// ============================================================================
undefined4 FUN_10004d10(int param_1,undefined4 param_2)

{
  int *piVar1;
  
  piVar1 = *(int **)(param_1 + 0x10);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 0x24))(piVar1,param_2);
  }
  return 0;
}


// ============================================================================
// Function: FUN_10004d30 at 10004d30
// Signature: undefined4 __stdcall FUN_10004d30(int param_1, undefined4 param_2)
// Entry: 10004d30
// Body: 10004d30 - 10004d4f
// ============================================================================
undefined4 FUN_10004d30(int param_1,undefined4 param_2)

{
  int *piVar1;
  
  piVar1 = *(int **)(param_1 + 0x10);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 0x2c))(piVar1,param_2);
  }
  return 0;
}


// ============================================================================
// Function: FUN_10004d50 at 10004d50
// Signature: CPropertyPage * __stdcall FUN_10004d50(void)
// Entry: 10004d50
// Body: 10004d50 - 10004da9
// ============================================================================
CPropertyPage * FUN_10004d50(void)

{
  CPropertyPage *pCVar1;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10005b8a;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  pCVar1 = operator_new(0xd4);
  local_4 = 0;
  if (pCVar1 != (CPropertyPage *)0x0) {
    pCVar1 = FUN_10004dd0(pCVar1);
    ExceptionList = local_c;
    return pCVar1;
  }
  ExceptionList = local_c;
  return (CPropertyPage *)0x0;
}


// ============================================================================
// Function: FUN_10004dd0 at 10004dd0
// Signature: CPropertyPage * __fastcall FUN_10004dd0(CPropertyPage * param_1)
// Entry: 10004dd0
// Body: 10004dd0 - 10004e2d
// ============================================================================
CPropertyPage * __fastcall FUN_10004dd0(CPropertyPage *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10005ba8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  CPropertyPage::CPropertyPage(param_1,0xca,0);
  local_4 = 0;
  CWnd::CWnd((CWnd *)(param_1 + 0x94));
  *(undefined ***)(param_1 + 0x94) = &PTR_LAB_10006d48;
  *(undefined ***)param_1 = &PTR_LAB_10006c50;
  ExceptionList = local_c;
  return param_1;
}


// ============================================================================
// Function: FUN_10004e30 at 10004e30
// Signature: CPropertyPage * __thiscall FUN_10004e30(void * this, byte param_1)
// Entry: 10004e30
// Body: 10004e30 - 10004e4d
// ============================================================================
CPropertyPage * __thiscall FUN_10004e30(void *this,byte param_1)

{
  FUN_10004e50(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: FUN_10004e50 at 10004e50
// Signature: undefined __fastcall FUN_10004e50(CPropertyPage * param_1)
// Entry: 10004e50
// Body: 10004e50 - 10004ea4
// ============================================================================
void __fastcall FUN_10004e50(CPropertyPage *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_10005bc8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined ***)param_1 = &PTR_LAB_10006c50;
  local_4 = 0;
  CComboBox::~CComboBox((CComboBox *)(param_1 + 0x94));
  local_4 = 0xffffffff;
  CPropertyPage::~CPropertyPage(param_1);
  ExceptionList = local_c;
  return;
}


// ============================================================================
// Function: FUN_10004f90 at 10004f90
// Signature: undefined __fastcall FUN_10004f90(CPropertyPage * param_1)
// Entry: 10004f90
// Body: 10004f90 - 10004fba
// ============================================================================
void __fastcall FUN_10004f90(CPropertyPage *param_1)

{
  LRESULT LVar1;
  
  LVar1 = SendMessageA(*(HWND *)(param_1 + 0xb4),0x147,0,0);
  *(LRESULT *)(*(int *)(param_1 + 0x90) + 0x34) = LVar1;
  CPropertyPage::OnOK(param_1);
  return;
}


// ============================================================================
// Function: FUN_10004fc0 at 10004fc0
// Signature: CComboBox * __thiscall FUN_10004fc0(void * this, byte param_1)
// Entry: 10004fc0
// Body: 10004fc0 - 10004fdd
// ============================================================================
CComboBox * __thiscall FUN_10004fc0(void *this,byte param_1)

{
  CComboBox::~CComboBox(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: DestroyWindow at 10005058
// Signature: int __thiscall DestroyWindow(CWnd * this)
// Entry: 10005058
// Body: 10005058 - 1000505d
// ============================================================================
int __thiscall CWnd::DestroyWindow(CWnd *this)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10005058. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = DestroyWindow(this);
  return iVar1;
}


// ============================================================================
// Function: ~CWnd at 100050d0
// Signature: void __thiscall ~CWnd(CWnd * this)
// Entry: 100050d0
// Body: 100050d0 - 100050d5
// ============================================================================
void __thiscall CWnd::~CWnd(CWnd *this)

{
                    /* WARNING: Could not recover jumptable at 0x100050d0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~CWnd(this);
  return;
}


// ============================================================================
// Function: CreateEx at 100050d6
// Signature: int __thiscall CreateEx(CWnd * this, ulong param_1, char * param_2, char * param_3, ulong param_4, int param_5, int param_6, int param_7, int param_8, HWND__ * param_9, HMENU__ * param_10, void * param_11)
// Entry: 100050d6
// Body: 100050d6 - 100050db
// ============================================================================
int __thiscall
CWnd::CreateEx(CWnd *this,ulong param_1,char *param_2,char *param_3,ulong param_4,int param_5,
              int param_6,int param_7,int param_8,HWND__ *param_9,HMENU__ *param_10,void *param_11)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x100050d6. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = CreateEx(this,param_1,param_2,param_3,param_4,param_5,param_6,param_7,param_8,param_9,
                   param_10,param_11);
  return iVar1;
}


// ============================================================================
// Function: AfxRegisterWndClass at 100050dc
// Signature: char * __stdcall AfxRegisterWndClass(uint param_1, HICON__ * param_2, HBRUSH__ * param_3, HICON__ * param_4)
// Entry: 100050dc
// Body: 100050dc - 100050e1
// ============================================================================
char * AfxRegisterWndClass(uint param_1,HICON__ *param_2,HBRUSH__ *param_3,HICON__ *param_4)

{
  char *pcVar1;
  
                    /* WARNING: Could not recover jumptable at 0x100050dc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pcVar1 = AfxRegisterWndClass(param_1,param_2,param_3,param_4);
  return pcVar1;
}


// ============================================================================
// Function: CWnd at 100050e2
// Signature: undefined __thiscall CWnd(CWnd * this)
// Entry: 100050e2
// Body: 100050e2 - 100050e7
// ============================================================================
void __thiscall CWnd::CWnd(CWnd *this)

{
                    /* WARNING: Could not recover jumptable at 0x100050e2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  CWnd(this);
  return;
}


// ============================================================================
// Function: operator_delete at 100050e8
// Signature: void __cdecl operator_delete(void * param_1)
// Entry: 100050e8
// Body: 100050e8 - 100050ed
// ============================================================================
void __cdecl operator_delete(void *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x100050e8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  operator_delete(param_1);
  return;
}


// ============================================================================
// Function: ~CPropertySheet at 1000511e
// Signature: void __thiscall ~CPropertySheet(CPropertySheet * this)
// Entry: 1000511e
// Body: 1000511e - 10005123
// ============================================================================
void __thiscall CPropertySheet::~CPropertySheet(CPropertySheet *this)

{
                    /* WARNING: Could not recover jumptable at 0x1000511e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~CPropertySheet(this);
  return;
}


// ============================================================================
// Function: AddPage at 10005124
// Signature: void __thiscall AddPage(CPropertySheet * this, CPropertyPage * param_1)
// Entry: 10005124
// Body: 10005124 - 10005129
// ============================================================================
void __thiscall CPropertySheet::AddPage(CPropertySheet *this,CPropertyPage *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x10005124. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AddPage(this,param_1);
  return;
}


// ============================================================================
// Function: CPropertySheet at 1000512a
// Signature: undefined __thiscall CPropertySheet(CPropertySheet * this, uint param_1, CWnd * param_2, uint param_3)
// Entry: 1000512a
// Body: 1000512a - 1000512f
// ============================================================================
void __thiscall
CPropertySheet::CPropertySheet(CPropertySheet *this,uint param_1,CWnd *param_2,uint param_3)

{
                    /* WARNING: Could not recover jumptable at 0x1000512a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  CPropertySheet(this,param_1,param_2,param_3);
  return;
}


// ============================================================================
// Function: ~CWinApp at 10005130
// Signature: void __thiscall ~CWinApp(CWinApp * this)
// Entry: 10005130
// Body: 10005130 - 10005135
// ============================================================================
void __thiscall CWinApp::~CWinApp(CWinApp *this)

{
                    /* WARNING: Could not recover jumptable at 0x10005130. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~CWinApp(this);
  return;
}


// ============================================================================
// Function: CWinApp at 100051ae
// Signature: undefined __thiscall CWinApp(CWinApp * this, char * param_1)
// Entry: 100051ae
// Body: 100051ae - 100051b3
// ============================================================================
void __thiscall CWinApp::CWinApp(CWinApp *this,char *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x100051ae. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  CWinApp(this,param_1);
  return;
}


// ============================================================================
// Function: InitInstance at 100051b4
// Signature: int __thiscall InitInstance(CWinApp * this)
// Entry: 100051b4
// Body: 100051b4 - 100051b9
// ============================================================================
int __thiscall CWinApp::InitInstance(CWinApp *this)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x100051b4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = InitInstance(this);
  return iVar1;
}


// ============================================================================
// Function: ExitInstance at 100051ba
// Signature: int __thiscall ExitInstance(CWinApp * this)
// Entry: 100051ba
// Body: 100051ba - 100051bf
// ============================================================================
int __thiscall CWinApp::ExitInstance(CWinApp *this)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x100051ba. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = ExitInstance(this);
  return iVar1;
}


// ============================================================================
// Function: AfxDllCanUnloadNow at 100051c0
// Signature: long __stdcall AfxDllCanUnloadNow(void)
// Entry: 100051c0
// Body: 100051c0 - 100051c5
// ============================================================================
long AfxDllCanUnloadNow(void)

{
  long lVar1;
  
                    /* WARNING: Could not recover jumptable at 0x100051c0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  lVar1 = AfxDllCanUnloadNow();
  return lVar1;
}


// ============================================================================
// Function: AFX_MAINTAIN_STATE2 at 100051c6
// Signature: undefined __thiscall AFX_MAINTAIN_STATE2(AFX_MAINTAIN_STATE2 * this, AFX_MODULE_STATE * param_1)
// Entry: 100051c6
// Body: 100051c6 - 100051cb
// ============================================================================
void __thiscall
AFX_MAINTAIN_STATE2::AFX_MAINTAIN_STATE2(AFX_MAINTAIN_STATE2 *this,AFX_MODULE_STATE *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x100051c6. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AFX_MAINTAIN_STATE2(this,param_1);
  return;
}


// ============================================================================
// Function: operator_new at 100051cc
// Signature: void * __cdecl operator_new(uint param_1)
// Entry: 100051cc
// Body: 100051cc - 100051d1
// ============================================================================
void * __cdecl operator_new(uint param_1)

{
  void *pvVar1;
  
                    /* WARNING: Could not recover jumptable at 0x100051cc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pvVar1 = operator_new(param_1);
  return pvVar1;
}


// ============================================================================
// Function: ~CString at 100051d2
// Signature: void __thiscall ~CString(CString * this)
// Entry: 100051d2
// Body: 100051d2 - 100051d7
// ============================================================================
void __thiscall CString::~CString(CString *this)

{
                    /* WARNING: Could not recover jumptable at 0x100051d2. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~CString(this);
  return;
}


// ============================================================================
// Function: Remove at 100051d8
// Signature: void __stdcall Remove(char * param_1)
// Entry: 100051d8
// Body: 100051d8 - 100051dd
// ============================================================================
void CFile::Remove(char *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x100051d8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  Remove(param_1);
  return;
}


// ============================================================================
// Function: AfxGetModuleState at 100051de
// Signature: AFX_MODULE_STATE * __stdcall AfxGetModuleState(void)
// Entry: 100051de
// Body: 100051de - 100051e3
// ============================================================================
AFX_MODULE_STATE * AfxGetModuleState(void)

{
  AFX_MODULE_STATE *pAVar1;
  
                    /* WARNING: Could not recover jumptable at 0x100051de. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pAVar1 = AfxGetModuleState();
  return pAVar1;
}

// clang-format on
