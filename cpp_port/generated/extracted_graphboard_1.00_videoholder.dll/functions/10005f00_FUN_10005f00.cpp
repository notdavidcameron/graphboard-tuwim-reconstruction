/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10005f00 @ 10005f00
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10005f00 at 10005f00
// Signature: CEdit * __thiscall FUN_10005f00(void * this, byte param_1)
// Entry: 10005f00
// Body: 10005f00 - 10005f1d
// ============================================================================
CEdit * __thiscall FUN_10005f00(void *this,byte param_1)

{
  CEdit::~CEdit(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
