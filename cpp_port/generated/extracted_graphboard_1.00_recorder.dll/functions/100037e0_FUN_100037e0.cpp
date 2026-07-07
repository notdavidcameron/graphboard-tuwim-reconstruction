/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_100037e0 @ 100037e0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

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

// clang-format on
