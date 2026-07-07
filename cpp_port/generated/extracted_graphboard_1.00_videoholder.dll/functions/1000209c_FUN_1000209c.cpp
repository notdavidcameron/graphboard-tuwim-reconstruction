/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_1000209c @ 1000209c
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_1000209c at 1000209c
// Signature: undefined4 * __stdcall FUN_1000209c(void)
// Entry: 1000209c
// Body: 1000209c - 1000210d
// ============================================================================
undefined4 * FUN_1000209c(void)

{
  undefined4 *puVar1;
  undefined4 *unaff_EBX;
  int unaff_EBP;
  undefined4 *unaff_ESI;
  
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  if (unaff_ESI != unaff_EBX) {
    puVar1 = (undefined4 *)(**(code **)*unaff_ESI)();
    if (puVar1 != unaff_EBX) {
      *unaff_ESI = &PTR_LAB_100078d0;
      unaff_ESI[1] = 1;
      InterlockedDecrement(&DAT_1000a41c);
      FUN_10003af0(unaff_ESI + 3);
      operator_delete(unaff_ESI);
    }
    ExceptionList = *(void **)(unaff_EBP + -0xc);
    return puVar1;
  }
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  return *(undefined4 **)(unaff_EBP + -0x18);
}

// clang-format on
