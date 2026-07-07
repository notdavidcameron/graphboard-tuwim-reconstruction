/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: DllRegisterServer @ 100013e0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: DllRegisterServer at 100013e0
// Signature: HRESULT __stdcall DllRegisterServer(void)
// Entry: 100013e0
// Body: 100013e0 - 10001462
// ============================================================================
/* WARNING: Removing unreachable block (ram,0x100013e9) */

HRESULT DllRegisterServer(void)

{
  int iVar1;
  int *piVar2;
  HRESULT HVar3;
  int *piVar4;
  int iVar5;
  
                    /* 0x13e0  3  DllRegisterServer */
  iVar5 = 0;
  piVar4 = DAT_1000b478;
  if (*DAT_1000b478 != 0) {
    do {
      if (((code *)piVar4[6] == (code *)0x0) || (iVar1 = (*(code *)piVar4[6])(), iVar1 == 0)) {
        iVar5 = (*(code *)piVar4[1])(1);
        if (iVar5 < 0) {
          return iVar5;
        }
        if (DAT_1000b468 == 0x84) {
          iVar5 = 1;
          piVar2 = (int *)(*(code *)piVar4[7])();
          iVar5 = FUN_10002ee0(*piVar4,piVar2,iVar5);
          if (iVar5 < 0) {
            return iVar5;
          }
        }
      }
      if (DAT_1000b468 == 100) {
        piVar4 = piVar4 + 7;
      }
      else {
        piVar4 = piVar4 + 9;
      }
    } while (*piVar4 != 0);
    if (iVar5 < 0) {
      return iVar5;
    }
  }
  HVar3 = FUN_10003020(0x1000b468,(LPCWSTR)0x0);
  return HVar3;
}

// clang-format on
