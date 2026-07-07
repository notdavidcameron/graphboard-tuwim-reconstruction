/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_100064d0 @ 100064d0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100064d0 at 100064d0
// Signature: CButton * __thiscall FUN_100064d0(void * this, byte param_1)
// Entry: 100064d0
// Body: 100064d0 - 100064ed
// ============================================================================
CButton * __thiscall FUN_100064d0(void *this,byte param_1)

{
  CButton::~CButton(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
