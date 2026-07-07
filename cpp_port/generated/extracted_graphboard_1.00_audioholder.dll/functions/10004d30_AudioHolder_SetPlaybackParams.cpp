/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: AudioHolder_SetPlaybackParams @ 10004d30
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: AudioHolder_SetPlaybackParams at 10004d30
// Signature: undefined4 __stdcall AudioHolder_SetPlaybackParams(int param_1, int param_2, int param_3, int param_4, int param_5)
// Entry: 10004d30
// Body: 10004d30 - 10004ddd
// ============================================================================
undefined4
AudioHolder_SetPlaybackParams(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  
  if (((-1 < param_2) && (param_2 < *(int *)(param_1 + 0x218))) &&
     (puVar1 = *(undefined4 **)(*(int *)(param_1 + 0x38 + param_2 * 4) + 0x58),
     puVar1 != (undefined4 *)0x0)) {
    if ((param_5 < 100) || (100000 < param_5)) {
      param_5 = 0;
    }
    if ((0 < param_3) || (param_3 < -10000)) {
      param_3 = 0;
    }
    if ((param_4 < -10000) || (10000 < param_4)) {
      param_4 = 0;
    }
    piVar2 = (int *)*puVar1;
    iVar3 = (**(code **)(*piVar2 + 0x44))(piVar2,param_5);
    if (iVar3 == 0) {
      piVar2 = (int *)**(undefined4 **)(*(int *)(param_1 + 0x38 + param_2 * 4) + 0x58);
      iVar3 = (**(code **)(*piVar2 + 0x40))(piVar2,param_4);
      if (iVar3 == 0) {
        piVar2 = (int *)**(undefined4 **)(*(int *)(param_1 + 0x38 + param_2 * 4) + 0x58);
        iVar3 = (**(code **)(*piVar2 + 0x3c))(piVar2,param_3);
        if (iVar3 == 0) {
          return 0;
        }
      }
    }
  }
  return 1;
}

// clang-format on
