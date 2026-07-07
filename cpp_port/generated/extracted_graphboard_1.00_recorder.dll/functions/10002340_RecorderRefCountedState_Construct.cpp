/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: RecorderRefCountedState_Construct @ 10002340
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: RecorderRefCountedState_Construct at 10002340
// Signature: undefined4 * __fastcall RecorderRefCountedState_Construct(undefined4 * param_1)
// Entry: 10002340
// Body: 10002340 - 10002356
// ============================================================================
undefined4 * __fastcall RecorderRefCountedState_Construct(undefined4 *param_1)

{
  *param_1 = 0;
  InitializeCriticalSection((LPCRITICAL_SECTION)(param_1 + 1));
  return param_1;
}

// clang-format on
