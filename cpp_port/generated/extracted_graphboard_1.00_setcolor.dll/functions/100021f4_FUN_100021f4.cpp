/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_100021f4 @ 100021f4
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100021f4 at 100021f4
// Signature: int __stdcall FUN_100021f4(void)
// Entry: 100021f4
// Body: 100021f4 - 1000226a
// ============================================================================
int FUN_100021f4(void)

{
  int iVar1;
  int unaff_EBP;
  undefined4 *unaff_ESI;
  
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  if (unaff_ESI != (undefined4 *)0x0) {
    unaff_ESI[8] = *(undefined4 *)(unaff_EBP + 8);
    iVar1 = (**(code **)*unaff_ESI)();
    if (iVar1 != 0) {
      *unaff_ESI = &PTR_SetColor_QueryInterfaceFromMap_1000674c;
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
