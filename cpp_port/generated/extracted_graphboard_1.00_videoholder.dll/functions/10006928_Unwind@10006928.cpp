/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: Unwind@10006928 @ 10006928
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Unwind@10006928 at 10006928
// Signature: undefined __stdcall Unwind@10006928(void)
// Entry: 10006928
// Body: 10006928 - 10006935
// ============================================================================
void Unwind_10006928(void)

{
  int unaff_EBP;
  
  VideoSettingsPage_Destroy((CPropertyPage *)(*(int *)(unaff_EBP + -0x10) + 0x8c));
  return;
}

// clang-format on
