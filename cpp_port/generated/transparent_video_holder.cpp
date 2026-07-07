/*
  Direct C++ port seed generated from Ghidra MCP localhost.
  Source program: /GraphBoardComponents/TransparentVideoHolder.dll
  Notes: raw Ghidra decompiler output with only file-level framing added.
*/
#include "../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: TVH_ComputeZRange at 100073b0
// Signature: undefined4 __stdcall TVH_ComputeZRange(int param_1, int * param_2, int * param_3)
// Entry: 100073b0
// Body: 100073b0 - 1000742d
// ============================================================================
undefined4 TVH_ComputeZRange(int param_1,int *param_2,int *param_3)

{
  int *piVar1;
  int iVar2;
  
  *param_2 = 100000;
  iVar2 = 0;
  *param_3 = 0;
  if (0 < *(int *)(param_1 + 0x364)) {
    piVar1 = (int *)(param_1 + 0x1d4);
    do {
      if (*(int *)(*piVar1 + 0x554) < *param_2) {
        *param_2 = *(int *)(*piVar1 + 0x554);
      }
      if (*param_3 < *(int *)(*piVar1 + 0x554)) {
        *param_3 = *(int *)(*piVar1 + 0x554);
      }
      iVar2 = iVar2 + 1;
      piVar1 = piVar1 + 1;
    } while (iVar2 < *(int *)(param_1 + 0x364));
  }
  if ((*param_2 == 100000) && (*param_3 == 0)) {
    *param_3 = -1;
    *param_2 = -1;
  }
  return 0;
}


// ============================================================================
// Function: TVH_ResetVideo at 10007880
// Signature: undefined4 __stdcall TVH_ResetVideo(int param_1, undefined4 param_2)
// Entry: 10007880
// Body: 10007880 - 10007951
// ============================================================================
undefined4 TVH_ResetVideo(int param_1,undefined4 param_2)

{
  bool bVar1;
  undefined4 uVar2;
  undefined3 extraout_var;
  int iVar3;
  int *piVar4;
  
  piVar4 = *(int **)(param_1 + 0x40);
  if (piVar4 != (int *)0x0) {
    uVar2 = (**(code **)(*piVar4 + 0x28))(piVar4);
    *(undefined4 *)(param_1 + 0xe0) = uVar2;
  }
  iVar3 = 0;
  *(undefined4 *)(param_1 + 0x48) = param_2;
  if (0 < *(int *)(param_1 + 0x364)) {
    piVar4 = (int *)(param_1 + 0x1d4);
    do {
      *(undefined4 *)(*piVar4 + 0x554) = *(undefined4 *)(*piVar4 + 0x510);
      *(undefined4 *)(*piVar4 + 0x4e8) = *(undefined4 *)(*piVar4 + 0x500);
      *(undefined4 *)(*piVar4 + 0x4ec) = *(undefined4 *)(*piVar4 + 0x504);
      *(undefined4 *)(*piVar4 + 0x54c) = *(undefined4 *)(*piVar4 + 0x508);
      *(undefined4 *)(*piVar4 + 0x550) = *(undefined4 *)(*piVar4 + 0x50c);
      *(undefined4 *)(*piVar4 + 0x4f0) = 0;
      if (*(void **)(*piVar4 + 0x564) != (void *)0x0) {
        operator_delete(*(void **)(*piVar4 + 0x564));
        *(undefined4 *)(*piVar4 + 0x564) = 0;
      }
      iVar3 = iVar3 + 1;
      piVar4 = piVar4 + 1;
    } while (iVar3 < *(int *)(param_1 + 0x364));
  }
  if (*(int *)(param_1 + 0x48) == 0) {
    bVar1 = TVH_IsEntryPlaying(param_1 + 0x54);
    if (CONCAT31(extraout_var,bVar1) != 0) {
      TVH_StopEntryPlayback(param_1 + 0x54);
    }
  }
  return 0;
}


// ============================================================================
// Function: TVH_WriteArchiveChunked at 10007990
// Signature: undefined4 __stdcall TVH_WriteArchiveChunked(CArchive * param_1, int * param_2, uint param_3)
// Entry: 10007990
// Body: 10007990 - 10007a32
// ============================================================================
undefined4 TVH_WriteArchiveChunked(CArchive *param_1,int *param_2,uint param_3)

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
    if (param_3 == 0) goto LAB_10007a04;
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
LAB_10007a04:
  CArchive::Flush(param_1);
  pvVar1 = GlobalHandle(pMem);
  GlobalUnlock(pvVar1);
  pvVar1 = GlobalHandle(pMem);
  GlobalFree(pvVar1);
  return 1;
}


// ============================================================================
// Function: TVH_ReadArchiveChunked at 10007a40
// Signature: undefined4 __stdcall TVH_ReadArchiveChunked(int * param_1, CArchive * param_2, uint param_3)
// Entry: 10007a40
// Body: 10007a40 - 10007ae1
// ============================================================================
undefined4 TVH_ReadArchiveChunked(int *param_1,CArchive *param_2,uint param_3)

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
    if (param_3 == 0) goto LAB_10007ab4;
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
LAB_10007ab4:
  (**(code **)(*param_1 + 0x50))();
  pvVar1 = GlobalHandle(pMem);
  GlobalUnlock(pvVar1);
  pvVar1 = GlobalHandle(pMem);
  GlobalFree(pvVar1);
  return 1;
}


// ============================================================================
// Function: TVH_SerializeOrDeserialize at 10007af0
// Signature: undefined4 __stdcall TVH_SerializeOrDeserialize(int param_1, CArchive * param_2)
// Entry: 10007af0
// Body: 10007af0 - 10008113
// ============================================================================
/* Recovered private-block layout (see graphboard_file_formats.md):
   u32 version; u32 entryCount; repeat entryCount:
     u32 boardVideoByteSize; u8 boardVideoBytes[boardVideoByteSize]  (TVH_BoardVideoHeader + chunk
   records)
     u8 holderEntry[0x568]  (TVH_HolderEntry)
     u8 stillFrame[((width*8+31)>>3 & ~3) * height]
   Entries live in an array at param_1+0x1d4, count at param_1+0x364. */

undefined4 TVH_SerializeOrDeserialize(int param_1,CArchive *param_2)

