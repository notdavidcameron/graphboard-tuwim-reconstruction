/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: AudioHolderCriticalSection_Destroy @ 10003840
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: AudioHolderCriticalSection_Destroy at 10003840
// Signature: undefined __fastcall AudioHolderCriticalSection_Destroy(int param_1)
// Entry: 10003840
// Body: 10003840 - 1000384a
// ============================================================================
void __fastcall AudioHolderCriticalSection_Destroy(int param_1)

{
  DeleteCriticalSection((LPCRITICAL_SECTION)(param_1 + 4));
  return;
}

// clang-format on
