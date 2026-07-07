/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10001230 @ 10001230
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001230 at 10001230
// Signature: CPropertySheet * __thiscall FUN_10001230(void * this, byte param_1)
// Entry: 10001230
// Body: 10001230 - 1000124d
// ============================================================================
CPropertySheet * __thiscall FUN_10001230(void *this,byte param_1)

{
  FUN_10001250(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
