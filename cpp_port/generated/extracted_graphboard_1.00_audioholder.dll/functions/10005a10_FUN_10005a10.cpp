/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10005a10 @ 10005a10
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

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

// clang-format on
