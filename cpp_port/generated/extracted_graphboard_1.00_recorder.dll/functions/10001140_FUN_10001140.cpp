/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10001140 @ 10001140
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001140 at 10001140
// Signature: UINT_PTR __thiscall FUN_10001140(void * this, UINT param_1)
// Entry: 10001140
// Body: 10001140 - 1000117b
// ============================================================================
UINT_PTR __thiscall FUN_10001140(void *this,UINT param_1)

{
  int *piVar1;
  UINT_PTR UVar2;
  uint uVar3;
  
  uVar3 = 0;
  piVar1 = (int *)((int)this + 0x48);
  do {
    if (*piVar1 == 0) {
      UVar2 = SetTimer(*(HWND *)((int)this + 0x20),uVar3 + 1,param_1,(TIMERPROC)0x0);
      *(UINT_PTR *)((int)this + uVar3 * 4 + 0x48) = UVar2;
      return UVar2;
    }
    uVar3 = uVar3 + 1;
    piVar1 = piVar1 + 1;
  } while (uVar3 < 0x14);
  return 0;
}

// clang-format on
