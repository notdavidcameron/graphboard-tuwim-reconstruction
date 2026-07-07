/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10001564 @ 10001564
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001564 at 10001564
// Signature: int __stdcall FUN_10001564(void)
// Entry: 10001564
// Body: 10001564 - 100015da
// ============================================================================
int FUN_10001564(void)

{
  int iVar1;
  int unaff_EBP;
  undefined4 *unaff_ESI;
  
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  if (unaff_ESI != (undefined4 *)0x0) {
    unaff_ESI[8] = *(undefined4 *)(unaff_EBP + 8);
    iVar1 = (**(code **)*unaff_ESI)();
    if (iVar1 != 0) {
      *unaff_ESI = &PTR_AudioHolder_QueryInterfaceFromMap_10008654;
      unaff_ESI[1] = 1;
      DeleteCriticalSection((LPCRITICAL_SECTION)(unaff_ESI + 9));
      DeleteCriticalSection((LPCRITICAL_SECTION)(unaff_ESI + 2));
      operator_delete(unaff_ESI);
    }
    ExceptionList = *(void **)(unaff_EBP + -0xc);
    return iVar1;
  }
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  return *(int *)(unaff_EBP + -0x18);
}

// clang-format on
