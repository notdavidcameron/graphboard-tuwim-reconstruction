/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: ComTypeInfo_CleanupDispatchMap @ 10003b20
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: ComTypeInfo_CleanupDispatchMap at 10003b20
// Signature: undefined __stdcall ComTypeInfo_CleanupDispatchMap(int param_1)
// Entry: 10003b20
// Body: 10003b20 - 10003b74
// ============================================================================
void ComTypeInfo_CleanupDispatchMap(int param_1)

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
    FUN_10005506(iVar2,0xc,*(int *)(iVar2 + -4),FUN_10001390);
    operator_delete((void *)(iVar2 + -4));
    *(undefined4 *)(param_1 + 0x14) = 0;
    return;
  }
  *(undefined4 *)(param_1 + 0x14) = 0;
  return;
}

// clang-format on
