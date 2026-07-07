/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Notes: raw Ghidra decompiler output with only file-level framing added.
*/
#include "../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: AudioHolderCriticalSection_Destroy at 10003840
// Signature: undefined __fastcall AudioHolderCriticalSection_Destroy(int param_1)
// Entry: 10003840
// Body: 10003840 - 1000384a
// ============================================================================
void __fastcall AudioHolderCriticalSection_Destroy(int param_1)

{
  DeleteCriticalSection((LPCRITICAL_SECTION)(param_1 + 4));
  return;
}


// ============================================================================
// Function: FUN_10003850 at 10003850
// Signature: undefined __fastcall FUN_10003850(undefined4 * param_1)
// Entry: 10003850
// Body: 10003850 - 10003969
// ============================================================================
void __fastcall FUN_10003850(undefined4 *param_1)

{
  undefined4 *puVar1;
  int iVar2;
  char *pcStack_14;
  undefined4 *local_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  uint local_4;
  
  puStack_8 = &LAB_1000738f;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  *param_1 = &PTR_LAB_10008978;
  param_1[1] = &PTR_LAB_10008948;
  param_1[2] = &PTR_LAB_10008928;
  param_1[3] = &PTR_LAB_10008918;
  local_4 = 0;
  local_10 = param_1;
  if ((int *)param_1[0x8a] != (int *)0x0) {
    (**(code **)(*(int *)param_1[0x8a] + 4))(1);
  }
  iVar2 = 0;
  if (0 < (int)param_1[0x87]) {
    puVar1 = param_1 + 0xf;
    do {
      AudioHolder_StopPlaybackForEntry(param_1,iVar2);
      operator_delete((void *)*puVar1);
      iVar2 = iVar2 + 1;
      puVar1 = puVar1 + 1;
    } while (iVar2 < (int)param_1[0x87]);
  }
  if ((int *)param_1[0x88] != (int *)0x0) {
    iVar2 = *(int *)param_1[0x88];
    if (param_1[0x8d] == 0) {
      (**(code **)(iVar2 + 0x20))(&pcStack_14);
      puStack_8 = (undefined1 *)CONCAT31(puStack_8._1_3_,1);
      (**(code **)(*(int *)param_1[0x88] + 0x54))();
      if ((int *)param_1[0x88] != (int *)0x0) {
        (**(code **)(*(int *)param_1[0x88] + 4))(1);
      }
      CFile::Remove(pcStack_14);
      local_4 = local_4 & 0xffffff00;
      CString::~CString((CString *)&pcStack_14);
    }
    else {
      (**(code **)(iVar2 + 0x54))();
      if ((int *)param_1[0x88] != (int *)0x0) {
        (**(code **)(*(int *)param_1[0x88] + 4))(1);
      }
    }
  }
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
// Function: AudioHolder_AttachBackPointer at 10003b00
// Signature: undefined4 __stdcall AudioHolder_AttachBackPointer(int param_1)
// Entry: 10003b00
// Body: 10003b00 - 10003b1b
// ============================================================================
undefined4 AudioHolder_AttachBackPointer(int param_1)

{
  (**(code **)(*(int *)(param_1 + 4) + 0x2c))(param_1 + 4);
  *(int *)(param_1 + 0x34) = param_1;
  return 0;
}


// ============================================================================
// Function: AudioArchive_WriteFileRange at 10003b40
// Signature: undefined4 __stdcall AudioArchive_WriteFileRange(CArchive * param_1, int * param_2, uint param_3)
// Entry: 10003b40
// Body: 10003b40 - 10003be2
// ============================================================================
undefined4 AudioArchive_WriteFileRange(CArchive *param_1,int *param_2,uint param_3)

{
  HGLOBAL pvVar1;
  LPVOID pMem;
  uint uVar2;
  uint uVar3;
  
  pvVar1 = GlobalAlloc(2,0xffff);
  pMem = GlobalLock(pvVar1);
  if (pMem == (LPVOID)0x0) {
    return 0;
  }
  if (param_3 < 0x10000) {
    uVar3 = param_3;
    if (param_3 == 0) goto LAB_10003bb4;
  }
  else {
    uVar3 = 0xffff;
  }
  do {
    while( true ) {
      uVar2 = (**(code **)(*param_2 + 0x3c))(pMem,uVar3);
      if (uVar2 != uVar3) {
        return 0;
      }
      CArchive::Write(param_1,pMem,uVar3);
      param_3 = param_3 - uVar3;
      if (param_3 < 0x10000) break;
      uVar3 = 0xffff;
    }
    uVar3 = param_3;
  } while (param_3 != 0);
LAB_10003bb4:
  CArchive::Flush(param_1);
  pvVar1 = GlobalHandle(pMem);
  GlobalUnlock(pvVar1);
  pvVar1 = GlobalHandle(pMem);
  GlobalFree(pvVar1);
  return 1;
}


// ============================================================================
// Function: AudioArchive_ReadFileRange at 10003bf0
// Signature: undefined4 __stdcall AudioArchive_ReadFileRange(int * param_1, CArchive * param_2, uint param_3)
// Entry: 10003bf0
// Body: 10003bf0 - 10003c91
// ============================================================================
undefined4 AudioArchive_ReadFileRange(int *param_1,CArchive *param_2,uint param_3)

{
  HGLOBAL pvVar1;
  LPVOID pMem;
  uint uVar2;
  uint uVar3;
  
  pvVar1 = GlobalAlloc(2,0xffff);
  pMem = GlobalLock(pvVar1);
  if (pMem == (LPVOID)0x0) {
    return 0;
  }
  if (param_3 < 0x10000) {
    uVar3 = param_3;
    if (param_3 == 0) goto LAB_10003c64;
  }
  else {
    uVar3 = 0xffff;
  }
  do {
    while( true ) {
      uVar2 = CArchive::Read(param_2,pMem,uVar3);
      if (uVar2 != uVar3) {
        return 0;
      }
      (**(code **)(*param_1 + 0x40))(pMem,uVar3);
      param_3 = param_3 - uVar3;
      if (param_3 < 0x10000) break;
      uVar3 = 0xffff;
    }
    uVar3 = param_3;
  } while (param_3 != 0);
LAB_10003c64:
  (**(code **)(*param_1 + 0x50))();
  pvVar1 = GlobalHandle(pMem);
  GlobalUnlock(pvVar1);
  pvVar1 = GlobalHandle(pMem);
  GlobalFree(pvVar1);
  return 1;
}


// ============================================================================
// Function: AudioHolder_StartPlaybackForEntry at 10004280
// Signature: undefined4 __thiscall AudioHolder_StartPlaybackForEntry(void * this, int param_1)
// Entry: 10004280
// Body: 10004280 - 100047a6
// ============================================================================
undefined4 __thiscall AudioHolder_StartPlaybackForEntry(void *this,int param_1)

{
  undefined4 *puVar1;
  int *piVar2;
  int *piVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  void *pvVar7;
  int **ppiVar8;
  UINT_PTR UVar9;
  int unaff_EBX;
  undefined4 unaff_EBP;
  int **ppiVar10;
  int **ppiVar11;
  uint uVar12;
  int *piStack_70;
  int *piStack_6c;
  int *piStack_68;
  undefined4 local_54;
  undefined4 uStack_50;
  int local_4c;
  uint local_48;
  uint uStack_44;
  undefined2 *puStack_40;
  undefined2 uStack_26;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined2 uStack_1c;
  undefined2 uStack_1a;
  uint uStack_14;
  
  piStack_68 = *(int **)((int)this + 0x2c);
  if (piStack_68 != (int *)0x0) {
    piStack_6c = (int *)0x10004298;
    uVar4 = (**(code **)(*piStack_68 + 0x28))();
    *(undefined4 *)((int)this + 0x38) = uVar4;
  }
  if (param_1 < 0) {
    return 0;
  }
  if (*(int *)((int)this + 0x21c) <= param_1) {
    return 0;
  }
  if (*(int *)((int)this + 0x38) == 0) {
    return 0;
  }
  if (*(int **)((int)this + 0x220) == (int *)0x0) {
    return 0;
  }
  iVar5 = *(int *)((int)this + param_1 * 4 + 0x3c);
  local_54 = 0;
  local_48 = 0;
  local_4c = 0;
  iVar6 = *(int *)(iVar5 + 0x58);
  if (iVar6 != 0) {
    *(undefined4 *)(iVar6 + 0x28) = 0;
    *(undefined4 *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 0x2c) = 0;
    iVar5 = *(int *)((int)this + param_1 * 4 + 0x3c);
    *(int *)(*(int *)(iVar5 + 0x58) + 0x30) = (*(int *)(iVar5 + 8) - *(int *)(iVar5 + 4)) + -0x2c;
    puVar1 = *(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
    if (puVar1[1] == 0) {
      if (puVar1 != (undefined4 *)0x0) {
        piStack_6c = (int *)*puVar1;
        piStack_68 = &local_4c;
        piStack_70 = (int *)0x10004324;
        (**(code **)(*piStack_6c + 0x24))();
      }
      if (local_4c != 1) {
        return 1;
      }
      piStack_68 = (int *)0x0;
      piStack_6c = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
      piStack_70 = (int *)0x1000433f;
      iVar5 = (**(code **)(*piStack_6c + 0x34))();
      if (iVar5 == 0) {
        return 1;
      }
    }
    else {
      iVar5 = *(int *)((int)this + param_1 * 4 + 0x3c);
      piStack_68 = (int *)0x0;
      piStack_6c = (int *)(*(int *)(iVar5 + 4) + 0x2c);
      piStack_70 = (int *)0x1000436d;
      iVar6 = (**(code **)(**(int **)((int)this + 0x220) + 0x30))();
      if (iVar6 == *(int *)(iVar5 + 4) + 0x2c) {
        piStack_70 = (int *)**(undefined4 **)(iVar5 + 0x58);
        (**(code **)(*piStack_70 + 0x48))();
        puVar1 = *(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
        piVar2 = (int *)*puVar1;
        iVar5 = (**(code **)(*piVar2 + 0x2c))
                          (piVar2,0,puVar1[2],&stack0xffffffa0,&stack0xffffffa4,&local_54,&uStack_50
                          );
        if ((iVar5 == 0) && (unaff_EBX != 0)) {
          iVar5 = *(int *)((int)this + param_1 * 4 + 0x3c);
          puVar1 = *(undefined4 **)(iVar5 + 0x58);
          piStack_70 = *(int **)(*(int *)(iVar5 + 0x58) + 8);
          iVar5 = (**(code **)(**(int **)((int)this + 0x220) + 0x3c))();
          if (iVar5 == puVar1[2]) {
            piVar2 = (int *)*puVar1;
            piStack_70 = (int *)0x0;
            iVar5 = (**(code **)(*piVar2 + 0x4c))(piVar2,unaff_EBP,unaff_EBX);
            if (iVar5 == 0) {
              piStack_70 = (int *)0x10004409;
              uVar4 = (**(code **)(**(int **)((int)this + 0x220) + 0x14))();
              piStack_70 = (int *)0x1;
              *(undefined4 *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 4) = uVar4
              ;
              *(undefined4 *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 0x10) = 1;
              *(undefined4 *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 0xc) = 0;
              piVar2 = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
              iVar5 = (**(code **)(*piVar2 + 0x30))(piVar2,0);
              if (iVar5 == 0) {
                return 1;
              }
            }
          }
        }
      }
    }
    piStack_70 = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
    iVar5 = (**(code **)(*piStack_70 + 8))();
    while (iVar5 != 0) {
      iVar5 = (**(code **)(*(int *)**(undefined4 **)
                                     (*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 8))();
    }
    operator_delete(*(void **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58));
    *(undefined4 *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) = 0;
    return 0;
  }
  piStack_6c = *(int **)(iVar5 + 4);
  piStack_68 = (int *)0x0;
  piStack_70 = (int *)0x100044a4;
  iVar5 = (**(code **)(**(int **)((int)this + 0x220) + 0x30))();
  if (iVar5 != *(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 4)) {
    return 0;
  }
  piStack_70 = (int *)0x2c;
  iVar5 = (**(code **)(**(int **)((int)this + 0x220) + 0x3c))();
  if (iVar5 != 0x2c) {
    return 0;
  }
  uStack_50 = 0;
  local_4c = 0;
  local_48 = 0;
  uStack_44 = 0;
  puStack_40 = (undefined2 *)0x0;
  pvVar7 = operator_new(0x34);
  *(void **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) = pvVar7;
  *(undefined4 *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 0x28) = 0;
  *(undefined4 *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 0x2c) = 0;
  iVar5 = *(int *)((int)this + param_1 * 4 + 0x3c);
  *(int *)(*(int *)(iVar5 + 0x58) + 0x30) = (*(int *)(iVar5 + 8) - *(int *)(iVar5 + 4)) + -0x2c;
  uStack_50 = 0x14;
  local_4c = 0xe2;
  iVar5 = *(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
  puStack_40 = (undefined2 *)(iVar5 + 0x14);
  *puStack_40 = 1;
  *(undefined2 *)(iVar5 + 0x16) = uStack_26;
  *(undefined4 *)(iVar5 + 0x18) = uStack_24;
  *(undefined4 *)(iVar5 + 0x1c) = uStack_20;
  *(undefined2 *)(iVar5 + 0x20) = uStack_1c;
  *(undefined2 *)(iVar5 + 0x22) = uStack_1a;
  *(undefined2 *)(iVar5 + 0x24) = 0;
  local_48 = (uint)(*(ushort *)(iVar5 + 0x22) >> 3) * (uint)*(ushort *)(iVar5 + 0x16) *
             *(int *)(iVar5 + 0x18) * 6;
  if (uStack_14 <= local_48) {
    local_48 = uStack_14;
  }
  uVar12 = 0;
  iVar5 = (**(code **)(**(int **)((int)this + 0x38) + 0xc))
                    (*(int **)((int)this + 0x38),&uStack_50,
                     *(undefined4 *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58));
  if (iVar5 != 0) goto LAB_10004784;
  piVar2 = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
  ppiVar11 = &piStack_68;
  ppiVar10 = &piStack_70;
  iVar5 = (**(code **)(*piVar2 + 0x2c))
                    (piVar2,0,unaff_EBX,&stack0xffffff8c,ppiVar10,ppiVar11,&stack0xffffff9c,0);
  if ((iVar5 == 0) && (ppiVar11 != (int **)0x0)) {
    *(uint *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 8) = uVar12 >> 1;
    if (uVar12 == uStack_44) {
      *(undefined4 *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 4) = 0;
      ppiVar8 = (int **)(**(code **)(**(int **)((int)this + 0x220) + 0x3c))(ppiVar10,ppiVar11);
      if ((ppiVar8 == ppiVar11) &&
         (piVar2 = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58),
         iVar5 = (**(code **)(*piVar2 + 0x4c))(piVar2,ppiVar10,ppiVar11,0,0), iVar5 == 0)) {
        piVar2 = *(int **)((int)this + param_1 * 4 + 0x3c);
        piVar3 = *(int **)piVar2[0x16];
        iVar5 = (**(code **)(*piVar3 + 0x30))(piVar3,0,0,*piVar2 != 0);
joined_r0x10004725:
        if (iVar5 == 0) {
          if (*(int *)((int)this + 0x230) == 0) {
            UVar9 = FUN_100065f0(*(void **)((int)this + 0x228),100);
            *(UINT_PTR *)((int)this + 0x22c) = UVar9;
          }
          *(int *)((int)this + 0x230) = *(int *)((int)this + 0x230) + 1;
          return 1;
        }
      }
    }
    else {
      iVar5 = (**(code **)(**(int **)((int)this + 0x220) + 0x3c))
                        (ppiVar10,*(undefined4 *)
                                   (*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 8));
      if (iVar5 == *(int *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 8)) {
        uVar4 = (**(code **)(**(int **)((int)this + 0x220) + 0x14))();
        *(undefined4 *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 4) = uVar4;
        piVar2 = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
        iVar5 = (**(code **)(*piVar2 + 0x4c))(piVar2,ppiVar10,ppiVar11,0,0);
        if (iVar5 == 0) {
          *(undefined4 *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 0x10) = 1;
          *(undefined4 *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 0xc) = 0;
          piVar2 = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
          iVar5 = (**(code **)(*piVar2 + 0x30))(piVar2,0,0,1);
          goto joined_r0x10004725;
        }
      }
    }
  }
  piVar2 = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
  iVar5 = (**(code **)(*piVar2 + 8))(piVar2);
  while (iVar5 != 0) {
    piVar2 = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
    iVar5 = (**(code **)(*piVar2 + 8))(piVar2);
  }
LAB_10004784:
  operator_delete(*(void **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58));
  *(undefined4 *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) = 0;
  return 0;
}


// ============================================================================
// Function: AudioHolder_StopPlaybackForEntry at 100047b0
// Signature: undefined4 __thiscall AudioHolder_StopPlaybackForEntry(void * this, int param_1)
// Entry: 100047b0
// Body: 100047b0 - 10004872
// ============================================================================
undefined4 __thiscall AudioHolder_StopPlaybackForEntry(void *this,int param_1)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  
  if ((((-1 < param_1) && (param_1 < *(int *)((int)this + 0x21c))) &&
      (*(int *)((int)this + 0x38) != 0)) &&
     (puVar1 = *(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58),
     puVar1 != (undefined4 *)0x0)) {
    piVar2 = (int *)*puVar1;
    (**(code **)(*piVar2 + 0x48))(piVar2);
    piVar2 = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
    iVar3 = (**(code **)(*piVar2 + 8))(piVar2);
    while (iVar3 != 0) {
      piVar2 = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
      iVar3 = (**(code **)(*piVar2 + 8))(piVar2);
    }
    operator_delete(*(void **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58));
    *(undefined4 *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) = 0;
    iVar3 = *(int *)((int)this + 0x230) + -1;
    *(int *)((int)this + 0x230) = iVar3;
    if (iVar3 == 0) {
      FUN_10006630(*(void **)((int)this + 0x228),*(UINT_PTR *)((int)this + 0x22c));
    }
    if (*(int *)((int)this + 0x34) != 0) {
      (**(code **)(*(int *)((int)this + 8) + 0x1c))((int)this + 8,param_1);
    }
    return 1;
  }
  return 0;
}


// ============================================================================
// Function: FUN_10004880 at 10004880
// Signature: undefined4 __stdcall FUN_10004880(int param_1, undefined4 param_2)
// Entry: 10004880
// Body: 10004880 - 1000489f
// ============================================================================
undefined4 FUN_10004880(int param_1,undefined4 param_2)

{
  int *piVar1;
  
  piVar1 = *(int **)(param_1 + 0x28);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 0x1c))(piVar1,param_2);
  }
  return 0;
}


