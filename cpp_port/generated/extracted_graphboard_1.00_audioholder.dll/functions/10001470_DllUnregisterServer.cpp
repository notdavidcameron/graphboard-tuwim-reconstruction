/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: DllUnregisterServer @ 10001470
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: DllUnregisterServer at 10001470
// Signature: undefined4 __stdcall DllUnregisterServer(void)
// Entry: 10001470
// Body: 10001470 - 100014d0
// ============================================================================
undefined4 DllUnregisterServer(void)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  
                    /* 0x1470  4  DllUnregisterServer */
  iVar1 = *DAT_1000b478;
  piVar3 = DAT_1000b478;
  do {
    if (iVar1 == 0) {
      return 0;
    }
    if ((((code *)piVar3[6] == (code *)0x0) || (iVar1 = (*(code *)piVar3[6])(), iVar1 == 0)) &&
       ((*(code *)piVar3[1])(0), DAT_1000b468 == 0x84)) {
      if ((code *)piVar3[7] != (code *)0x0) {
        iVar1 = 0;
        piVar2 = (int *)(*(code *)piVar3[7])();
        FUN_10002ee0(*piVar3,piVar2,iVar1);
        goto LAB_100014b7;
      }
LAB_100014c5:
      piVar3 = piVar3 + 9;
    }
    else {
LAB_100014b7:
      if (DAT_1000b468 != 100) goto LAB_100014c5;
      piVar3 = piVar3 + 7;
    }
    iVar1 = *piVar3;
  } while( true );
}

// clang-format on
