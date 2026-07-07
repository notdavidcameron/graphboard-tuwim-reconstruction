/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: SetColorClassFactory_AddRef @ 100022a0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: SetColorClassFactory_AddRef at 100022a0
// Signature: LONG __stdcall SetColorClassFactory_AddRef(int param_1)
// Entry: 100022a0
// Body: 100022a0 - 100022dd
// ============================================================================
LONG SetColorClassFactory_AddRef(int param_1)

{
  LONG LVar1;
  
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x24));
  LVar1 = InterlockedIncrement((LONG *)(param_1 + 4));
  if (*(LONG *)(param_1 + 4) == 2) {
    InterlockedIncrement(&DAT_100082ac);
  }
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x24));
  return LVar1;
}

// clang-format on
