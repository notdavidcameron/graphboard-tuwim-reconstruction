/*
  Direct C++ port seed generated from Ghidra MCP localhost.
  Source program: /Tuwim.exe
  Notes: raw Ghidra decompiler output with only file-level framing added.
*/
#include "../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: GraphBrdDoc_Construct at 00405780
// Signature: COleDocument * __fastcall GraphBrdDoc_Construct(COleDocument * param_1)
// Entry: 00405780
// Body: 00405780 - 00405a4c
// ============================================================================
/* CGraphBrdDoc constructor. Allocates page/group component list dialogs (+0xa4/+0xa8), script
   editor dialogs (+0x9c/+0xa0), script engine (+0xb4), audio manager (+0xbc), initializes
   runtime/page state and the 50-entry cursor bitmap table at +0x124 with count at +0x1ec. */

COleDocument * __fastcall GraphBrdDoc_Construct(COleDocument *param_1)

{
  void *pvVar1;
  CDialog *pCVar2;
  CCmdTarget *pCVar3;
  int iVar4;
  COleDocument *pCVar5;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_0042f748;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  COleDocument::COleDocument(param_1);
  local_4 = 0;
  CString::CString((CString *)(param_1 + 0xc0));
  local_4._0_1_ = 1;
  CString::CString((CString *)(param_1 + 0xc4));
  local_4._0_1_ = 2;
  CString::CString((CString *)(param_1 + 0x1f0));
  local_4._0_1_ = 3;
  CString::CString((CString *)(param_1 + 500));
  local_4._0_1_ = 4;
  CString::CString((CString *)(param_1 + 0x1f8));
  local_4._0_1_ = 5;
  CString::CString((CString *)(param_1 + 0x1fc));
  *(undefined ***)(param_1 + 0x210) = &PTR_GraphBrdDoc_ExternalQueryInterfaceThunk_00433f58;
  *(undefined ***)param_1 = &PTR_LAB_00433e70;
  local_4._0_1_ = 6;
  *(undefined4 *)(param_1 + 0x78) = 1;
  CCmdTarget::EnableAutomation((CCmdTarget *)param_1);
  AfxOleLockApp();
  *(undefined4 *)(param_1 + 0xfc) = 0;
  *(undefined4 *)(param_1 + 0xe0) = 0;
  *(undefined4 *)(param_1 + 0x120) = 0;
  *(undefined4 *)(param_1 + 0xb0) = 0;
  *(undefined4 *)(param_1 + 0xac) = 0;
  pvVar1 = operator_new(0x400);
  local_4._0_1_ = 7;
  if (pvVar1 == (void *)0x0) {
    pCVar2 = (CDialog *)0x0;
  }
  else {
    pCVar2 = GraphBrdComponentListDialog_Construct(pvVar1,param_1,(CWnd *)0x0);
  }
  local_4._0_1_ = 6;
  *(CDialog **)(param_1 + 0xa4) = pCVar2;
  pvVar1 = operator_new(0x400);
  local_4._0_1_ = 8;
  if (pvVar1 == (void *)0x0) {
    pCVar2 = (CDialog *)0x0;
  }
  else {
    pCVar2 = GraphBrdComponentListDialog_Construct(pvVar1,param_1,(CWnd *)0x0);
  }
  local_4._0_1_ = 6;
  *(CDialog **)(param_1 + 0xa8) = pCVar2;
  pvVar1 = operator_new(0x1ec);
  local_4._0_1_ = 9;
  if (pvVar1 == (void *)0x0) {
    pCVar2 = (CDialog *)0x0;
  }
  else {
    pCVar2 = GraphBrdScriptEditorDialog_Construct(pvVar1,param_1);
  }
  local_4._0_1_ = 6;
  *(CDialog **)(param_1 + 0x9c) = pCVar2;
  pvVar1 = operator_new(0x1ec);
  local_4._0_1_ = 10;
  if (pvVar1 == (void *)0x0) {
    pCVar2 = (CDialog *)0x0;
  }
  else {
    pCVar2 = GraphBrdScriptEditorDialog_Construct(pvVar1,param_1);
  }
  local_4._0_1_ = 6;
  *(CDialog **)(param_1 + 0xa0) = pCVar2;
  pCVar3 = operator_new(0x14e4);
  local_4._0_1_ = 0xb;
  if (pCVar3 == (CCmdTarget *)0x0) {
    pCVar3 = (CCmdTarget *)0x0;
  }
  else {
    pCVar3 = GraphBrdScriptEngine_Construct(pCVar3);
  }
  *(CCmdTarget **)(param_1 + 0xb4) = pCVar3;
  *(COleDocument **)(pCVar3 + 0x20) = param_1;
  local_4._0_1_ = 6;
  *(undefined4 *)(*(int *)(param_1 + 0xb4) + 0x24) = *(undefined4 *)(param_1 + 0x9c);
  *(undefined4 *)(*(int *)(param_1 + 0xb4) + 0x28) = *(undefined4 *)(param_1 + 0xa0);
  pCVar3 = operator_new(0x44);
  local_4 = CONCAT31(local_4._1_3_,0xc);
  if (pCVar3 == (CCmdTarget *)0x0) {
    pCVar3 = (CCmdTarget *)0x0;
  }
  else {
    pCVar3 = GraphBrdAudioManager_Construct(pCVar3);
  }
  *(CCmdTarget **)(param_1 + 0xbc) = pCVar3;
  *(COleDocument **)(pCVar3 + 0x3c) = param_1;
  *(undefined4 *)(param_1 + 0x100) = 10000;
  *(undefined4 *)(param_1 + 0x104) = 0;
  *(undefined4 *)(param_1 + 200) = 0;
  *(undefined4 *)(param_1 + 0xe4) = 0;
  *(undefined4 *)(param_1 + 0xdc) = 0;
  *(undefined4 *)(param_1 + 0xd8) = 0;
  *(undefined4 *)(param_1 + 0xd4) = 0;
  *(undefined4 *)(param_1 + 0x118) = 0;
  param_1[0x11c] = (COleDocument)0x0;
  *(undefined4 *)(param_1 + 0x108) = 0;
  *(undefined4 *)(param_1 + 0x10c) = 0;
  *(undefined4 *)(param_1 + 0x110) = 0;
  *(undefined4 *)(param_1 + 0x114) = 0;
  pCVar5 = param_1 + 0x124;
  for (iVar4 = 0x32; iVar4 != 0; iVar4 = iVar4 + -1) {
    *(undefined4 *)pCVar5 = 0;
    pCVar5 = pCVar5 + 4;
  }
  *(undefined4 *)(param_1 + 0x1ec) = 0;
  *(undefined4 *)(param_1 + 0x98) = 0;
  *(undefined4 *)(param_1 + 0x94) = 0;
  *(undefined4 *)(param_1 + 0x90) = 0;
  *(undefined4 *)(param_1 + 0x8c) = 0;
  *(undefined4 *)(param_1 + 0xe8) = 0;
  *(undefined4 *)(param_1 + 0xec) = 0;
  *(undefined4 *)(param_1 + 0xf0) = 0;
  *(undefined4 *)(param_1 + 0x200) = 0;
  *(undefined4 *)(param_1 + 0x204) = 0;
  *(undefined4 *)(param_1 + 0x208) = 0;
  *(undefined4 *)(param_1 + 0x20c) = 0;
  *(undefined4 *)(param_1 + 0xd0) = 0;
  ExceptionList = local_c;
  return param_1;
}


// ============================================================================
// Function: GraphBrdDoc_SerializeProjectState at 00406020
// Signature: undefined4 __thiscall GraphBrdDoc_SerializeProjectState(void * this, CArchive * param_1)
// Entry: 00406020
// Body: 00406020 - 004064b3
// ============================================================================
undefined4 __thiscall GraphBrdDoc_SerializeProjectState(void *this,CArchive *param_1)

