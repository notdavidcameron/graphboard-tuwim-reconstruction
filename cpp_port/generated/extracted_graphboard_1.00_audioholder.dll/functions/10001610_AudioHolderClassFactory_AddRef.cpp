/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: AudioHolderClassFactory_AddRef @ 10001610
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: AudioHolderClassFactory_AddRef at 10001610
// Signature: LONG __stdcall AudioHolderClassFactory_AddRef(int param_1)
// Entry: 10001610
// Body: 10001610 - 1000164d
// ============================================================================
LONG AudioHolderClassFactory_AddRef(int param_1)

{
  LONG LVar1;
  
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x24));
  LVar1 = InterlockedIncrement((LONG *)(param_1 + 4));
  if (*(LONG *)(param_1 + 4) == 2) {
    InterlockedIncrement(&DAT_1000b47c);
  }
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x24));
  return LVar1;
}

// clang-format on
