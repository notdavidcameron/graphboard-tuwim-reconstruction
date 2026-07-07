/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10005aa0 @ 10005aa0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

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

// clang-format on
