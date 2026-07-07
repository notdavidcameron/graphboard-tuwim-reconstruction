/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_10004960 @ 10004960
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10004960 at 10004960
// Signature: AFX_MODULE_STATE * __thiscall FUN_10004960(void * this, byte param_1)
// Entry: 10004960
// Body: 10004960 - 1000497a
// ============================================================================
AFX_MODULE_STATE * __thiscall FUN_10004960(void *this,byte param_1)

{
  AFX_MODULE_STATE::~AFX_MODULE_STATE(this);
  if ((param_1 & 1) != 0) {
    CNoTrackObject::operator_delete(this);
  }
  return this;
}

// clang-format on