{
  CString *pCVar1;
  void **ppvVar2;
  int iVar3;
  void *pvVar4;
  undefined4 uVar5;
  CWinThread *pCVar6;
  int iVar7;
  int iVar8;
  CString *local_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_0042f836;
  pvStack_c = ExceptionList;
  local_10 = this;
  if ((~*(uint *)(param_1 + 0x14) & 1) == 0) {
    iVar7 = *(int *)((int)this + 0x204);
    iVar8 = 0;
    ExceptionList = &pvStack_c;
    ppvVar2 = &pvStack_c;
    if (0 < iVar7) {
      do {
        pCVar1 = *(CString **)(*(int *)((int)this + 0x200) + iVar8 * 4);
        if (pCVar1 != (CString *)0x0) {
          CString::~CString(pCVar1);
          operator_delete(pCVar1);
        }
        iVar7 = *(int *)((int)this + 0x204);
        iVar8 = iVar8 + 1;
        ppvVar2 = ExceptionList;
      } while (iVar8 < iVar7);
    }
    ExceptionList = ppvVar2;
    if (iVar7 != 0) {
      operator_delete(*(void **)((int)this + 0x200));
    }
    iVar7 = *(int *)((int)this + 0x20c);
    iVar8 = 0;
    if (0 < iVar7) {
      do {
        pCVar1 = *(CString **)(*(int *)((int)this + 0x208) + iVar8 * 4);
        if (pCVar1 != (CString *)0x0) {
          CString::~CString(pCVar1);
          operator_delete(pCVar1);
        }
        iVar7 = *(int *)((int)this + 0x20c);
        iVar8 = iVar8 + 1;
      } while (iVar8 < iVar7);
    }
    if (iVar7 != 0) {
      operator_delete(*(void **)((int)this + 0x208));
    }
    if (*(uint *)(param_1 + 0x28) < *(int *)(param_1 + 0x24) + 4U) {
      CArchive::FillBuffer(param_1,(*(int *)(param_1 + 0x24) - *(uint *)(param_1 + 0x28)) + 4);
    }
    iVar7 = **(int **)(param_1 + 0x24);
    *(int **)(param_1 + 0x24) = *(int **)(param_1 + 0x24) + 1;
    operator>>(param_1,(CString *)((int)this + 500));
    operator>>(param_1,(CString *)((int)this + 0x1f8));
    iVar8 = *(int *)((int)this + 0xbc);
    if (*(uint *)(param_1 + 0x28) < *(int *)(param_1 + 0x24) + 4U) {
      CArchive::FillBuffer(param_1,(*(int *)(param_1 + 0x24) - *(uint *)(param_1 + 0x28)) + 4);
    }
    *(undefined4 *)(iVar8 + 0x20) = **(undefined4 **)(param_1 + 0x24);
    iVar8 = *(int *)(param_1 + 0x24);
    iVar3 = iVar8 + 4;
    *(int *)(param_1 + 0x24) = iVar3;
    if (*(uint *)(param_1 + 0x28) < iVar8 + 8U) {
      CArchive::FillBuffer(param_1,(iVar3 - *(uint *)(param_1 + 0x28)) + 4);
    }
    *(undefined4 *)((int)this + 0x204) = **(undefined4 **)(param_1 + 0x24);
    *(int *)(param_1 + 0x24) = *(int *)(param_1 + 0x24) + 4;
    if (*(int *)((int)this + 0x204) != 0) {
      pvVar4 = operator_new(*(int *)((int)this + 0x204) * 4);
      *(void **)((int)this + 0x200) = pvVar4;
    }
    iVar8 = 0;
    if (0 < *(int *)((int)this + 0x204)) {
      do {
        local_10 = operator_new(4);
        local_4 = 0;
        if (local_10 == (CString *)0x0) {
          uVar5 = 0;
        }
        else {
          uVar5 = CString::CString(local_10);
        }
        local_4 = 0xffffffff;
        *(undefined4 *)(*(int *)((int)this + 0x200) + iVar8 * 4) = uVar5;
        operator>>(param_1,*(CString **)(*(int *)((int)this + 0x200) + iVar8 * 4));
        iVar8 = iVar8 + 1;
      } while (iVar8 < *(int *)((int)this + 0x204));
    }
    if (*(uint *)(param_1 + 0x28) < *(int *)(param_1 + 0x24) + 4U) {
      CArchive::FillBuffer(param_1,(*(int *)(param_1 + 0x24) - *(uint *)(param_1 + 0x28)) + 4);
    }
    *(undefined4 *)((int)this + 0x20c) = **(undefined4 **)(param_1 + 0x24);
    *(int *)(param_1 + 0x24) = *(int *)(param_1 + 0x24) + 4;
    if (*(int *)((int)this + 0x20c) != 0) {
      pvVar4 = operator_new(*(int *)((int)this + 0x20c) * 4);
      *(void **)((int)this + 0x208) = pvVar4;
    }
    iVar8 = 0;
    if (0 < *(int *)((int)this + 0x20c)) {
      do {
        local_10 = operator_new(4);
        local_4 = 1;
        if (local_10 == (CString *)0x0) {
          uVar5 = 0;
        }
        else {
          uVar5 = CString::CString(local_10);
        }
        local_4 = 0xffffffff;
        *(undefined4 *)(*(int *)((int)this + 0x208) + iVar8 * 4) = uVar5;
        operator>>(param_1,*(CString **)(*(int *)((int)this + 0x208) + iVar8 * 4));
        iVar8 = iVar8 + 1;
      } while (iVar8 < *(int *)((int)this + 0x20c));
    }
    if (iVar7 != 0) {
      pCVar1 = (CString *)((int)this + 0x1fc);
      operator>>(param_1,pCVar1);
      iVar7 = *(int *)pCVar1;
      iVar8 = 0;
      if (0 < *(int *)(iVar7 + -8)) {
        do {
          CString::SetAt(pCVar1,iVar8,*(char *)(iVar7 + iVar8) + -0x21);
          iVar7 = *(int *)pCVar1;
          iVar8 = iVar8 + 1;
        } while (iVar8 < *(int *)(iVar7 + -8));
      }
      iVar7 = _mbscmp(*(uchar **)pCVar1,(uchar *)s_Julian_Tuwim_0043ac6c);
      if (iVar7 != 0) {
        pCVar6 = AfxGetThread();
        if (pCVar6 == (CWinThread *)0x0) {
          iVar7 = 0;
        }
        else {
          iVar7 = (**(code **)(*(int *)pCVar6 + 0x7c))();
        }
        PostMessageA(*(HWND *)(iVar7 + 0x20),0x10,0,0);
      }
    }
  }
  else {
    ExceptionList = &pvStack_c;
    if (*(uint *)(param_1 + 0x28) < *(int *)(param_1 + 0x24) + 4U) {
      ExceptionList = &pvStack_c;
      CArchive::Flush(param_1);
    }
    **(undefined4 **)(param_1 + 0x24) = 1;
    *(int *)(param_1 + 0x24) = *(int *)(param_1 + 0x24) + 4;
    operator<<(param_1,(CString *)((int)this + 500));
    operator<<(param_1,(CString *)((int)this + 0x1f8));
    uVar5 = *(undefined4 *)(*(int *)((int)this + 0xbc) + 0x20);
    if (*(uint *)(param_1 + 0x28) < *(int *)(param_1 + 0x24) + 4U) {
      CArchive::Flush(param_1);
    }
    **(undefined4 **)(param_1 + 0x24) = uVar5;
    iVar7 = *(int *)(param_1 + 0x24);
    *(int *)(param_1 + 0x24) = iVar7 + 4;
    uVar5 = *(undefined4 *)((int)this + 0x204);
    if (*(uint *)(param_1 + 0x28) < iVar7 + 8U) {
      CArchive::Flush(param_1);
    }
    **(undefined4 **)(param_1 + 0x24) = uVar5;
    iVar7 = 0;
    *(int *)(param_1 + 0x24) = *(int *)(param_1 + 0x24) + 4;
    if (0 < *(int *)((int)this + 0x204)) {
      do {
        operator<<(param_1,*(CString **)(*(int *)((int)this + 0x200) + iVar7 * 4));
        iVar7 = iVar7 + 1;
      } while (iVar7 < *(int *)((int)this + 0x204));
    }
    uVar5 = *(undefined4 *)((int)this + 0x20c);
    if (*(uint *)(param_1 + 0x28) < *(int *)(param_1 + 0x24) + 4U) {
      CArchive::Flush(param_1);
    }
    **(undefined4 **)(param_1 + 0x24) = uVar5;
    iVar7 = 0;
    *(int *)(param_1 + 0x24) = *(int *)(param_1 + 0x24) + 4;
    if (0 < *(int *)((int)this + 0x20c)) {
      do {
        operator<<(param_1,*(CString **)(*(int *)((int)this + 0x208) + iVar7 * 4));
        iVar7 = iVar7 + 1;
      } while (iVar7 < *(int *)((int)this + 0x20c));
    }
    iVar7 = *(int *)((int)this + 0x1fc);
    pCVar1 = (CString *)((int)this + 0x1fc);
    iVar8 = 0;
    if (0 < *(int *)(iVar7 + -8)) {
      do {
        CString::SetAt(pCVar1,iVar8,*(char *)(iVar7 + iVar8) + '!');
        iVar7 = *(int *)pCVar1;
        iVar8 = iVar8 + 1;
      } while (iVar8 < *(int *)(iVar7 + -8));
    }
    operator<<(param_1,pCVar1);
    iVar7 = *(int *)pCVar1;
    iVar8 = 0;
    if (0 < *(int *)(iVar7 + -8)) {
      do {
        CString::SetAt(pCVar1,iVar8,*(char *)(iVar7 + iVar8) + -0x21);
        iVar7 = *(int *)pCVar1;
        iVar8 = iVar8 + 1;
      } while (iVar8 < *(int *)(iVar7 + -8));
    }
  }
  (**(code **)(**(int **)((int)this + 0xa0) + 8))(param_1);
  if ((~*(uint *)(param_1 + 0x14) & 1) == 0) {
    GraphBrdScript_RunGlobalSetupBlock(*(void **)((int)this + 0xb4));
  }
  ExceptionList = local_10;
  return 1;
}


// ============================================================================
// Function: GraphBrdDoc_SerializeCursorAndGroupState at 004064c0
// Signature: undefined4 __thiscall GraphBrdDoc_SerializeCursorAndGroupState(void * this, CArchive * param_1)
// Entry: 004064c0
// Body: 004064c0 - 004066ee
// ============================================================================
/* Serializes cursor bitmap records then delegates group component list serialization. Each cursor
   table entry is a 0x2c GraphBrdCursorBitmapRecord2c followed by a separately sized pixel buffer
   allocated with GlobalAlloc/GlobalLock. */

undefined4 __thiscall GraphBrdDoc_SerializeCursorAndGroupState(void *this,CArchive *param_1)

