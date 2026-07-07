/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10006790 @ 10006790
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10006790 at 10006790
// Signature: CPropertySheet * __thiscall FUN_10006790(void * this, byte param_1)
// Entry: 10006790
// Body: 10006790 - 100067ad
// ============================================================================
CPropertySheet * __thiscall FUN_10006790(void *this,byte param_1)

{
  FUN_100067b0(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
