/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10001040 @ 10001040
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001040 at 10001040
// Signature: CWnd * __thiscall FUN_10001040(void * this, byte param_1)
// Entry: 10001040
// Body: 10001040 - 1000105d
// ============================================================================
CWnd * __thiscall FUN_10001040(void *this,byte param_1)

{
  FUN_10001060(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
