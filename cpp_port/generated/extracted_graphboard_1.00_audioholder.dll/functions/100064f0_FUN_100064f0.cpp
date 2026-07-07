/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_100064f0 @ 100064f0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100064f0 at 100064f0
// Signature: CListBox * __thiscall FUN_100064f0(void * this, byte param_1)
// Entry: 100064f0
// Body: 100064f0 - 1000650d
// ============================================================================
CListBox * __thiscall FUN_100064f0(void *this,byte param_1)

{
  CListBox::~CListBox(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