{
  uint uVar1;
  undefined4 uVar2;
  int iVar3;
  CString *pCVar4;
  DWORD DVar5;
  UINT UVar6;
  void *pvVar7;
  undefined4 unaff_EBX;
  int iVar8;
  int *piVar9;
  char *pcVar10;
  char *local_228;
  char *local_224;
  int iStack_220;
  CFile *local_21c;
  undefined4 uStack_218;
  CHAR local_214 [260];
  CHAR aCStack_110 [260];
  void *pvStack_c;
  undefined1 uStack_8;
  undefined3 uStack_7;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  uStack_8 = 0xd6;
  uStack_7 = 0x1000f5;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  uVar1 = CArchive::GetObjectSchema(param_2);
  *(uint *)(param_1 + 0x2c) = uVar1;
  *(uint *)(param_2 + 0xc) = uVar1;
  if ((~*(uint *)(param_2 + 0x14) & 1) == 0) {
    piVar9 = *(int **)(param_1 + 0x4c);
    iVar8 = 0;
    if (*(int *)(param_1 + 0x2c) == 0) {
      if (piVar9 == (int *)0x0) {
        local_21c = operator_new(0x10);
        local_4 = 5;
        if (local_21c == (CFile *)0x0) {
          uVar2 = 0;
        }
        else {
          uVar2 = CFile::CFile(local_21c);
        }
        local_4 = 0xffffffff;
        *(undefined4 *)(param_1 + 0x4c) = uVar2;
      }
      else {
        (**(code **)(*piVar9 + 0x20))(&local_224);
        uStack_8 = 4;
        uStack_7 = 0;
        (**(code **)(**(int **)(param_1 + 0x4c) + 0x54))();
        CFile::Remove(local_224);
        local_4 = 0xffffffff;
        CString::~CString((CString *)&local_224);
      }
    }
    else {
      if (piVar9 == (int *)0x0) {
        local_21c = operator_new(0x10);
        local_4 = 0;
        if (local_21c == (CFile *)0x0) {
          uVar2 = 0;
        }
        else {
          uVar2 = CFile::CFile(local_21c);
        }
        local_4 = 0xffffffff;
        *(undefined4 *)(param_1 + 0x4c) = uVar2;
      }
      else {
        (**(code **)(*piVar9 + 0x54))();
      }
      CString::CString((CString *)&local_228);
      local_4 = 1;
      pCVar4 = (CString *)(**(code **)(**(int **)(param_2 + 0x20) + 0x20))(&local_224);
      uStack_8 = 2;
      CString::operator=((CString *)&stack0xfffffdd4,pCVar4);
      uStack_8 = 1;
      CString::~CString((CString *)&local_228);
      iVar3 = (**(code **)(**(int **)(param_1 + 0x4c) + 0x28))(unaff_EBX,0x8020,0);
      if (iVar3 != 0) {
        local_4 = 0xffffffff;
        CString::~CString((CString *)&local_228);
        goto LAB_10007ea5;
      }
      (**(code **)(**(int **)(param_2 + 0x20) + 0x20))(&iStack_220);
      uStack_8 = 3;
      CString::Format((CString *)&stack0xfffffdd4,(char *)&stack0xfffffdd4);
      local_4 = CONCAT31(local_4._1_3_,1);
      CString::~CString((CString *)&iStack_220);
      AfxMessageBox(local_228,0,0);
      *(undefined4 *)(param_1 + 0x2c) = 0;
      local_4 = 0xffffffff;
      CString::~CString((CString *)&local_228);
    }
    DVar5 = GetTempPathA(0x104,local_214);
    if (((DVar5 != 0) &&
        (UVar6 = GetTempFileNameA(local_214,&DAT_10014578,0,aCStack_110), UVar6 != 0)) &&
       (iVar3 = (**(code **)(**(int **)(param_1 + 0x4c) + 0x28))(aCStack_110,0x8002,0), iVar3 != 0))
    {
LAB_10007ea5:
      if (0 < *(int *)(param_1 + 0x364)) {
        piVar9 = (int *)(param_1 + 0x1d4);
        do {
          if (*(void **)(*piVar9 + 0x560) != (void *)0x0) {
            operator_delete(*(void **)(*piVar9 + 0x560));
          }
          if (*(void **)(*piVar9 + 0x564) != (void *)0x0) {
            operator_delete(*(void **)(*piVar9 + 0x564));
          }
          operator_delete((void *)*piVar9);
          iVar8 = iVar8 + 1;
          piVar9 = piVar9 + 1;
        } while (iVar8 < *(int *)(param_1 + 0x364));
      }
      if (*(uint *)(param_2 + 0x28) < *(int *)(param_2 + 0x24) + 4U) {
        CArchive::FillBuffer(param_2,(*(int *)(param_2 + 0x24) - *(uint *)(param_2 + 0x28)) + 4);
      }
      piVar9 = *(int **)(param_2 + 0x24);
      iStack_220 = *piVar9;
      *(int **)(param_2 + 0x24) = piVar9 + 1;
      if (*(int **)(param_2 + 0x28) < piVar9 + 2) {
        CArchive::FillBuffer(param_2,(int)(piVar9 + 1) + (4 - (int)*(int **)(param_2 + 0x28)));
      }
      local_228 = (char *)0x0;
      *(undefined4 *)(param_1 + 0x364) = **(undefined4 **)(param_2 + 0x24);
      *(int *)(param_2 + 0x24) = *(int *)(param_2 + 0x24) + 4;
      if (*(int *)(param_1 + 0x364) < 1) {
        ExceptionList = pvStack_c;
        return 0;
      }
      piVar9 = (int *)(param_1 + 0x1d4);
      do {
        if (*(uint *)(param_2 + 0x28) < *(int *)(param_2 + 0x24) + 4U) {
          CArchive::FillBuffer(param_2,(*(int *)(param_2 + 0x24) - *(uint *)(param_2 + 0x28)) + 4);
        }
        uVar1 = **(uint **)(param_2 + 0x24);
        *(uint **)(param_2 + 0x24) = *(uint **)(param_2 + 0x24) + 1;
        if (*(int *)(param_1 + 0x2c) == 0) {
          uStack_218 = (**(code **)(**(int **)(param_1 + 0x4c) + 0x38))();
          iVar8 = TVH_ReadArchiveChunked(*(int **)(param_1 + 0x4c),param_2,uVar1);
          if (iVar8 == 0) {
            pcVar10 = s_Load_VideoHolder_data_error_100145a0;
            goto LAB_100080ef;
          }
          local_21c = (CFile *)(**(code **)(**(int **)(param_1 + 0x4c) + 0x38))();
        }
        else {
          CArchive::Flush(param_2);
          (**(code **)(**(int **)(param_2 + 0x20) + 0x30))(uVar1,1);
        }
        pvVar7 = operator_new(0x568);
        *piVar9 = (int)pvVar7;
        CArchive::Read(param_2,pvVar7,0x568);
        if (iStack_220 < 3) {
          *(undefined4 *)(*piVar9 + 0x510) = *(undefined4 *)(*piVar9 + 0x554);
        }
        pvVar7 = operator_new((*(int *)(*piVar9 + 0x80) * 8 + 0x1fU >> 3 & 0x1ffffffc) *
                              *(int *)(*piVar9 + 0x84));
        *(void **)(*piVar9 + 0x560) = pvVar7;
        iVar8 = *piVar9;
        if (*(void **)(iVar8 + 0x560) == (void *)0x0) {
          pcVar10 = s_End_of_memory_1001424c;
          goto LAB_100080ef;
        }
        CArchive::Read(param_2,*(void **)(iVar8 + 0x560),
                       (*(int *)(iVar8 + 0x80) * 8 + 0x1fU >> 3 & 0x1ffffffc) *
                       *(int *)(iVar8 + 0x84));
        *(undefined4 *)(*piVar9 + 0x564) = 0;
        *(undefined4 *)(*piVar9 + 0x4f0) = 0;
        if (*(int *)(param_1 + 0x2c) == 0) {
          *(undefined4 *)(*piVar9 + 0x558) = uStack_218;
          *(CFile **)(*piVar9 + 0x55c) = local_21c;
        }
        *(undefined4 *)(*piVar9 + 0x508) = *(undefined4 *)(*piVar9 + 0x54c);
        *(undefined4 *)(*piVar9 + 0x50c) = *(undefined4 *)(*piVar9 + 0x550);
        local_228 = local_228 + 1;
        piVar9 = piVar9 + 1;
        if (*(int *)(param_1 + 0x364) <= (int)local_228) {
          ExceptionList = pvStack_c;
          return 0;
        }
      } while( true );
    }
    pcVar10 = s_Cannot_create_Video_temporary_fi_1001457c;
  }
  else {
    if (*(uint *)(param_2 + 0x28) < *(int *)(param_2 + 0x24) + 4U) {
      CArchive::Flush(param_2);
    }
    **(undefined4 **)(param_2 + 0x24) = 3;
    iVar8 = *(int *)(param_2 + 0x24);
    *(int *)(param_2 + 0x24) = iVar8 + 4;
    uVar2 = *(undefined4 *)(param_1 + 0x364);
    if (*(uint *)(param_2 + 0x28) < iVar8 + 8U) {
      CArchive::Flush(param_2);
    }
    local_224 = (char *)0x0;
    **(undefined4 **)(param_2 + 0x24) = uVar2;
    *(int *)(param_2 + 0x24) = *(int *)(param_2 + 0x24) + 4;
    if (*(int *)(param_1 + 0x364) < 1) {
      ExceptionList = pvStack_c;
      return 0;
    }
    piVar9 = (int *)(param_1 + 0x1d4);
    while( true ) {
      iVar8 = *(int *)(*piVar9 + 0x55c);
      iVar3 = *(int *)(*piVar9 + 0x558);
      if (*(uint *)(param_2 + 0x28) < *(int *)(param_2 + 0x24) + 4U) {
        CArchive::Flush(param_2);
      }
      **(int **)(param_2 + 0x24) = iVar8 - iVar3;
      *(int *)(param_2 + 0x24) = *(int *)(param_2 + 0x24) + 4;
      iVar8 = *(int *)(*piVar9 + 0x558);
      local_228 = *(char **)(*piVar9 + 0x55c);
      CArchive::Flush(param_2);
      uVar2 = (**(code **)(**(int **)(param_2 + 0x20) + 0x38))();
      *(undefined4 *)(*piVar9 + 0x558) = uVar2;
      iVar3 = (**(code **)(**(int **)(param_1 + 0x4c) + 0x30))(iVar8,0);
      if ((iVar3 != iVar8) ||
         (iVar3 = TVH_WriteArchiveChunked(param_2,*(int **)(param_1 + 0x4c),(int)local_228 - iVar8),
         iVar3 == 0)) break;
      uVar2 = (**(code **)(**(int **)(param_2 + 0x20) + 0x38))();
      *(undefined4 *)(*piVar9 + 0x55c) = uVar2;
      CArchive::Write(param_2,(void *)*piVar9,0x568);
      iVar3 = *piVar9;
      CArchive::Write(param_2,*(void **)(iVar3 + 0x560),
                      (*(int *)(iVar3 + 0x80) * 8 + 0x1fU >> 3 & 0x1ffffffc) *
                      *(int *)(iVar3 + 0x84));
      *(int *)(*piVar9 + 0x558) = iVar8;
      *(char **)(*piVar9 + 0x55c) = local_228;
      local_224 = local_224 + 1;
      piVar9 = piVar9 + 1;
      if (*(int *)(param_1 + 0x364) <= (int)local_224) {
        ExceptionList = pvStack_c;
        return 0;
      }
    }
    pcVar10 = s_Save_VideoHolder_data_error_100145dc;
  }
LAB_100080ef:
  AfxMessageBox(pcVar10,0,0);
  ExceptionList = pvStack_c;
  return 1;
}


