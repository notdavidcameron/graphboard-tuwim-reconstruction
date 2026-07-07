/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Notes: raw Ghidra decompiler output with only file-level framing added.
*/
#include "../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10003b60 at 10003b60
// Signature: undefined __fastcall FUN_10003b60(undefined4 * param_1)
// Entry: 10003b60
// Body: 10003b60 - 10003b69
// ============================================================================
void __fastcall FUN_10003b60(undefined4 *param_1)

{
  SysFreeString((BSTR)*param_1);
  return;
}


// ============================================================================
// Function: FUN_10003b70 at 10003b70
// Signature: int __stdcall FUN_10003b70(short * param_1)
// Entry: 10003b70
// Body: 10003b70 - 10003bab
// ============================================================================
int FUN_10003b70(short *param_1)

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
// Function: FUN_10003c40 at 10003c40
// Signature: HRESULT __thiscall FUN_10003c40(void * this, BSTR param_1)
// Entry: 10003c40
// Body: 10003c40 - 10003ed5
// ============================================================================
/* WARNING: Exceeded maximum restarts with more pending */

HRESULT __thiscall FUN_10003c40(void *this,BSTR param_1)

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
  puStack_8 = &DAT_1000528b;
  local_c = ExceptionList;
  if (*(int *)((int)this + 0xc) == 0) {
    local_1c = -0x7fffbffb;
    ExceptionList = &local_c;
    EnterCriticalSection((LPCRITICAL_SECTION)&DAT_100082b4);
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
        HVar3 = (*pIStack_28->lpVtbl->QueryInterface)(pIStack_28,(IID *)&DAT_10008130,&pIStack_20);
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
              FUN_10004ce3(pOVar4,0xc,uVar7,&LAB_10003fe0);
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
    LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_100082b4);
    ComTypeInfo_AddDispatchMapEntry(0x10008298,FUN_10003f80,this);
    ExceptionList = local_c;
    return local_1c;
  }
  return 0;
}


// ============================================================================
// Function: ComTypeInfo_AddDispatchMapEntry at 10003ee0
// Signature: undefined4 __stdcall ComTypeInfo_AddDispatchMapEntry(int param_1, undefined4 param_2, undefined4 param_3)
// Entry: 10003ee0
// Body: 10003ee0 - 10003f76
// ============================================================================
undefined4 ComTypeInfo_AddDispatchMapEntry(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_100052a0;
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
// Function: Catch@10003f2e at 10003f2e
// Signature: undefined4 __stdcall Catch@10003f2e(void)
// Entry: 10003f2e
// Body: 10003f2e - 10003f33
// ============================================================================
undefined4 Catch_10003f2e(void)

{
  return 0x10003f16;
}


// ============================================================================
// Function: FUN_10003f80 at 10003f80
// Signature: undefined __stdcall FUN_10003f80(Editor * param_1)
// Entry: 10003f80
// Body: 10003f80 - 10003fd4
// ============================================================================
void FUN_10003f80(Editor *param_1)

{
  int *piVar1;
  void *pvVar2;
  
  piVar1 = (param_1->secondaryDispatch).vtable;
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
  }
  pvVar2 = param_1->invalidateTarget;
  (param_1->secondaryDispatch).vtable = (void *)0x0;
  if (pvVar2 != (void *)0x0) {
    FUN_10004bef(pvVar2,0xc,*(int *)((int)pvVar2 + -4),FUN_10003b60);
    operator_delete((void *)((int)pvVar2 + -4));
    param_1->invalidateTarget = (void *)0x0;
    return;
  }
  param_1->invalidateTarget = (void *)0x0;
  return;
}


// ============================================================================
// Function: SetColorObject_Construct at 10004050
// Signature: undefined4 * __fastcall SetColorObject_Construct(undefined4 * param_1)
// Entry: 10004050
// Body: 10004050 - 1000407f
// ============================================================================
undefined4 * __fastcall SetColorObject_Construct(undefined4 *param_1)

