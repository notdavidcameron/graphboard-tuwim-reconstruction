/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_1000711d @ 1000711d
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_1000711d at 1000711d
// Signature: type_info * __thiscall FUN_1000711d(void * this, byte param_1)
// Entry: 1000711d
// Body: 1000711d - 10007138
// ============================================================================
type_info * __thiscall FUN_1000711d(void *this,byte param_1)

{
  type_info::~type_info(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
