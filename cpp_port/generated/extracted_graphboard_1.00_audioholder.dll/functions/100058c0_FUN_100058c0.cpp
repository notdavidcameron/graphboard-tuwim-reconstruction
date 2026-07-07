/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_100058c0 @ 100058c0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

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

// clang-format on
