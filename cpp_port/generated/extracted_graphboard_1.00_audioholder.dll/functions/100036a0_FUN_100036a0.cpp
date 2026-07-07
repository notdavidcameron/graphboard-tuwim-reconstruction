/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_100036a0 @ 100036a0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100036a0 at 100036a0
// Signature: undefined __stdcall FUN_100036a0(int param_1)
// Entry: 100036a0
// Body: 100036a0 - 100036f4
// ============================================================================
void FUN_100036a0(int param_1)

{
  int *piVar1;
  int iVar2;
  
  piVar1 = *(int **)(param_1 + 0xc);
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
  }
  iVar2 = *(int *)(param_1 + 0x14);
  *(undefined4 *)(param_1 + 0xc) = 0;
  if (iVar2 != 0) {
    FUN_10006e5f(iVar2,0xc,*(int *)(iVar2 + -4),FUN_10003350);
    operator_delete((void *)(iVar2 + -4));
    *(undefined4 *)(param_1 + 0x14) = 0;
    return;
  }
  *(undefined4 *)(param_1 + 0x14) = 0;
  return;
}

// clang-format on
