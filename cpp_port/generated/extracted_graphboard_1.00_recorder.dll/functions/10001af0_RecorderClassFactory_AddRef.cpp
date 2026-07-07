/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: RecorderClassFactory_AddRef @ 10001af0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: RecorderClassFactory_AddRef at 10001af0
// Signature: LONG __stdcall RecorderClassFactory_AddRef(int param_1)
// Entry: 10001af0
// Body: 10001af0 - 10001b2d
// ============================================================================
LONG RecorderClassFactory_AddRef(int param_1)

{
  LONG LVar1;
  
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x24));
  LVar1 = InterlockedIncrement((LONG *)(param_1 + 4));
  if (*(LONG *)(param_1 + 4) == 2) {
    InterlockedIncrement(&DAT_100083ac);
  }
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x24));
  return LVar1;
}

// clang-format on