// ============================================================================
// Function: TVH_SeekEntryVideo at 10008120
// Signature: undefined4 __stdcall TVH_SeekEntryVideo(int param_1, int param_2)
// Entry: 10008120
// Body: 10008120 - 100081f7
// ============================================================================
undefined4 TVH_SeekEntryVideo(int param_1,int param_2)

{
  void *this;
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  
  if (((-1 < param_2) && (param_2 < *(int *)(param_1 + 0x360))) && (*(int *)(param_1 + 0x48) != 0))
  {
    TVH_HideVideoRect(param_1 + -4);
    piVar1 = *(int **)(param_1 + 0x3c);
    if (piVar1 != (int *)0x0) {
      uVar2 = (**(code **)(*piVar1 + 0x28))(piVar1);
      *(undefined4 *)(param_1 + 0xdc) = uVar2;
    }
    iVar4 = *(int *)(param_1 + 0x1d0 + param_2 * 4);
    iVar3 = *(int *)(iVar4 + 0x558);
    if (iVar3 != *(int *)(iVar4 + 0x55c)) {
      iVar4 = *(int *)(param_1 + 0x1d0 + param_2 * 4);
      iVar3 = (**(code **)(**(int **)(param_1 + 0x48) + 0x30))(iVar3,0);
      if (iVar3 == *(int *)(iVar4 + 0x558)) {
        *(undefined4 *)(iVar4 + 0x4e8) = *(undefined4 *)(iVar4 + 0x500);
        iVar4 = *(int *)(param_1 + 0x1d0 + param_2 * 4);
        this = (void *)(param_1 + 0x50);
        *(undefined4 *)(iVar4 + 0x4ec) = *(undefined4 *)(iVar4 + 0x504);
        TVH_SetPlaybackRect(this,(undefined4 *)(param_1 + 0x2c));
        iVar4 = TVH_OpenEntryPlayback(this,*(int **)(param_1 + 0x48),0,param_2);
        if (iVar4 != 0) {
          iVar4 = TVH_StartEntryPlayback(this,param_1);
          if (iVar4 != 0) {
            return 0;
          }
        }
      }
    }
  }
  return 1;
}


// ============================================================================
// Function: TVH_HideVideoRect at 10008200
// Signature: undefined4 __fastcall TVH_HideVideoRect(int param_1)
// Entry: 10008200
// Body: 10008200 - 1000828d
// ============================================================================
undefined4 __fastcall TVH_HideVideoRect(int param_1)

{
  int *piVar1;
  int iVar2;
  int local_10;
  int local_c;
  int local_8;
  int local_4;
  
  iVar2 = *(int *)(param_1 + 0xd8);
  if (iVar2 < 0) {
    return 0;
  }
  local_10 = *(int *)(*(int *)(param_1 + 0x1d4 + iVar2 * 4) + 0x54c);
  local_8 = *(int *)(*(int *)(param_1 + 0x1d4 + iVar2 * 4) + 0x80) + local_10;
  local_c = *(int *)(*(int *)(param_1 + 0x1d4 + iVar2 * 4) + 0x550);
  local_4 = *(int *)(*(int *)(param_1 + 0x1d4 + iVar2 * 4) + 0x84) + local_c;
  iVar2 = TVH_StopEntryPlayback(param_1 + 0x54);
  if (iVar2 == 0) {
    return 0;
  }
  piVar1 = *(int **)(param_1 + 0x40);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 0xc))(piVar1,&local_10,0xffffffff);
  }
  return 1;
}


// ============================================================================
// Function: TVH_FreeEntryBuffers at 10009550
// Signature: undefined __fastcall TVH_FreeEntryBuffers(int param_1)
// Entry: 10009550
// Body: 10009550 - 100095ac
// ============================================================================
void __fastcall TVH_FreeEntryBuffers(int param_1)

