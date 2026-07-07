/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: SetColor_QueryInterfaceFromMap @ 10002370
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: SetColor_QueryInterfaceFromMap at 10002370
// Signature: int __stdcall SetColor_QueryInterfaceFromMap(int * param_1, int * param_2, undefined4 * param_3)
// Entry: 10002370
// Body: 10002370 - 10002462
// ============================================================================
int SetColor_QueryInterfaceFromMap(int *param_1,int *param_2,undefined4 *param_3)

{
  undefined **ppuVar1;
  undefined *puVar2;
  int iVar3;
  undefined **ppuVar4;
  int *piVar5;
  
  ppuVar4 = &PTR_DAT_10006778;
  if (param_3 == (undefined4 *)0x0) {
    return -0x7fffbffd;
  }
  *param_3 = 0;
  puVar2 = DAT_10006780;
  if ((((*param_2 == 0) && (param_2[1] == 0)) && (param_2[2] == 0xc0)) && (param_2[3] == 0x46000000)
     ) {
    iVar3 = DAT_1000677c + (int)param_1;
    (**(code **)(*(int *)(DAT_1000677c + (int)param_1) + 4))(iVar3);
    *param_3 = iVar3;
    return 0;
  }
  do {
    if (puVar2 == (undefined *)0x0) {
      return -0x7fffbffe;
    }
    piVar5 = (int *)*ppuVar4;
    if ((piVar5 == (int *)0x0) ||
       (((*piVar5 == *param_2 && (piVar5[1] == param_2[1])) &&
        ((piVar5[2] == param_2[2] && (piVar5[3] == param_2[3])))))) {
      if ((code *)ppuVar4[2] == (code *)0x1) {
        piVar5 = (int *)(ppuVar4[1] + (int)param_1);
        (**(code **)(*piVar5 + 4))(piVar5);
        *param_3 = piVar5;
        return 0;
      }
      iVar3 = (*(code *)ppuVar4[2])(param_1,param_2,param_3,ppuVar4[1]);
      if (iVar3 == 0) {
        return 0;
      }
      if ((piVar5 != (int *)0x0) && (iVar3 < 0)) {
        return iVar3;
      }
    }
    ppuVar1 = ppuVar4 + 5;
    ppuVar4 = ppuVar4 + 3;
    puVar2 = *ppuVar1;
  } while( true );
}

// clang-format on
