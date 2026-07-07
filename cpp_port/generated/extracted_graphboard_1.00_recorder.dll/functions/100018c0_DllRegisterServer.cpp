/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: DllRegisterServer @ 100018c0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: DllRegisterServer at 100018c0
// Signature: HRESULT __stdcall DllRegisterServer(void)
// Entry: 100018c0
// Body: 100018c0 - 10001942
// ============================================================================
/* WARNING: Removing unreachable block (ram,0x100018c9) */

HRESULT DllRegisterServer(void)

{
  int iVar1;
  int *piVar2;
  HRESULT HVar3;
  int *piVar4;
  int iVar5;
  
                    /* 0x18c0  3  DllRegisterServer */
  iVar5 = 0;
  piVar4 = DAT_100083a8;
  if (*DAT_100083a8 != 0) {
    do {
      if (((code *)piVar4[6] == (code *)0x0) || (iVar1 = (*(code *)piVar4[6])(), iVar1 == 0)) {
        iVar5 = (*(code *)piVar4[1])(1);
        if (iVar5 < 0) {
          return iVar5;
        }
        if (DAT_10008398 == 0x84) {
          iVar5 = 1;
          piVar2 = (int *)(*(code *)piVar4[7])();
          iVar5 = FUN_10003370(*piVar4,piVar2,iVar5);
          if (iVar5 < 0) {
            return iVar5;
          }
        }
      }
      if (DAT_10008398 == 100) {
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
  HVar3 = FUN_100034b0(0x10008398,(LPCWSTR)0x0);
  return HVar3;
}

// clang-format on