{
  HGLOBAL pvVar1;
  
  if (*(LPCVOID *)(param_1 + 0x4c) != (LPCVOID)0x0) {
    pvVar1 = GlobalHandle(*(LPCVOID *)(param_1 + 0x4c));
    GlobalUnlock(pvVar1);
    pvVar1 = GlobalHandle(*(LPCVOID *)(param_1 + 0x4c));
    GlobalFree(pvVar1);
  }
  *(undefined4 *)(param_1 + 0x4c) = 0;
  if (*(LPCVOID *)(param_1 + 0x48) != (LPCVOID)0x0) {
    pvVar1 = GlobalHandle(*(LPCVOID *)(param_1 + 0x48));
    GlobalUnlock(pvVar1);
    pvVar1 = GlobalHandle(*(LPCVOID *)(param_1 + 0x48));
    GlobalFree(pvVar1);
    *(undefined4 *)(param_1 + 0x48) = 0;
    return;
  }
  *(undefined4 *)(param_1 + 0x48) = 0;
  return;
}


// ============================================================================
// Function: TVH_OpenAudioDevice at 10009690
// Signature: undefined4 __thiscall TVH_OpenAudioDevice(void * this, int param_1)
// Entry: 10009690
// Body: 10009690 - 100097f0
// ============================================================================
undefined4 __thiscall TVH_OpenAudioDevice(void *this,int param_1)

{
  int *piVar1;
  int iVar2;
  MMRESULT MVar3;
  undefined4 *puVar4;
  WAVEFORMATEX local_28;
  undefined4 local_14;
  undefined4 local_10;
  int local_c;
  undefined4 local_8;
  WAVEFORMATEX *local_4;
  
  local_28.wFormatTag = 1;
  local_28.nChannels = *(WORD *)(param_1 + 0x98);
  iVar2 = *(int *)(param_1 + 0x94);
  local_28.nSamplesPerSec = *(DWORD *)(param_1 + 0x90);
  local_28.wBitsPerSample = (WORD)iVar2;
  local_28.nAvgBytesPerSec =
       ((int)(iVar2 * local_28.nSamplesPerSec +
             ((int)(iVar2 * local_28.nSamplesPerSec) >> 0x1f & 7U)) >> 3) * *(int *)(param_1 + 0x98)
  ;
  iVar2 = *(int *)(param_1 + 0x98) * iVar2;
  local_28.nBlockAlign = (WORD)((int)(iVar2 + (iVar2 >> 0x1f & 7U)) >> 3);
  piVar1 = *(int **)((int)this + 0x8c);
  local_28.cbSize = 0;
  if (piVar1 == (int *)0x0) {
    *(undefined4 *)((int)this + 0xa0) = 0;
    puVar4 = (undefined4 *)((int)this + 0xa4);
    for (iVar2 = 0x28; iVar2 != 0; iVar2 = iVar2 + -1) {
      *puVar4 = 0;
      puVar4 = puVar4 + 1;
    }
    MVar3 = waveOutOpen((LPHWAVEOUT)0x0,0xffffffff,&local_28,0,0,1);
    if (MVar3 == 0) {
      MVar3 = waveOutOpen((LPHWAVEOUT)((int)this + 0x80),0xffffffff,&local_28,
                          *(DWORD_PTR *)((int)this + 0x20),0,0x10000);
      if (MVar3 != 0) {
        return 0;
      }
    }
  }
  else {
    local_c = *(int *)(param_1 + 0xe0);
    *(undefined4 *)((int)this + 0x98) = 0;
    local_c = local_c * 3;
    *(int *)((int)this + 0x94) = local_c;
    local_8 = 0;
    local_4 = &local_28;
    *(undefined4 *)((int)this + 0x9c) = 0;
    local_14 = 0x14;
    local_10 = 0xe2;
    iVar2 = (**(code **)(*piVar1 + 0xc))(piVar1,&local_14,(undefined4 *)((int)this + 0x90),0);
    if (iVar2 != 0) {
      return 0;
    }
    piVar1 = *(int **)((int)this + 0x90);
    (**(code **)(*piVar1 + 0x40))(piVar1,*(undefined4 *)(param_1 + 0x78));
  }
  return 1;
}


// ============================================================================
// Function: TVH_CloseAudioDevice at 10009800
// Signature: undefined4 __fastcall TVH_CloseAudioDevice(int param_1)
// Entry: 10009800
// Body: 10009800 - 10009815
// ============================================================================
undefined4 __fastcall TVH_CloseAudioDevice(int param_1)

{
  TVH_StopAudioPlayback(param_1);
  TVH_FreeEntryBuffers(param_1);
  return 1;
}


// ============================================================================
// Function: TVH_StopAudioPlayback at 10009820
// Signature: undefined __fastcall TVH_StopAudioPlayback(int param_1)
// Entry: 10009820
// Body: 10009820 - 10009925
// ============================================================================
void __fastcall TVH_StopAudioPlayback(int param_1)

{
  LPWAVEHDR pwh;
  int *piVar1;
  MMRESULT MVar2;
  HGLOBAL pvVar3;
  int iVar4;
  undefined4 *puVar5;
  int iStack_4;
  
  if (*(int *)(param_1 + 0x8c) == 0) {
    if (*(HWAVEOUT *)(param_1 + 0x80) != (HWAVEOUT)0x0) {
      waveOutBreakLoop(*(HWAVEOUT *)(param_1 + 0x80));
      MVar2 = waveOutReset(*(HWAVEOUT *)(param_1 + 0x80));
      if (MVar2 == 0) {
        puVar5 = (undefined4 *)(param_1 + 0xa4);
        iStack_4 = 0x28;
        do {
          pwh = (LPWAVEHDR)*puVar5;
          if ((pwh != (LPWAVEHDR)0x0) && ((pwh->dwFlags & 1) != 0)) {
            waveOutUnprepareHeader(*(HWAVEOUT *)(param_1 + 0x80),pwh,0x20);
            pvVar3 = GlobalHandle(*(LPCVOID *)*puVar5);
            GlobalUnlock(pvVar3);
            pvVar3 = GlobalHandle(*(LPCVOID *)*puVar5);
            GlobalFree(pvVar3);
            operator_delete((void *)*puVar5);
            *puVar5 = 0;
          }
          puVar5 = puVar5 + 1;
          iStack_4 = iStack_4 + -1;
        } while (iStack_4 != 0);
        waveOutClose(*(HWAVEOUT *)(param_1 + 0x80));
        *(undefined4 *)(param_1 + 0x80) = 0;
        return;
      }
    }
  }
  else {
    piVar1 = *(int **)(param_1 + 0x90);
    if (piVar1 != (int *)0x0) {
      (**(code **)(*piVar1 + 0x48))(piVar1);
      iVar4 = (**(code **)(**(int **)(param_1 + 0x90) + 8))(*(int **)(param_1 + 0x90));
      while (iVar4 != 0) {
        iVar4 = (**(code **)(**(int **)(param_1 + 0x90) + 8))(*(int **)(param_1 + 0x90));
      }
      *(undefined4 *)(param_1 + 0x90) = 0;
    }
  }
  return;
}


// ============================================================================
// Function: TVH_SetPlaybackRect at 10009930
// Signature: undefined __thiscall TVH_SetPlaybackRect(void * this, undefined4 * param_1)
// Entry: 10009930
// Body: 10009930 - 1000994f
// ============================================================================
void __thiscall TVH_SetPlaybackRect(void *this,undefined4 *param_1)

