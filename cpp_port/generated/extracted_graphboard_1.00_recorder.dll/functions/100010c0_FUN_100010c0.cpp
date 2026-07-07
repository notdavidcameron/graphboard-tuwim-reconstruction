/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_100010c0 @ 100010c0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100010c0 at 100010c0
// Signature: CWnd * __thiscall FUN_100010c0(void * this, byte param_1)
// Entry: 100010c0
// Body: 100010c0 - 100010dd
// ============================================================================
CWnd * __thiscall FUN_100010c0(void *this,byte param_1)

{
  FUN_100010e0(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
