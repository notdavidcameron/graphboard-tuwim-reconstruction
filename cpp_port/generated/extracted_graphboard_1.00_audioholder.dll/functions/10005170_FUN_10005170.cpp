/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10005170 @ 10005170
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10005170 at 10005170
// Signature: CPropertyPage * __thiscall FUN_10005170(void * this, byte param_1)
// Entry: 10005170
// Body: 10005170 - 1000518d
// ============================================================================
CPropertyPage * __thiscall FUN_10005170(void *this,byte param_1)

{
  FUN_10005190(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
