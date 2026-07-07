/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: entry @ 100067bc
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: entry at 100067bc
// Signature: int __stdcall entry(HINSTANCE__ * param_1, int param_2, undefined4 param_3)
// Entry: 100067bc
// Body: 100067bc - 10006858
// ============================================================================
int entry(HINSTANCE__ *param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = param_2;
  iVar2 = DAT_1000b55c;
  if (param_2 != 0) {
    if ((param_2 != 1) && (param_2 != 2)) goto LAB_10006804;
    if ((PTR_FUN_1000a2ac != (undefined *)0x0) &&
       (iVar2 = (*(code *)PTR_FUN_1000a2ac)(param_1,param_2,param_3), iVar2 == 0)) {
      return 0;
    }
    iVar2 = FUN_10006711(param_1,param_2);
  }
  if (iVar2 == 0) {
    return 0;
  }
LAB_10006804:
  iVar2 = FUN_10006399(param_1,param_2);
  if (param_2 == 1) {
    if (iVar2 != 0) {
      return iVar2;
    }
    FUN_10006711(param_1,0);
  }
  if ((param_2 != 0) && (param_2 != 3)) {
    return iVar2;
  }
  iVar3 = FUN_10006711(param_1,param_2);
  param_2 = iVar2;
  if (iVar3 == 0) {
    param_2 = 0;
  }
  if (param_2 != 0) {
    if (PTR_FUN_1000a2ac != (undefined *)0x0) {
      iVar2 = (*(code *)PTR_FUN_1000a2ac)(param_1,iVar1,param_3);
      return iVar2;
    }
    return param_2;
  }
  return 0;
}

// clang-format on