{
  CArchive *pCVar1;
  undefined4 uVar2;
  CArchive *this_00;
  HGLOBAL pvVar3;
  SIZE_T SVar4;
  void *pvVar5;
  LPVOID pvVar6;
  CWinThread *pCVar7;
  int iVar8;
  int *piVar9;
  
  this_00 = param_1;
  if ((~*(uint *)(param_1 + 0x14) & 1) == 0) {
    iVar8 = 0;
    if (0 < *(int *)((int)this + 0x1ec)) {
      piVar9 = (int *)((int)this + 0x124);
      do {
        pvVar3 = GlobalHandle(*(LPCVOID *)(*piVar9 + 0x28));
        GlobalUnlock(pvVar3);
        pvVar3 = GlobalHandle(*(LPCVOID *)(*piVar9 + 0x28));
        GlobalFree(pvVar3);
        operator_delete((void *)*piVar9);
        iVar8 = iVar8 + 1;
        piVar9 = piVar9 + 1;
      } while (iVar8 < *(int *)((int)this + 0x1ec));
    }
    if (*(uint *)(param_1 + 0x28) < *(int *)(param_1 + 0x24) + 4U) {
      CArchive::FillBuffer(param_1,(*(int *)(param_1 + 0x24) - *(uint *)(param_1 + 0x28)) + 4);
    }
    pCVar1 = param_1 + 0x24;
    param_1 = (CArchive *)0x0;
    *(undefined4 *)((int)this + 0x1ec) = **(undefined4 **)pCVar1;
    *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 4;
    if (0 < *(int *)((int)this + 0x1ec)) {
      piVar9 = (int *)((int)this + 0x124);
      do {
        pvVar5 = operator_new(0x2c);
        *piVar9 = (int)pvVar5;
        CArchive::Read(this_00,pvVar5,0x2c);
        if (*(uint *)(this_00 + 0x28) < *(int *)(this_00 + 0x24) + 4U) {
          CArchive::FillBuffer(this_00,(*(int *)(this_00 + 0x24) - *(uint *)(this_00 + 0x28)) + 4);
        }
        SVar4 = **(SIZE_T **)(this_00 + 0x24);
        *(SIZE_T **)(this_00 + 0x24) = *(SIZE_T **)(this_00 + 0x24) + 1;
        pvVar3 = GlobalAlloc(2,SVar4);
        pvVar6 = GlobalLock(pvVar3);
        *(LPVOID *)(*piVar9 + 0x28) = pvVar6;
        if (*(void **)(*piVar9 + 0x28) == (void *)0x0) {
          MessageBoxA((HWND)0x0,s_End_of_memory_0043a0f8,s_Read_Project_0043ac7c,0x30);
          pCVar7 = AfxGetThread();
          if (pCVar7 == (CWinThread *)0x0) {
            iVar8 = 0;
          }
          else {
            iVar8 = (**(code **)(*(int *)pCVar7 + 0x7c))();
          }
          PostMessageA(*(HWND *)(iVar8 + 0x20),0x10,0,0);
          return 0;
        }
        CArchive::Read(this_00,*(void **)(*piVar9 + 0x28),SVar4);
        param_1 = param_1 + 1;
        piVar9 = piVar9 + 1;
      } while ((int)param_1 < *(int *)((int)this + 0x1ec));
    }
  }
  else {
    uVar2 = *(undefined4 *)((int)this + 0x1ec);
    if (*(uint *)(param_1 + 0x28) < *(int *)(param_1 + 0x24) + 4U) {
      CArchive::Flush(param_1);
    }
    pCVar1 = param_1 + 0x24;
    param_1 = (CArchive *)0x0;
    **(undefined4 **)pCVar1 = uVar2;
    *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 4;
    if (0 < *(int *)((int)this + 0x1ec)) {
      piVar9 = (int *)((int)this + 0x124);
      do {
        CArchive::Write(this_00,(void *)*piVar9,0x2c);
        pvVar3 = GlobalHandle(*(LPCVOID *)(*piVar9 + 0x28));
        SVar4 = GlobalSize(pvVar3);
        if (*(uint *)(this_00 + 0x28) < *(int *)(this_00 + 0x24) + 4U) {
          CArchive::Flush(this_00);
        }
        **(SIZE_T **)(this_00 + 0x24) = SVar4;
        *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 4;
        CArchive::Write(this_00,*(void **)(*piVar9 + 0x28),SVar4);
        param_1 = param_1 + 1;
        piVar9 = piVar9 + 1;
      } while ((int)param_1 < *(int *)((int)this + 0x1ec));
    }
  }
  (**(code **)(**(int **)((int)this + 0xa8) + 8))(this_00);
  return 1;
}


// ============================================================================
// Function: GraphBrdDoc_SerializePageData at 004066f0
// Signature: undefined __fastcall GraphBrdDoc_SerializePageData(COleDocument * param_1)
// Entry: 004066f0
// Body: 004066f0 - 00406f5d
// ============================================================================
/* Serializes/deserializes GraphBoard page data: page rectangle/timing flags, palette, DIB
   background, page/group component lists, script engine data, then COleDocument state. */

void __fastcall GraphBrdDoc_SerializePageData(COleDocument *param_1)

