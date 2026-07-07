/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10005ec0 @ 10005ec0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10005ec0 at 10005ec0
// Signature: CButton * __thiscall FUN_10005ec0(void * this, byte param_1)
// Entry: 10005ec0
// Body: 10005ec0 - 10005edd
// ============================================================================
CButton * __thiscall FUN_10005ec0(void *this,byte param_1)

{
  CButton::~CButton(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
