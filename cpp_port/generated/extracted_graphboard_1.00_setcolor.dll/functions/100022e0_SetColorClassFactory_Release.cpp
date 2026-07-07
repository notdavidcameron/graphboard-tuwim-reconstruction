/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: SetColorClassFactory_Release @ 100022e0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: SetColorClassFactory_Release at 100022e0
// Signature: int __stdcall SetColorClassFactory_Release(undefined4 * param_1)
// Entry: 100022e0
// Body: 100022e0 - 1000233c
// ============================================================================
int SetColorClassFactory_Release(undefined4 *param_1)

{
  int iVar1;
  
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  InterlockedDecrement(param_1 + 1);
  iVar1 = param_1[1];
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  if (iVar1 == 0) {
    if (param_1 != (undefined4 *)0x0) {
      SetColorClassFactory_Destroy(param_1);
      operator_delete(param_1);
      return 0;
    }
  }
  else if (iVar1 == 1) {
    InterlockedDecrement(&DAT_100082ac);
  }
  return iVar1;
}

// clang-format on
