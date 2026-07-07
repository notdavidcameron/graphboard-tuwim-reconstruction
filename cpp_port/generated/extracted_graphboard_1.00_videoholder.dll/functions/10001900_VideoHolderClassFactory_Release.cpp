/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: VideoHolderClassFactory_Release @ 10001900
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: VideoHolderClassFactory_Release at 10001900
// Signature: int __stdcall VideoHolderClassFactory_Release(undefined4 * param_1)
// Entry: 10001900
// Body: 10001900 - 1000195c
// ============================================================================
int VideoHolderClassFactory_Release(undefined4 *param_1)

{
  int iVar1;
  
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  InterlockedDecrement(param_1 + 1);
  iVar1 = param_1[1];
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  if (iVar1 == 0) {
    if (param_1 != (undefined4 *)0x0) {
      VideoHolderClassFactory_Destroy(param_1);
      operator_delete(param_1);
      return 0;
    }
  }
  else if (iVar1 == 1) {
    InterlockedDecrement(&DAT_1000a41c);
  }
  return iVar1;
}

// clang-format on
