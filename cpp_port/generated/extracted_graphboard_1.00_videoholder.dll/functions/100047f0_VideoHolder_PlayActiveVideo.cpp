/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: VideoHolder_PlayActiveVideo @ 100047f0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: VideoHolder_PlayActiveVideo at 100047f0
// Signature: undefined __fastcall VideoHolder_PlayActiveVideo(int param_1)
// Entry: 100047f0
// Body: 100047f0 - 100048c4
// ============================================================================
void __fastcall VideoHolder_PlayActiveVideo(int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x11c);
  if (((-1 < iVar1) && (*(int *)(param_1 + 0x44) != 0)) && (*(int *)(param_1 + 0x13c) != 0)) {
    if (*(int *)(*(int *)(param_1 + 0x50 + iVar1 * 4) + 8) != 0) {
      if (*(int *)(param_1 + 0x144) == 0) {
        (**(code **)(**(int **)(param_1 + 0x2c) + 0x30))(*(int **)(param_1 + 0x2c));
      }
      *(undefined4 *)(param_1 + 0x144) = 1;
      if ((-1 < *(int *)(param_1 + 0x130)) && (-1 < *(int *)(param_1 + 0x134))) {
        SendMessageA(*(HWND *)(param_1 + 0x48),0x807,0,*(int *)(param_1 + 0x130));
        SendMessageA(*(HWND *)(param_1 + 0x48),0x47b,0,*(LPARAM *)(param_1 + 0x134));
        return;
      }
      SendMessageA(*(HWND *)(param_1 + 0x48),0x807,0,0);
      SendMessageA(*(HWND *)(param_1 + 0x48),0x806,0,0);
      return;
    }
    (**(code **)(*(int *)(param_1 + 8) + 0x1c))(param_1 + 8,iVar1);
    (**(code **)(*(int *)(param_1 + 4) + 0x24))(param_1 + 4);
  }
  return;
}

// clang-format on
