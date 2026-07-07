/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10006be0 @ 10006be0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10006be0 at 10006be0
// Signature: AFX_MODULE_STATE * __thiscall FUN_10006be0(void * this, byte param_1)
// Entry: 10006be0
// Body: 10006be0 - 10006bfa
// ============================================================================
AFX_MODULE_STATE * __thiscall FUN_10006be0(void *this,byte param_1)

{
  AFX_MODULE_STATE::~AFX_MODULE_STATE(this);
  if ((param_1 & 1) != 0) {
    CNoTrackObject::operator_delete(this);
  }
  return this;
}

// clang-format on
