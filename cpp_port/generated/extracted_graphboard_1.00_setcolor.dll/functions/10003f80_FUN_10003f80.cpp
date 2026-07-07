/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_10003f80 @ 10003f80
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10003f80 at 10003f80
// Signature: undefined __stdcall FUN_10003f80(Editor * param_1)
// Entry: 10003f80
// Body: 10003f80 - 10003fd4
// ============================================================================
void FUN_10003f80(Editor *param_1)

{
  int *piVar1;
  void *pvVar2;
  
  piVar1 = (param_1->secondaryDispatch).vtable;
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
  }
  pvVar2 = param_1->invalidateTarget;
  (param_1->secondaryDispatch).vtable = (void *)0x0;
  if (pvVar2 != (void *)0x0) {
    FUN_10004bef(pvVar2,0xc,*(int *)((int)pvVar2 + -4),FUN_10003b60);
    operator_delete((void *)((int)pvVar2 + -4));
    param_1->invalidateTarget = (void *)0x0;
    return;
  }
  param_1->invalidateTarget = (void *)0x0;
  return;
}

// clang-format on
