/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: VideoHolder_MciWindowSubclassProc @ 100040d0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: VideoHolder_MciWindowSubclassProc at 100040d0
// Signature: undefined __stdcall VideoHolder_MciWindowSubclassProc(HWND param_1, UINT param_2, WPARAM param_3, LPARAM param_4)
// Entry: 100040d0
// Body: 100040d0 - 10004117
// ============================================================================
void VideoHolder_MciWindowSubclassProc(HWND param_1,UINT param_2,WPARAM param_3,LPARAM param_4)

{
  if ((param_2 == 0x201) || (param_2 == 0x204)) {
    (**(code **)(*(int *)(DAT_1000a498 + 8) + 0x20))((int *)(DAT_1000a498 + 8),DAT_1000a4a0);
  }
  CallWindowProcA(DAT_1000a49c,param_1,param_2,param_3,param_4);
  return;
}

// clang-format on
