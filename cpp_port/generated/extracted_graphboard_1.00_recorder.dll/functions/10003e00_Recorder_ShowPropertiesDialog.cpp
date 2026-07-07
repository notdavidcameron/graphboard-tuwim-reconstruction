/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: Recorder_ShowPropertiesDialog @ 10003e00
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Recorder_ShowPropertiesDialog at 10003e00
// Signature: undefined __fastcall Recorder_ShowPropertiesDialog(int param_1)
// Entry: 10003e00
// Body: 10003e00 - 10003ea9
// ============================================================================
void __fastcall Recorder_ShowPropertiesDialog(int param_1)

{
  undefined4 uVar1;
  int *piVar2;
  undefined4 uVar3;
  AFX_MODULE_STATE *pAVar4;
  void *this;
  CPropertySheet *pCVar5;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10005b2a;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  pAVar4 = AfxGetModuleState();
  uVar3 = DAT_100083a0;
  uVar1 = *(undefined4 *)(pAVar4 + 0xc);
  pAVar4 = AfxGetModuleState();
  *(undefined4 *)(pAVar4 + 0xc) = uVar3;
  this = operator_new(0x160);
  local_4 = 0;
  if (this == (void *)0x0) {
    pCVar5 = (CPropertySheet *)0x0;
  }
  else {
    pCVar5 = FUN_100013c0(this,0x65,param_1,0);
  }
  local_4 = 0xffffffff;
  (**(code **)(*(int *)pCVar5 + 0xc0))();
  if (pCVar5 != (CPropertySheet *)0x0) {
    (**(code **)(*(int *)pCVar5 + 4))(1);
  }
  pAVar4 = AfxGetModuleState();
  *(undefined4 *)(pAVar4 + 0xc) = uVar1;
  piVar2 = *(int **)(param_1 + 0x14);
  if (piVar2 != (int *)0x0) {
    (**(code **)(*piVar2 + 0x1c))(piVar2,0,this);
  }
  ExceptionList = pvStack_c;
  return;
}

// clang-format on
