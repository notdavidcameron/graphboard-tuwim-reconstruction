/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10004ab0 @ 10004ab0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10004ab0 at 10004ab0
// Signature: undefined4 __stdcall FUN_10004ab0(int param_1)
// Entry: 10004ab0
// Body: 10004ab0 - 10004ad8
// ============================================================================
undefined4 FUN_10004ab0(int param_1)

{
  int iVar1;
  
  iVar1 = Recorder_StopCapture(param_1 + -4);
  if (iVar1 == 0) {
    Recorder_StopPlaybackPreview(param_1 + -4);
    return 0;
  }
  return 1;
}

// clang-format on
