/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_100064b0 @ 100064b0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100064b0 at 100064b0
// Signature: CStatic * __thiscall FUN_100064b0(void * this, byte param_1)
// Entry: 100064b0
// Body: 100064b0 - 100064cd
// ============================================================================
CStatic * __thiscall FUN_100064b0(void *this,byte param_1)

{
  CStatic::~CStatic(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
