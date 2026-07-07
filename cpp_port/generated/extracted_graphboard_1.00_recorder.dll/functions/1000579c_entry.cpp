/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: entry @ 1000579c
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: entry at 1000579c
// Signature: int __stdcall entry(HINSTANCE__ * param_1, int param_2, undefined4 param_3)
// Entry: 1000579c
// Body: 1000579c - 10005838
// ============================================================================
int entry(HINSTANCE__ *param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = param_2;
  iVar2 = DAT_100094d4;
  if (param_2 != 0) {
    if ((param_2 != 1) && (param_2 != 2)) goto LAB_100057e4;
    if ((PTR_FUN_10008240 != (undefined *)0x0) &&
       (iVar2 = (*(code *)PTR_FUN_10008240)(param_1,param_2,param_3), iVar2 == 0)) {
      return 0;
    }
    iVar2 = FUN_100056f1(param_1,param_2);
  }
  if (iVar2 == 0) {
    return 0;
  }
LAB_100057e4:
  iVar2 = FUN_10005375(param_1,param_2);
  if (param_2 == 1) {
    if (iVar2 != 0) {
      return iVar2;
    }
    FUN_100056f1(param_1,0);
  }
  if ((param_2 != 0) && (param_2 != 3)) {
    return iVar2;
  }
  iVar3 = FUN_100056f1(param_1,param_2);
  param_2 = iVar2;
  if (iVar3 == 0) {
    param_2 = 0;
  }
  if (param_2 != 0) {
    if (PTR_FUN_10008240 != (undefined *)0x0) {
      iVar2 = (*(code *)PTR_FUN_10008240)(param_1,iVar1,param_3);
      return iVar2;
    }
    return param_2;
  }
  return 0;
}

// clang-format on
