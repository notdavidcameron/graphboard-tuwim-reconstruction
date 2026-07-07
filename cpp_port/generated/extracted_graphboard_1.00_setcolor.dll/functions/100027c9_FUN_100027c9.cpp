/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_100027c9 @ 100027c9
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100027c9 at 100027c9
// Signature: undefined4 * __stdcall FUN_100027c9(void)
// Entry: 100027c9
// Body: 100027c9 - 1000284a
// ============================================================================
undefined4 * FUN_100027c9(void)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *unaff_EBX;
  int unaff_EBP;
  undefined4 *unaff_ESI;
  
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  if (unaff_ESI != unaff_EBX) {
    puVar1 = (undefined4 *)(**(code **)*unaff_ESI)();
    if (puVar1 != unaff_EBX) {
      *unaff_ESI = &PTR_LAB_100067c0;
      unaff_ESI[1] = 1;
      InterlockedDecrement(&DAT_100082ac);
      if (unaff_ESI + 3 == unaff_EBX) {
        puVar2 = (undefined4 *)0x0;
      }
      else {
        puVar2 = unaff_ESI + 5;
      }
      DeleteCriticalSection((LPCRITICAL_SECTION)(puVar2 + 1));
      operator_delete(unaff_ESI);
    }
    ExceptionList = *(void **)(unaff_EBP + -0xc);
    return puVar1;
  }
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  return *(undefined4 **)(unaff_EBP + -0x18);
}

// clang-format on
