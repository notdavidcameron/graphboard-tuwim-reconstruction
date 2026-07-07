/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: AudioHolderRefCountedState_Construct @ 10001e60
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: AudioHolderRefCountedState_Construct at 10001e60
// Signature: undefined4 * __fastcall AudioHolderRefCountedState_Construct(undefined4 * param_1)
// Entry: 10001e60
// Body: 10001e60 - 10001e76
// ============================================================================
undefined4 * __fastcall AudioHolderRefCountedState_Construct(undefined4 *param_1)

{
  *param_1 = 0;
  InitializeCriticalSection((LPCRITICAL_SECTION)(param_1 + 1));
  return param_1;
}

// clang-format on
