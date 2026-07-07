/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10001440 @ 10001440
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001440 at 10001440
// Signature: CPropertySheet * __thiscall FUN_10001440(void * this, byte param_1)
// Entry: 10001440
// Body: 10001440 - 1000145d
// ============================================================================
CPropertySheet * __thiscall FUN_10001440(void *this,byte param_1)

{
  FUN_10001460(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