// ============================================================================
// Function: FUN_100048a0 at 100048a0
// Signature: undefined __fastcall FUN_100048a0(void * param_1)
// Entry: 100048a0
// Body: 100048a0 - 10004c4a
// ============================================================================
void __fastcall FUN_100048a0(void *param_1)

{
  void *pvVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  uint uVar5;
  int iVar6;
  undefined4 uVar7;
  uint uVar8;
  void *unaff_EBX;
  int *piVar9;
  undefined4 *puVar10;
  void *local_28;
  int iStack_24;
  int iStack_20;
  int iStack_1c;
  int iStack_18;
  int iStack_14;
  undefined4 uStack_10;
  int iStack_c;
  undefined1 auStack_8 [8];
  
  local_28 = param_1;
  iVar2 = (**(code **)(**(int **)((int)param_1 + 0x2c) + 0x28))(*(int **)((int)param_1 + 0x2c));
  if ((iVar2 == 0) || (iStack_24 = 0, *(int *)((int)param_1 + 0x21c) < 1)) {
    return;
  }
  piVar9 = (int *)((int)param_1 + 0x3c);
  do {
    iVar2 = iStack_24;
    piVar4 = *(int **)(*piVar9 + 0x58);
    if ((piVar4 != (int *)0x0) && (*piVar4 != 0)) {
      if (piVar4[1] == 0) {
        iVar3 = (**(code **)(*(int *)*piVar4 + 0x24))((int *)*piVar4,&iStack_c);
        if (iVar3 == 0) {
          iVar3 = (**(code **)(*(int *)**(undefined4 **)(*piVar9 + 0x58) + 0x10))
                            ((int *)**(undefined4 **)(*piVar9 + 0x58),&local_28,auStack_8);
          if (iVar3 != 0) goto LAB_10004c39;
          if ((iStack_c == 0) ||
             (iVar3 = *(int *)(*piVar9 + 0x58),
             *(void **)(iVar3 + 0x30) <=
             (void *)((int)local_28 + *(int *)(iVar3 + 0x2c) * *(int *)(iVar3 + 8) * 2))) {
            AudioHolder_StopPlaybackForEntry(param_1,iVar2);
          }
        }
      }
      else {
        iStack_1c = 0;
        iStack_20 = 0;
        piVar4 = (int *)**(undefined4 **)(*piVar9 + 0x58);
        iVar3 = (**(code **)(*piVar4 + 0x10))(piVar4,&local_28,auStack_8);
        if (iVar3 != 0) goto LAB_10004c39;
        piVar4 = (int *)*piVar9;
        if (*piVar4 == 0) {
          if (local_28 < *(void **)(piVar4[0x16] + 0x28)) {
            piVar4 = (int *)(piVar4[0x16] + 0x2c);
            *piVar4 = *piVar4 + 1;
          }
          *(void **)(*(int *)(*piVar9 + 0x58) + 0x28) = local_28;
          piVar4 = (int *)*piVar9;
          iVar3 = piVar4[0x16];
          if (*(void **)(iVar3 + 0x30) <=
              (void *)((int)local_28 + *(int *)(iVar3 + 0x2c) * *(int *)(iVar3 + 8) * 2)) {
            AudioHolder_StopPlaybackForEntry(param_1,iVar2);
            goto LAB_10004bbf;
          }
        }
        puVar10 = (undefined4 *)piVar4[0x16];
        pvVar1 = (void *)puVar10[2];
        if (((local_28 < pvVar1) && (puVar10[4] != 0)) || ((pvVar1 < local_28 && (puVar10[3] != 0)))
           ) {
          piVar4 = (int *)*puVar10;
          if (puVar10[4] == 0) {
            iVar3 = (**(code **)(*piVar4 + 0x2c))
                              (piVar4,0,pvVar1,&iStack_1c,&uStack_10,&iStack_20,&iStack_14,0);
            if (iVar3 != 0) goto LAB_10004c39;
            *(undefined4 *)(*(int *)(*piVar9 + 0x58) + 0xc) = 0;
            *(undefined4 *)(*(int *)(*piVar9 + 0x58) + 0x10) = 1;
          }
          else {
            iVar3 = (**(code **)(*piVar4 + 0x2c))(piVar4,pvVar1);
            if (iVar3 != 0) goto LAB_10004c39;
            *(undefined4 *)(*(int *)(*piVar9 + 0x58) + 0x10) = 0;
            *(undefined4 *)(*(int *)(*piVar9 + 0x58) + 0xc) = 1;
          }
          iVar2 = *(int *)(*piVar9 + 0x58);
          iVar3 = *(int *)(iVar2 + 4);
          uVar8 = *(uint *)(iVar2 + 8);
          uVar5 = *(int *)(*piVar9 + 8) - iVar3;
          if (uVar5 <= uVar8) {
            uVar8 = uVar5;
          }
          iVar6 = iStack_1c;
          if (iStack_1c == 0) {
            iVar6 = iStack_20;
          }
          iStack_18 = iVar6;
          iVar3 = (**(code **)(**(int **)((int)param_1 + 0x220) + 0x30))(iVar3,0);
          iVar2 = iStack_24;
          if ((iVar3 != *(int *)(*(int *)(*piVar9 + 0x58) + 4)) ||
             (uVar5 = (**(code **)(**(int **)((int)param_1 + 0x220) + 0x3c))(iVar6,uVar8),
             iVar2 = iStack_24, uVar5 != uVar8)) goto LAB_10004c39;
          puVar10 = (undefined4 *)((int *)*piVar9)[0x16];
          uVar5 = puVar10[2];
          param_1 = unaff_EBX;
          if (uVar8 != uVar5) {
            if (*(int *)*piVar9 == 0) {
              if (*(short *)((int)puVar10 + 0x22) == 8) {
                puVar10 = (undefined4 *)(uVar8 + iVar6);
                if (puVar10 < (undefined4 *)(uVar5 + iVar6)) {
                  uVar5 = (int)(uVar5 + iVar6) - (int)puVar10;
                  for (uVar8 = uVar5 >> 2; uVar8 != 0; uVar8 = uVar8 - 1) {
                    *puVar10 = 0x7f7f7f7f;
                    puVar10 = puVar10 + 1;
                  }
                  for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
                    *(undefined1 *)puVar10 = 0x7f;
                    puVar10 = (undefined4 *)((int)puVar10 + 1);
                  }
                }
              }
              else if (*(short *)((int)puVar10 + 0x22) == 0x10) {
                puVar10 = (undefined4 *)(uVar8 + iVar6);
                if (puVar10 < (undefined4 *)(uVar5 + iVar6)) {
                  uVar8 = (int)(uVar5 + iVar6) + (1 - (int)puVar10);
                  for (uVar5 = uVar8 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
                    *puVar10 = 0;
                    puVar10 = puVar10 + 1;
                  }
                  for (uVar8 = (uint)((uVar8 >> 1 & 1) != 0); uVar8 != 0; uVar8 = uVar8 - 1) {
                    *(undefined2 *)puVar10 = 0;
                    puVar10 = (undefined4 *)((int)puVar10 + 2);
                  }
                }
              }
              puVar10 = *(undefined4 **)(*piVar9 + 0x58);
              if (puVar10[4] == 0) {
                *(undefined4 *)
                 (*(int *)(*(int *)((int)unaff_EBX + iStack_24 * 4 + 0x3c) + 0x58) + 4) = 0;
                piVar9 = (int *)**(undefined4 **)
                                  (*(int *)((int)unaff_EBX + iStack_24 * 4 + 0x3c) + 0x58);
                (**(code **)(*piVar9 + 0x4c))(piVar9,iStack_1c,uStack_10,iStack_20,iStack_14);
                piVar9 = (int *)**(undefined4 **)
                                  (*(int *)((int)unaff_EBX + iVar2 * 4 + 0x3c) + 0x58);
                iVar3 = (**(code **)(*piVar9 + 0x30))(piVar9,0,0,0);
                iVar2 = iStack_24;
                if (iVar3 == 0) {
                  return;
                }
                goto LAB_10004c39;
              }
            }
            else {
              iVar3 = *piVar9;
              iVar6 = (**(code **)(**(int **)((int)unaff_EBX + 0x220) + 0x30))
                                (*(int *)(iVar3 + 4) + 0x2c,0);
              iVar2 = iStack_24;
              if (iVar6 != *(int *)(iVar3 + 4) + 0x2c) {
LAB_10004c39:
                AudioHolder_StopPlaybackForEntry(param_1,iVar2);
                return;
              }
              puVar10 = *(undefined4 **)(*piVar9 + 0x58);
              iVar3 = (**(code **)(**(int **)((int)unaff_EBX + 0x220) + 0x3c))
                                (iStack_18 + uVar8,*(int *)(*(int *)(iVar3 + 0x58) + 8) - uVar8);
              iVar2 = iStack_24;
              if (iVar3 != puVar10[2] - uVar8) goto LAB_10004c39;
            }
          }
          iVar2 = iStack_14;
          (**(code **)(*(int *)*puVar10 + 0x4c))((int *)*puVar10,iStack_1c,uStack_10,iStack_20);
          uVar7 = (**(code **)(**(int **)(iVar2 + 0x220) + 0x14))();
          *(undefined4 *)(*(int *)(*piVar9 + 0x58) + 4) = uVar7;
          iVar2 = iStack_24;
        }
      }
    }
LAB_10004bbf:
    iStack_24 = iVar2 + 1;
    piVar9 = piVar9 + 1;
    if (*(int *)((int)param_1 + 0x21c) <= iStack_24) {
      return;
    }
  } while( true );
}


