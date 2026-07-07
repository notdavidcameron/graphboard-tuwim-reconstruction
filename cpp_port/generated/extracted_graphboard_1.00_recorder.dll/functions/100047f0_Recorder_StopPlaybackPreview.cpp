/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: Recorder_StopPlaybackPreview @ 100047f0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Recorder_StopPlaybackPreview at 100047f0
// Signature: undefined __fastcall Recorder_StopPlaybackPreview(int param_1)
// Entry: 100047f0
// Body: 100047f0 - 10004846
// ============================================================================
void __fastcall Recorder_StopPlaybackPreview(int param_1)

{
  int *piVar1;
  int iVar2;
  undefined4 uVar3;
  
  if ((*(int *)(param_1 + 0x5c) != 0) && (piVar1 = *(int **)(param_1 + 0x60), piVar1 != (int *)0x0))
  {
    (**(code **)(*piVar1 + 0x48))(piVar1);
    iVar2 = (**(code **)(**(int **)(param_1 + 0x60) + 8))(*(int **)(param_1 + 0x60));
    while (iVar2 != 0) {
      iVar2 = (**(code **)(**(int **)(param_1 + 0x60) + 8))(*(int **)(param_1 + 0x60));
    }
    if (*(UINT_PTR *)(param_1 + 0x58) != 0) {
      uVar3 = FUN_10001180(*(void **)(param_1 + 0x38),*(UINT_PTR *)(param_1 + 0x58));
      *(undefined4 *)(param_1 + 0x58) = uVar3;
    }
    *(undefined4 *)(param_1 + 0x60) = 0;
    (**(code **)(*(int *)(param_1 + 8) + 0x28))(param_1 + 8);
  }
  return;
}

// clang-format on
