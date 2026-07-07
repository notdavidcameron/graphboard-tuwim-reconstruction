/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: Unwind@10007360 @ 10007360
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Unwind@10007360 at 10007360
// Signature: undefined __stdcall Unwind@10007360(void)
// Entry: 10007360
// Body: 10007360 - 10007386
// ============================================================================
void Unwind_10007360(void)

{
  int unaff_EBP;
  
  if (*(int *)(unaff_EBP + -0x10) == 0) {
    *(undefined4 *)(unaff_EBP + -0x28) = 0;
  }
  else {
    *(int *)(unaff_EBP + -0x28) = *(int *)(unaff_EBP + -0x10) + 0x10;
  }
  AudioHolderCriticalSection_Destroy(*(int *)(unaff_EBP + -0x28));
  return;
}

// clang-format on
