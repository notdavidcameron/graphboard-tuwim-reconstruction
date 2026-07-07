/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: VideoHolder_CloseAllVideos @ 10003da0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: VideoHolder_CloseAllVideos at 10003da0
// Signature: undefined4 __stdcall VideoHolder_CloseAllVideos(int param_1)
// Entry: 10003da0
// Body: 10003da0 - 10003e16
// ============================================================================
undefined4 VideoHolder_CloseAllVideos(int param_1)

{
  int *piVar1;
  int iVar2;
  
  piVar1 = (int *)(param_1 + 4);
  *(undefined4 *)(param_1 + 0x140) = 0;
  (**(code **)(*(int *)(param_1 + 4) + 0x24))(piVar1);
  if (*(int *)(param_1 + 0x13c) != 0) {
    SendMessageA(*(HWND *)(param_1 + 0x48),0x804,0,0);
  }
  if (*(int *)(param_1 + 0x144) != 0) {
    (**(code **)(**(int **)(param_1 + 0x2c) + 0x2c))(*(int **)(param_1 + 0x2c));
  }
  iVar2 = 0;
  *(undefined4 *)(param_1 + 0x144) = 0;
  if (0 < *(int *)(param_1 + 0x118)) {
    do {
      (**(code **)(*piVar1 + 0x38))(piVar1,iVar2);
      iVar2 = iVar2 + 1;
    } while (iVar2 < *(int *)(param_1 + 0x118));
  }
  *(undefined4 *)(param_1 + 0x13c) = 0;
  return 0;
}

// clang-format on
