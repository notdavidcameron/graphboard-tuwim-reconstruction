/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: VideoHolderClassFactory_AddRef @ 100018c0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: VideoHolderClassFactory_AddRef at 100018c0
// Signature: LONG __stdcall VideoHolderClassFactory_AddRef(int param_1)
// Entry: 100018c0
// Body: 100018c0 - 100018fd
// ============================================================================
LONG VideoHolderClassFactory_AddRef(int param_1)

{
  LONG LVar1;
  
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x24));
  LVar1 = InterlockedIncrement((LONG *)(param_1 + 4));
  if (*(LONG *)(param_1 + 4) == 2) {
    InterlockedIncrement(&DAT_1000a41c);
  }
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x24));
  return LVar1;
}

// clang-format on
