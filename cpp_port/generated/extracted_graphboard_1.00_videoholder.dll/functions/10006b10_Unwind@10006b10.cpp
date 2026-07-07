/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: Unwind@10006b10 @ 10006b10
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Unwind@10006b10 at 10006b10
// Signature: undefined __stdcall Unwind@10006b10(void)
// Entry: 10006b10
// Body: 10006b10 - 10006b36
// ============================================================================
void Unwind_10006b10(void)

{
  int unaff_EBP;
  
  if (*(int *)(unaff_EBP + -0x10) == 0) {
    *(undefined4 *)(unaff_EBP + -0x1c) = 0;
  }
  else {
    *(int *)(unaff_EBP + -0x1c) = *(int *)(unaff_EBP + -0x10) + 0x10;
  }
  VideoHolderCriticalSection_Destroy(*(int *)(unaff_EBP + -0x1c));
  return;
}

// clang-format on
