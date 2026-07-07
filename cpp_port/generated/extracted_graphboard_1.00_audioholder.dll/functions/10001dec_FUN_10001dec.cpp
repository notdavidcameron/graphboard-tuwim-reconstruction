/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10001dec @ 10001dec
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001dec at 10001dec
// Signature: undefined4 * __stdcall FUN_10001dec(void)
// Entry: 10001dec
// Body: 10001dec - 10001e5d
// ============================================================================
undefined4 * FUN_10001dec(void)

{
  undefined4 *puVar1;
  undefined4 *unaff_EBX;
  int unaff_EBP;
  undefined4 *unaff_ESI;
  
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  if (unaff_ESI != unaff_EBX) {
    puVar1 = (undefined4 *)(**(code **)*unaff_ESI)();
    if (puVar1 != unaff_EBX) {
      *unaff_ESI = &PTR_LAB_10008770;
      unaff_ESI[1] = 1;
      InterlockedDecrement(&DAT_1000b47c);
      FUN_10003850(unaff_ESI + 3);
      operator_delete(unaff_ESI);
    }
    ExceptionList = *(void **)(unaff_EBP + -0xc);
    return puVar1;
  }
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  return *(undefined4 **)(unaff_EBP + -0x18);
}

// clang-format on
