/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10005b50 @ 10005b50
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

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

// clang-format on
