/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_100052f4 @ 100052f4
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100052f4 at 100052f4
// Signature: AFX_MODULE_STATE * __thiscall FUN_100052f4(void * this, byte param_1)
// Entry: 100052f4
// Body: 100052f4 - 1000530e
// ============================================================================
AFX_MODULE_STATE * __thiscall FUN_100052f4(void *this,byte param_1)

{
  AFX_MODULE_STATE::~AFX_MODULE_STATE(this);
  if ((param_1 & 1) != 0) {
    CNoTrackObject::operator_delete(this);
  }
  return this;
}

// clang-format on