// ============================================================================
// Function: AudioHolder_DeletePropertiesDialog at 10004c50
// Signature: undefined __stdcall AudioHolder_DeletePropertiesDialog(void * param_1)
// Entry: 10004c50
// Body: 10004c50 - 10004c5d
// ============================================================================
void AudioHolder_DeletePropertiesDialog(void *param_1)

{
  operator_delete(param_1);
  return;
}


// ============================================================================
// Function: AudioHolder_SetPlaybackParams at 10004d30
// Signature: undefined4 __stdcall AudioHolder_SetPlaybackParams(int param_1, int param_2, int param_3, int param_4, int param_5)
// Entry: 10004d30
// Body: 10004d30 - 10004ddd
// ============================================================================
undefined4
AudioHolder_SetPlaybackParams(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  
  if (((-1 < param_2) && (param_2 < *(int *)(param_1 + 0x218))) &&
     (puVar1 = *(undefined4 **)(*(int *)(param_1 + 0x38 + param_2 * 4) + 0x58),
     puVar1 != (undefined4 *)0x0)) {
    if ((param_5 < 100) || (100000 < param_5)) {
      param_5 = 0;
    }
    if ((0 < param_3) || (param_3 < -10000)) {
      param_3 = 0;
    }
    if ((param_4 < -10000) || (10000 < param_4)) {
      param_4 = 0;
    }
    piVar2 = (int *)*puVar1;
    iVar3 = (**(code **)(*piVar2 + 0x44))(piVar2,param_5);
    if (iVar3 == 0) {
      piVar2 = (int *)**(undefined4 **)(*(int *)(param_1 + 0x38 + param_2 * 4) + 0x58);
      iVar3 = (**(code **)(*piVar2 + 0x40))(piVar2,param_4);
      if (iVar3 == 0) {
        piVar2 = (int *)**(undefined4 **)(*(int *)(param_1 + 0x38 + param_2 * 4) + 0x58);
        iVar3 = (**(code **)(*piVar2 + 0x3c))(piVar2,param_3);
        if (iVar3 == 0) {
          return 0;
        }
      }
    }
  }
  return 1;
}


