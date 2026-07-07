/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_10001c90 @ 10001c90
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001c90 at 10001c90
// Signature: CButton * __thiscall FUN_10001c90(void * this, byte param_1)
// Entry: 10001c90
// Body: 10001c90 - 10001cad
// ============================================================================
CButton * __thiscall FUN_10001c90(void *this,byte param_1)

{
  CButton::~CButton(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
