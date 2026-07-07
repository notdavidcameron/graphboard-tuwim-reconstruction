/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10005ea0 @ 10005ea0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10005ea0 at 10005ea0
// Signature: CStatic * __thiscall FUN_10005ea0(void * this, byte param_1)
// Entry: 10005ea0
// Body: 10005ea0 - 10005ebd
// ============================================================================
CStatic * __thiscall FUN_10005ea0(void *this,byte param_1)

{
  CStatic::~CStatic(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
