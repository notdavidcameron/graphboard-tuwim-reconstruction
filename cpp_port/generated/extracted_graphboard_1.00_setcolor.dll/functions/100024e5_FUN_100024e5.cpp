/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_100024e5 @ 100024e5
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100024e5 at 100024e5
// Signature: int __stdcall FUN_100024e5(void)
// Entry: 100024e5
// Body: 100024e5 - 1000255f
// ============================================================================
int FUN_100024e5(void)

{
  int iVar1;
  int unaff_EBP;
  undefined4 *unaff_ESI;
  
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  if (unaff_ESI != (undefined4 *)0x0) {
    iVar1 = (**(code **)*unaff_ESI)();
    if (iVar1 != 0) {
      *unaff_ESI = &PTR_LAB_100067a0;
      unaff_ESI[1] = &PTR_LAB_10006790;
      unaff_ESI[2] = 1;
      InterlockedDecrement(&DAT_100082ac);
      DeleteCriticalSection((LPCRITICAL_SECTION)(unaff_ESI + 3));
      operator_delete(unaff_ESI);
    }
    ExceptionList = *(void **)(unaff_EBP + -0xc);
    return iVar1;
  }
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  return *(int *)(unaff_EBP + -0x18);
}

// clang-format on
