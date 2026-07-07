/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10006318 @ 10006318
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10006318 at 10006318
// Signature: AFX_MODULE_STATE * __thiscall FUN_10006318(void * this, byte param_1)
// Entry: 10006318
// Body: 10006318 - 10006332
// ============================================================================
AFX_MODULE_STATE * __thiscall FUN_10006318(void *this,byte param_1)

{
  AFX_MODULE_STATE::~AFX_MODULE_STATE(this);
  if ((param_1 & 1) != 0) {
    CNoTrackObject::operator_delete(this);
  }
  return this;
}

// clang-format on
