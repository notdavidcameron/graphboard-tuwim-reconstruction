/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: AudioHolderClassFactory_Release @ 10001650
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: AudioHolderClassFactory_Release at 10001650
// Signature: int __stdcall AudioHolderClassFactory_Release(undefined4 * param_1)
// Entry: 10001650
// Body: 10001650 - 100016ac
// ============================================================================
int AudioHolderClassFactory_Release(undefined4 *param_1)

{
  int iVar1;
  
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  InterlockedDecrement(param_1 + 1);
  iVar1 = param_1[1];
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  if (iVar1 == 0) {
    if (param_1 != (undefined4 *)0x0) {
      AudioHolderClassFactory_Destroy(param_1);
      operator_delete(param_1);
      return 0;
    }
  }
  else if (iVar1 == 1) {
    InterlockedDecrement(&DAT_1000b47c);
  }
  return iVar1;
}

// clang-format on
