/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: VideoHolderCallback_ClearPlaybackState @ 10004e50
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: VideoHolderCallback_ClearPlaybackState at 10004e50
// Signature: undefined4 __stdcall VideoHolderCallback_ClearPlaybackState(int param_1, undefined4 param_2)
// Entry: 10004e50
// Body: 10004e50 - 10004e85
// ============================================================================
undefined4 VideoHolderCallback_ClearPlaybackState(int param_1,undefined4 param_2)

{
  int *piVar1;
  
  piVar1 = *(int **)(param_1 + 0x28);
  if ((piVar1 != (int *)0x0) && (*(int *)(param_1 + 0x138) != 0)) {
    (**(code **)(*piVar1 + 0x1c))(piVar1,param_2);
    *(undefined4 *)(param_1 + 0x138) = 0;
  }
  return 0;
}

// clang-format on