// ============================================================================
// Function: AudioSettingsPage_Construct at 10005010
// Signature: CPropertyPage * __fastcall AudioSettingsPage_Construct(CPropertyPage * param_1)
// Entry: 10005010
// Body: 10005010 - 10005167
// ============================================================================
CPropertyPage * __fastcall AudioSettingsPage_Construct(CPropertyPage *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_100074d4;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  CPropertyPage::CPropertyPage(param_1,0xc9,0);
  local_4 = 0;
  CWnd::CWnd((CWnd *)(param_1 + 0x9c));
  *(undefined ***)(param_1 + 0x9c) = &PTR_LAB_10008e58;
  local_4._0_1_ = 1;
  CWnd::CWnd((CWnd *)(param_1 + 0xdc));
  *(undefined ***)(param_1 + 0xdc) = &PTR_LAB_10008e58;
  local_4._0_1_ = 2;
  CWnd::CWnd((CWnd *)(param_1 + 0x11c));
  *(undefined ***)(param_1 + 0x11c) = &PTR_LAB_10008e58;
  local_4._0_1_ = 3;
  CWnd::CWnd((CWnd *)(param_1 + 0x15c));
  *(undefined ***)(param_1 + 0x15c) = &PTR_LAB_10008d98;
  local_4._0_1_ = 4;
  CWnd::CWnd((CWnd *)(param_1 + 0x19c));
  *(undefined ***)(param_1 + 0x19c) = &PTR_LAB_10008e58;
  local_4._0_1_ = 5;
  CWnd::CWnd((CWnd *)(param_1 + 0x1dc));
  *(undefined ***)(param_1 + 0x1dc) = &PTR_LAB_10008e58;
  local_4._0_1_ = 6;
  CWnd::CWnd((CWnd *)(param_1 + 0x21c));
  *(undefined ***)(param_1 + 0x21c) = &PTR_LAB_10008e58;
  local_4._0_1_ = 7;
  CWnd::CWnd((CWnd *)(param_1 + 0x25c));
  *(undefined ***)(param_1 + 0x25c) = &PTR_LAB_10008cc0;
  local_4._0_1_ = 8;
  CWnd::CWnd((CWnd *)(param_1 + 0x29c));
  *(undefined ***)(param_1 + 0x29c) = &PTR_LAB_10008e58;
  local_4._0_1_ = 9;
  CWnd::CWnd((CWnd *)(param_1 + 0x2dc));
  *(undefined ***)(param_1 + 0x2dc) = &PTR_LAB_10008e58;
  local_4 = CONCAT31(local_4._1_3_,10);
  CWnd::CWnd((CWnd *)(param_1 + 0x31c));
  *(undefined ***)(param_1 + 0x31c) = &PTR_LAB_10008e58;
  *(undefined ***)param_1 = &PTR_LAB_10008bc8;
  *(undefined4 *)(param_1 + 0x94) = 0;
  ExceptionList = local_c;
  return param_1;
}


// ============================================================================
// Function: FUN_10005170 at 10005170
// Signature: CPropertyPage * __thiscall FUN_10005170(void * this, byte param_1)
// Entry: 10005170
// Body: 10005170 - 1000518d
// ============================================================================
CPropertyPage * __thiscall FUN_10005170(void *this,byte param_1)

