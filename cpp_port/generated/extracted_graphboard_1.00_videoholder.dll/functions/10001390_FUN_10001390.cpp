/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10001390 @ 10001390
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001390 at 10001390
// Signature: CWinApp * __thiscall FUN_10001390(void * this, byte param_1)
// Entry: 10001390
// Body: 10001390 - 100013ad
// ============================================================================
CWinApp * __thiscall FUN_10001390(void *this,byte param_1)

{
  CWinApp::~CWinApp(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
