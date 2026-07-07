/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_100037c0 @ 100037c0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100037c0 at 100037c0
// Signature: int __stdcall FUN_100037c0(undefined4 param_1, int * param_2, int param_3)
// Entry: 100037c0
// Body: 100037c0 - 100038ef
// ============================================================================
int FUN_100037c0(undefined4 param_1,int *param_2,int param_3)

{
  int *piVar1;
  undefined4 *puVar2;
  HRESULT HVar3;
  int iVar4;
  int *local_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_10005228;
  local_c = ExceptionList;
  local_20 = (int *)0x0;
  local_4 = 0;
  if (param_2 != (int *)0x0) {
    ExceptionList = &local_c;
    HVar3 = CoCreateInstance((IID *)&DAT_10006948,(LPUNKNOWN)0x0,1,(IID *)&DAT_10006958,&local_20);
    if (-1 < HVar3) {
      iVar4 = *param_2;
      while (iVar4 != 0) {
        puVar2 = (undefined4 *)param_2[1];
        uStack_1c = *puVar2;
        uStack_18 = puVar2[1];
        uStack_14 = puVar2[2];
        uStack_10 = puVar2[3];
        if (param_3 == 0) {
          if (*param_2 == 1) {
            (**(code **)(*local_20 + 0x18))();
          }
          else {
            (**(code **)(*local_20 + 0x20))(local_20,param_1,1,&uStack_1c);
          }
        }
        else {
          if (*param_2 == 1) {
            iVar4 = (**(code **)(*local_20 + 0x14))();
          }
          else {
            iVar4 = (**(code **)(*local_20 + 0x1c))(local_20,param_1,1,&uStack_1c);
          }
          if (iVar4 < 0) {
            local_4 = 0xffffffff;
            if (local_20 == (int *)0x0) {
              ExceptionList = local_c;
              return iVar4;
            }
            (**(code **)(*local_20 + 8))(local_20);
            ExceptionList = local_c;
            return iVar4;
          }
        }
        piVar1 = param_2 + 2;
        param_2 = param_2 + 2;
        iVar4 = *piVar1;
      }
    }
    if (local_20 != (int *)0x0) {
      local_4 = 0xffffffff;
      (**(code **)(*local_20 + 8))(local_20);
    }
  }
  ExceptionList = local_c;
  return 0;
}

// clang-format on
