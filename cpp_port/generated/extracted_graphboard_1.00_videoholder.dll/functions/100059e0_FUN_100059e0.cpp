/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_100059e0 @ 100059e0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

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

// clang-format on
