/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10001180 @ 10001180
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001180 at 10001180
// Signature: undefined __thiscall FUN_10001180(void * this, UINT_PTR param_1)
// Entry: 10001180
// Body: 10001180 - 100011bb
// ============================================================================
void __thiscall FUN_10001180(void *this,UINT_PTR param_1)

{
  int iVar1;
  UINT_PTR *pUVar2;
  
  if (param_1 != 0) {
    pUVar2 = (UINT_PTR *)((int)this + 0x48);
    iVar1 = 0x14;
    do {
      if (*pUVar2 == param_1) {
        KillTimer(*(HWND *)((int)this + 0x20),param_1);
        *pUVar2 = 0;
        param_1 = 0;
      }
      pUVar2 = pUVar2 + 1;
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
  }
  return;
}

// clang-format on