{
  FUN_10005190(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: FUN_10005190 at 10005190
// Signature: undefined __fastcall FUN_10005190(CPropertyPage * param_1)
// Entry: 10005190
// Body: 10005190 - 10005284
// ============================================================================
void __fastcall FUN_10005190(CPropertyPage *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  puStack_8 = &LAB_10007574;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined ***)param_1 = &PTR_LAB_10008bc8;
  local_4 = 10;
  CButton::~CButton((CButton *)(param_1 + 0x31c));
  local_4._0_1_ = 9;
  CButton::~CButton((CButton *)(param_1 + 0x2dc));
  local_4._0_1_ = 8;
  CButton::~CButton((CButton *)(param_1 + 0x29c));
  local_4._0_1_ = 7;
  CListBox::~CListBox((CListBox *)(param_1 + 0x25c));
  local_4._0_1_ = 6;
  CButton::~CButton((CButton *)(param_1 + 0x21c));
  local_4._0_1_ = 5;
  CButton::~CButton((CButton *)(param_1 + 0x1dc));
  local_4._0_1_ = 4;
  CButton::~CButton((CButton *)(param_1 + 0x19c));
  local_4._0_1_ = 3;
  CStatic::~CStatic((CStatic *)(param_1 + 0x15c));
  local_4._0_1_ = 2;
  CButton::~CButton((CButton *)(param_1 + 0x11c));
  local_4._0_1_ = 1;
  CButton::~CButton((CButton *)(param_1 + 0xdc));
  local_4 = (uint)local_4._1_3_ << 8;
  CButton::~CButton((CButton *)(param_1 + 0x9c));
  local_4 = 0xffffffff;
  CPropertyPage::~CPropertyPage(param_1);
  ExceptionList = local_c;
  return;
}


// ============================================================================
// Function: FUN_10005290 at 10005290
// Signature: undefined __thiscall FUN_10005290(void * this, CDataExchange * param_1)
// Entry: 10005290
// Body: 10005290 - 1000541d
// ============================================================================
void __thiscall FUN_10005290(void *this,CDataExchange *param_1)

{
  CDataExchange *pCVar1;
  CString *pCVar2;
  void *local_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  pCVar1 = param_1;
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10007590;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  local_10 = this;
  DDX_Control(param_1,0xd4,(CWnd *)((int)this + 0x9c));
  DDX_Control(pCVar1,0xd0,(CWnd *)((int)this + 0xdc));
  DDX_Control(pCVar1,0xce,(CWnd *)((int)this + 0x11c));
  DDX_Control(pCVar1,0xc9,(CWnd *)((int)this + 0x15c));
  DDX_Control(pCVar1,0xcc,(CWnd *)((int)this + 0x19c));
  DDX_Control(pCVar1,0x10f,(CWnd *)((int)this + 0x1dc));
  DDX_Control(pCVar1,0x121,(CWnd *)((int)this + 0x21c));
  DDX_Control(pCVar1,0x11e,(CWnd *)((int)this + 0x25c));
  DDX_Control(pCVar1,0xcd,(CWnd *)((int)this + 0x29c));
  DDX_Control(pCVar1,0xd2,(CWnd *)((int)this + 0x2dc));
  DDX_Control(pCVar1,0xd3,(CWnd *)((int)this + 0x31c));
  if (*(int *)(*(int *)((int)this + 0x90) + 0x21c) != 0) {
    DDX_Check(pCVar1,0xd2,
              *(int **)(*(int *)((int)this + 0x90) + 0x3c + *(int *)((int)this + 0x94) * 4));
  }
  FUN_10005440((int)this);
  CString::CString((CString *)&param_1,s_Data_file_path__1000b25c);
  local_4 = 0;
  pCVar2 = (CString *)
           (**(code **)(**(int **)(*(int *)((int)this + 0x90) + 0x220) + 0x20))(&local_10);
  puStack_8._0_1_ = 1;
  CString::operator+=((CString *)&stack0x00000000,pCVar2);
  puStack_8 = (undefined1 *)((uint)puStack_8._1_3_ << 8);
  CString::~CString((CString *)&stack0xffffffec);
  DDX_Text(pCVar1,0xc9,(CString *)&stack0x00000000);
  puStack_8 = (undefined1 *)0xffffffff;
  CString::~CString((CString *)&stack0x00000000);
  ExceptionList = local_10;
  return;
}


// ============================================================================
// Function: FUN_10005440 at 10005440
// Signature: undefined __fastcall FUN_10005440(int param_1)
// Entry: 10005440
// Body: 10005440 - 1000562f
// ============================================================================
void __fastcall FUN_10005440(int param_1)

{
  int iVar1;
  
  if ((*(int *)(*(int *)(param_1 + 0x90) + 0x21c) == 0) ||
     (*(int *)(*(int *)(*(int *)(param_1 + 0x90) + 0x3c + *(int *)(param_1 + 0x94) * 4) + 0x58) != 0
     )) {
    iVar1 = 0;
  }
  else {
    iVar1 = 1;
  }
  CWnd::EnableWindow((CWnd *)(param_1 + 0x9c),iVar1);
  if ((*(int *)(*(int *)(param_1 + 0x90) + 0x21c) == 0) ||
     (*(int *)(*(int *)(*(int *)(param_1 + 0x90) + 0x3c + *(int *)(param_1 + 0x94) * 4) + 0x58) == 0
     )) {
    iVar1 = 0;
  }
  else {
    iVar1 = 1;
  }
  CWnd::EnableWindow((CWnd *)(param_1 + 0xdc),iVar1);
  if ((*(int *)(*(int *)(param_1 + 0x90) + 0x21c) == 0) ||
     (*(int *)(*(int *)(*(int *)(param_1 + 0x90) + 0x3c + *(int *)(param_1 + 0x94) * 4) + 0x58) != 0
     )) {
    iVar1 = 0;
  }
  else {
    iVar1 = 1;
  }
  CWnd::EnableWindow((CWnd *)(param_1 + 0x11c),iVar1);
  CWnd::EnableWindow((CWnd *)(param_1 + 0x19c),
                     (uint)(*(int *)(*(int *)(param_1 + 0x90) + 0x21c) < 0x77));
  iVar1 = *(int *)(*(int *)(param_1 + 0x90) + 0x21c);
  if (((iVar1 == 0) || (iVar1 + -1 <= *(int *)(param_1 + 0x94))) ||
     (*(int *)(*(int *)(param_1 + 0x90) + 0x230) != 0)) {
    iVar1 = 0;
  }
  else {
    iVar1 = 1;
  }
  CWnd::EnableWindow((CWnd *)(param_1 + 0x1dc),iVar1);
  if (((*(int *)(*(int *)(param_1 + 0x90) + 0x21c) == 0) || (*(int *)(param_1 + 0x94) == 0)) ||
     (*(int *)(*(int *)(param_1 + 0x90) + 0x230) != 0)) {
    iVar1 = 0;
  }
  else {
    iVar1 = 1;
  }
  CWnd::EnableWindow((CWnd *)(param_1 + 0x21c),iVar1);
  CWnd::EnableWindow((CWnd *)(param_1 + 0x25c),*(int *)(*(int *)(param_1 + 0x90) + 0x21c));
  if ((*(int *)(*(int *)(param_1 + 0x90) + 0x21c) == 0) ||
     (*(int *)(*(int *)(param_1 + 0x90) + 0x230) != 0)) {
    iVar1 = 0;
  }
  else {
    iVar1 = 1;
  }
  CWnd::EnableWindow((CWnd *)(param_1 + 0x29c),iVar1);
  if ((*(int *)(*(int *)(param_1 + 0x90) + 0x21c) == 0) ||
     (*(int *)(*(int *)(*(int *)(param_1 + 0x90) + 0x3c + *(int *)(param_1 + 0x94) * 4) + 0x58) != 0
     )) {
    iVar1 = 0;
  }
  else {
    iVar1 = 1;
  }
  CWnd::EnableWindow((CWnd *)(param_1 + 0x2dc),iVar1);
  if ((*(int *)(*(int *)(param_1 + 0x90) + 0x21c) != 0) &&
     (*(int *)(*(int *)(param_1 + 0x90) + 0x230) != 0)) {
    CWnd::EnableWindow((CWnd *)(param_1 + 0x31c),1);
    return;
  }
  CWnd::EnableWindow((CWnd *)(param_1 + 0x31c),0);
  return;
}


// ============================================================================
// Function: FUN_10005630 at 10005630
// Signature: undefined4 __thiscall FUN_10005630(void * this, int param_1)
// Entry: 10005630
// Body: 10005630 - 100058b8
// ============================================================================
undefined4 __thiscall FUN_10005630(void *this,int param_1)

{
  int *piVar1;
  HGLOBAL pvVar2;
  LPVOID lpBuffer;
  int iVar3;
  int iVar4;
  DWORD DVar5;
  BOOL BVar6;
  int iVar7;
  DWORD DVar8;
  int iVar9;
  DWORD nNumberOfBytesToWrite;
  int iStack_10;
  DWORD DStack_8;
  int iStack_4;
  
  if ((param_1 < *(int *)(*(int *)((int)this + 0x90) + 0x21c)) &&
     (piVar1 = *(int **)(*(int *)((int)this + 0x90) + 0x220), piVar1 != (int *)0x0)) {
    iStack_4 = (**(code **)(*piVar1 + 0x38))();
    iVar4 = *(int *)(*(int *)((int)this + 0x90) + 0x3c + param_1 * 4);
    iVar9 = *(int *)(iVar4 + 4);
    iVar4 = *(int *)(iVar4 + 8);
    if (iVar9 == iVar4) {
      return 1;
    }
    pvVar2 = GlobalAlloc(2,0xffff);
    lpBuffer = GlobalLock(pvVar2);
    if (lpBuffer != (LPVOID)0x0) {
      iVar3 = *(int *)((int)this + 0x90);
      iStack_10 = 0;
      if (0 < *(int *)(iVar3 + 0x21c)) {
        do {
          iVar3 = *(int *)(iVar3 + 0x3c + iStack_10 * 4);
          iVar7 = *(int *)(iVar3 + 4);
          if (iVar7 == iVar4) {
            *(int *)(iVar3 + 4) = iVar9;
            for (DVar8 = *(int *)(iVar3 + 8) - iVar7; 0 < (int)DVar8;
                DVar8 = DVar8 - nNumberOfBytesToWrite) {
              nNumberOfBytesToWrite = 0xffff;
              if ((int)DVar8 < 0x10000) {
                nNumberOfBytesToWrite = DVar8;
              }
              iVar4 = (**(code **)(**(int **)(*(int *)((int)this + 0x90) + 0x220) + 0x30))(iVar7,0);
              if ((((iVar4 != iVar7) ||
                   (DVar5 = (**(code **)(**(int **)(*(int *)((int)this + 0x90) + 0x220) + 0x3c))
                                      (lpBuffer,nNumberOfBytesToWrite),
                   DVar5 != nNumberOfBytesToWrite)) ||
                  (iVar4 = (**(code **)(**(int **)(*(int *)((int)this + 0x90) + 0x220) + 0x30))
                                     (iVar9,0), iVar4 != iVar9)) ||
                 ((BVar6 = WriteFile(*(HANDLE *)(*(int *)(*(int *)((int)this + 0x90) + 0x220) + 4),
                                     lpBuffer,nNumberOfBytesToWrite,&DStack_8,(LPOVERLAPPED)0x0),
                  BVar6 == 0 || (DStack_8 != nNumberOfBytesToWrite)))) {
                pvVar2 = GlobalHandle(lpBuffer);
                GlobalUnlock(pvVar2);
                pvVar2 = GlobalHandle(lpBuffer);
                GlobalFree(pvVar2);
                (**(code **)(**(int **)(*(int *)((int)this + 0x90) + 0x220) + 0x54))();
                piVar1 = *(int **)(*(int *)((int)this + 0x90) + 0x220);
                if (piVar1 != (int *)0x0) {
                  (**(code **)(*piVar1 + 4))(1);
                }
                *(undefined4 *)(*(int *)((int)this + 0x90) + 0x220) = 0;
                return 0;
              }
              iVar7 = iVar7 + nNumberOfBytesToWrite;
              iVar9 = iVar9 + nNumberOfBytesToWrite;
            }
            iVar3 = *(int *)(*(int *)((int)this + 0x90) + 0x3c + iStack_10 * 4);
            iStack_10 = -1;
            iVar4 = *(int *)(iVar3 + 8);
            *(int *)(iVar3 + 8) = iVar9;
          }
          iVar3 = *(int *)((int)this + 0x90);
          iStack_10 = iStack_10 + 1;
        } while (iStack_10 < *(int *)(iVar3 + 0x21c));
      }
      pvVar2 = GlobalHandle(lpBuffer);
      GlobalUnlock(pvVar2);
      pvVar2 = GlobalHandle(lpBuffer);
      GlobalFree(pvVar2);
      iVar4 = *(int *)(*(int *)((int)this + 0x90) + 0x3c + param_1 * 4);
      (**(code **)(**(int **)(*(int *)((int)this + 0x90) + 0x220) + 0x34))
                ((*(int *)(iVar4 + 4) - *(int *)(iVar4 + 8)) + iStack_4);
      *(undefined4 *)(*(int *)(*(int *)((int)this + 0x90) + 0x3c + param_1 * 4) + 4) = 0;
      *(undefined4 *)(*(int *)(*(int *)((int)this + 0x90) + 0x3c + param_1 * 4) + 8) = 0;
      return 1;
    }
  }
  return 0;
}


// ============================================================================
// Function: FUN_100058c0 at 100058c0
// Signature: undefined __fastcall FUN_100058c0(CWnd * param_1)
// Entry: 100058c0
// Body: 100058c0 - 100059ac
// ============================================================================
void __fastcall FUN_100058c0(CWnd *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  AudioHolder_StopPlaybackForEntry(*(void **)(param_1 + 0x90),*(int *)(param_1 + 0x94));
  iVar1 = FUN_10005630(param_1,*(int *)(param_1 + 0x94));
  if (iVar1 != 0) {
    operator_delete(*(void **)(*(int *)(param_1 + 0x90) + 0x3c + *(int *)(param_1 + 0x94) * 4));
    iVar1 = *(int *)(param_1 + 0x90);
    iVar3 = *(int *)(param_1 + 0x94);
    if (iVar3 < *(int *)(iVar1 + 0x21c) + -1) {
      iVar2 = iVar3 * 4 + 0x3c;
      do {
        iVar3 = iVar3 + 1;
        *(undefined4 *)(iVar2 + iVar1) = *(undefined4 *)(iVar2 + 4 + iVar1);
        iVar1 = *(int *)(param_1 + 0x90);
        iVar2 = iVar2 + 4;
      } while (iVar3 < *(int *)(iVar1 + 0x21c) + -1);
    }
    *(int *)(*(int *)(param_1 + 0x90) + 0x21c) = *(int *)(*(int *)(param_1 + 0x90) + 0x21c) + -1;
    iVar1 = *(int *)(*(int *)(param_1 + 0x90) + 0x21c);
    if (iVar1 <= *(int *)(param_1 + 0x94)) {
      if (iVar1 == 0) {
        *(undefined4 *)(param_1 + 0x94) = 0;
      }
      else {
        *(int *)(param_1 + 0x94) = iVar1 + -1;
      }
    }
    CWnd::UpdateData(param_1,0);
    FUN_10005f80((int)param_1);
    SendMessageA(*(HWND *)(param_1 + 0x27c),0x186,*(WPARAM *)(param_1 + 0x94),0);
  }
  return;
}


// ============================================================================
// Function: FUN_100059b0 at 100059b0
// Signature: undefined __fastcall FUN_100059b0(CWnd * param_1)
// Entry: 100059b0
// Body: 100059b0 - 10005a00
// ============================================================================
void __fastcall FUN_100059b0(CWnd *param_1)

{
  LRESULT LVar1;
  
  LVar1 = SendMessageA(*(HWND *)(param_1 + 0x27c),0x188,0,0);
  if (*(int *)(param_1 + 0x94) != LVar1) {
    CWnd::UpdateData(param_1,1);
    LVar1 = SendMessageA(*(HWND *)(param_1 + 0x27c),0x188,0,0);
    *(LRESULT *)(param_1 + 0x94) = LVar1;
    CWnd::UpdateData(param_1,0);
  }
  return;
}


// ============================================================================
// Function: FUN_10005a10 at 10005a10
// Signature: undefined __fastcall FUN_10005a10(CWnd * param_1)
// Entry: 10005a10
// Body: 10005a10 - 10005a96
// ============================================================================
void __fastcall FUN_10005a10(CWnd *param_1)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  
  CWnd::UpdateData(param_1,1);
  iVar3 = *(int *)(param_1 + 0x94);
  uVar1 = *(undefined4 *)(*(int *)(param_1 + 0x90) + 0x3c + iVar3 * 4);
  if (0 < iVar3) {
    iVar2 = iVar3 * 4 + 0x3c;
    do {
      iVar3 = iVar3 + -1;
      *(undefined4 *)(iVar2 + *(int *)(param_1 + 0x90)) =
           *(undefined4 *)(iVar2 + -4 + *(int *)(param_1 + 0x90));
      iVar2 = iVar2 + -4;
    } while (iVar3 != 0);
  }
  *(undefined4 *)(*(int *)(param_1 + 0x90) + 0x3c) = uVar1;
  *(undefined4 *)(param_1 + 0x94) = 0;
  CWnd::UpdateData(param_1,0);
  CPropertyPage::SetModified((CPropertyPage *)param_1,1);
  FUN_10005f80((int)param_1);
  SendMessageA(*(HWND *)(param_1 + 0x27c),0x186,*(WPARAM *)(param_1 + 0x94),0);
  return;
}


