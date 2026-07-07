/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: AudioHolder_StopPlaybackForEntry @ 100047b0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: AudioHolder_StopPlaybackForEntry at 100047b0
// Signature: undefined4 __thiscall AudioHolder_StopPlaybackForEntry(void * this, int param_1)
// Entry: 100047b0
// Body: 100047b0 - 10004872
// ============================================================================
undefined4 __thiscall AudioHolder_StopPlaybackForEntry(void *this,int param_1)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  
  if ((((-1 < param_1) && (param_1 < *(int *)((int)this + 0x21c))) &&
      (*(int *)((int)this + 0x38) != 0)) &&
     (puVar1 = *(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58),
     puVar1 != (undefined4 *)0x0)) {
    piVar2 = (int *)*puVar1;
    (**(code **)(*piVar2 + 0x48))(piVar2);
    piVar2 = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
    iVar3 = (**(code **)(*piVar2 + 8))(piVar2);
    while (iVar3 != 0) {
      piVar2 = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
      iVar3 = (**(code **)(*piVar2 + 8))(piVar2);
    }
    operator_delete(*(void **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58));
    *(undefined4 *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) = 0;
    iVar3 = *(int *)((int)this + 0x230) + -1;
    *(int *)((int)this + 0x230) = iVar3;
    if (iVar3 == 0) {
      FUN_10006630(*(void **)((int)this + 0x228),*(UINT_PTR *)((int)this + 0x22c));
    }
    if (*(int *)((int)this + 0x34) != 0) {
      (**(code **)(*(int *)((int)this + 8) + 0x1c))((int)this + 8,param_1);
    }
    return 1;
  }
  return 0;
}

// clang-format on