{
  COleDocument CVar1;
  undefined4 uVar2;
  int *piVar3;
  int iVar4;
  uint uVar5;
  CGdiObject *this;
  SIZE_T dwBytes;
  void *this_00;
  HGLOBAL pvVar6;
  LOGPALETTE *plpal;
  undefined4 *puVar7;
  HPALETTE pHVar8;
  LPVOID pvVar9;
  CWinThread *pCVar10;
  int iVar11;
  int unaff_EBP;
  int unaff_ESI;
  int iVar12;
  CArchive *unaff_retaddr;
  char *pcVar13;
  undefined1 *puVar14;
  void *apvStack_94 [3];
  undefined1 auStack_88 [12];
  int iStack_7c;
  undefined1 auStack_78 [4];
  undefined4 auStack_74 [25];
  void *pvStack_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_0042f84b;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  auStack_88._0_4_ = (**(code **)(*(int *)param_1 + 0x68))();
  this_00 = (void *)(**(code **)(*(int *)param_1 + 0x6c))(auStack_88);
  apvStack_94[0] = this_00;
  pvVar6 = GlobalAlloc(2,0x408);
  plpal = GlobalLock(pvVar6);
  if (plpal == (LOGPALETTE *)0x0) goto LAB_00406daf;
  if ((~*(uint *)(unaff_retaddr + 0x14) & 1) == 0) {
    if (*(LPCVOID *)(param_1 + 0xfc) != (LPCVOID)0x0) {
      pvVar6 = GlobalHandle(*(LPCVOID *)(param_1 + 0xfc));
      GlobalUnlock(pvVar6);
      pvVar6 = GlobalHandle(*(LPCVOID *)(param_1 + 0xfc));
      GlobalFree(pvVar6);
    }
    *(undefined4 *)(param_1 + 0xfc) = 0;
    CArchive::Read(unaff_retaddr,auStack_74,100);
    if (*(uint *)(unaff_retaddr + 0x28) < *(int *)(unaff_retaddr + 0x24) + 4U) {
      CArchive::FillBuffer
                (unaff_retaddr,
                 (*(int *)(unaff_retaddr + 0x24) - *(uint *)(unaff_retaddr + 0x28)) + 4);
    }
    iVar11 = *(int *)(unaff_retaddr + 0x24);
    iVar12 = iVar11 + 4;
    *(int *)(unaff_retaddr + 0x24) = iVar12;
    if (*(uint *)(unaff_retaddr + 0x28) < iVar11 + 8U) {
      CArchive::FillBuffer(unaff_retaddr,(iVar12 - *(uint *)(unaff_retaddr + 0x28)) + 4);
    }
    *(undefined4 *)(param_1 + 0x108) = **(undefined4 **)(unaff_retaddr + 0x24);
    iVar11 = *(int *)(unaff_retaddr + 0x24);
    iVar12 = iVar11 + 4;
    *(int *)(unaff_retaddr + 0x24) = iVar12;
    if (*(uint *)(unaff_retaddr + 0x28) < iVar11 + 8U) {
      CArchive::FillBuffer(unaff_retaddr,(iVar12 - *(uint *)(unaff_retaddr + 0x28)) + 4);
    }
    *(undefined4 *)(param_1 + 0x10c) = **(undefined4 **)(unaff_retaddr + 0x24);
    iVar11 = *(int *)(unaff_retaddr + 0x24);
    iVar12 = iVar11 + 4;
    *(int *)(unaff_retaddr + 0x24) = iVar12;
    if (*(uint *)(unaff_retaddr + 0x28) < iVar11 + 8U) {
      CArchive::FillBuffer(unaff_retaddr,(iVar12 - *(uint *)(unaff_retaddr + 0x28)) + 4);
    }
    *(undefined4 *)(param_1 + 0x110) = **(undefined4 **)(unaff_retaddr + 0x24);
    iVar11 = *(int *)(unaff_retaddr + 0x24);
    iVar12 = iVar11 + 4;
    *(int *)(unaff_retaddr + 0x24) = iVar12;
    if (*(uint *)(unaff_retaddr + 0x28) < iVar11 + 8U) {
      CArchive::FillBuffer(unaff_retaddr,(iVar12 - *(uint *)(unaff_retaddr + 0x28)) + 4);
    }
    *(undefined4 *)(param_1 + 0x114) = **(undefined4 **)(unaff_retaddr + 0x24);
    iVar11 = *(int *)(unaff_retaddr + 0x24);
    iVar12 = iVar11 + 4;
    *(int *)(unaff_retaddr + 0x24) = iVar12;
    if (*(uint *)(unaff_retaddr + 0x28) < iVar11 + 8U) {
      CArchive::FillBuffer(unaff_retaddr,(iVar12 - *(uint *)(unaff_retaddr + 0x28)) + 4);
    }
    *(undefined4 *)(param_1 + 0x100) = **(undefined4 **)(unaff_retaddr + 0x24);
    iVar11 = *(int *)(unaff_retaddr + 0x24);
    iVar12 = iVar11 + 4;
    *(int *)(unaff_retaddr + 0x24) = iVar12;
    if (*(uint *)(unaff_retaddr + 0x28) < iVar11 + 8U) {
      CArchive::FillBuffer(unaff_retaddr,(iVar12 - *(uint *)(unaff_retaddr + 0x28)) + 4);
    }
    *(undefined4 *)(param_1 + 0x104) = **(undefined4 **)(unaff_retaddr + 0x24);
    iVar11 = *(int *)(unaff_retaddr + 0x24);
    iVar12 = iVar11 + 4;
    *(int *)(unaff_retaddr + 0x24) = iVar12;
    if (*(uint *)(unaff_retaddr + 0x28) < iVar11 + 8U) {
      CArchive::FillBuffer(unaff_retaddr,(iVar12 - *(uint *)(unaff_retaddr + 0x28)) + 4);
    }
    *(undefined4 *)(param_1 + 0x118) = **(undefined4 **)(unaff_retaddr + 0x24);
    iVar11 = *(int *)(unaff_retaddr + 0x24);
    iVar12 = iVar11 + 4;
    *(int *)(unaff_retaddr + 0x24) = iVar12;
    if (*(uint *)(unaff_retaddr + 0x28) < iVar11 + 5U) {
      CArchive::FillBuffer(unaff_retaddr,(iVar12 - *(uint *)(unaff_retaddr + 0x28)) + 1);
    }
    param_1[0x11c] = **(COleDocument **)(unaff_retaddr + 0x24);
    iVar11 = *(int *)(unaff_retaddr + 0x24);
    iVar12 = iVar11 + 1;
    *(int *)(unaff_retaddr + 0x24) = iVar12;
    if (*(uint *)(unaff_retaddr + 0x28) < iVar11 + 5U) {
      CArchive::FillBuffer(unaff_retaddr,(iVar12 - *(uint *)(unaff_retaddr + 0x28)) + 4);
    }
    uVar5 = **(uint **)(unaff_retaddr + 0x24);
    *(uint **)(unaff_retaddr + 0x24) = *(uint **)(unaff_retaddr + 0x24) + 1;
    if (uVar5 != 0) {
      if (*(int *)((int)this_00 + 0x68) == 0) {
        if (*(CGdiObject **)((int)this_00 + 100) == (CGdiObject *)0x0) {
          puVar7 = operator_new(8);
          if (puVar7 == (undefined4 *)0x0) {
            puVar7 = (undefined4 *)0x0;
          }
          else {
            puVar7[1] = 0;
            *puVar7 = &PTR_LAB_00433f80;
          }
          *(undefined4 **)((int)this_00 + 100) = puVar7;
        }
        else {
          CGdiObject::DeleteObject(*(CGdiObject **)((int)this_00 + 100));
        }
        CArchive::Read(unaff_retaddr,plpal->palPalEntry,uVar5);
        plpal->palVersion = 0x300;
        plpal->palNumEntries = 0x100;
        this = *(CGdiObject **)((int)this_00 + 100);
        pHVar8 = CreatePalette(plpal);
        iVar11 = CGdiObject::Attach(this,pHVar8);
        if (iVar11 == 0) {
          if (*(int **)((int)this_00 + 100) != (int *)0x0) {
            (**(code **)(**(int **)((int)this_00 + 100) + 4))(1);
          }
          *(undefined4 *)((int)this_00 + 100) = 0;
        }
      }
      else {
        CArchive::Read(unaff_retaddr,plpal->palPalEntry,uVar5);
        plpal->palVersion = 0x300;
        plpal->palNumEntries = 0x100;
      }
    }
    if (*(uint *)(unaff_retaddr + 0x28) < *(int *)(unaff_retaddr + 0x24) + 4U) {
      CArchive::FillBuffer
                (unaff_retaddr,
                 (*(int *)(unaff_retaddr + 0x24) - *(uint *)(unaff_retaddr + 0x28)) + 4);
    }
    dwBytes = **(SIZE_T **)(unaff_retaddr + 0x24);
    *(SIZE_T **)(unaff_retaddr + 0x24) = *(SIZE_T **)(unaff_retaddr + 0x24) + 1;
    if (dwBytes == 0) {
      if ((*(int *)(param_1 + 0x110) != 0) && (*(int *)(param_1 + 0x114) != 0)) {
        GraphBrdView_CreateOrResizeBackBufferFromDib(this_00,(int *)0x0);
      }
    }
    else {
      pvVar6 = GlobalAlloc(2,dwBytes);
      pvVar9 = GlobalLock(pvVar6);
      *(LPVOID *)(param_1 + 0xfc) = pvVar9;
      if (pvVar9 == (LPVOID)0x0) {
LAB_00406daf:
        AfxMessageBox(s_Fatal_Error__End_of_memory_0043ac8c,0,0);
        pCVar10 = AfxGetThread();
        if (pCVar10 == (CWinThread *)0x0) {
          iVar11 = 0;
        }
        else {
          iVar11 = (**(code **)(*(int *)pCVar10 + 0x7c))();
        }
        PostMessageA(*(HWND *)(iVar11 + 0x20),0x10,0,0);
        if (plpal == (LOGPALETTE *)0x0) {
          ExceptionList = pvStack_10;
          return;
        }
        pvVar6 = GlobalHandle(plpal);
        GlobalUnlock(pvVar6);
        pvVar6 = GlobalHandle(plpal);
        GlobalFree(pvVar6);
        ExceptionList = pvStack_10;
        return;
      }
      CArchive::Read(unaff_retaddr,pvVar9,dwBytes);
      iVar11 = GraphBrdView_CreateOrResizeBackBufferFromDib(this_00,*(int **)(param_1 + 0xfc));
      if (iVar11 == 0) goto LAB_00406daf;
    }
    if (this_00 != (void *)0x0) {
      GetClientRect(*(HWND *)((int)this_00 + 0x20),(LPRECT)auStack_88);
      *(int *)((int)this_00 + 0xac) =
           (((*(int *)(param_1 + 0x108) - *(int *)(param_1 + 0x110)) - auStack_88._0_4_) +
           auStack_88._8_4_) / 2;
      *(int *)((int)this_00 + 0xb0) =
           (((*(int *)(param_1 + 0x10c) - auStack_88._4_4_) - *(int *)(param_1 + 0x114)) + iStack_7c
           ) / 2;
      GetWindowRect(*(HWND *)((int)this_00 + 0x20),(LPRECT)auStack_88);
      iVar11 = *(int *)((int)this_00 + 0xac) + auStack_88._0_4_;
      *(int *)((int)this_00 + 0x94) = iVar11;
      iVar12 = auStack_88._4_4_ + *(int *)((int)this_00 + 0xb0);
      *(int *)((int)this_00 + 0x98) = iVar12;
      *(int *)((int)this_00 + 0xa0) =
           (iVar12 - *(int *)(param_1 + 0x10c)) + *(int *)(param_1 + 0x114);
      *(int *)((int)this_00 + 0x9c) =
           (*(int *)(param_1 + 0x110) - *(int *)(param_1 + 0x108)) + iVar11;
    }
  }
  else {
    if (*(int *)(param_1 + 0xe0) != 0) {
      GraphBrdDoc_SetRuntimeMode(param_1,0);
    }
    GraphBrdScriptEngine_ParsePageScript(*(void **)(param_1 + 0xb4));
    puVar14 = auStack_78;
    puVar7 = auStack_74;
    for (iVar11 = 0x19; iVar11 != 0; iVar11 = iVar11 + -1) {
      *puVar7 = 0;
      puVar7 = puVar7 + 1;
    }
    CTime::GetTickCount();
    pcVar13 = s__A___B__d___Y_0043ace8;
    CTime::Format((CTime *)&iStack_7c,(char *)apvStack_94);
    pvStack_10 = (void *)0x0;
    sprintf((char *)&iStack_7c,s_YDP_Board_data_file__Ver__d_1996_0043aca8,1,plpal,pcVar13,puVar14);
    CArchive::Write(unaff_retaddr,&iStack_7c,100);
    if (*(uint *)(unaff_retaddr + 0x28) < *(int *)(unaff_retaddr + 0x24) + 4U) {
      CArchive::Flush(unaff_retaddr);
    }
    **(undefined4 **)(unaff_retaddr + 0x24) = 1;
    iVar11 = *(int *)(unaff_retaddr + 0x24);
    *(int *)(unaff_retaddr + 0x24) = iVar11 + 4;
    uVar2 = *(undefined4 *)(param_1 + 0x108);
    if (*(uint *)(unaff_retaddr + 0x28) < iVar11 + 8U) {
      CArchive::Flush(unaff_retaddr);
    }
    **(undefined4 **)(unaff_retaddr + 0x24) = uVar2;
    iVar11 = *(int *)(unaff_retaddr + 0x24);
    *(int *)(unaff_retaddr + 0x24) = iVar11 + 4;
    uVar2 = *(undefined4 *)(param_1 + 0x10c);
    if (*(uint *)(unaff_retaddr + 0x28) < iVar11 + 8U) {
      CArchive::Flush(unaff_retaddr);
    }
    **(undefined4 **)(unaff_retaddr + 0x24) = uVar2;
    iVar11 = *(int *)(unaff_retaddr + 0x24);
    *(int *)(unaff_retaddr + 0x24) = iVar11 + 4;
    uVar2 = *(undefined4 *)(param_1 + 0x110);
    if (*(uint *)(unaff_retaddr + 0x28) < iVar11 + 8U) {
      CArchive::Flush(unaff_retaddr);
    }
    **(undefined4 **)(unaff_retaddr + 0x24) = uVar2;
    iVar11 = *(int *)(unaff_retaddr + 0x24);
    *(int *)(unaff_retaddr + 0x24) = iVar11 + 4;
    uVar2 = *(undefined4 *)(param_1 + 0x114);
    if (*(uint *)(unaff_retaddr + 0x28) < iVar11 + 8U) {
      CArchive::Flush(unaff_retaddr);
    }
    **(undefined4 **)(unaff_retaddr + 0x24) = uVar2;
    iVar11 = *(int *)(unaff_retaddr + 0x24);
    *(int *)(unaff_retaddr + 0x24) = iVar11 + 4;
    uVar2 = *(undefined4 *)(param_1 + 0x100);
    if (*(uint *)(unaff_retaddr + 0x28) < iVar11 + 8U) {
      CArchive::Flush(unaff_retaddr);
    }
    **(undefined4 **)(unaff_retaddr + 0x24) = uVar2;
    iVar11 = *(int *)(unaff_retaddr + 0x24);
    *(int *)(unaff_retaddr + 0x24) = iVar11 + 4;
    uVar2 = *(undefined4 *)(param_1 + 0x104);
    if (*(uint *)(unaff_retaddr + 0x28) < iVar11 + 8U) {
      CArchive::Flush(unaff_retaddr);
    }
    **(undefined4 **)(unaff_retaddr + 0x24) = uVar2;
    iVar11 = *(int *)(unaff_retaddr + 0x24);
    *(int *)(unaff_retaddr + 0x24) = iVar11 + 4;
    uVar2 = *(undefined4 *)(param_1 + 0x118);
    if (*(uint *)(unaff_retaddr + 0x28) < iVar11 + 8U) {
      CArchive::Flush(unaff_retaddr);
    }
    **(undefined4 **)(unaff_retaddr + 0x24) = uVar2;
    iVar11 = *(int *)(unaff_retaddr + 0x24);
    *(int *)(unaff_retaddr + 0x24) = iVar11 + 4;
    CVar1 = param_1[0x11c];
    if (*(uint *)(unaff_retaddr + 0x28) < iVar11 + 5U) {
      CArchive::Flush(unaff_retaddr);
    }
    **(COleDocument **)(unaff_retaddr + 0x24) = CVar1;
    iVar11 = *(int *)(unaff_retaddr + 0x24);
    *(int *)(unaff_retaddr + 0x24) = iVar11 + 1;
    if (*(int *)(unaff_EBP + 100) == 0) {
      if (*(uint *)(unaff_retaddr + 0x28) < iVar11 + 5U) {
        CArchive::Flush(unaff_retaddr);
      }
      **(undefined4 **)(unaff_retaddr + 0x24) = 0;
      *(int *)(unaff_retaddr + 0x24) = *(int *)(unaff_retaddr + 0x24) + 4;
    }
    else {
      if (*(uint *)(unaff_retaddr + 0x28) < iVar11 + 5U) {
        CArchive::Flush(unaff_retaddr);
      }
      **(undefined4 **)(unaff_retaddr + 0x24) = 0x400;
      *(int *)(unaff_retaddr + 0x24) = *(int *)(unaff_retaddr + 0x24) + 4;
      GetPaletteEntries(*(HPALETTE *)(*(int *)(unaff_EBP + 100) + 4),0,0x100,
                        (LPPALETTEENTRY)(unaff_ESI + 4));
      CArchive::Write(unaff_retaddr,(LPPALETTEENTRY)(unaff_ESI + 4),0x400);
    }
    piVar3 = *(int **)(param_1 + 0xfc);
    if (piVar3 == (int *)0x0) {
      if (*(uint *)(unaff_retaddr + 0x28) < *(int *)(unaff_retaddr + 0x24) + 4U) {
        CArchive::Flush(unaff_retaddr);
      }
      **(undefined4 **)(unaff_retaddr + 0x24) = 0;
      *(int *)(unaff_retaddr + 0x24) = *(int *)(unaff_retaddr + 0x24) + 4;
    }
    else {
      iVar11 = piVar3[5];
      if (iVar11 == 0) {
        iVar11 = piVar3[2];
        if (iVar11 < 0) {
          iVar11 = -iVar11;
        }
        iVar11 = ((uint)*(ushort *)((int)piVar3 + 0xe) * piVar3[1] + 0x1f >> 3 & 0x1ffffffc) *
                 iVar11;
      }
      iVar12 = piVar3[8];
      if ((iVar12 == 0) && (*(ushort *)((int)piVar3 + 0xe) < 9)) {
        iVar12 = 1 << ((byte)*(ushort *)((int)piVar3 + 0xe) & 0x1f);
      }
      iVar4 = *piVar3;
      if (*(uint *)(unaff_retaddr + 0x28) < *(int *)(unaff_retaddr + 0x24) + 4U) {
        CArchive::Flush(unaff_retaddr);
      }
      **(int **)(unaff_retaddr + 0x24) = iVar11 + iVar12 * 4 + iVar4;
      *(int *)(unaff_retaddr + 0x24) = *(int *)(unaff_retaddr + 0x24) + 4;
      piVar3 = *(int **)(param_1 + 0xfc);
      iVar11 = piVar3[5];
      if (iVar11 == 0) {
        iVar11 = piVar3[2];
        if (iVar11 < 0) {
          iVar11 = -iVar11;
        }
        iVar11 = ((uint)*(ushort *)((int)piVar3 + 0xe) * piVar3[1] + 0x1f >> 3 & 0x1ffffffc) *
                 iVar11;
      }
      iVar12 = piVar3[8];
      if ((iVar12 == 0) && (*(ushort *)((int)piVar3 + 0xe) < 9)) {
        iVar12 = 1 << ((byte)*(ushort *)((int)piVar3 + 0xe) & 0x1f);
      }
      CArchive::Write(unaff_retaddr,piVar3,iVar11 + iVar12 * 4 + *piVar3);
    }
    pvStack_10 = (void *)0xffffffff;
    CString::~CString((CString *)&stack0xffffff68);
  }
  pvVar6 = GlobalHandle(plpal);
  GlobalUnlock(pvVar6);
  pvVar6 = GlobalHandle(plpal);
  GlobalFree(pvVar6);
  (**(code **)(**(int **)(param_1 + 0xa4) + 8))(unaff_retaddr);
  (**(code **)(**(int **)(param_1 + 0x9c) + 8))(unaff_retaddr);
  (**(code **)(**(int **)(param_1 + 0xb4) + 8))(unaff_retaddr);
  if (*(int *)(param_1 + 0xe4) == 0) {
    GraphBrdScriptEngine_ParsePageScript(*(void **)(param_1 + 0xb4));
  }
  COleDocument::Serialize(param_1,unaff_retaddr);
  ExceptionList = pvStack_10;
  return;
}


