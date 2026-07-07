/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: VideoHolderRefCountedState_Construct @ 10002110
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: VideoHolderRefCountedState_Construct at 10002110
// Signature: undefined4 * __fastcall VideoHolderRefCountedState_Construct(undefined4 * param_1)
// Entry: 10002110
// Body: 10002110 - 10002126
// ============================================================================
undefined4 * __fastcall VideoHolderRefCountedState_Construct(undefined4 *param_1)

{
  *param_1 = 0;
  InitializeCriticalSection((LPCRITICAL_SECTION)(param_1 + 1));
  return param_1;
}

// clang-format on
