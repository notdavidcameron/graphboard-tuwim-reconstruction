/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_10004ead @ 10004ead
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10004ead at 10004ead
// Signature: type_info * __thiscall FUN_10004ead(void * this, byte param_1)
// Entry: 10004ead
// Body: 10004ead - 10004ec8
// ============================================================================
type_info * __thiscall FUN_10004ead(void *this,byte param_1)

{
  type_info::~type_info(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
