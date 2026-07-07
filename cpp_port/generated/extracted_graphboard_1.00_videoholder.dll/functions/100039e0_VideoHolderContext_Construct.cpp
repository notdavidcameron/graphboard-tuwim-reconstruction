/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: VideoHolderContext_Construct @ 100039e0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: VideoHolderContext_Construct at 100039e0
// Signature: undefined4 * __fastcall VideoHolderContext_Construct(undefined4 * param_1)
// Entry: 100039e0
// Body: 100039e0 - 10003ad1
// ============================================================================
undefined4 * __fastcall VideoHolderContext_Construct(undefined4 *param_1)

{
  void *this;
  CWnd *pCVar1;
  int iVar2;
  undefined4 *puVar3;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_10006b03;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  *param_1 = &PTR_LAB_10007ba8;
  param_1[1] = &PTR_LAB_10007b8c;
  param_1[2] = &PTR_LAB_10007b70;
  param_1[4] = 0;
  InitializeCriticalSection((LPCRITICAL_SECTION)(param_1 + 5));
  uStack_4 = 0;
  CString::CString((CString *)(param_1 + 0x4e));
  *param_1 = &PTR_LAB_10007af8;
  param_1[1] = &PTR_LAB_10007abc;
  param_1[2] = &PTR_LAB_10007a98;
  param_1[3] = &PTR_LAB_10007a88;
  param_1[0xb] = 0;
  param_1[0x11] = 0;
  param_1[0xc] = 0;
  puVar3 = param_1 + 0x14;
  for (iVar2 = 0x32; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  uStack_4._0_1_ = 1;
  param_1[0x46] = 0;
  this = operator_new(0x44);
  uStack_4 = CONCAT31(uStack_4._1_3_,2);
  if (this == (void *)0x0) {
    pCVar1 = (CWnd *)0x0;
  }
  else {
    pCVar1 = FUN_10001020(this,param_1);
  }
  param_1[0x13] = pCVar1;
  param_1[0x12] = 0;
  param_1[0x47] = 0xffffffff;
  param_1[0x4d] = 0xffffffff;
  param_1[0x4c] = 0xffffffff;
  param_1[0x4f] = 0;
  param_1[0x50] = 0;
  param_1[0x51] = 0;
  ExceptionList = pvStack_c;
  return param_1;
}

// clang-format on
