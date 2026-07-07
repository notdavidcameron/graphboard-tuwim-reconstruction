/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10004fc0 @ 10004fc0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10004fc0 at 10004fc0
// Signature: CComboBox * __thiscall FUN_10004fc0(void * this, byte param_1)
// Entry: 10004fc0
// Body: 10004fc0 - 10004fdd
// ============================================================================
CComboBox * __thiscall FUN_10004fc0(void *this,byte param_1)

{
  CComboBox::~CComboBox(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
