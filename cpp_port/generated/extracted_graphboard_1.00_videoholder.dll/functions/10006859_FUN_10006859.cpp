/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10006859 @ 10006859
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10006859 at 10006859
// Signature: type_info * __thiscall FUN_10006859(void * this, byte param_1)
// Entry: 10006859
// Body: 10006859 - 10006874
// ============================================================================
type_info * __thiscall FUN_10006859(void *this,byte param_1)

{
  type_info::~type_info(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
