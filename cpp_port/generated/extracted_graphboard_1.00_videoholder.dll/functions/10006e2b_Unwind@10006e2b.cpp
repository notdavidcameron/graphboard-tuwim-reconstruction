/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: Unwind@10006e2b @ 10006e2b
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Unwind@10006e2b at 10006e2b
// Signature: undefined __stdcall Unwind@10006e2b(void)
// Entry: 10006e2b
// Body: 10006e2b - 10006e47
// ============================================================================
void Unwind_10006e2b(void)

{
  int unaff_EBP;
  
  if ((*(uint *)(unaff_EBP + -0x210) & 1) != 0) {
    CString::~CString((CString *)(unaff_EBP + -0x20c));
    return;
  }
  return;
}

// clang-format on
