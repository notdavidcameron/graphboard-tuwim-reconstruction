/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: Unwind@100076b8 @ 100076b8
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Unwind@100076b8 at 100076b8
// Signature: undefined __stdcall Unwind@100076b8(void)
// Entry: 100076b8
// Body: 100076b8 - 100076d4
// ============================================================================
void Unwind_100076b8(void)

{
  int unaff_EBP;
  
  if ((*(uint *)(unaff_EBP + -0x224) & 1) != 0) {
    CString::~CString((CString *)(unaff_EBP + -0x220));
    return;
  }
  return;
}

// clang-format on
