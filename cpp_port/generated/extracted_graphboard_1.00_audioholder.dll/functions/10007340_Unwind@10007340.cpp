/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: Unwind@10007340 @ 10007340
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Unwind@10007340 at 10007340
// Signature: undefined __stdcall Unwind@10007340(void)
// Entry: 10007340
// Body: 10007340 - 1000734a
// ============================================================================
void Unwind_10007340(void)

{
  int unaff_EBP;
  
  AudioHolderCriticalSection_Destroy(*(int *)(unaff_EBP + -0x14) + 0x10);
  return;
}

// clang-format on
