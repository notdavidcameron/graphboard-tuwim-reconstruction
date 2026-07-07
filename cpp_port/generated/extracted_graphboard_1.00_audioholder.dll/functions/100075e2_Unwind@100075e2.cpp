/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: Unwind@100075e2 @ 100075e2
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Unwind@100075e2 at 100075e2
// Signature: undefined __stdcall Unwind@100075e2(void)
// Entry: 100075e2
// Body: 100075e2 - 100075fe
// ============================================================================
void Unwind_100075e2(void)

{
  int unaff_EBP;
  
  if ((*(uint *)(unaff_EBP + -0x224) & 1) != 0) {
    CString::~CString((CString *)(unaff_EBP + -0x220));
    return;
  }
  return;
}

// clang-format on
