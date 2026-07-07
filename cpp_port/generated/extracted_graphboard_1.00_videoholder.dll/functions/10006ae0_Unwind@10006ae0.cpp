/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: Unwind@10006ae0 @ 10006ae0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Unwind@10006ae0 at 10006ae0
// Signature: undefined __stdcall Unwind@10006ae0(void)
// Entry: 10006ae0
// Body: 10006ae0 - 10006aea
// ============================================================================
void Unwind_10006ae0(void)

{
  int unaff_EBP;
  
  VideoHolderCriticalSection_Destroy(*(int *)(unaff_EBP + -0x14) + 0x10);
  return;
}

// clang-format on
