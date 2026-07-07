/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: DllUnregisterServer @ 10002120
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: DllUnregisterServer at 10002120
// Signature: undefined4 __stdcall DllUnregisterServer(void)
// Entry: 10002120
// Body: 10002120 - 1000216e
// ============================================================================
undefined4 DllUnregisterServer(void)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  
                    /* 0x2120  4  DllUnregisterServer */
  iVar1 = *DAT_100082a8;
  piVar3 = DAT_100082a8;
  while (iVar1 != 0) {
    if (((code *)piVar3[6] == (code *)0x0) || (iVar1 = (*(code *)piVar3[6])(), iVar1 == 0)) {
      iVar1 = 0;
      (*(code *)piVar3[1])();
      piVar2 = (int *)(*(code *)piVar3[7])(0);
      FUN_100037c0(*piVar3,piVar2,iVar1);
    }
    if (DAT_10008298 == 100) {
      piVar3 = piVar3 + 7;
    }
    else {
      piVar3 = piVar3 + 9;
    }
    iVar1 = *piVar3;
  }
  return 0;
}

// clang-format on
