/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10004e30 @ 10004e30
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10004e30 at 10004e30
// Signature: CPropertyPage * __thiscall FUN_10004e30(void * this, byte param_1)
// Entry: 10004e30
// Body: 10004e30 - 10004e4d
// ============================================================================
CPropertyPage * __thiscall FUN_10004e30(void *this,byte param_1)

{
  FUN_10004e50(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
