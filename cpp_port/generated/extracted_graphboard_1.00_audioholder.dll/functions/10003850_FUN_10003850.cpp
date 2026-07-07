/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10003850 @ 10003850
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10003850 at 10003850
// Signature: undefined __fastcall FUN_10003850(undefined4 * param_1)
// Entry: 10003850
// Body: 10003850 - 10003969
// ============================================================================
void __fastcall FUN_10003850(undefined4 *param_1)

{
  undefined4 *puVar1;
  int iVar2;
  char *pcStack_14;
  undefined4 *local_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  uint local_4;
  
  puStack_8 = &LAB_1000738f;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  *param_1 = &PTR_LAB_10008978;
  param_1[1] = &PTR_LAB_10008948;
  param_1[2] = &PTR_LAB_10008928;
  param_1[3] = &PTR_LAB_10008918;
  local_4 = 0;
  local_10 = param_1;
  if ((int *)param_1[0x8a] != (int *)0x0) {
    (**(code **)(*(int *)param_1[0x8a] + 4))(1);
  }
  iVar2 = 0;
  if (0 < (int)param_1[0x87]) {
    puVar1 = param_1 + 0xf;
    do {
      AudioHolder_StopPlaybackForEntry(param_1,iVar2);
      operator_delete((void *)*puVar1);
      iVar2 = iVar2 + 1;
      puVar1 = puVar1 + 1;
    } while (iVar2 < (int)param_1[0x87]);
  }
  if ((int *)param_1[0x88] != (int *)0x0) {
    iVar2 = *(int *)param_1[0x88];
    if (param_1[0x8d] == 0) {
      (**(code **)(iVar2 + 0x20))(&pcStack_14);
      puStack_8 = (undefined1 *)CONCAT31(puStack_8._1_3_,1);
      (**(code **)(*(int *)param_1[0x88] + 0x54))();
      if ((int *)param_1[0x88] != (int *)0x0) {
        (**(code **)(*(int *)param_1[0x88] + 4))(1);
      }
      CFile::Remove(pcStack_14);
      local_4 = local_4 & 0xffffff00;
      CString::~CString((CString *)&pcStack_14);
    }
    else {
      (**(code **)(iVar2 + 0x54))();
      if ((int *)param_1[0x88] != (int *)0x0) {
        (**(code **)(*(int *)param_1[0x88] + 4))(1);
      }
    }
  }
  if (param_1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = param_1 + 4;
  }
  DeleteCriticalSection((LPCRITICAL_SECTION)(puVar1 + 1));
  ExceptionList = pvStack_c;
  return;
}

// clang-format on
