/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_100015a0 @ 100015a0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100015a0 at 100015a0
// Signature: CWinApp * __thiscall FUN_100015a0(void * this, byte param_1)
// Entry: 100015a0
// Body: 100015a0 - 100015bd
// ============================================================================
CWinApp * __thiscall FUN_100015a0(void *this,byte param_1)

{
  CWinApp::~CWinApp(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
