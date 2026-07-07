/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10005ee0 @ 10005ee0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10005ee0 at 10005ee0
// Signature: CListBox * __thiscall FUN_10005ee0(void * this, byte param_1)
// Entry: 10005ee0
// Body: 10005ee0 - 10005efd
// ============================================================================
CListBox * __thiscall FUN_10005ee0(void *this,byte param_1)

{
  CListBox::~CListBox(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