{
  *param_1 = &PTR_LAB_100068d8;
  param_1[2] = 0;
  InitializeCriticalSection((LPCRITICAL_SECTION)(param_1 + 3));
  *param_1 = &PTR_LAB_100068b8;
  param_1[1] = &PTR_LAB_100068a8;
  CpuFeatures_DetectMmxSupport();
  return param_1;
}


// ============================================================================
// Function: SetColor_ShowColorIndexDialog at 10004080
// Signature: undefined4 __stdcall SetColor_ShowColorIndexDialog(undefined4 param_1, byte * param_2, undefined4 param_3, int * param_4)
// Entry: 10004080
// Body: 10004080 - 10004276
// ============================================================================
undefined4
SetColor_ShowColorIndexDialog(undefined4 param_1,byte *param_2,undefined4 param_3,int *param_4)

{
  byte bVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  AFX_MODULE_STATE *pAVar4;
  int iVar5;
  HGLOBAL pvVar6;
  undefined **local_25c [25];
  LPCVOID local_1f8;
  uint local_1f0;
  int *local_1e8;
  CString aCStack_1e4 [4];
  CString aCStack_1e0 [4];
  CString aCStack_1dc [4];
  CString aCStack_1d8 [4];
  byte *local_1d4;
  CStatic aCStack_1d0 [64];
  CStatic aCStack_190 [64];
  CStatic aCStack_150 [64];
  CButton aCStack_110 [64];
  CStatic aCStack_d0 [64];
  CStatic aCStack_90 [64];
  CButton local_50 [64];
  undefined4 local_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10005334;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  pAVar4 = AfxGetModuleState();
  uVar3 = DAT_100082a0;
  uVar2 = *(undefined4 *)(pAVar4 + 0xc);
  pAVar4 = AfxGetModuleState();
  *(undefined4 *)(pAVar4 + 0xc) = uVar3;
  FUN_10001000(local_25c,param_3);
  local_4 = 0;
  local_1e8 = param_4;
  bVar1 = *param_2;
  local_1f0 = (uint)bVar1;
  if (param_4 != (int *)0x0) {
    local_10 = 1;
  }
  local_1d4 = param_2;
  iVar5 = CDialog::DoModal((CDialog *)local_25c);
  if (iVar5 == 1) {
    *param_2 = (byte)local_1f0;
  }
  else {
    *param_2 = bVar1;
    if (local_1e8 != (int *)0x0) {
      (**(code **)(*local_1e8 + 0xc))(local_1e8);
    }
  }
  pAVar4 = AfxGetModuleState();
  *(undefined4 *)(pAVar4 + 0xc) = uVar2;
  local_25c[0] = &PTR_LAB_100063e8;
  local_4 = 0xb;
  if (local_1f8 != (LPCVOID)0x0) {
    pvVar6 = GlobalHandle(local_1f8);
    GlobalUnlock(pvVar6);
    pvVar6 = GlobalHandle(local_1f8);
    GlobalFree(pvVar6);
  }
  CButton::~CButton(local_50);
  local_4._0_1_ = 10;
  CStatic::~CStatic(aCStack_90);
  local_4._0_1_ = 9;
  CStatic::~CStatic(aCStack_d0);
  local_4._0_1_ = 8;
  CButton::~CButton(aCStack_110);
  local_4._0_1_ = 7;
  CStatic::~CStatic(aCStack_150);
  local_4._0_1_ = 6;
  CStatic::~CStatic(aCStack_190);
  local_4._0_1_ = 5;
  CStatic::~CStatic(aCStack_1d0);
  local_4._0_1_ = 4;
  CString::~CString(aCStack_1d8);
  local_4._0_1_ = 3;
  CString::~CString(aCStack_1dc);
  local_4._0_1_ = 2;
  CString::~CString(aCStack_1e0);
  local_4 = CONCAT31(local_4._1_3_,1);
  CString::~CString(aCStack_1e4);
  local_4 = 0xffffffff;
  CDialog::~CDialog((CDialog *)local_25c);
  ExceptionList = pvStack_c;
  return 0;
}