// ============================================================================
// Function: FUN_10005aa0 at 10005aa0
// Signature: undefined __fastcall FUN_10005aa0(CWnd * param_1)
// Entry: 10005aa0
// Body: 10005aa0 - 10005b44
// ============================================================================
void __fastcall FUN_10005aa0(CWnd *param_1)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  CWnd::UpdateData(param_1,1);
  iVar2 = *(int *)(param_1 + 0x90);
  iVar3 = *(int *)(param_1 + 0x94);
  uVar1 = *(undefined4 *)(iVar2 + 0x3c + iVar3 * 4);
  if (iVar3 < *(int *)(iVar2 + 0x21c) + -1) {
    iVar4 = iVar3 * 4 + 0x3c;
    do {
      iVar3 = iVar3 + 1;
      *(undefined4 *)(iVar4 + iVar2) = *(undefined4 *)(iVar4 + 4 + iVar2);
      iVar2 = *(int *)(param_1 + 0x90);
      iVar4 = iVar4 + 4;
    } while (iVar3 < *(int *)(iVar2 + 0x21c) + -1);
  }
  *(undefined4 *)(*(int *)(param_1 + 0x90) + 0x38 + *(int *)(*(int *)(param_1 + 0x90) + 0x21c) * 4)
       = uVar1;
  *(int *)(param_1 + 0x94) = *(int *)(*(int *)(param_1 + 0x90) + 0x21c) + -1;
  CWnd::UpdateData(param_1,0);
  CPropertyPage::SetModified((CPropertyPage *)param_1,1);
  FUN_10005f80((int)param_1);
  SendMessageA(*(HWND *)(param_1 + 0x27c),0x186,*(WPARAM *)(param_1 + 0x94),0);
  return;
}


// ============================================================================
// Function: FUN_10005b50 at 10005b50
// Signature: undefined __fastcall FUN_10005b50(CWnd * param_1)
// Entry: 10005b50
// Body: 10005b50 - 10005ec6
// ============================================================================
void __fastcall FUN_10005b50(CWnd *param_1)

