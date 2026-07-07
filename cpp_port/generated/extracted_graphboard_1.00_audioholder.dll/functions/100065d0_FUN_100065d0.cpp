/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_100065d0 @ 100065d0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100065d0 at 100065d0
// Signature: CWnd * __thiscall FUN_100065d0(void * this, byte param_1)
// Entry: 100065d0
// Body: 100065d0 - 100065ed
// ============================================================================
CWnd * __thiscall FUN_100065d0(void *this,byte param_1)

{
  FUN_10006670(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
