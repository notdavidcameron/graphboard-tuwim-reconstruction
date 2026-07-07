/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10006100 @ 10006100
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

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

// clang-format on
