/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: DllUnregisterServer @ 10001950
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: DllUnregisterServer at 10001950
// Signature: undefined4 __stdcall DllUnregisterServer(void)
// Entry: 10001950
// Body: 10001950 - 100019b0
// ============================================================================
undefined4 DllUnregisterServer(void)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  
                    /* 0x1950  4  DllUnregisterServer */
  iVar1 = *DAT_100083a8;
  piVar3 = DAT_100083a8;
  do {
    if (iVar1 == 0) {
      return 0;
    }
    if ((((code *)piVar3[6] == (code *)0x0) || (iVar1 = (*(code *)piVar3[6])(), iVar1 == 0)) &&
       ((*(code *)piVar3[1])(0), DAT_10008398 == 0x84)) {
      if ((code *)piVar3[7] != (code *)0x0) {
        iVar1 = 0;
        piVar2 = (int *)(*(code *)piVar3[7])();
        FUN_10003370(*piVar3,piVar2,iVar1);
        goto LAB_10001997;
      }
LAB_100019a5:
      piVar3 = piVar3 + 9;
    }
    else {
LAB_10001997:
      if (DAT_10008398 != 100) goto LAB_100019a5;
      piVar3 = piVar3 + 7;
    }
    iVar1 = *piVar3;
  } while( true );
}

// clang-format on
