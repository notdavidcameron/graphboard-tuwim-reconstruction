/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10005f30 @ 10005f30
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10005f30 at 10005f30
// Signature: undefined __fastcall FUN_10005f30(CWnd * param_1)
// Entry: 10005f30
// Body: 10005f30 - 10005f54
// ============================================================================
void __fastcall FUN_10005f30(CWnd *param_1)

{
  CWnd::UpdateData(param_1,1);
  AudioHolder_StartPlaybackForEntry(*(void **)(param_1 + 0x90),*(int *)(param_1 + 0x94));
  FUN_10005440((int)param_1);
  return;
}

// clang-format on