// ============================================================================
// Function: GraphBrdScriptEngine_Serialize at 0041aad0
// Signature: undefined __thiscall GraphBrdScriptEngine_Serialize(void * this, CArchive * param_1)
// Entry: 0041aad0
// Body: 0041aad0 - 0041b2c8
// ============================================================================
void __thiscall GraphBrdScriptEngine_Serialize(void *this,CArchive *param_1)

{
  CArchive *pCVar1;
  undefined4 uVar2;
  uint uVar3;
  CArchive *this_00;
  HGLOBAL pvVar4;
  undefined4 *puVar5;
  LPVOID pvVar6;
  void *pvVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int *piVar12;
  
  this_00 = param_1;
  if ((~*(uint *)(param_1 + 0x14) & 1) == 0) {
    param_1 = (CArchive *)0x0;
    if (0 < *(int *)((int)this + 0x11b0)) {
      piVar12 = (int *)((int)this + 0x1020);
      do {
        iVar8 = *piVar12;
        iVar11 = 0;
        if (0 < *(int *)(iVar8 + 4)) {
          do {
            operator_delete(*(void **)(*(int *)(iVar8 + 8) + iVar11 * 4));
            iVar8 = *piVar12;
            iVar11 = iVar11 + 1;
          } while (iVar11 < *(int *)(iVar8 + 4));
        }
        pvVar4 = GlobalHandle(*(LPCVOID *)(*piVar12 + 8));
        GlobalUnlock(pvVar4);
        pvVar4 = GlobalHandle(*(LPCVOID *)(*piVar12 + 8));
        GlobalFree(pvVar4);
        operator_delete((void *)*piVar12);
        param_1 = param_1 + 1;
        piVar12 = piVar12 + 1;
      } while ((int)param_1 < *(int *)((int)this + 0x11b0));
    }
    if (*(uint *)(this_00 + 0x28) < *(int *)(this_00 + 0x24) + 4U) {
      CArchive::FillBuffer(this_00,(*(int *)(this_00 + 0x24) - *(uint *)(this_00 + 0x28)) + 4);
    }
    uVar3 = **(uint **)(this_00 + 0x24);
    *(uint **)(this_00 + 0x24) = *(uint **)(this_00 + 0x24) + 1;
    CArchive_ReadDwordInline(this_00,(int *)((int)this + 0x11b0));
    CArchive_ReadDwordInline(this_00,(undefined4 *)((int)this + 0x38));
    CArchive_ReadDwordInline(this_00,(undefined4 *)((int)this + 0x3c));
    CArchive_ReadDwordInline(this_00,(undefined4 *)((int)this + 0x30));
    if (*(uint *)(this_00 + 0x28) < *(int *)(this_00 + 0x24) + 4U) {
      CArchive::FillBuffer(this_00,(*(int *)(this_00 + 0x24) - *(uint *)(this_00 + 0x28)) + 4);
    }
    param_1 = (CArchive *)0x0;
    *(undefined4 *)((int)this + 0x34) = **(undefined4 **)(this_00 + 0x24);
    *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 4;
    if (0 < *(int *)((int)this + 0x11b0)) {
      piVar12 = (int *)((int)this + 0x1020);
      do {
        puVar5 = operator_new(0x14);
        *piVar12 = (int)puVar5;
        if (*(uint *)(this_00 + 0x28) < *(int *)(this_00 + 0x24) + 4U) {
          CArchive::FillBuffer(this_00,(*(int *)(this_00 + 0x24) - *(uint *)(this_00 + 0x28)) + 4);
        }
        *puVar5 = **(undefined4 **)(this_00 + 0x24);
        iVar8 = *(int *)(this_00 + 0x24);
        iVar11 = iVar8 + 4;
        *(int *)(this_00 + 0x24) = iVar11;
        puVar5 = (undefined4 *)*piVar12;
        if (*(uint *)(this_00 + 0x28) < iVar8 + 8U) {
          CArchive::FillBuffer(this_00,(iVar11 - *(uint *)(this_00 + 0x28)) + 4);
        }
        *puVar5 = **(undefined4 **)(this_00 + 0x24);
        iVar8 = *(int *)(this_00 + 0x24);
        iVar9 = iVar8 + 4;
        *(int *)(this_00 + 0x24) = iVar9;
        iVar11 = *piVar12;
        if (*(uint *)(this_00 + 0x28) < iVar8 + 8U) {
          CArchive::FillBuffer(this_00,(iVar9 - *(uint *)(this_00 + 0x28)) + 4);
        }
        *(undefined4 *)(iVar11 + 4) = **(undefined4 **)(this_00 + 0x24);
        iVar8 = *(int *)(this_00 + 0x24);
        iVar9 = iVar8 + 4;
        *(int *)(this_00 + 0x24) = iVar9;
        iVar11 = *piVar12;
        if (*(uint *)(this_00 + 0x28) < iVar8 + 8U) {
          CArchive::FillBuffer(this_00,(iVar9 - *(uint *)(this_00 + 0x28)) + 4);
        }
        *(undefined4 *)(iVar11 + 0xc) = **(undefined4 **)(this_00 + 0x24);
        iVar8 = *(int *)(this_00 + 0x24);
        iVar9 = iVar8 + 4;
        *(int *)(this_00 + 0x24) = iVar9;
        iVar11 = *piVar12;
        if (*(uint *)(this_00 + 0x28) < iVar8 + 8U) {
          CArchive::FillBuffer(this_00,(iVar9 - *(uint *)(this_00 + 0x28)) + 4);
        }
        *(undefined4 *)(iVar11 + 0x10) = **(undefined4 **)(this_00 + 0x24);
        *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 4;
        pvVar4 = GlobalAlloc(0x42,*(int *)(*piVar12 + 4) << 2);
        pvVar6 = GlobalLock(pvVar4);
        iVar8 = 0;
        *(LPVOID *)(*piVar12 + 8) = pvVar6;
        if (0 < *(int *)(*piVar12 + 4)) {
          do {
            pvVar7 = operator_new(8);
            *(void **)(*(int *)(*piVar12 + 8) + iVar8 * 4) = pvVar7;
            puVar5 = *(undefined4 **)(*(int *)(*piVar12 + 8) + iVar8 * 4);
            if (*(uint *)(this_00 + 0x28) < *(int *)(this_00 + 0x24) + 4U) {
              CArchive::FillBuffer
                        (this_00,(*(int *)(this_00 + 0x24) - *(uint *)(this_00 + 0x28)) + 4);
            }
            *puVar5 = **(undefined4 **)(this_00 + 0x24);
            iVar11 = *(int *)(this_00 + 0x24);
            iVar10 = iVar11 + 4;
            *(int *)(this_00 + 0x24) = iVar10;
            iVar9 = *(int *)(*(int *)(*piVar12 + 8) + iVar8 * 4);
            if (*(uint *)(this_00 + 0x28) < iVar11 + 8U) {
              CArchive::FillBuffer(this_00,(iVar10 - *(uint *)(this_00 + 0x28)) + 4);
            }
            *(undefined4 *)(iVar9 + 4) = **(undefined4 **)(this_00 + 0x24);
            iVar8 = iVar8 + 1;
            *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 4;
          } while (iVar8 < *(int *)(*piVar12 + 4));
        }
        param_1 = param_1 + 1;
        piVar12 = piVar12 + 1;
      } while ((int)param_1 < *(int *)((int)this + 0x11b0));
    }
    if (*(uint *)(this_00 + 0x28) < *(int *)(this_00 + 0x24) + 4U) {
      CArchive::FillBuffer(this_00,(*(int *)(this_00 + 0x24) - *(uint *)(this_00 + 0x28)) + 4);
    }
    *(undefined4 *)((int)this + 0x14d4) = **(undefined4 **)(this_00 + 0x24);
    *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 4;
    if (*(int *)((int)this + 0x14d4) != 0) {
      CArchive::Read(this_00,(void *)((int)this + 0x11b4),*(int *)((int)this + 0x14d4) * 4);
      CArchive::Read(this_00,(void *)((int)this + 0x1344),*(int *)((int)this + 0x14d4) << 2);
    }
    if (1 < uVar3) {
      if (*(uint *)(this_00 + 0x28) < *(int *)(this_00 + 0x24) + 4U) {
        CArchive::FillBuffer(this_00,(*(int *)(this_00 + 0x24) - *(uint *)(this_00 + 0x28)) + 4);
      }
      *(undefined4 *)((int)this + 0x44) = **(undefined4 **)(this_00 + 0x24);
      iVar8 = *(int *)(this_00 + 0x24);
      iVar11 = iVar8 + 4;
      *(int *)(this_00 + 0x24) = iVar11;
      if (*(uint *)(this_00 + 0x28) < iVar8 + 8U) {
        CArchive::FillBuffer(this_00,(iVar11 - *(uint *)(this_00 + 0x28)) + 4);
      }
      *(undefined4 *)((int)this + 0x48) = **(undefined4 **)(this_00 + 0x24);
      iVar8 = *(int *)(this_00 + 0x24);
      iVar11 = iVar8 + 4;
      *(int *)(this_00 + 0x24) = iVar11;
      if (*(uint *)(this_00 + 0x28) < iVar8 + 8U) {
        CArchive::FillBuffer(this_00,(iVar11 - *(uint *)(this_00 + 0x28)) + 4);
      }
      *(undefined4 *)((int)this + 0x4c) = **(undefined4 **)(this_00 + 0x24);
      iVar8 = *(int *)(this_00 + 0x24);
      iVar11 = iVar8 + 4;
      *(int *)(this_00 + 0x24) = iVar11;
      if (*(uint *)(this_00 + 0x28) < iVar8 + 8U) {
        CArchive::FillBuffer(this_00,(iVar11 - *(uint *)(this_00 + 0x28)) + 4);
      }
      *(undefined4 *)((int)this + 0x50) = **(undefined4 **)(this_00 + 0x24);
      *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 4;
    }
    if (2 < uVar3) {
      if (*(uint *)(this_00 + 0x28) < *(int *)(this_00 + 0x24) + 4U) {
        CArchive::FillBuffer(this_00,(*(int *)(this_00 + 0x24) - *(uint *)(this_00 + 0x28)) + 4);
      }
      *(undefined4 *)((int)this + 0x54) = **(undefined4 **)(this_00 + 0x24);
      *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 4;
    }
    if (3 < uVar3) {
      if (*(uint *)(this_00 + 0x28) < *(int *)(this_00 + 0x24) + 4U) {
        CArchive::FillBuffer(this_00,(*(int *)(this_00 + 0x24) - *(uint *)(this_00 + 0x28)) + 4);
      }
      *(undefined4 *)((int)this + 0x58) = **(undefined4 **)(this_00 + 0x24);
      *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 4;
    }
    *(undefined4 *)((int)this + 0x14e0) = 0;
    return;
  }
  if (*(uint *)(param_1 + 0x28) < *(int *)(param_1 + 0x24) + 4U) {
    CArchive::Flush(param_1);
  }
  **(undefined4 **)(param_1 + 0x24) = 4;
  iVar8 = *(int *)(param_1 + 0x24);
  *(int *)(param_1 + 0x24) = iVar8 + 4;
  uVar2 = *(undefined4 *)((int)this + 0x11b0);
  if (*(uint *)(param_1 + 0x28) < iVar8 + 8U) {
    CArchive::Flush(param_1);
  }
  **(undefined4 **)(param_1 + 0x24) = uVar2;
  iVar8 = *(int *)(param_1 + 0x24);
  *(int *)(param_1 + 0x24) = iVar8 + 4;
  uVar2 = *(undefined4 *)((int)this + 0x38);
  if (*(uint *)(param_1 + 0x28) < iVar8 + 8U) {
    CArchive::Flush(param_1);
  }
  **(undefined4 **)(param_1 + 0x24) = uVar2;
  iVar8 = *(int *)(param_1 + 0x24);
  *(int *)(param_1 + 0x24) = iVar8 + 4;
  uVar2 = *(undefined4 *)((int)this + 0x3c);
  if (*(uint *)(param_1 + 0x28) < iVar8 + 8U) {
    CArchive::Flush(param_1);
  }
  **(undefined4 **)(param_1 + 0x24) = uVar2;
  iVar8 = *(int *)(param_1 + 0x24);
  *(int *)(param_1 + 0x24) = iVar8 + 4;
  uVar2 = *(undefined4 *)((int)this + 0x30);
  if (*(uint *)(param_1 + 0x28) < iVar8 + 8U) {
    CArchive::Flush(param_1);
  }
  **(undefined4 **)(param_1 + 0x24) = uVar2;
  iVar8 = *(int *)(param_1 + 0x24);
  *(int *)(param_1 + 0x24) = iVar8 + 4;
  uVar2 = *(undefined4 *)((int)this + 0x34);
  if (*(uint *)(param_1 + 0x28) < iVar8 + 8U) {
    CArchive::Flush(param_1);
  }
  pCVar1 = param_1 + 0x24;
  param_1 = (CArchive *)0x0;
  **(undefined4 **)pCVar1 = uVar2;
  iVar8 = *(int *)(this_00 + 0x24) + 4;
  *(int *)(this_00 + 0x24) = iVar8;
  if (0 < *(int *)((int)this + 0x11b0)) {
    piVar12 = (int *)((int)this + 0x1020);
    do {
      uVar2 = *(undefined4 *)*piVar12;
      if (*(uint *)(this_00 + 0x28) < iVar8 + 4U) {
        CArchive::Flush(this_00);
      }
      **(undefined4 **)(this_00 + 0x24) = uVar2;
      iVar8 = *(int *)(this_00 + 0x24);
      *(int *)(this_00 + 0x24) = iVar8 + 4;
      uVar2 = *(undefined4 *)*piVar12;
      if (*(uint *)(this_00 + 0x28) < iVar8 + 8U) {
        CArchive::Flush(this_00);
      }
      **(undefined4 **)(this_00 + 0x24) = uVar2;
      iVar8 = *(int *)(this_00 + 0x24);
      *(int *)(this_00 + 0x24) = iVar8 + 4;
      uVar2 = *(undefined4 *)(*piVar12 + 4);
      if (*(uint *)(this_00 + 0x28) < iVar8 + 8U) {
        CArchive::Flush(this_00);
      }
      **(undefined4 **)(this_00 + 0x24) = uVar2;
      iVar8 = *(int *)(this_00 + 0x24);
      *(int *)(this_00 + 0x24) = iVar8 + 4;
      uVar2 = *(undefined4 *)(*piVar12 + 0xc);
      if (*(uint *)(this_00 + 0x28) < iVar8 + 8U) {
        CArchive::Flush(this_00);
      }
      **(undefined4 **)(this_00 + 0x24) = uVar2;
      iVar8 = *(int *)(this_00 + 0x24);
      *(int *)(this_00 + 0x24) = iVar8 + 4;
      uVar2 = *(undefined4 *)(*piVar12 + 0x10);
      if (*(uint *)(this_00 + 0x28) < iVar8 + 8U) {
        CArchive::Flush(this_00);
      }
      **(undefined4 **)(this_00 + 0x24) = uVar2;
      iVar8 = *(int *)(this_00 + 0x24) + 4;
      *(int *)(this_00 + 0x24) = iVar8;
      iVar11 = 0;
      if (0 < *(int *)(*piVar12 + 4)) {
        do {
          uVar2 = **(undefined4 **)(*(int *)(*piVar12 + 8) + iVar11 * 4);
          if (*(uint *)(this_00 + 0x28) < iVar8 + 4U) {
            CArchive::Flush(this_00);
          }
          **(undefined4 **)(this_00 + 0x24) = uVar2;
          iVar8 = *(int *)(this_00 + 0x24);
          *(int *)(this_00 + 0x24) = iVar8 + 4;
          uVar2 = *(undefined4 *)(*(int *)(*(int *)(*piVar12 + 8) + iVar11 * 4) + 4);
          if (*(uint *)(this_00 + 0x28) < iVar8 + 8U) {
            CArchive::Flush(this_00);
          }
          **(undefined4 **)(this_00 + 0x24) = uVar2;
          iVar8 = *(int *)(this_00 + 0x24) + 4;
          iVar11 = iVar11 + 1;
          *(int *)(this_00 + 0x24) = iVar8;
        } while (iVar11 < *(int *)(*piVar12 + 4));
      }
      param_1 = param_1 + 1;
      piVar12 = piVar12 + 1;
    } while ((int)param_1 < *(int *)((int)this + 0x11b0));
  }
  uVar2 = *(undefined4 *)((int)this + 0x14d4);
  if (*(uint *)(this_00 + 0x28) < iVar8 + 4U) {
    CArchive::Flush(this_00);
  }
  **(undefined4 **)(this_00 + 0x24) = uVar2;
  *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 4;
  if (*(int *)((int)this + 0x14d4) != 0) {
    CArchive::Write(this_00,(void *)((int)this + 0x11b4),*(int *)((int)this + 0x14d4) * 4);
    CArchive::Write(this_00,(void *)((int)this + 0x1344),*(int *)((int)this + 0x14d4) << 2);
  }
  uVar2 = *(undefined4 *)((int)this + 0x44);
  if (*(uint *)(this_00 + 0x28) < *(int *)(this_00 + 0x24) + 4U) {
    CArchive::Flush(this_00);
  }
  **(undefined4 **)(this_00 + 0x24) = uVar2;
  iVar8 = *(int *)(this_00 + 0x24);
  *(int *)(this_00 + 0x24) = iVar8 + 4;
  uVar2 = *(undefined4 *)((int)this + 0x48);
  if (*(uint *)(this_00 + 0x28) < iVar8 + 8U) {
    CArchive::Flush(this_00);
  }
  **(undefined4 **)(this_00 + 0x24) = uVar2;
  iVar8 = *(int *)(this_00 + 0x24);
  *(int *)(this_00 + 0x24) = iVar8 + 4;
  uVar2 = *(undefined4 *)((int)this + 0x4c);
  if (*(uint *)(this_00 + 0x28) < iVar8 + 8U) {
    CArchive::Flush(this_00);
  }
  **(undefined4 **)(this_00 + 0x24) = uVar2;
  iVar8 = *(int *)(this_00 + 0x24);
  *(int *)(this_00 + 0x24) = iVar8 + 4;
  uVar2 = *(undefined4 *)((int)this + 0x50);
  if (*(uint *)(this_00 + 0x28) < iVar8 + 8U) {
    CArchive::Flush(this_00);
  }
  **(undefined4 **)(this_00 + 0x24) = uVar2;
  iVar8 = *(int *)(this_00 + 0x24);
  *(int *)(this_00 + 0x24) = iVar8 + 4;
  uVar2 = *(undefined4 *)((int)this + 0x54);
  if (*(uint *)(this_00 + 0x28) < iVar8 + 8U) {
    CArchive::Flush(this_00);
  }
  **(undefined4 **)(this_00 + 0x24) = uVar2;
  iVar8 = *(int *)(this_00 + 0x24);
  *(int *)(this_00 + 0x24) = iVar8 + 4;
  uVar2 = *(undefined4 *)((int)this + 0x58);
  if (*(uint *)(this_00 + 0x28) < iVar8 + 8U) {
    CArchive::Flush(this_00);
  }
  **(undefined4 **)(this_00 + 0x24) = uVar2;
  *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 4;
  return;
}


