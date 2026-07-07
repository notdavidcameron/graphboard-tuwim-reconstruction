/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10005839 @ 10005839
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10005839 at 10005839
// Signature: type_info * __thiscall FUN_10005839(void * this, byte param_1)
// Entry: 10005839
// Body: 10005839 - 10005854
// ============================================================================
type_info * __thiscall FUN_10005839(void *this,byte param_1)

{
  type_info::~type_info(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