{
  *(undefined4 *)((int)this + 0x54) = *param_1;
  *(undefined4 *)((int)this + 0x58) = param_1[1];
  *(undefined4 *)((int)this + 0x5c) = param_1[2];
  *(undefined4 *)((int)this + 0x60) = param_1[3];
  return;
}


// ============================================================================
// Function: TVH_OpenEntryPlayback at 10009970
// Signature: undefined4 __thiscall TVH_OpenEntryPlayback(void * this, int * param_1, undefined4 param_2, int param_3)
// Entry: 10009970
// Body: 10009970 - 100099f3
// ============================================================================
undefined4 __thiscall TVH_OpenEntryPlayback(void *this,int *param_1,undefined4 param_2,int param_3)

{
  undefined4 uVar1;
  uint dwBytes;
  HGLOBAL hMem;
  LPVOID pvVar2;
  uint uVar3;
  
  if (param_1 == (int *)0x0) {
    return 0;
  }
  *(int **)((int)this + 0x40) = param_1;
  uVar1 = (**(code **)(*param_1 + 0x14))();
  *(undefined4 *)((int)this + 0x44) = uVar1;
  *(undefined4 *)((int)this + 0x50) =
       *(undefined4 *)(*(int *)((int)this + 0x144) + 0x1d4 + param_3 * 4);
  TVH_FreeEntryBuffers((int)this);
  *(int *)((int)this + 0x84) = param_3;
  dwBytes = *(uint *)(*(int *)((int)this + 0x50) + 0xe4);
  uVar3 = *(int *)(*(int *)((int)this + 0x50) + 0xe0) + 0x4c;
  if (dwBytes <= uVar3) {
    dwBytes = uVar3;
  }
  hMem = GlobalAlloc(0x42,dwBytes);
  pvVar2 = GlobalLock(hMem);
  *(LPVOID *)((int)this + 0x4c) = pvVar2;
  if (pvVar2 == (LPVOID)0x0) {
    return 0;
  }
  *(undefined4 *)((int)this + 0x74) = param_2;
  uVar1 = TVH_ReadFirstChunk(this);
  return uVar1;
}


// ============================================================================
// Function: TVH_DrawStillFrame at 10009a00
// Signature: undefined4 __thiscall TVH_DrawStillFrame(void * this, int param_1)
// Entry: 10009a00
// Body: 10009a00 - 10009a98
// ============================================================================
undefined4 __thiscall TVH_DrawStillFrame(void *this,int param_1)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  
  iVar1 = *(int *)((int)this + 0x4c);
  if ((iVar1 != 0) && (*(int *)(iVar1 + 4) == -0xffffff)) {
    if (*(int *)(*(int *)((int)this + 0x50) + 0x70) == 0) {
      uVar3 = 1000;
    }
    else {
      uVar3 = *(uint *)(*(int *)((int)this + 0x50) + 0x74);
    }
    iVar2 = *(int *)(*(int *)((int)this + 0x144) + 0x1d4 + *(int *)((int)this + 0x84) * 4);
    FUN_1000b8a0(*(int *)((int)this + 0x5c) - *(int *)((int)this + 0x54),
                 *(int *)((int)this + 0x60) - *(int *)((int)this + 0x58),param_1,
                 *(int *)(iVar2 + 0x54c) + *(int *)(iVar1 + 0x14),
                 *(int *)(iVar2 + 0x550) + *(int *)(iVar1 + 0x18),(ushort *)(iVar1 + 0x48),
                 *(int *)(iVar1 + 0x1c) - *(int *)(iVar1 + 0x14),
                 *(int *)(iVar1 + 0x20) - *(int *)(iVar1 + 0x18),uVar3);
    return 1;
  }
  return 1;
}


// ============================================================================
// Function: TVH_ProcessVideoChunks at 10009aa0
// Signature: undefined __thiscall TVH_ProcessVideoChunks(void * this, uint param_1)
// Entry: 10009aa0
// Body: 10009aa0 - 1000a17e
// ============================================================================
void __thiscall TVH_ProcessVideoChunks(void *this,uint param_1)

