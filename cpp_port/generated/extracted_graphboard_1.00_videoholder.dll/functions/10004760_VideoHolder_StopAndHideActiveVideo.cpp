/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: VideoHolder_StopAndHideActiveVideo @ 10004760
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: VideoHolder_StopAndHideActiveVideo at 10004760
// Signature: undefined4 __stdcall VideoHolder_StopAndHideActiveVideo(int param_1)
// Entry: 10004760
// Body: 10004760 - 100047ea
// ============================================================================
undefined4 VideoHolder_StopAndHideActiveVideo(int param_1)

{
  if ((*(HWND *)(param_1 + 0x44) != (HWND)0x0) && (*(int *)(param_1 + 0x138) != 0)) {
    if (*(int *)(*(int *)(param_1 + 0x4c + *(int *)(param_1 + 0x118) * 4) + 0xc) == 0) {
      ShowWindow(*(HWND *)(param_1 + 0x44),0);
    }
    SendMessageA(*(HWND *)(param_1 + 0x44),0x808,0,0);
    if (*(int *)(param_1 + 0x140) != 0) {
      (**(code **)(**(int **)(param_1 + 0x28) + 0x2c))(*(int **)(param_1 + 0x28));
    }
    *(undefined4 *)(param_1 + 0x140) = 0;
    (**(code **)(**(int **)(param_1 + 0x28) + 0xc))(*(int **)(param_1 + 0x28),param_1 + 0x11c,1);
    *(undefined4 *)(param_1 + 0x13c) = 0;
    return 0;
  }
  return 1;
}

// clang-format on
