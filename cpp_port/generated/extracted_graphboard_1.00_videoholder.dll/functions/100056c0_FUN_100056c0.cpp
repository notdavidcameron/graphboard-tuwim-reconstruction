/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_100056c0 @ 100056c0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100056c0 at 100056c0
// Signature: undefined __fastcall FUN_100056c0(CWnd * param_1)
// Entry: 100056c0
// Body: 100056c0 - 1000573d
// ============================================================================
void __fastcall FUN_100056c0(CWnd *param_1)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  
  CWnd::UpdateData(param_1,1);
  iVar3 = *(int *)(param_1 + 0x94);
  uVar1 = *(undefined4 *)(*(int *)(param_1 + 0x90) + 0x50 + iVar3 * 4);
  if (0 < iVar3) {
    iVar2 = iVar3 * 4 + 0x50;
    do {
      iVar3 = iVar3 + -1;
      *(undefined4 *)(iVar2 + *(int *)(param_1 + 0x90)) =
           *(undefined4 *)(iVar2 + -4 + *(int *)(param_1 + 0x90));
      iVar2 = iVar2 + -4;
    } while (iVar3 != 0);
  }
  *(undefined4 *)(*(int *)(param_1 + 0x90) + 0x50) = uVar1;
  *(undefined4 *)(param_1 + 0x94) = 0;
  CWnd::UpdateData(param_1,0);
  FUN_10005d90((int)param_1);
  SendMessageA(*(HWND *)(param_1 + 700),0x186,*(WPARAM *)(param_1 + 0x94),0);
  return;
}

// clang-format on
