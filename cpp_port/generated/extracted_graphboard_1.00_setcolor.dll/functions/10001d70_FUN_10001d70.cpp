/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_10001d70 @ 10001d70
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001d70 at 10001d70
// Signature: CWinApp * __thiscall FUN_10001d70(void * this, byte param_1)
// Entry: 10001d70
// Body: 10001d70 - 10001d8d
// ============================================================================
CWinApp * __thiscall FUN_10001d70(void *this,byte param_1)

{
  CWinApp::~CWinApp(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