{
  char cVar1;
  int iVar2;
  HGLOBAL pvVar3;
  LPVOID pMem;
  undefined4 *puVar4;
  void *pvVar5;
  undefined4 uVar6;
  uint uVar7;
  uint uVar8;
  byte unaff_BP;
  char *pcVar9;
  undefined4 *puVar10;
  undefined1 *puVar11;
  CString aCStack_22c [8];
  undefined4 local_224;
  CFile aCStack_21c [4];
  CFile aCStack_218 [4];
  CFileDialog aCStack_214 [4];
  CFileDialog aCStack_210 [4];
  CFileDialog aCStack_20c [4];
  CFileDialog local_208 [176];
  CString local_158 [324];
  undefined4 uStack_14;
  undefined1 uStack_10;
  void *pvStack_c;
  int iStack_8;
  uint local_4;
  
  local_4 = 0xffffffff;
  iStack_8._0_1_ = 10;
  iStack_8._1_3_ = 0x100076;
  pvStack_c = ExceptionList;
  local_224 = 0;
  ExceptionList = &pvStack_c;
  CFileDialog::CFileDialog
            (local_208,1,&DAT_1000b290,(char *)0x0,0x1004,
             s_Wav_Files____wav____wav_All_File_1000b294,(CWnd *)0x0);
  local_4 = 0;
  iVar2 = CFileDialog::DoModal(local_208);
  if (iVar2 == 1) {
    pvVar3 = GlobalAlloc(2,0xffff);
    pMem = GlobalLock(pvVar3);
    if (pMem == (LPVOID)0x0) {
      AfxMessageBox(s_End_of_memory_1000b280,0,0);
      local_4 = 1;
      goto LAB_10005e8e;
    }
    CFile::CFile(aCStack_218);
    local_4 = CONCAT31(local_4._1_3_,2);
    puVar4 = (undefined4 *)CFileDialog::GetPathName(local_208);
    iStack_8._0_1_ = 3;
    iVar2 = CFile::Open(aCStack_21c,(char *)*puVar4,0,(CFileException *)0x0);
    iStack_8._0_1_ = 2;
    CString::~CString(aCStack_22c);
    if (iVar2 == 0) {
      AfxMessageBox(s_Can_t_open_file_1000b270,0,0);
      iStack_8 = (uint)iStack_8._1_3_ << 8;
      CFile::~CFile(aCStack_21c);
      iStack_8._0_1_ = 4;
      iStack_8._1_3_ = 0;
      goto LAB_10005e8e;
    }
    pvVar5 = operator_new(0x5c);
    *(void **)(*(int *)(param_1 + 0x90) + 0x3c + *(int *)(*(int *)(param_1 + 0x90) + 0x21c) * 4) =
         pvVar5;
    puVar4 = *(undefined4 **)
              (*(int *)(param_1 + 0x90) + 0x3c + *(int *)(*(int *)(param_1 + 0x90) + 0x21c) * 4);
    for (iVar2 = 0x17; iVar2 != 0; iVar2 = iVar2 + -1) {
      *puVar4 = 0;
      puVar4 = puVar4 + 1;
    }
    puVar4 = (undefined4 *)CFileDialog::GetFileTitle(aCStack_20c);
    uVar7 = 0xffffffff;
    pvStack_c = (void *)CONCAT31(pvStack_c._1_3_,5);
    pcVar9 = (char *)*puVar4;
    do {
      if (uVar7 == 0) break;
      uVar7 = uVar7 - 1;
      cVar1 = *pcVar9;
      pcVar9 = pcVar9 + 1;
    } while (cVar1 != '\0');
    if (~uVar7 - 1 < 9) {
      puVar4 = (undefined4 *)CFileDialog::GetFileTitle(aCStack_210);
      uVar7 = 0xffffffff;
      uStack_10 = 6;
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
    puVar11 = &stack0xfffffdcc;
    puVar4 = (undefined4 *)CFileDialog::GetFileTitle(aCStack_214);
    puVar4 = (undefined4 *)*puVar4;
    puVar10 = (undefined4 *)
              (*(int *)(*(int *)(param_1 + 0x90) + 0x3c +
                       *(int *)(*(int *)(param_1 + 0x90) + 0x21c) * 4) + 0xc);
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
    uStack_14 = 5;
    if ((unaff_BP & 1) != 0) {
      CString::~CString((CString *)&stack0xfffffdd0);
    }
    uStack_14 = CONCAT31(uStack_14._1_3_,2);
    CString::~CString(aCStack_22c);
    (**(code **)(**(int **)(*(int *)(param_1 + 0x90) + 0x220) + 0x50))(puVar11);
    iVar2 = *(int *)(param_1 + 0x90);
    uVar6 = (**(code **)(**(int **)(iVar2 + 0x220) + 0x38))();
    *(undefined4 *)(*(int *)(iVar2 + 0x3c + *(int *)(iVar2 + 0x21c) * 4) + 4) = uVar6;
    (**(code **)(**(int **)(*(int *)(param_1 + 0x90) + 0x220) + 0x30))(0,2);
    uVar7 = CFile::Read((CFile *)&stack0xfffffdd0,pMem,0xffff);
    while (uVar7 != 0) {
      (**(code **)(**(int **)(*(int *)(param_1 + 0x90) + 0x220) + 0x40))(pMem,uVar7);
      uVar7 = CFile::Read((CFile *)&stack0xfffffdd0,pMem,0xffff);
    }
    pvVar3 = GlobalHandle(pMem);
    GlobalUnlock(pvVar3);
    pvVar3 = GlobalHandle(pMem);
    GlobalFree(pvVar3);
    CFile::Close((CFile *)&stack0xfffffdd0);
    (**(code **)(**(int **)(*(int *)(param_1 + 0x90) + 0x220) + 0x50))();
    iVar2 = *(int *)(param_1 + 0x90);
    uVar6 = (**(code **)(**(int **)(iVar2 + 0x220) + 0x38))();
    *(undefined4 *)(*(int *)(iVar2 + 0x3c + *(int *)(iVar2 + 0x21c) * 4) + 8) = uVar6;
    iVar2 = *(int *)(param_1 + 0x90);
    *(undefined4 *)(param_1 + 0x94) = *(undefined4 *)(iVar2 + 0x21c);
    *(int *)(iVar2 + 0x21c) = *(int *)(iVar2 + 0x21c) + 1;
    CWnd::UpdateData(param_1,0);
    FUN_10005f80((int)param_1);
    SendMessageA(*(HWND *)(param_1 + 0x27c),0x186,*(WPARAM *)(param_1 + 0x94),0);
    local_4 = local_4 & 0xffffff00;
    CFile::~CFile(aCStack_218);
  }
  local_4 = 7;
LAB_10005e8e:
  CString::~CString(local_158);
  local_4 = 0xffffffff;
  CDialog::~CDialog((CDialog *)local_208);
  ExceptionList = pvStack_c;
  return;
}


// ============================================================================
// Function: ~CDialog at 10005ed0
// Signature: void __thiscall ~CDialog(CDialog * this)
// Entry: 10005ed0
// Body: 10005ed0 - 10005ed4
// ============================================================================
void __thiscall CDialog::~CDialog(CDialog *this)

{
  ~CDialog(this);
  return;
}


// ============================================================================
// Function: FUN_10005ee0 at 10005ee0
// Signature: undefined __fastcall FUN_10005ee0(CDialog * param_1)
// Entry: 10005ee0
// Body: 10005ee0 - 10005f2e
// ============================================================================
void __fastcall FUN_10005ee0(CDialog *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_10007628;
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
// Function: FUN_10005f30 at 10005f30
// Signature: undefined __fastcall FUN_10005f30(CWnd * param_1)
// Entry: 10005f30
// Body: 10005f30 - 10005f54
// ============================================================================
void __fastcall FUN_10005f30(CWnd *param_1)

{
  CWnd::UpdateData(param_1,1);
  AudioHolder_StartPlaybackForEntry(*(void **)(param_1 + 0x90),*(int *)(param_1 + 0x94));
  FUN_10005440((int)param_1);
  return;
}


// ============================================================================
// Function: FUN_10005f60 at 10005f60
// Signature: undefined __fastcall FUN_10005f60(int param_1)
// Entry: 10005f60
// Body: 10005f60 - 10005f7d
// ============================================================================
void __fastcall FUN_10005f60(int param_1)

{
  AudioHolder_StopPlaybackForEntry(*(void **)(param_1 + 0x90),*(int *)(param_1 + 0x94));
  FUN_10005440(param_1);
  return;
}


// ============================================================================
// Function: FUN_10005f80 at 10005f80
// Signature: undefined __fastcall FUN_10005f80(int param_1)
// Entry: 10005f80
// Body: 10005f80 - 10006038
// ============================================================================
void __fastcall FUN_10005f80(int param_1)

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
  local_8 = &LAB_10007648;
  local_c = ExceptionList;
  iVar3 = 0;
  ExceptionList = &local_c;
  local_10.unused = param_1;
  SendMessageA(*(HWND *)(param_1 + 0x27c),0x184,0,0);
  CString::CString((CString *)&local_10);
  iVar1 = *(int *)(param_1 + 0x90);
  local_4 = 0;
  if (0 < *(int *)(iVar1 + 0x21c)) {
    iVar2 = 0x3c;
    do {
      hWnd = &local_10;
      CString::Format((CString *)(*(int *)(iVar2 + iVar1) + 0xc),(char *)hWnd);
      SendMessageA(hWnd,*(UINT *)(param_1 + 0x27c),0x180,0);
      iVar1 = *(int *)(param_1 + 0x90);
      iVar3 = iVar3 + 1;
      iVar2 = iVar2 + 4;
    } while (iVar3 < *(int *)(iVar1 + 0x21c));
  }
  local_4 = 0xffffffff;
  CString::~CString((CString *)&local_10);
  ExceptionList = local_c;
  return;
}


// ============================================================================
// Function: FUN_10006040 at 10006040
// Signature: undefined4 __fastcall FUN_10006040(CDialog * param_1)
// Entry: 10006040
// Body: 10006040 - 10006087
// ============================================================================
undefined4 __fastcall FUN_10006040(CDialog *param_1)

{
  CDialog::OnInitDialog(param_1);
  FUN_10005440((int)param_1);
  FUN_10005f80((int)param_1);
  SendMessageA(*(HWND *)(param_1 + 0x27c),0x186,*(WPARAM *)(param_1 + 0x94),0);
  SetTimer(*(HWND *)(param_1 + 0x20),1,100,(TIMERPROC)0x0);
  return 1;
}


// ============================================================================
// Function: FUN_10006090 at 10006090
// Signature: undefined __fastcall FUN_10006090(int param_1)
// Entry: 10006090
// Body: 10006090 - 100060c4
// ============================================================================
void __fastcall FUN_10006090(int param_1)

{
  void *this;
  int iVar1;
  
  iVar1 = 0;
  this = *(void **)(param_1 + 0x90);
  if (0 < *(int *)((int)this + 0x21c)) {
    do {
      AudioHolder_StopPlaybackForEntry(this,iVar1);
      this = *(void **)(param_1 + 0x90);
      iVar1 = iVar1 + 1;
    } while (iVar1 < *(int *)((int)this + 0x21c));
  }
  FUN_10005440(param_1);
  return;
}


// ============================================================================
// Function: FUN_100060d0 at 100060d0
// Signature: undefined __fastcall FUN_100060d0(CWnd * param_1)
// Entry: 100060d0
// Body: 100060d0 - 100060e2
// ============================================================================
void __fastcall FUN_100060d0(CWnd *param_1)

{
  FUN_10005440((int)param_1);
  CWnd::Default(param_1);
  return;
}


// ============================================================================
// Function: FUN_10006100 at 10006100
// Signature: undefined __fastcall FUN_10006100(CWnd * param_1)
// Entry: 10006100
// Body: 10006100 - 1000647a
// ============================================================================
void __fastcall FUN_10006100(CWnd *param_1)

{
  char cVar1;
  int iVar2;
  HGLOBAL pvVar3;
  LPVOID pMem;
  undefined4 *puVar4;
  undefined4 uVar5;
  uint uVar6;
  uint uVar7;
  byte unaff_BP;
  char *pcVar8;
  undefined4 *puVar9;
  undefined1 *puVar10;
  CString aCStack_22c [8];
  undefined4 local_224;
  CFile aCStack_21c [4];
  CFile aCStack_218 [4];
  CFileDialog aCStack_214 [4];
  CFileDialog aCStack_210 [4];
  CFileDialog aCStack_20c [4];
  CFileDialog local_208 [176];
  CString local_158 [324];
  undefined4 uStack_14;
  undefined1 uStack_10;
  void *local_c;
  int iStack_8;
  uint local_4;
  
  local_4 = 0xffffffff;
  iStack_8._0_1_ = 0xe0;
  iStack_8._1_3_ = 0x100076;
  local_c = ExceptionList;
  local_224 = 0;
  ExceptionList = &local_c;
  CFileDialog::CFileDialog
            (local_208,1,&DAT_1000b290,(char *)0x0,0x1004,
             s_Video_Files____wav____wav_All_Fi_1000b2cc,(CWnd *)0x0);
  local_4 = 0;
  iVar2 = CFileDialog::DoModal(local_208);
  if (iVar2 == 1) {
    iVar2 = FUN_10005630(param_1,*(int *)(param_1 + 0x94));
    if (iVar2 == 0) {
      local_4 = 2;
    }
    else {
      pvVar3 = GlobalAlloc(2,0xffff);
      pMem = GlobalLock(pvVar3);
      if (pMem == (LPVOID)0x0) {
        AfxMessageBox(s_End_of_memory_1000b280,0,0);
        local_4 = 3;
      }
      else {
        CFile::CFile(aCStack_218);
        local_4 = CONCAT31(local_4._1_3_,4);
        puVar4 = (undefined4 *)CFileDialog::GetPathName(local_208);
        iStack_8._0_1_ = 5;
        iVar2 = CFile::Open(aCStack_21c,(char *)*puVar4,0,(CFileException *)0x0);
        iStack_8._0_1_ = 4;
        CString::~CString(aCStack_22c);
        if (iVar2 == 0) {
          AfxMessageBox(s_Can_t_open_file_1000b270,0,0);
          iStack_8 = (uint)iStack_8._1_3_ << 8;
          CFile::~CFile(aCStack_21c);
          iStack_8._0_1_ = 6;
          iStack_8._1_3_ = 0;
        }
        else {
          iVar2 = *(int *)(*(int *)(param_1 + 0x90) + 0x3c + *(int *)(param_1 + 0x94) * 4);
          *(undefined4 *)(iVar2 + 0xc) = 0;
          *(undefined4 *)(iVar2 + 0x10) = 0;
          *(undefined2 *)(iVar2 + 0x14) = 0;
          puVar4 = (undefined4 *)CFileDialog::GetFileTitle(aCStack_20c);
          uVar6 = 0xffffffff;
          local_c = (void *)CONCAT31(local_c._1_3_,7);
          pcVar8 = (char *)*puVar4;
          do {
            if (uVar6 == 0) break;
            uVar6 = uVar6 - 1;
            cVar1 = *pcVar8;
            pcVar8 = pcVar8 + 1;
          } while (cVar1 != '\0');
          if (~uVar6 - 1 < 9) {
            puVar4 = (undefined4 *)CFileDialog::GetFileTitle(aCStack_210);
            uVar6 = 0xffffffff;
            uStack_10 = 8;
            pcVar8 = (char *)*puVar4;
            do {
              if (uVar6 == 0) break;
              uVar6 = uVar6 - 1;
              cVar1 = *pcVar8;
              pcVar8 = pcVar8 + 1;
            } while (cVar1 != '\0');
            uVar6 = ~uVar6 - 1;
          }
          else {
            uVar6 = 9;
          }
          puVar10 = &stack0xfffffdcc;
          puVar4 = (undefined4 *)CFileDialog::GetFileTitle(aCStack_214);
          puVar4 = (undefined4 *)*puVar4;
          puVar9 = (undefined4 *)
                   (*(int *)(*(int *)(param_1 + 0x90) + 0x3c + *(int *)(param_1 + 0x94) * 4) + 0xc);
          for (uVar7 = uVar6 >> 2; uVar7 != 0; uVar7 = uVar7 - 1) {
            *puVar9 = *puVar4;
            puVar4 = puVar4 + 1;
            puVar9 = puVar9 + 1;
          }
          for (uVar6 = uVar6 & 3; uVar6 != 0; uVar6 = uVar6 - 1) {
            *(undefined1 *)puVar9 = *(undefined1 *)puVar4;
            puVar4 = (undefined4 *)((int)puVar4 + 1);
            puVar9 = (undefined4 *)((int)puVar9 + 1);
          }
          CString::~CString((CString *)&stack0xfffffdc8);
          uStack_14 = 7;
          if ((unaff_BP & 1) != 0) {
            CString::~CString((CString *)&stack0xfffffdd0);
          }
          uStack_14 = CONCAT31(uStack_14._1_3_,4);
          CString::~CString(aCStack_22c);
          (**(code **)(**(int **)(*(int *)(param_1 + 0x90) + 0x220) + 0x50))(puVar10);
          uVar5 = (**(code **)(**(int **)(*(int *)(param_1 + 0x90) + 0x220) + 0x38))();
          *(undefined4 *)
           (*(int *)(*(int *)(param_1 + 0x90) + 0x3c + *(int *)(param_1 + 0x94) * 4) + 4) = uVar5;
          (**(code **)(**(int **)(*(int *)(param_1 + 0x90) + 0x220) + 0x30))(0,2);
          uVar6 = CFile::Read((CFile *)&stack0xfffffdd0,pMem,0xffff);
          while (uVar6 != 0) {
            (**(code **)(**(int **)(*(int *)(param_1 + 0x90) + 0x220) + 0x40))(pMem,uVar6);
            uVar6 = CFile::Read((CFile *)&stack0xfffffdd0,pMem,0xffff);
          }
          pvVar3 = GlobalHandle(pMem);
          GlobalUnlock(pvVar3);
          pvVar3 = GlobalHandle(pMem);
          GlobalFree(pvVar3);
          CFile::Close((CFile *)&stack0xfffffdd0);
          (**(code **)(**(int **)(*(int *)(param_1 + 0x90) + 0x220) + 0x50))();
          uVar5 = (**(code **)(**(int **)(*(int *)(param_1 + 0x90) + 0x220) + 0x38))();
          *(undefined4 *)
           (*(int *)(*(int *)(param_1 + 0x90) + 0x3c + *(int *)(param_1 + 0x94) * 4) + 8) = uVar5;
          CWnd::UpdateData(param_1,0);
          FUN_10005f80((int)param_1);
          SendMessageA(*(HWND *)(param_1 + 0x27c),0x186,*(WPARAM *)(param_1 + 0x94),0);
          local_4 = local_4 & 0xffffff00;
          CFile::~CFile(aCStack_218);
          local_4 = 9;
        }
      }
    }
  }
  else {
    local_4 = 1;
  }
  CString::~CString(local_158);
  local_4 = 0xffffffff;
  CDialog::~CDialog((CDialog *)local_208);
  ExceptionList = local_c;
  return;
}


// ============================================================================
// Function: FUN_10006480 at 10006480
// Signature: undefined __stdcall FUN_10006480(void)
// Entry: 10006480
// Body: 10006480 - 10006482
// ============================================================================
void FUN_10006480(void)

{
  return;
}


// ============================================================================
// Function: FUN_100064b0 at 100064b0
// Signature: CStatic * __thiscall FUN_100064b0(void * this, byte param_1)
// Entry: 100064b0
// Body: 100064b0 - 100064cd
// ============================================================================
CStatic * __thiscall FUN_100064b0(void *this,byte param_1)

{
  CStatic::~CStatic(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: FUN_100064d0 at 100064d0
// Signature: CButton * __thiscall FUN_100064d0(void * this, byte param_1)
// Entry: 100064d0
// Body: 100064d0 - 100064ed
// ============================================================================
CButton * __thiscall FUN_100064d0(void *this,byte param_1)

{
  CButton::~CButton(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: FUN_100064f0 at 100064f0
// Signature: CListBox * __thiscall FUN_100064f0(void * this, byte param_1)
// Entry: 100064f0
// Body: 100064f0 - 1000650d
// ============================================================================
CListBox * __thiscall FUN_100064f0(void *this,byte param_1)

{
  CListBox::~CListBox(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: FUN_10006530 at 10006530
// Signature: CWnd * __thiscall FUN_10006530(void * this, undefined4 param_1)
// Entry: 10006530
// Body: 10006530 - 100065cf
// ============================================================================
CWnd * __thiscall FUN_10006530(void *this,undefined4 param_1)

{
  char *pcVar1;
  undefined4 *puVar2;
  char *pcVar3;
  ulong uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  HWND__ *pHVar9;
  HMENU__ *pHVar10;
  void *pvVar11;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_100076f8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  CWnd::CWnd(this);
  pvVar11 = (void *)0x0;
  pHVar10 = (HMENU__ *)0x0;
  pHVar9 = (HWND__ *)0x0;
  iVar8 = 0;
  iVar7 = 0;
  iVar6 = 0;
  iVar5 = 0;
  uVar4 = 0x80000000;
  pcVar3 = s_Timer_Wnd_1000b2fc;
  local_4 = 0;
  *(undefined ***)this = &PTR_LAB_10008f58;
  pcVar1 = AfxRegisterWndClass(0,(HICON__ *)0x0,(HBRUSH__ *)0x0,(HICON__ *)0x0);
  iVar5 = CWnd::CreateEx(this,0,pcVar1,pcVar3,uVar4,iVar5,iVar6,iVar7,iVar8,pHVar9,pHVar10,pvVar11);
  if (iVar5 != 0) {
    *(undefined4 *)((int)this + 0x40) = param_1;
    puVar2 = (undefined4 *)((int)this + 0x44);
    for (iVar5 = 0x14; iVar5 != 0; iVar5 = iVar5 + -1) {
      *puVar2 = 0;
      puVar2 = puVar2 + 1;
    }
    ExceptionList = local_c;
    return this;
  }
  ExceptionList = local_c;
  return this;
}


// ============================================================================
// Function: FUN_100065d0 at 100065d0
// Signature: CWnd * __thiscall FUN_100065d0(void * this, byte param_1)
// Entry: 100065d0
// Body: 100065d0 - 100065ed
// ============================================================================
CWnd * __thiscall FUN_100065d0(void *this,byte param_1)

{
  FUN_10006670(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: FUN_100065f0 at 100065f0
// Signature: UINT_PTR __thiscall FUN_100065f0(void * this, UINT param_1)
// Entry: 100065f0
// Body: 100065f0 - 1000662b
// ============================================================================
UINT_PTR __thiscall FUN_100065f0(void *this,UINT param_1)

{
  int *piVar1;
  UINT_PTR UVar2;
  uint uVar3;
  
  uVar3 = 0;
  piVar1 = (int *)((int)this + 0x44);
  do {
    if (*piVar1 == 0) {
      UVar2 = SetTimer(*(HWND *)((int)this + 0x20),uVar3 + 1,param_1,(TIMERPROC)0x0);
      *(UINT_PTR *)((int)this + uVar3 * 4 + 0x44) = UVar2;
      return UVar2;
    }
    uVar3 = uVar3 + 1;
    piVar1 = piVar1 + 1;
  } while (uVar3 < 0x14);
  return 0;
}


// ============================================================================
// Function: FUN_10006630 at 10006630
// Signature: undefined __thiscall FUN_10006630(void * this, UINT_PTR param_1)
// Entry: 10006630
// Body: 10006630 - 1000666b
// ============================================================================
void __thiscall FUN_10006630(void *this,UINT_PTR param_1)

{
  int iVar1;
  UINT_PTR *pUVar2;
  
  if (param_1 != 0) {
    pUVar2 = (UINT_PTR *)((int)this + 0x44);
    iVar1 = 0x14;
    do {
      if (*pUVar2 == param_1) {
        KillTimer(*(HWND *)((int)this + 0x20),param_1);
        *pUVar2 = 0;
        param_1 = 0;
      }
      pUVar2 = pUVar2 + 1;
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
  }
  return;
}


// ============================================================================
// Function: FUN_10006670 at 10006670
// Signature: undefined __fastcall FUN_10006670(CWnd * param_1)
// Entry: 10006670
// Body: 10006670 - 100066c5
// ============================================================================
void __fastcall FUN_10006670(CWnd *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_10007718;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined ***)param_1 = &PTR_LAB_10008f58;
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
// Function: FUN_10006710 at 10006710
// Signature: CPropertySheet * __thiscall FUN_10006710(void * this, uint param_1, undefined4 param_2, uint param_3)
// Entry: 10006710
// Body: 10006710 - 10006787
// ============================================================================
CPropertySheet * __thiscall FUN_10006710(void *this,uint param_1,undefined4 param_2,uint param_3)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10007746;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  CPropertySheet::CPropertySheet(this,param_1,(CWnd *)0x0,param_3);
  local_4 = 0;
  AudioSettingsPage_Construct((CPropertyPage *)((int)this + 0x8c));
  local_4 = CONCAT31(local_4._1_3_,1);
  *(undefined ***)this = &PTR_LAB_10009050;
  *(undefined4 *)((int)this + 0x11c) = param_2;
  CPropertySheet::AddPage(this,(CPropertyPage *)((int)this + 0x8c));
  ExceptionList = local_c;
  return this;
}


// ============================================================================
// Function: FUN_10006790 at 10006790
// Signature: CPropertySheet * __thiscall FUN_10006790(void * this, byte param_1)
// Entry: 10006790
// Body: 10006790 - 100067ad
// ============================================================================
CPropertySheet * __thiscall FUN_10006790(void *this,byte param_1)

{
  FUN_100067b0(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: FUN_100067b0 at 100067b0
// Signature: undefined __fastcall FUN_100067b0(CPropertySheet * param_1)
// Entry: 100067b0
// Body: 100067b0 - 10006804
// ============================================================================
void __fastcall FUN_100067b0(CPropertySheet *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_10007758;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined ***)param_1 = &PTR_LAB_10009050;
  local_4 = 0;
  FUN_10005190((CPropertyPage *)(param_1 + 0x8c));
  local_4 = 0xffffffff;
  CPropertySheet::~CPropertySheet(param_1);
  ExceptionList = local_c;
  return;
}


// ============================================================================
// Function: ~CWinApp at 10006830
// Signature: void __thiscall ~CWinApp(CWinApp * this)
// Entry: 10006830
// Body: 10006830 - 10006835
// ============================================================================
void __thiscall CWinApp::~CWinApp(CWinApp *this)

{
                    /* WARNING: Could not recover jumptable at 0x10006830. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  ~CWinApp(this);
  return;
}


// ============================================================================
// Function: CWinApp at 10006908
// Signature: undefined __thiscall CWinApp(CWinApp * this, char * param_1)
// Entry: 10006908
// Body: 10006908 - 1000690d
// ============================================================================
void __thiscall CWinApp::CWinApp(CWinApp *this,char *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x10006908. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  CWinApp(this,param_1);
  return;
}


// ============================================================================
// Function: operator_delete at 1000690e
// Signature: void __cdecl operator_delete(void * param_1)
// Entry: 1000690e
// Body: 1000690e - 10006913
// ============================================================================
void __cdecl operator_delete(void *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x1000690e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  operator_delete(param_1);
  return;
}


// ============================================================================
// Function: InitInstance at 10006914
// Signature: int __thiscall InitInstance(CWinApp * this)
// Entry: 10006914
// Body: 10006914 - 10006919
// ============================================================================
int __thiscall CWinApp::InitInstance(CWinApp *this)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10006914. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = InitInstance(this);
  return iVar1;
}


// ============================================================================
// Function: ExitInstance at 1000691a
// Signature: int __thiscall ExitInstance(CWinApp * this)
// Entry: 1000691a
// Body: 1000691a - 1000691f
// ============================================================================
int __thiscall CWinApp::ExitInstance(CWinApp *this)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x1000691a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = ExitInstance(this);
  return iVar1;
}


// ============================================================================
// Function: AfxDllCanUnloadNow at 10006920
// Signature: long __stdcall AfxDllCanUnloadNow(void)
// Entry: 10006920
// Body: 10006920 - 10006925
// ============================================================================
long AfxDllCanUnloadNow(void)

{
  long lVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10006920. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  lVar1 = AfxDllCanUnloadNow();
  return lVar1;
}


// ============================================================================
// Function: AFX_MAINTAIN_STATE2 at 10006926
// Signature: undefined __thiscall AFX_MAINTAIN_STATE2(AFX_MAINTAIN_STATE2 * this, AFX_MODULE_STATE * param_1)
// Entry: 10006926
// Body: 10006926 - 1000692b
// ============================================================================
void __thiscall
AFX_MAINTAIN_STATE2::AFX_MAINTAIN_STATE2(AFX_MAINTAIN_STATE2 *this,AFX_MODULE_STATE *param_1)

{
                    /* WARNING: Could not recover jumptable at 0x10006926. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  AFX_MAINTAIN_STATE2(this,param_1);
  return;
}

// clang-format on