// ============================================================================
// Function: GraphBrdCntrItem_Serialize at 0042b920
// Signature: undefined __thiscall GraphBrdCntrItem_Serialize(void * this, CArchive * param_1)
// Entry: 0042b920
// Body: 0042b920 - 0042bf91
// ============================================================================
void __thiscall GraphBrdCntrItem_Serialize(void *this,CArchive *param_1)

{
  undefined2 uVar1;
  CArchive *this_00;
  int iVar2;
  undefined4 *puVar3;
  void *pvVar4;
  CString *pCVar5;
  undefined4 uVar6;
  HGLOBAL hMem;
  LPVOID pvVar7;
  int iVar8;
  int iVar9;
  int *piVar10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  this_00 = param_1;
  local_4 = 0xffffffff;
  puStack_8 = &LAB_0043183c;
  local_c = ExceptionList;
  if ((~*(uint *)(param_1 + 0x14) & 1) == 0) {
    ExceptionList = &local_c;
    GraphBrdCntrItem_ClearDispatchMemberCache((int)this);
    if (*(uint *)(param_1 + 0x28) < *(int *)(param_1 + 0x24) + 4U) {
      CArchive::FillBuffer(param_1,(*(int *)(param_1 + 0x24) - *(uint *)(param_1 + 0x28)) + 4);
    }
    iVar9 = *(int *)(param_1 + 0x24);
    iVar2 = iVar9 + 4;
    *(int *)(param_1 + 0x24) = iVar2;
    if (*(uint *)(param_1 + 0x28) < iVar9 + 8U) {
      CArchive::FillBuffer(param_1,(iVar2 - *(uint *)(param_1 + 0x28)) + 4);
    }
    *(undefined4 *)((int)this + 0xe8) = **(undefined4 **)(param_1 + 0x24);
    *(int *)(param_1 + 0x24) = *(int *)(param_1 + 0x24) + 4;
    operator>>(param_1,(CString *)((int)this + 0xf0));
    if (*(uint *)(param_1 + 0x28) < *(int *)(param_1 + 0x24) + 4U) {
      CArchive::FillBuffer(param_1,(*(int *)(param_1 + 0x24) - *(uint *)(param_1 + 0x28)) + 4);
    }
    *(undefined4 *)((int)this + 0x104) = **(undefined4 **)(param_1 + 0x24);
    *(int *)(param_1 + 0x24) = *(int *)(param_1 + 0x24) + 4;
    CArchive::Read(param_1,(void *)((int)this + 0xf4),0x10);
    param_1 = (CArchive *)0x0;
    if (0 < *(int *)((int)this + 0xe8)) {
      piVar10 = (int *)((int)this + 0x20);
      do {
        puVar3 = operator_new(0x1c);
        *piVar10 = (int)puVar3;
        if (*(uint *)(this_00 + 0x28) < *(int *)(this_00 + 0x24) + 4U) {
          CArchive::FillBuffer(this_00,(*(int *)(this_00 + 0x24) - *(uint *)(this_00 + 0x28)) + 4);
        }
        *puVar3 = **(undefined4 **)(this_00 + 0x24);
        iVar9 = *(int *)(this_00 + 0x24);
        iVar8 = iVar9 + 4;
        *(int *)(this_00 + 0x24) = iVar8;
        iVar2 = *piVar10;
        if (*(uint *)(this_00 + 0x28) < iVar9 + 8U) {
          CArchive::FillBuffer(this_00,(iVar8 - *(uint *)(this_00 + 0x28)) + 4);
        }
        *(undefined4 *)(iVar2 + 4) = **(undefined4 **)(this_00 + 0x24);
        *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 4;
        pvVar4 = operator_new(*(int *)(*piVar10 + 4) + 1);
        *(void **)(*piVar10 + 8) = pvVar4;
        CArchive::Read(this_00,*(void **)(*piVar10 + 8),*(int *)(*piVar10 + 4) + 1);
        pCVar5 = operator_new(4);
        local_4 = 0;
        if (pCVar5 == (CString *)0x0) {
          uVar6 = 0;
        }
        else {
          uVar6 = CString::CString(pCVar5);
        }
        local_4 = 0xffffffff;
        *(undefined4 *)(*piVar10 + 0xc) = uVar6;
        operator>>(this_00,*(CString **)(*piVar10 + 0xc));
        pCVar5 = operator_new(4);
        local_4 = 1;
        if (pCVar5 == (CString *)0x0) {
          uVar6 = 0;
        }
        else {
          uVar6 = CString::CString(pCVar5);
        }
        local_4 = 0xffffffff;
        *(undefined4 *)(*piVar10 + 0x10) = uVar6;
        operator>>(this_00,*(CString **)(*piVar10 + 0x10));
        pCVar5 = operator_new(4);
        local_4 = 2;
        if (pCVar5 == (CString *)0x0) {
          uVar6 = 0;
        }
        else {
          uVar6 = CString::CString(pCVar5);
        }
        local_4 = 0xffffffff;
        *(undefined4 *)(*piVar10 + 0x14) = uVar6;
        operator>>(this_00,*(CString **)(*piVar10 + 0x14));
        iVar9 = *piVar10;
        if (*(uint *)(this_00 + 0x28) < *(int *)(this_00 + 0x24) + 2U) {
          CArchive::FillBuffer(this_00,(*(int *)(this_00 + 0x24) - *(uint *)(this_00 + 0x28)) + 2);
        }
        piVar10 = piVar10 + 1;
        *(undefined2 *)(iVar9 + 0x18) = **(undefined2 **)(this_00 + 0x24);
        *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 2;
        param_1 = param_1 + 1;
      } while ((int)param_1 < *(int *)((int)this + 0xe8));
    }
    hMem = GlobalAlloc(0x42,*(int *)((int)this + 0x104) * 4 + 4);
    pvVar7 = GlobalLock(hMem);
    *(LPVOID *)((int)this + 0x108) = pvVar7;
    iVar9 = 0;
    if (0 < *(int *)((int)this + 0x104)) {
      do {
        pvVar4 = operator_new(0x14);
        *(void **)(*(int *)((int)this + 0x108) + iVar9 * 4) = pvVar4;
        puVar3 = *(undefined4 **)(*(int *)((int)this + 0x108) + iVar9 * 4);
        if (*(uint *)(this_00 + 0x28) < *(int *)(this_00 + 0x24) + 4U) {
          CArchive::FillBuffer(this_00,(*(int *)(this_00 + 0x24) - *(uint *)(this_00 + 0x28)) + 4);
        }
        *puVar3 = **(undefined4 **)(this_00 + 0x24);
        *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 4;
        pvVar4 = operator_new(**(int **)(*(int *)((int)this + 0x108) + iVar9 * 4) + 1);
        *(void **)(*(int *)(*(int *)((int)this + 0x108) + iVar9 * 4) + 4) = pvVar4;
        piVar10 = *(int **)(*(int *)((int)this + 0x108) + iVar9 * 4);
        CArchive::Read(this_00,(void *)piVar10[1],*piVar10 + 1);
        iVar2 = *(int *)(*(int *)((int)this + 0x108) + iVar9 * 4);
        if (*(uint *)(this_00 + 0x28) < *(int *)(this_00 + 0x24) + 4U) {
          CArchive::FillBuffer(this_00,(*(int *)(this_00 + 0x24) - *(uint *)(this_00 + 0x28)) + 4);
        }
        *(undefined4 *)(iVar2 + 8) = **(undefined4 **)(this_00 + 0x24);
        *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 4;
        pCVar5 = operator_new(4);
        local_4 = 3;
        if (pCVar5 == (CString *)0x0) {
          uVar6 = 0;
        }
        else {
          uVar6 = CString::CString(pCVar5);
        }
        local_4 = 0xffffffff;
        *(undefined4 *)(*(int *)(*(int *)((int)this + 0x108) + iVar9 * 4) + 0xc) = uVar6;
        operator>>(this_00,*(CString **)(*(int *)(*(int *)((int)this + 0x108) + iVar9 * 4) + 0xc));
        iVar2 = *(int *)(*(int *)((int)this + 0x108) + iVar9 * 4);
        if (*(uint *)(this_00 + 0x28) < *(int *)(this_00 + 0x24) + 2U) {
          CArchive::FillBuffer(this_00,(*(int *)(this_00 + 0x24) - *(uint *)(this_00 + 0x28)) + 2);
        }
        *(undefined2 *)(iVar2 + 0x10) = **(undefined2 **)(this_00 + 0x24);
        iVar9 = iVar9 + 1;
        *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 2;
      } while (iVar9 < *(int *)((int)this + 0x104));
    }
  }
  else {
    ExceptionList = &local_c;
    if (*(uint *)(param_1 + 0x28) < *(int *)(param_1 + 0x24) + 4U) {
      ExceptionList = &local_c;
      CArchive::Flush(param_1);
    }
    **(undefined4 **)(param_1 + 0x24) = 1;
    iVar9 = *(int *)(param_1 + 0x24);
    *(int *)(param_1 + 0x24) = iVar9 + 4;
    uVar6 = *(undefined4 *)((int)this + 0xe8);
    if (*(uint *)(param_1 + 0x28) < iVar9 + 8U) {
      CArchive::Flush(param_1);
    }
    **(undefined4 **)(param_1 + 0x24) = uVar6;
    *(int *)(param_1 + 0x24) = *(int *)(param_1 + 0x24) + 4;
    operator<<(param_1,(CString *)((int)this + 0xf0));
    uVar6 = *(undefined4 *)((int)this + 0x104);
    if (*(uint *)(param_1 + 0x28) < *(int *)(param_1 + 0x24) + 4U) {
      CArchive::Flush(param_1);
    }
    **(undefined4 **)(param_1 + 0x24) = uVar6;
    *(int *)(param_1 + 0x24) = *(int *)(param_1 + 0x24) + 4;
    CArchive::Write(param_1,(void *)((int)this + 0xf4),0x10);
    param_1 = (CArchive *)0x0;
    if (0 < *(int *)((int)this + 0xe8)) {
      piVar10 = (int *)((int)this + 0x20);
      do {
        uVar6 = *(undefined4 *)*piVar10;
        if (*(uint *)(this_00 + 0x28) < *(int *)(this_00 + 0x24) + 4U) {
          CArchive::Flush(this_00);
        }
        **(undefined4 **)(this_00 + 0x24) = uVar6;
        iVar9 = *(int *)(this_00 + 0x24);
        *(int *)(this_00 + 0x24) = iVar9 + 4;
        uVar6 = *(undefined4 *)(*piVar10 + 4);
        if (*(uint *)(this_00 + 0x28) < iVar9 + 8U) {
          CArchive::Flush(this_00);
        }
        **(undefined4 **)(this_00 + 0x24) = uVar6;
        *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 4;
        CArchive::Write(this_00,*(void **)(*piVar10 + 8),*(int *)(*piVar10 + 4) + 1);
        operator<<(this_00,*(CString **)(*piVar10 + 0xc));
        operator<<(this_00,*(CString **)(*piVar10 + 0x10));
        operator<<(this_00,*(CString **)(*piVar10 + 0x14));
        uVar1 = *(undefined2 *)(*piVar10 + 0x18);
        if (*(uint *)(this_00 + 0x28) < *(int *)(this_00 + 0x24) + 2U) {
          CArchive::Flush(this_00);
        }
        piVar10 = piVar10 + 1;
        **(undefined2 **)(this_00 + 0x24) = uVar1;
        *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 2;
        param_1 = param_1 + 1;
      } while ((int)param_1 < *(int *)((int)this + 0xe8));
    }
    iVar9 = 0;
    if (0 < *(int *)((int)this + 0x104)) {
      do {
        uVar6 = **(undefined4 **)(*(int *)((int)this + 0x108) + iVar9 * 4);
        if (*(uint *)(this_00 + 0x28) < *(int *)(this_00 + 0x24) + 4U) {
          CArchive::Flush(this_00);
        }
        **(undefined4 **)(this_00 + 0x24) = uVar6;
        *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 4;
        piVar10 = *(int **)(*(int *)((int)this + 0x108) + iVar9 * 4);
        CArchive::Write(this_00,(void *)piVar10[1],*piVar10 + 1);
        uVar6 = *(undefined4 *)(*(int *)(*(int *)((int)this + 0x108) + iVar9 * 4) + 8);
        if (*(uint *)(this_00 + 0x28) < *(int *)(this_00 + 0x24) + 4U) {
          CArchive::Flush(this_00);
        }
        **(undefined4 **)(this_00 + 0x24) = uVar6;
        *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 4;
        operator<<(this_00,*(CString **)(*(int *)(*(int *)((int)this + 0x108) + iVar9 * 4) + 0xc));
        uVar1 = *(undefined2 *)(*(int *)(*(int *)((int)this + 0x108) + iVar9 * 4) + 0x10);
        if (*(uint *)(this_00 + 0x28) < *(int *)(this_00 + 0x24) + 2U) {
          CArchive::Flush(this_00);
        }
        **(undefined2 **)(this_00 + 0x24) = uVar1;
        iVar9 = iVar9 + 1;
        *(int *)(this_00 + 0x24) = *(int *)(this_00 + 0x24) + 2;
      } while (iVar9 < *(int *)((int)this + 0x104));
    }
  }
  piVar10 = *(int **)((int)this + 0x10c);
  if (piVar10 != (int *)0x0) {
    (**(code **)(*piVar10 + 0x44))(piVar10,this_00);
  }
  ExceptionList = local_c;
  return;
}

// clang-format on