{
  DWORD *lpBuffer;
  DWORD nNumberOfBytesToRead;
  DWORD DVar1;
  int iVar2;
  int iVar3;
  BOOL BVar4;
  void *pvVar5;
  HGLOBAL hMem;
  LPVOID pvVar6;
  int *piVar7;
  undefined4 uVar8;
  uint uVar9;
  uint uVar10;
  undefined4 *puVar11;
  int iVar12;
  undefined4 *puVar13;
  int *piVar14;
  DWORD local_24;
  undefined4 *puStack_20;
  undefined4 *puStack_1c;
  uint uStack_18;
  uint uStack_14;
  int iStack_10;
  int iStack_c;
  int iStack_8;
  int iStack_4;
  
  if ((*(uint *)((int)this + 0x7c) <= param_1) && (*(int *)((int)this + 0x40) != 0)) {
    lpBuffer = *(DWORD **)((int)this + 0x4c);
    *(uint *)((int)this + 0x7c) = *(int *)(*(int *)((int)this + 0x50) + 0x7c) + param_1;
    nNumberOfBytesToRead = *lpBuffer;
    while (nNumberOfBytesToRead != 0) {
      if ((lpBuffer[1] == 0xff000001) && (DVar1 = lpBuffer[4], DVar1 != 0)) {
        uVar10 = *(uint *)(*(int *)((int)this + 0x50) + 0x7c);
        if (param_1 <= (DVar1 + 1) * uVar10 + *(int *)((int)this + 0x78)) goto LAB_10009bc1;
        for (iVar12 = (param_1 - *(int *)((int)this + 0x78)) / uVar10 - DVar1; 0 < iVar12;
            iVar12 = iVar12 + -1) {
          BVar4 = ReadFile(*(HANDLE *)(*(int *)((int)this + 0x40) + 4),*(LPVOID *)((int)this + 0x4c)
                           ,0x4c,&local_24,(LPOVERLAPPED)0x0);
          if ((BVar4 == 0) || (local_24 != 0x4c)) goto LAB_10009f25;
          if ((iVar12 < 2) ||
             ((piVar7 = *(int **)((int)this + 0x4c), piVar7[2] == 0 || (piVar7[1] == -0xfffffe)))) {
            BVar4 = ReadFile(*(HANDLE *)(*(int *)((int)this + 0x40) + 4),
                             *(int **)((int)this + 0x4c) + 0x13,**(int **)((int)this + 0x4c) - 0x4c,
                             &local_24,(LPOVERLAPPED)0x0);
            if ((BVar4 == 0) || (local_24 != **(int **)((int)this + 0x4c) - 0x4cU))
            goto LAB_10009f25;
            break;
          }
          (**(code **)(**(int **)((int)this + 0x40) + 0x30))(*piVar7 + -0x4c,1);
        }
      }
      else {
LAB_10009bc1:
        BVar4 = ReadFile(*(HANDLE *)(*(int *)((int)this + 0x40) + 4),lpBuffer,nNumberOfBytesToRead,
                         &local_24,(LPOVERLAPPED)0x0);
        if ((BVar4 == 0) || (local_24 != **(DWORD **)((int)this + 0x4c))) goto LAB_10009f25;
      }
      iVar12 = *(int *)(*(int *)((int)this + 0x4c) + 4);
      if (iVar12 == -0xffffff) {
        iVar12 = *(int *)((int)this + 0x4c);
        if (*(int *)(iVar12 + 0x24) == 0) {
          if (*(int *)((int)this + 0x74) == 0) {
            iVar2 = *(int *)((int)this + 0x144);
            piVar7 = (int *)(iVar2 + 0x1d4 + *(int *)((int)this + 0x84) * 4);
            iStack_10 = *(int *)(*(int *)(iVar2 + 0x1d4 + *(int *)((int)this + 0x84) * 4) + 0x54c) +
                        *(int *)(iVar12 + 0x14);
            iStack_8 = *(int *)(*piVar7 + 0x54c) + *(int *)(iVar12 + 0x1c);
            iStack_c = *(int *)(*piVar7 + 0x550) + *(int *)(iVar12 + 0x18);
            iStack_4 = *(int *)(*piVar7 + 0x550) + *(int *)(iVar12 + 0x20);
            iVar3 = *(int *)((int)this + 0x50);
            if (*(int *)(iVar3 + 0x6c) == 0) {
              piVar14 = *(int **)(iVar2 + 0x40);
              if (piVar14 == (int *)0x0) goto LAB_1000a12d;
              if (*(int *)(iVar3 + 0x70) == 0) {
                iVar12 = *piVar14;
                uVar8 = *(undefined4 *)(*piVar7 + 0x554);
              }
              else {
                iVar12 = *piVar14;
                uVar8 = 0xffffffff;
              }
            }
            else {
              FUN_1000b8a0(*(int *)(iVar3 + 0x80),*(int *)(iVar3 + 0x84),*(int *)((int)this + 0x48),
                           *(int *)(iVar12 + 0x14),*(int *)(iVar12 + 0x18),(ushort *)(iVar12 + 0x48)
                           ,*(int *)(iVar12 + 0x1c) - *(int *)(iVar12 + 0x14),
                           *(int *)(iVar12 + 0x20) - *(int *)(iVar12 + 0x18),1000);
              piVar14 = *(int **)(*(int *)((int)this + 0x144) + 0x40);
              if (piVar14 == (int *)0x0) goto LAB_1000a12d;
              if (*(int *)(*(int *)((int)this + 0x50) + 0x70) == 0) {
                uVar8 = *(undefined4 *)
                         (*(int *)(*(int *)((int)this + 0x144) + 0x1d4 +
                                  *(int *)((int)this + 0x84) * 4) + 0x554);
              }
              else {
                uVar8 = 0xffffffff;
              }
              iVar12 = *piVar14;
            }
            (**(code **)(iVar12 + 0xc))(piVar14,&iStack_10,uVar8);
          }
          else {
            iStack_10 = *(int *)(iVar12 + 0x14);
            iStack_8 = *(int *)(iVar12 + 0x1c);
            iStack_c = *(int *)(iVar12 + 0x18);
            iStack_4 = *(int *)(iVar12 + 0x20);
            if ((*(int *)(*(int *)((int)this + 0x50) + 0x6c) == 0) &&
               (*(int *)(*(int *)((int)this + 0x50) + 0x70) != 0)) {
              piVar7 = *(int **)(*(int *)((int)this + 0x148) + 0xd4);
              if (piVar7[4] == 3) {
                iVar12 = *piVar7 + 0xc;
              }
              else {
                iVar12 = *piVar7 + piVar7[8] * 4;
              }
              FUN_1000d920((uint)piVar7,(undefined4 *)((int)piVar7 + iVar12),0);
            }
            if (*(int *)(*(int *)((int)this + 0x50) + 0x70) == 0) {
              uVar10 = 1000;
            }
            else {
              uVar10 = *(uint *)(*(int *)((int)this + 0x50) + 0x74);
            }
            FUN_1000b8a0(*(int *)((int)this + 0x6c) - *(int *)((int)this + 100),
                         *(int *)((int)this + 0x70) - *(int *)((int)this + 0x68),
                         *(int *)((int)this + 0x74),iStack_10,iStack_c,
                         (ushort *)(*(int *)((int)this + 0x4c) + 0x48),iStack_8 - iStack_10,
                         iStack_4 - iStack_c,uVar10);
            if (*(int *)((int)this + 0x148) != 0) {
              FUN_10003250(*(int *)((int)this + 0x148) + 0x94);
            }
          }
        }
LAB_1000a12d:
        **(undefined4 **)((int)this + 0x4c) = (*(undefined4 **)((int)this + 0x4c))[2];
        if (*(int *)(*(int *)((int)this + 0x4c) + 0x10) != 0) {
          return;
        }
        *(uint *)((int)this + 0x78) = param_1;
        return;
      }
      if (iVar12 != -0xfffffe) {
        return;
      }
      piVar7 = *(int **)((int)this + 0x90);
      if (piVar7 == (int *)0x0) {
        piVar7 = *(int **)((int)this + *(int *)((int)this + 0xa0) * 4 + 0xa4);
        if (piVar7 == (int *)0x0) {
          pvVar5 = operator_new(0x20);
          *(void **)((int)this + *(int *)((int)this + 0xa0) * 4 + 0xa4) = pvVar5;
          puVar11 = *(undefined4 **)((int)this + *(int *)((int)this + 0xa0) * 4 + 0xa4);
          for (iVar12 = 8; iVar12 != 0; iVar12 = iVar12 + -1) {
            *puVar11 = 0;
            puVar11 = puVar11 + 1;
          }
          hMem = GlobalAlloc(2,*(SIZE_T *)(*(int *)((int)this + 0x50) + 0xe0));
          pvVar6 = GlobalLock(hMem);
          **(undefined4 **)((int)this + *(int *)((int)this + 0xa0) * 4 + 0xa4) = pvVar6;
          piVar7 = *(int **)((int)this + *(int *)((int)this + 0xa0) * 4 + 0xa4);
          if (*piVar7 == 0) goto LAB_10009f25;
        }
        piVar7[1] = *(int *)(*(int *)((int)this + 0x50) + 0xe0);
        uVar10 = *(uint *)(*(int *)((int)this + 0x50) + 0xe0);
        puVar11 = (undefined4 *)(*(int *)((int)this + 0x4c) + 0x48);
        puVar13 = (undefined4 *)**(undefined4 **)((int)this + *(int *)((int)this + 0xa0) * 4 + 0xa4)
        ;
        for (uVar9 = uVar10 >> 2; uVar9 != 0; uVar9 = uVar9 - 1) {
          *puVar13 = *puVar11;
          puVar11 = puVar11 + 1;
          puVar13 = puVar13 + 1;
        }
        for (uVar10 = uVar10 & 3; uVar10 != 0; uVar10 = uVar10 - 1) {
          *(undefined1 *)puVar13 = *(undefined1 *)puVar11;
          puVar11 = (undefined4 *)((int)puVar11 + 1);
          puVar13 = (undefined4 *)((int)puVar13 + 1);
        }
        waveOutPrepareHeader
                  (*(HWAVEOUT *)((int)this + 0x80),
                   *(LPWAVEHDR *)((int)this + *(int *)((int)this + 0xa0) * 4 + 0xa4),0x20);
        waveOutWrite(*(HWAVEOUT *)((int)this + 0x80),
                     *(LPWAVEHDR *)((int)this + *(int *)((int)this + 0xa0) * 4 + 0xa4),0x20);
        iVar12 = *(int *)((int)this + 0xa0) + 1;
        *(int *)((int)this + 0xa0) = iVar12;
        if (0x27 < iVar12) {
          *(undefined4 *)((int)this + 0xa0) = 0;
        }
      }
      else {
        puStack_20 = (undefined4 *)0x0;
        puStack_1c = (undefined4 *)0x0;
        iVar12 = (**(code **)(*piVar7 + 0x2c))
                           (piVar7,*(undefined4 *)((int)this + 0x98),
                            *(undefined4 *)(*(int *)((int)this + 0x50) + 0xe0),&puStack_20,
                            &uStack_18,&puStack_1c,&uStack_14,0);
        if (iVar12 != 0) goto LAB_10009f25;
        if (puStack_20 == (undefined4 *)0x0) {
          if (puStack_1c == (undefined4 *)0x0) goto LAB_10009f25;
          puVar11 = (undefined4 *)(*(int *)((int)this + 0x4c) + 0x48);
          puVar13 = puStack_1c;
          for (uVar10 = uStack_14 >> 2; uVar10 != 0; uVar10 = uVar10 - 1) {
            *puVar13 = *puVar11;
            puVar11 = puVar11 + 1;
            puVar13 = puVar13 + 1;
          }
          for (uVar10 = uStack_14 & 3; uVar10 != 0; uVar10 = uVar10 - 1) {
            *(undefined1 *)puVar13 = *(undefined1 *)puVar11;
            puVar11 = (undefined4 *)((int)puVar11 + 1);
            puVar13 = (undefined4 *)((int)puVar13 + 1);
          }
          piVar7 = *(int **)((int)this + 0x90);
          iVar12 = *piVar7;
          puVar11 = puStack_1c;
          uVar10 = uStack_14;
        }
        else {
          puVar11 = (undefined4 *)(*(int *)((int)this + 0x4c) + 0x48);
          puVar13 = puStack_20;
          for (uVar10 = uStack_18 >> 2; uVar10 != 0; uVar10 = uVar10 - 1) {
            *puVar13 = *puVar11;
            puVar11 = puVar11 + 1;
            puVar13 = puVar13 + 1;
          }
          for (uVar10 = uStack_18 & 3; uVar10 != 0; uVar10 = uVar10 - 1) {
            *(undefined1 *)puVar13 = *(undefined1 *)puVar11;
            puVar11 = (undefined4 *)((int)puVar11 + 1);
            puVar13 = (undefined4 *)((int)puVar13 + 1);
          }
          piVar7 = *(int **)((int)this + 0x90);
          iVar12 = *piVar7;
          puVar11 = puStack_20;
          uVar10 = uStack_18;
        }
        (**(code **)(iVar12 + 0x4c))(piVar7,puVar11,uVar10,0,0);
        if (*(int *)((int)this + 0x9c) == 0) {
          iVar12 = (**(code **)(**(int **)((int)this + 0x90) + 0x30))
                             (*(int **)((int)this + 0x90),0,0,1);
          if (iVar12 != 0) goto LAB_10009f25;
          *(undefined4 *)((int)this + 0x9c) = 1;
        }
        iVar12 = *(int *)((int)this + 0x98) + *(int *)(*(int *)((int)this + 0x50) + 0xe0);
        *(int *)((int)this + 0x98) = iVar12;
        if (*(int *)((int)this + 0x94) <= iVar12) {
          *(undefined4 *)((int)this + 0x98) = 0;
        }
      }
      **(undefined4 **)((int)this + 0x4c) = (*(undefined4 **)((int)this + 0x4c))[2];
      lpBuffer = *(DWORD **)((int)this + 0x4c);
      nNumberOfBytesToRead = *lpBuffer;
    }
    if (*(int *)((int)this + 0x148) == 0) {
      *(undefined4 *)
       (*(int *)(*(int *)((int)this + 0x144) + 0x1d4 + *(int *)((int)this + 0x84) * 4) + 0x4f0) = 1;
      iVar12 = *(int *)(*(int *)((int)this + 0x144) + 0x1d4 + *(int *)((int)this + 0x84) * 4);
      if (*(int *)(iVar12 + 0x4e8) != 0) {
        if (*(int *)(iVar12 + 0x564) == 0) {
          pvVar5 = operator_new((*(int *)(*(int *)((int)this + 0x50) + 0x80) * 8 + 0x1fU >> 3 &
                                0x1ffffffc) * *(int *)(*(int *)((int)this + 0x50) + 0x84));
          *(void **)(*(int *)(*(int *)((int)this + 0x144) + 0x1d4 + *(int *)((int)this + 0x84) * 4)
                    + 0x564) = pvVar5;
        }
        puVar11 = *(undefined4 **)
                   (*(int *)(*(int *)((int)this + 0x144) + 0x1d4 + *(int *)((int)this + 0x84) * 4) +
                   0x564);
        if (puVar11 != (undefined4 *)0x0) {
          iVar12 = *(int *)((int)this + 0x50);
          if ((*(int *)(iVar12 + 0x6c) == 0) ||
             (*(undefined4 **)((int)this + 0x48) == (undefined4 *)0x0)) {
            iVar2 = *(int *)((int)this + 0x4c);
            FUN_1000b8a0(*(int *)(iVar12 + 0x80),*(int *)(iVar12 + 0x84),(int)puVar11,
                         *(int *)(iVar2 + 0x14),*(int *)(iVar2 + 0x18),(ushort *)(iVar2 + 0x48),
                         *(int *)(iVar2 + 0x1c) - *(int *)(iVar2 + 0x14),
                         *(int *)(iVar2 + 0x20) - *(int *)(iVar2 + 0x18),1000);
          }
          else {
            puVar13 = *(undefined4 **)((int)this + 0x48);
            for (uVar10 = (*(int *)(iVar12 + 0x80) * 8 + 0x1fU >> 3 & 0x1ffffffc) *
                          *(int *)(iVar12 + 0x84) >> 2; uVar10 != 0; uVar10 = uVar10 - 1) {
              *puVar11 = *puVar13;
              puVar13 = puVar13 + 1;
              puVar11 = puVar11 + 1;
            }
            for (iVar12 = 0; iVar12 != 0; iVar12 = iVar12 + -1) {
              *(undefined1 *)puVar11 = *(undefined1 *)puVar13;
              puVar13 = (undefined4 *)((int)puVar13 + 1);
              puVar11 = (undefined4 *)((int)puVar11 + 1);
            }
          }
        }
      }
    }
LAB_10009f25:
    TVH_StopAudioPlayback((int)this);
    TVH_NotifyPlaybackEnd(this,*(undefined4 *)((int)this + 0x84));
  }
  return;
}