// ============================================================================
// Function: CpuFeatures_DetectMmxSupport at 10004280
// Signature: undefined4 __stdcall CpuFeatures_DetectMmxSupport(void)
// Entry: 10004280
// Body: 10004280 - 100042c5
// ============================================================================
/* WARNING: Removing unreachable block (ram,0x100042b1) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 CpuFeatures_DetectMmxSupport(void)

{
  undefined4 *puVar1;
  uint uVar2;
  undefined4 uVar3;
  byte in_CF;
  byte in_PF;
  byte in_AF;
  byte in_ZF;
  byte in_SF;
  byte in_TF;
  byte in_IF;
  byte in_OF;
  byte in_NT;
  byte in_AC;
  byte in_VIF;
  byte in_VIP;
  byte in_ID;
  uint uVar4;
  
  _DAT_10008320 = 0;
  uVar4 = (uint)(in_NT & 1) * 0x4000 | (uint)(in_OF & 1) * 0x800 | (uint)(in_IF & 1) * 0x200 |
          (uint)(in_TF & 1) * 0x100 | (uint)(in_SF & 1) * 0x80 | (uint)(in_ZF & 1) * 0x40 |
          (uint)(in_AF & 1) * 0x10 | (uint)(in_PF & 1) * 4 | (uint)(in_CF & 1) |
          (uint)(in_ID & 1) * 0x200000 | (uint)(in_VIP & 1) * 0x100000 |
          (uint)(in_VIF & 1) * 0x80000 | (uint)(in_AC & 1) * 0x40000;
  uVar2 = uVar4 ^ 0x40000;
  uVar3 = 0;
  if (((uint)((uVar2 & 0x4000) != 0) * 0x4000 | (uint)((uVar2 & 0x800) != 0) * 0x800 |
       (uint)((uVar2 & 0x200) != 0) * 0x200 | (uint)((uVar2 & 0x100) != 0) * 0x100 |
       (uint)((uVar2 & 0x80) != 0) * 0x80 | (uint)((uVar2 & 0x40) != 0) * 0x40 |
       (uint)((uVar2 & 0x10) != 0) * 0x10 | (uint)((uVar2 & 4) != 0) * 4 | (uint)((uVar2 & 1) != 0)
       | (uint)((uVar2 & 0x200000) != 0) * 0x200000 | (uint)((uVar2 & 0x40000) != 0) * 0x40000) !=
      uVar4) {
    uVar2 = uVar4 ^ 0x200000;
    uVar3 = 0;
    if (((uint)((uVar2 & 0x4000) != 0) * 0x4000 | (uint)((uVar2 & 0x800) != 0) * 0x800 |
         (uint)((uVar2 & 0x200) != 0) * 0x200 | (uint)((uVar2 & 0x100) != 0) * 0x100 |
         (uint)((uVar2 & 0x80) != 0) * 0x80 | (uint)((uVar2 & 0x40) != 0) * 0x40 |
         (uint)((uVar2 & 0x10) != 0) * 0x10 | (uint)((uVar2 & 4) != 0) * 4 |
         (uint)((uVar2 & 1) != 0) | (uint)((uVar2 & 0x200000) != 0) * 0x200000 |
        (uint)((uVar2 & 0x40000) != 0) * 0x40000) != uVar4) {
      puVar1 = (undefined4 *)cpuid_Version_info(1);
      uVar3 = *puVar1;
      if ((puVar1[2] & 0x800000) != 0) {
        _DAT_10008320 = 1;
      }
    }
  }
  return uVar3;
}


// ============================================================================
// Function: DibUtil_CreateIndexedDib at 10004450
// Signature: undefined4 * __stdcall DibUtil_CreateIndexedDib(int param_1, int param_2, int param_3)
// Entry: 10004450
// Body: 10004450 - 100045a8
// ============================================================================
undefined4 * DibUtil_CreateIndexedDib(int param_1,int param_2,int param_3)

{
  HGLOBAL hMem;
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  
  iVar3 = (((int)(param_1 * param_2 + (param_1 * param_2 >> 0x1f & 7U)) >> 3) + 3U & 0xfffffffc) *
          param_3;
  hMem = GlobalAlloc(0x42,iVar3 + 0x428);
  puVar1 = GlobalLock(hMem);
  if (puVar1 == (undefined4 *)0x0) {
    return (undefined4 *)0x0;
  }
  *puVar1 = 0x28;
  puVar1[1] = param_2;
  puVar1[2] = param_3;
  *(undefined2 *)(puVar1 + 3) = 1;
  *(short *)((int)puVar1 + 0xe) = (short)param_1;
  puVar1[4] = 0;
  puVar1[5] = iVar3;
  puVar1[6] = 0;
  puVar1[7] = 0;
  puVar1[8] = 0;
  puVar1[9] = 0;
  if (param_1 == 4) {
    puVar1[8] = 0x10;
  }
  else if (param_1 == 8) {
    puVar1[8] = 0x100;
  }
  iVar3 = 0;
  puVar2 = puVar1 + 10;
  if (0 < (int)(puVar1[8] + ((int)puVar1[8] >> 0x1f & 0xfU)) >> 4) {
    do {
      *puVar2 = 0;
      puVar2[1] = 0x800000;
      puVar2[2] = 0x8000;
      puVar2[3] = 0x808000;
      puVar2[4] = 0x80;
      puVar2[5] = 0x800080;
      puVar2[6] = 0x8080;
      puVar2[7] = 0xc0c0c0;
      puVar2[8] = 0x808080;
      puVar2[9] = 0xff0000;
      puVar2[10] = 0xff00;
      puVar2[0xb] = 0xffff00;
      puVar2[0xc] = 0xff;
      puVar2[0xd] = 0xff00ff;
      puVar2[0xe] = 0xffff;
      puVar2[0xf] = 0xffffff;
      puVar2 = puVar2 + 0x10;
      iVar3 = iVar3 + 1;
    } while (iVar3 < (int)(puVar1[8] + ((int)puVar1[8] >> 0x1f & 0xfU)) >> 4);
  }
  return puVar1;
}


// ============================================================================
// Function: DibUtil_GetIndexedPixelAddress at 100045b0
// Signature: int __stdcall DibUtil_GetIndexedPixelAddress(int param_1, uint param_2, uint param_3)
// Entry: 100045b0
// Body: 100045b0 - 10004607
// ============================================================================
int DibUtil_GetIndexedPixelAddress(int param_1,uint param_2,uint param_3)

{
  int iVar1;
  
  if (param_2 < *(uint *)(param_1 + 4)) {
    if (param_3 < *(uint *)(param_1 + 8)) {
      iVar1 = *(int *)(param_1 + 0x20);
      if ((iVar1 == 0) && (*(ushort *)(param_1 + 0xe) < 9)) {
        iVar1 = 1 << ((byte)*(ushort *)(param_1 + 0xe) & 0x1f);
      }
      return param_1 + ((*(uint *)(param_1 + 8) - param_3) + -1) *
                       (*(uint *)(param_1 + 4) + 3 & 0xfffffffc) + 0x28 + iVar1 * 4 + param_2;
    }
  }
  return 0;
}


// ============================================================================
// Function: DibUtil_FillIndexedPixels at 10004610
// Signature: undefined4 __stdcall DibUtil_FillIndexedPixels(uint param_1, undefined4 * param_2, undefined1 param_3)
// Entry: 10004610
// Body: 10004610 - 10004691
// ============================================================================
undefined4 DibUtil_FillIndexedPixels(uint param_1,undefined4 *param_2,undefined1 param_3)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  
  if (((param_1 != 0) && (param_2 != (undefined4 *)0x0)) && (((uint)param_2 & 3) == 0)) {
    uVar2 = *(uint *)(param_1 + 0x14);
    if (*(uint *)(param_1 + 0x14) == 0) {
      iVar1 = *(int *)(param_1 + 8);
      if (iVar1 < 0) {
        iVar1 = -iVar1;
      }
      uVar2 = ((uint)*(ushort *)(param_1 + 0xe) * *(int *)(param_1 + 4) + 0x1f >> 3 & 0x1ffffffc) *
              iVar1;
    }
    param_1 = uVar2;
    uVar3 = param_1 & 3;
    for (uVar2 = param_1 >> 2; uVar2 != 0; uVar2 = uVar2 - 1) {
      *param_2 = CONCAT22(CONCAT11(param_3,param_3),CONCAT11(param_3,param_3));
      param_2 = param_2 + 1;
    }
    for (; uVar3 != 0; uVar3 = uVar3 - 1) {
      *(undefined1 *)param_2 = param_3;
      param_2 = (undefined4 *)((int)param_2 + 1);
    }
    return 1;
  }
  return 0;
}


// ============================================================================
// Function: DoModal at 100046b8
// Signature: int __thiscall DoModal(CDialog * this)
// Entry: 100046b8
// Body: 100046b8 - 100046bd
// ============================================================================
int __thiscall CDialog::DoModal(CDialog *this)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x100046b8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = DoModal(this);
  return iVar1;
}


// ============================================================================
// Function: ~CButton at 100047e4
// Signature: void __thiscall ~CButton(CButton * this)
// Entry: 100047e4
// Body: 100047e4 - 100047e9
// ============================================================================
void __thiscall CButton::~CButton(CButton *this)

{
                    /* WARNING: Could not recover jumptable at 0x100047e4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~CButton(this);
  return;
}


// ============================================================================
// Function: ~CStatic at 100047ea
// Signature: void __thiscall ~CStatic(CStatic * this)
// Entry: 100047ea
// Body: 100047ea - 100047ef
// ============================================================================
void __thiscall CStatic::~CStatic(CStatic *this)

{
                    /* WARNING: Could not recover jumptable at 0x100047ea. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~CStatic(this);
  return;
}


// ============================================================================
// Function: ~CString at 100047f0
// Signature: void __thiscall ~CString(CString * this)
// Entry: 100047f0
// Body: 100047f0 - 100047f5
// ============================================================================
void __thiscall CString::~CString(CString *this)

{
                    /* WARNING: Could not recover jumptable at 0x100047f0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~CString(this);
  return;
}


// ============================================================================
// Function: ~CDialog at 100047f6
// Signature: void __thiscall ~CDialog(CDialog * this)
// Entry: 100047f6
// Body: 100047f6 - 100047fb
// ============================================================================
void __thiscall CDialog::~CDialog(CDialog *this)

{
                    /* WARNING: Could not recover jumptable at 0x100047f6. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~CDialog(this);
  return;
}


// ============================================================================
// Function: CWnd at 100047fc
// Signature: undefined __thiscall CWnd(CWnd * this)
// Entry: 100047fc
// Body: 100047fc - 10004801
// ============================================================================
void __thiscall CWnd::CWnd(CWnd *this)

{
                    /* WARNING: Could not recover jumptable at 0x100047fc. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  CWnd(this);
  return;
}


// ============================================================================
// Function: CString at 10004802
// Signature: undefined __thiscall CString(CString * this)
// Entry: 10004802
// Body: 10004802 - 10004807
// ============================================================================
void __thiscall CString::CString(CString *this)

{
                    /* WARNING: Could not recover jumptable at 0x10004802. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  CString(this);
  return;
}


// ============================================================================
// Function: CDialog at 10004808
// Signature: undefined __thiscall CDialog(CDialog * this, uint param_1, CWnd * param_2)
// Entry: 10004808
// Body: 10004808 - 1000480d
// ============================================================================
void __thiscall CDialog::CDialog(CDialog *this,uint param_1,CWnd *param_2)

{
                    /* WARNING: Could not recover jumptable at 0x10004808. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  CDialog(this,param_1,param_2);
  return;
}


// ============================================================================
// Function: operator_delete at 1000480e
// Signature: void __cdecl operator_delete(void * param_1)
// Entry: 1000480e
// Body: 1000480e - 10004813
// ============================================================================
void __cdecl operator_delete(void *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x1000480e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  operator_delete(param_1);
  return;
}


// ============================================================================
// Function: operator= at 10004814
// Signature: CString * __thiscall operator=(CString * this, CString * param_1)
// Entry: 10004814
// Body: 10004814 - 10004819
// ============================================================================
CString * __thiscall CString::operator=(CString *this,CString *param_1)

{
  CString *pCVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10004814. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pCVar1 = operator=(this,param_1);
  return pCVar1;
}


// ============================================================================
// Function: operator+ at 1000481a
// Signature: CString __stdcall operator+(CString * param_1, CString * param_2)
// Entry: 1000481a
// Body: 1000481a - 1000481f
// ============================================================================
void operator+(CString *param_1,CString *param_2)

{
                    /* WARNING: Could not recover jumptable at 0x1000481a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  operator+(param_1,param_2);
  return;
}


// ============================================================================
// Function: SetWindowTextA at 10004820
// Signature: void __thiscall SetWindowTextA(CWnd * this, char * param_1)
// Entry: 10004820
// Body: 10004820 - 10004825
// ============================================================================
void __thiscall CWnd::SetWindowTextA(CWnd *this,char *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x10004820. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  SetWindowTextA(this,param_1);
  return;
}


// ============================================================================
// Function: Format at 10004826
// Signature: void __thiscall Format(CString * this, char * param_1, ...)
// Entry: 10004826
// Body: 10004826 - 1000482b
// ============================================================================
void __thiscall CString::Format(CString *this,char *param_1,...)

{
                    /* WARNING: Could not recover jumptable at 0x10004826. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  Format(this,param_1);
  return;
}


// ============================================================================
// Function: GetWindowTextA at 1000482c
// Signature: void __thiscall GetWindowTextA(CWnd * this, CString * param_1)
// Entry: 1000482c
// Body: 1000482c - 10004831
// ============================================================================
void __thiscall CWnd::GetWindowTextA(CWnd *this,CString *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x1000482c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  GetWindowTextA(this,param_1);
  return;
}


// ============================================================================
// Function: EnableWindow at 10004832
// Signature: int __thiscall EnableWindow(CWnd * this, int param_1)
// Entry: 10004832
// Body: 10004832 - 10004837
// ============================================================================
int __thiscall CWnd::EnableWindow(CWnd *this,int param_1)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10004832. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = EnableWindow(this,param_1);
  return iVar1;
}


// ============================================================================
// Function: DDX_Check at 10004838
// Signature: void __stdcall DDX_Check(CDataExchange * param_1, int param_2, int * param_3)
// Entry: 10004838
// Body: 10004838 - 1000483d
// ============================================================================
void DDX_Check(CDataExchange *param_1,int param_2,int *param_3)

{
                    /* WARNING: Could not recover jumptable at 0x10004838. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  DDX_Check(param_1,param_2,param_3);
  return;
}


// ============================================================================
// Function: DDX_Control at 1000483e
// Signature: void __stdcall DDX_Control(CDataExchange * param_1, int param_2, CWnd * param_3)
// Entry: 1000483e
// Body: 1000483e - 10004843
// ============================================================================
void DDX_Control(CDataExchange *param_1,int param_2,CWnd *param_3)

{
                    /* WARNING: Could not recover jumptable at 0x1000483e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  DDX_Control(param_1,param_2,param_3);
  return;
}


// ============================================================================
// Function: UpdateData at 10004844
// Signature: int __thiscall UpdateData(CWnd * this, int param_1)
// Entry: 10004844
// Body: 10004844 - 10004849
// ============================================================================
int __thiscall CWnd::UpdateData(CWnd *this,int param_1)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10004844. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = UpdateData(this,param_1);
  return iVar1;
}


// ============================================================================
// Function: AfxMessageBox at 1000484a
// Signature: int __stdcall AfxMessageBox(char * param_1, uint param_2, uint param_3)
// Entry: 1000484a
// Body: 1000484a - 1000484f
// ============================================================================
int AfxMessageBox(char *param_1,uint param_2,uint param_3)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x1000484a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = AfxMessageBox(param_1,param_2,param_3);
  return iVar1;
}


// ============================================================================
// Function: ShowWindow at 10004850
// Signature: int __thiscall ShowWindow(CWnd * this, int param_1)
// Entry: 10004850
// Body: 10004850 - 10004855
// ============================================================================
int __thiscall CWnd::ShowWindow(CWnd *this,int param_1)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10004850. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = ShowWindow(this,param_1);
  return iVar1;
}


// ============================================================================
// Function: OnInitDialog at 10004856
// Signature: int __thiscall OnInitDialog(CDialog * this)
// Entry: 10004856
// Body: 10004856 - 1000485b
// ============================================================================
int __thiscall CDialog::OnInitDialog(CDialog *this)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10004856. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = OnInitDialog(this);
  return iVar1;
}


// ============================================================================
// Function: Default at 1000485c
// Signature: long __thiscall Default(CWnd * this)
// Entry: 1000485c
// Body: 1000485c - 10004861
// ============================================================================
long __thiscall CWnd::Default(CWnd *this)

{
  long lVar1;
  
                    /* WARNING: Could not recover jumptable at 0x1000485c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  lVar1 = Default(this);
  return lVar1;
}


// ============================================================================
// Function: ~CPaintDC at 1000486e
// Signature: void __thiscall ~CPaintDC(CPaintDC * this)
// Entry: 1000486e
// Body: 1000486e - 10004873
// ============================================================================
void __thiscall CPaintDC::~CPaintDC(CPaintDC *this)

{
                    /* WARNING: Could not recover jumptable at 0x1000486e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~CPaintDC(this);
  return;
}


// ============================================================================
// Function: SelectPalette at 1000487a
// Signature: CPalette * __thiscall SelectPalette(CDC * this, CPalette * param_1, int param_2)
// Entry: 1000487a
// Body: 1000487a - 1000487f
// ============================================================================
CPalette * __thiscall CDC::SelectPalette(CDC *this,CPalette *param_1,int param_2)

{
  CPalette *pCVar1;
  
                    /* WARNING: Could not recover jumptable at 0x1000487a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pCVar1 = SelectPalette(this,param_1,param_2);
  return pCVar1;
}


// ============================================================================
// Function: CClientDC at 10004880
// Signature: undefined __thiscall CClientDC(CClientDC * this, CWnd * param_1)
// Entry: 10004880
// Body: 10004880 - 10004885
// ============================================================================
void __thiscall CClientDC::CClientDC(CClientDC *this,CWnd *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x10004880. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  CClientDC(this,param_1);
  return;
}


// ============================================================================
// Function: operator_new at 10004886
// Signature: void * __cdecl operator_new(uint param_1)
// Entry: 10004886
// Body: 10004886 - 1000488b
// ============================================================================
void * __cdecl operator_new(uint param_1)

{
  void *pvVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10004886. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pvVar1 = operator_new(param_1);
  return pvVar1;
}


// ============================================================================
// Function: ~CWinApp at 1000488c
// Signature: void __thiscall ~CWinApp(CWinApp * this)
// Entry: 1000488c
// Body: 1000488c - 10004891
// ============================================================================
void __thiscall CWinApp::~CWinApp(CWinApp *this)

{
                    /* WARNING: Could not recover jumptable at 0x1000488c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~CWinApp(this);
  return;
}


// ============================================================================
// Function: CWinApp at 1000490a
// Signature: undefined __thiscall CWinApp(CWinApp * this, char * param_1)
// Entry: 1000490a
// Body: 1000490a - 1000490f
// ============================================================================
void __thiscall CWinApp::CWinApp(CWinApp *this,char *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x1000490a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  CWinApp(this,param_1);
  return;
}


// ============================================================================
// Function: InitInstance at 10004910
// Signature: int __thiscall InitInstance(CWinApp * this)
// Entry: 10004910
// Body: 10004910 - 10004915
// ============================================================================
int __thiscall CWinApp::InitInstance(CWinApp *this)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10004910. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = InitInstance(this);
  return iVar1;
}


// ============================================================================
// Function: ExitInstance at 10004916
// Signature: int __thiscall ExitInstance(CWinApp * this)
// Entry: 10004916
// Body: 10004916 - 1000491b
// ============================================================================
int __thiscall CWinApp::ExitInstance(CWinApp *this)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10004916. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = ExitInstance(this);
  return iVar1;
}


// ============================================================================
// Function: AfxDllCanUnloadNow at 1000491c
// Signature: long __stdcall AfxDllCanUnloadNow(void)
// Entry: 1000491c
// Body: 1000491c - 10004921
// ============================================================================
long AfxDllCanUnloadNow(void)

{
  long lVar1;
  
                    /* WARNING: Could not recover jumptable at 0x1000491c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  lVar1 = AfxDllCanUnloadNow();
  return lVar1;
}


// ============================================================================
// Function: AFX_MAINTAIN_STATE2 at 10004922
// Signature: undefined __thiscall AFX_MAINTAIN_STATE2(AFX_MAINTAIN_STATE2 * this, AFX_MODULE_STATE * param_1)
// Entry: 10004922
// Body: 10004922 - 10004927
// ============================================================================
void __thiscall
AFX_MAINTAIN_STATE2::AFX_MAINTAIN_STATE2(AFX_MAINTAIN_STATE2 *this,AFX_MODULE_STATE *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x10004922. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AFX_MAINTAIN_STATE2(this,param_1);
  return;
}


// ============================================================================
// Function: AfxGetModuleState at 10004928
// Signature: AFX_MODULE_STATE * __stdcall AfxGetModuleState(void)
// Entry: 10004928
// Body: 10004928 - 1000492d
// ============================================================================
AFX_MODULE_STATE * AfxGetModuleState(void)

{
  AFX_MODULE_STATE *pAVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10004928. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pAVar1 = AfxGetModuleState();
  return pAVar1;
}


// ============================================================================
// Function: FUN_10004938 at 10004938
// Signature: undefined4 * __stdcall FUN_10004938(void)
// Entry: 10004938
// Body: 10004938 - 1000495f
// ============================================================================
undefined4 * FUN_10004938(void)

{
  AFX_MODULE_STATE::AFX_MODULE_STATE((AFX_MODULE_STATE *)&DAT_10008330,1,FUN_10004996,0x600);
  DAT_10008330 = &PTR_FUN_100068fc;
  return &DAT_10008330;
}


// ============================================================================
// Function: FUN_10004960 at 10004960
// Signature: AFX_MODULE_STATE * __thiscall FUN_10004960(void * this, byte param_1)
// Entry: 10004960
// Body: 10004960 - 1000497a
// ============================================================================
AFX_MODULE_STATE * __thiscall FUN_10004960(void *this,byte param_1)

{
  AFX_MODULE_STATE::~AFX_MODULE_STATE(this);
  if ((param_1 & 1) != 0) {
    CNoTrackObject::operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: ~AFX_MODULE_STATE at 1000497b
// Signature: void __thiscall ~AFX_MODULE_STATE(AFX_MODULE_STATE * this)
// Entry: 1000497b
// Body: 1000497b - 1000497f
// ============================================================================
void __thiscall AFX_MODULE_STATE::~AFX_MODULE_STATE(AFX_MODULE_STATE *this)

{
                    /* WARNING: Could not recover jumptable at 0x10004efa. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~AFX_MODULE_STATE(this);
  return;
}


// ============================================================================
// Function: FUN_10004980 at 10004980
// Signature: undefined __stdcall FUN_10004980(void)
// Entry: 10004980
// Body: 10004980 - 1000498b
// ============================================================================
void FUN_10004980(void)

{
  FUN_10004b9c((_onexit_t)&LAB_1000498c);
  return;
}


// ============================================================================
// Function: FUN_10004996 at 10004996
// Signature: undefined __stdcall FUN_10004996(HWND__ * param_1, uint param_2, uint param_3, long param_4)
// Entry: 10004996
// Body: 10004996 - 100049da
// ============================================================================
/* WARNING: Function: _EH_prolog replaced with injection: EH_prolog */

void FUN_10004996(HWND__ *param_1,uint param_2,uint param_3,long param_4)

{
  undefined4 local_18;
  int local_14;
  void *local_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &DAT_10005348;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  AFX_MAINTAIN_STATE2::AFX_MAINTAIN_STATE2
            ((AFX_MAINTAIN_STATE2 *)&local_18,(AFX_MODULE_STATE *)&DAT_10008330);
  local_8 = 0;
  AfxWndProc(param_1,param_2,param_3,param_4);
  *(undefined4 *)(local_14 + 4) = local_18;
  ExceptionList = local_10;
  return;
}

// clang-format on
