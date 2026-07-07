/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: DllRegisterServer @ 10002090
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: DllRegisterServer at 10002090
// Signature: HRESULT __stdcall DllRegisterServer(void)
// Entry: 10002090
// Body: 10002090 - 10002112
// ============================================================================
/* WARNING: Removing unreachable block (ram,0x10002099) */

HRESULT DllRegisterServer(void)

{
  int iVar1;
  int *piVar2;
  HRESULT HVar3;
  int *piVar4;
  int iVar5;
  
                    /* 0x2090  3  DllRegisterServer */
  iVar5 = 0;
  piVar4 = DAT_100082a8;
  if (*DAT_100082a8 != 0) {
    do {
      if (((code *)piVar4[6] == (code *)0x0) || (iVar1 = (*(code *)piVar4[6])(), iVar1 == 0)) {
        iVar5 = (*(code *)piVar4[1])(1);
        if (iVar5 < 0) {
          return iVar5;
        }
        if (DAT_10008298 == 0x84) {
          iVar5 = 1;
          piVar2 = (int *)(*(code *)piVar4[7])();
          iVar5 = FUN_100037c0(*piVar4,piVar2,iVar5);
          if (iVar5 < 0) {
            return iVar5;
          }
        }
      }
      if (DAT_10008298 == 100) {
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
  HVar3 = FUN_10003900(0x10008298,(LPCWSTR)0x0);
  return HVar3;
}

// clang-format on