// ============================================================================
// Function: TVH_StopEntryPlayback at 1000a180
// Signature: undefined4 __fastcall TVH_StopEntryPlayback(int param_1)
// Entry: 1000a180
// Body: 1000a180 - 1000a1ac
// ============================================================================
undefined4 __fastcall TVH_StopEntryPlayback(int param_1)

{
  if (*(int *)(param_1 + 0x40) == 0) {
    return 0;
  }
  *(undefined4 *)(param_1 + 0x74) = 0;
  TVH_StopAudioPlayback(param_1);
  *(undefined4 *)(param_1 + 0x84) = 0xffffffff;
  return 1;
}


// ============================================================================
// Function: TVH_IsEntryPlaying at 1000a1b0
// Signature: bool __fastcall TVH_IsEntryPlaying(int param_1)
// Entry: 1000a1b0
// Body: 1000a1b0 - 1000a1bd
// ============================================================================
bool __fastcall TVH_IsEntryPlaying(int param_1)

{
  return -1 < *(int *)(param_1 + 0x84);
}


// ============================================================================
// Function: TVH_ReadFirstChunk at 1000a1c0
// Signature: undefined4 __fastcall TVH_ReadFirstChunk(void * param_1)
// Entry: 1000a1c0
// Body: 1000a1c0 - 1000a3ca
// ============================================================================
undefined4 __fastcall TVH_ReadFirstChunk(void *param_1)

