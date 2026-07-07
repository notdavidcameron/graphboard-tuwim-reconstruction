/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10001b16 @ 10001b16
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001b16 at 10001b16
// Signature: int __stdcall FUN_10001b16(void)
// Entry: 10001b16
// Body: 10001b16 - 10001b9b
// ============================================================================
int FUN_10001b16(void)

{
  int iVar1;
  int unaff_EBP;
  undefined4 *unaff_ESI;
  
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  if (unaff_ESI != (undefined4 *)0x0) {
    iVar1 = (**(code **)*unaff_ESI)();
    if (iVar1 != 0) {
      *unaff_ESI = &PTR_LAB_10007858;
      unaff_ESI[1] = &PTR_LAB_1000781c;
      unaff_ESI[2] = &PTR_LAB_100077f8;
      unaff_ESI[3] = &PTR_LAB_100077e8;
      unaff_ESI[4] = 1;
      InterlockedDecrement(&DAT_1000a41c);
      FUN_10003af0(unaff_ESI);
      operator_delete(unaff_ESI);
    }
    ExceptionList = *(void **)(unaff_EBP + -0xc);
    return iVar1;
  }
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  return *(int *)(unaff_EBP + -0x18);
}

// clang-format on
