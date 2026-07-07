/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_10001c70 @ 10001c70
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001c70 at 10001c70
// Signature: CStatic * __thiscall FUN_10001c70(void * this, byte param_1)
// Entry: 10001c70
// Body: 10001c70 - 10001c8d
// ============================================================================
CStatic * __thiscall FUN_10001c70(void *this,byte param_1)

{
  CStatic::~CStatic(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