{
  DWORD nNumberOfBytesToRead;
  DWORD *pDVar1;
  int iVar2;
  HGLOBAL hMem;
  LPVOID pvVar3;
  uint uVar4;
  DWORD DStack_1c;
  undefined4 uStack_18;
  
  if (*(int **)((int)param_1 + 0x40) != (int *)0x0) {
    DStack_1c = *(int *)((int)param_1 + 0x44) + 0x4e8;
    uStack_18 = 0;
    (**(code **)(**(int **)((int)param_1 + 0x40) + 0x30))();
    (**(code **)(**(int **)((int)param_1 + 0x40) + 0x3c))(*(undefined4 *)((int)param_1 + 0x4c),0x4c)
    ;
    (**(code **)(**(int **)((int)param_1 + 0x40) + 0x30))(*(int *)((int)param_1 + 0x44) + 0x4e8,0);
    nNumberOfBytesToRead = **(DWORD **)((int)param_1 + 0x4c);
    iVar2 = ReadFile(*(HANDLE *)(*(int *)((int)param_1 + 0x40) + 4),*(DWORD **)((int)param_1 + 0x4c)
                     ,nNumberOfBytesToRead,&DStack_1c,(LPOVERLAPPED)0x0);
    while( true ) {
      if (iVar2 == 0) {
        return 0;
      }
      pDVar1 = *(DWORD **)((int)param_1 + 0x4c);
      if (DStack_1c != *pDVar1) {
        return 0;
      }
      *pDVar1 = pDVar1[2];
      pDVar1 = *(DWORD **)((int)param_1 + 0x4c);
      if ((pDVar1[1] == 0xff000001) || (pDVar1[2] == 0)) break;
      iVar2 = ReadFile(*(HANDLE *)(*(int *)((int)param_1 + 0x40) + 4),pDVar1,*pDVar1,&DStack_1c,
                       (LPOVERLAPPED)0x0);
    }
    if (*(int *)(*(int *)((int)param_1 + 0x4c) + 4) == -0xffffff) {
      (**(code **)(**(int **)((int)param_1 + 0x40) + 0x30))(*(int *)((int)param_1 + 0x44) + 0x4e8,0)
      ;
      **(DWORD **)((int)param_1 + 0x4c) = nNumberOfBytesToRead;
      if ((*(int *)(*(int *)((int)param_1 + 0x50) + 0x90) != 0) &&
         (iVar2 = TVH_OpenAudioDevice(param_1,*(int *)((int)param_1 + 0x50)), iVar2 == 0)) {
        AfxMessageBox(s_Can_t_open_audio_device_10014614,0,0);
        return 0;
      }
      if (*(int *)((int)param_1 + 0x74) == 0) {
        iVar2 = *(int *)((int)param_1 + 0x50);
        if (*(int *)(iVar2 + 0x6c) != 0) {
          hMem = GlobalAlloc(0x42,(*(int *)(iVar2 + 0x80) * 8 + 0x1fU >> 3 & 0x1ffffffc) *
                                  *(int *)(iVar2 + 0x84));
          pvVar3 = GlobalLock(hMem);
          *(LPVOID *)((int)param_1 + 0x48) = pvVar3;
          if (pvVar3 == (LPVOID)0x0) {
            return 0;
          }
          iVar2 = *(int *)((int)param_1 + 0x4c);
          FUN_1000b8a0(*(int *)(*(int *)((int)param_1 + 0x50) + 0x80),
                       *(int *)(*(int *)((int)param_1 + 0x50) + 0x84),(int)pvVar3,
                       *(int *)(iVar2 + 0x14),*(int *)(iVar2 + 0x18),(ushort *)(iVar2 + 0x48),
                       *(int *)(iVar2 + 0x1c) - *(int *)(iVar2 + 0x14),
                       *(int *)(iVar2 + 0x20) - *(int *)(iVar2 + 0x18),1000);
        }
      }
      else {
        iVar2 = *(int *)((int)param_1 + 0x4c);
        if (*(int *)(*(int *)((int)param_1 + 0x50) + 0x70) == 0) {
          uVar4 = 1000;
        }
        else {
          uVar4 = *(uint *)(*(int *)((int)param_1 + 0x50) + 0x74);
        }
        FUN_1000b8a0(*(int *)((int)param_1 + 0x6c) - *(int *)((int)param_1 + 100),
                     *(int *)((int)param_1 + 0x70) - *(int *)((int)param_1 + 0x68),
                     *(int *)((int)param_1 + 0x74),*(int *)(iVar2 + 0x14),*(int *)(iVar2 + 0x18),
                     (ushort *)(iVar2 + 0x48),*(int *)(iVar2 + 0x1c) - *(int *)(iVar2 + 0x14),
                     *(int *)(iVar2 + 0x20) - *(int *)(iVar2 + 0x18),uVar4);
        if (*(int *)((int)param_1 + 0x148) != 0) {
          FUN_10003250(*(int *)((int)param_1 + 0x148) + 0x94);
        }
      }
      *(undefined4 *)
       (*(int *)(*(int *)((int)param_1 + 0x144) + 0x1d4 + *(int *)((int)param_1 + 0x84) * 4) + 0x4f0
       ) = 0;
      return 1;
    }
  }
  return 0;
}


// ============================================================================
// Function: TVH_StartEntryPlayback at 1000a3d0
// Signature: undefined4 __thiscall TVH_StartEntryPlayback(void * this, undefined4 param_1)
// Entry: 1000a3d0
// Body: 1000a3d0 - 1000a417
// ============================================================================
undefined4 __thiscall TVH_StartEntryPlayback(void *this,undefined4 param_1)

{
  int *piVar1;
  DWORD DVar2;
  
  if (*(int *)((int)this + 0x40) == 0) {
    return 0;
  }
  DVar2 = GetTickCount();
  *(DWORD *)((int)this + 0x78) = DVar2;
  *(undefined4 *)((int)this + 0x7c) = 0;
  if ((*(int *)((int)this + 0x8c) != 0) &&
     (piVar1 = *(int **)((int)this + 0x90), piVar1 != (int *)0x0)) {
    (**(code **)(*piVar1 + 0x3c))(piVar1,param_1);
  }
  return 1;
}


// ============================================================================
// Function: TVH_NotifyPlaybackEnd at 1000a420
// Signature: undefined __thiscall TVH_NotifyPlaybackEnd(void * this, undefined4 param_1)
// Entry: 1000a420
// Body: 1000a420 - 1000a459
// ============================================================================
void __thiscall TVH_NotifyPlaybackEnd(void *this,undefined4 param_1)

{
  if (*(int *)((int)this + 0x148) != 0) {
    FUN_10003510(*(int *)((int)this + 0x148));
    return;
  }
  TVH_HideVideoRect(*(int *)((int)this + 0x144));
  (**(code **)(*(int *)(*(int *)((int)this + 0x144) + 8) + 0x1c))
            (*(int *)((int)this + 0x144) + 8,param_1);
  return;
}

// clang-format on
