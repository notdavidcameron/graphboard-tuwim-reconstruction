/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_100065f0 @ 100065f0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100065f0 at 100065f0
// Signature: UINT_PTR __thiscall FUN_100065f0(void * this, UINT param_1)
// Entry: 100065f0
// Body: 100065f0 - 1000662b
// ============================================================================
UINT_PTR __thiscall FUN_100065f0(void *this,UINT param_1)

{
  int *piVar1;
  UINT_PTR UVar2;
  uint uVar3;
  
  uVar3 = 0;
  piVar1 = (int *)((int)this + 0x44);
  do {
    if (*piVar1 == 0) {
      UVar2 = SetTimer(*(HWND *)((int)this + 0x20),uVar3 + 1,param_1,(TIMERPROC)0x0);
      *(UINT_PTR *)((int)this + uVar3 * 4 + 0x44) = UVar2;
      return UVar2;
    }
    uVar3 = uVar3 + 1;
    piVar1 = piVar1 + 1;
  } while (uVar3 < 0x14);
  return 0;
}

// clang-format on
