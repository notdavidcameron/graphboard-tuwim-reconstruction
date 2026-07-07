/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: VideoHolderCriticalSection_Destroy @ 10003ae0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: VideoHolderCriticalSection_Destroy at 10003ae0
// Signature: undefined __fastcall VideoHolderCriticalSection_Destroy(int param_1)
// Entry: 10003ae0
// Body: 10003ae0 - 10003aea
// ============================================================================
void __fastcall VideoHolderCriticalSection_Destroy(int param_1)

{
  DeleteCriticalSection((LPCRITICAL_SECTION)(param_1 + 4));
  return;
}

// clang-format on
