/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10005f60 @ 10005f60
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10005f60 at 10005f60
// Signature: undefined __fastcall FUN_10005f60(int param_1)
// Entry: 10005f60
// Body: 10005f60 - 10005f7d
// ============================================================================
void __fastcall FUN_10005f60(int param_1)

{
  AudioHolder_StopPlaybackForEntry(*(void **)(param_1 + 0x90),*(int *)(param_1 + 0x94));
  FUN_10005440(param_1);
  return;
}

// clang-format on
