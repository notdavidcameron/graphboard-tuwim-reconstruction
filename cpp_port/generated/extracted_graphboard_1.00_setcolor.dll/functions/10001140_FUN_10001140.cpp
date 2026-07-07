/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_10001140 @ 10001140
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001140 at 10001140
// Signature: CDialog * __thiscall FUN_10001140(void * this, byte param_1)
// Entry: 10001140
// Body: 10001140 - 1000115d
// ============================================================================
CDialog * __thiscall FUN_10001140(void *this,byte param_1)

{
  FUN_10001160(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
