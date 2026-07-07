/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: AudioHolderContext_Construct @ 10003770
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: AudioHolderContext_Construct at 10003770
// Signature: undefined4 * __fastcall AudioHolderContext_Construct(undefined4 * param_1)
// Entry: 10003770
// Body: 10003770 - 1000383c
// ============================================================================
undefined4 * __fastcall AudioHolderContext_Construct(undefined4 *param_1)

{
  void *this;
  CWnd *pCVar1;
  int iVar2;
  undefined4 *puVar3;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_10007355;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  *param_1 = &PTR_LAB_10008a28;
  param_1[1] = &PTR_LAB_10008a0c;
  param_1[2] = &PTR_LAB_100089f0;
  param_1[4] = 0;
  InitializeCriticalSection((LPCRITICAL_SECTION)(param_1 + 5));
  *param_1 = &PTR_LAB_10008978;
  param_1[1] = &PTR_LAB_10008948;
  param_1[2] = &PTR_LAB_10008928;
  param_1[3] = &PTR_LAB_10008918;
  param_1[0xb] = 0;
  param_1[0xd] = 0;
  param_1[0xc] = 0;
  param_1[0xe] = 0;
  puVar3 = param_1 + 0xf;
  for (iVar2 = 0x78; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  uStack_4 = 0;
  param_1[0x87] = 0;
  param_1[0x88] = 0;
  this = operator_new(0x94);
  uStack_4 = CONCAT31(uStack_4._1_3_,1);
  if (this == (void *)0x0) {
    pCVar1 = (CWnd *)0x0;
  }
  else {
    pCVar1 = FUN_10006530(this,param_1);
  }
  param_1[0x8a] = pCVar1;
  param_1[0x8c] = 0;
  ExceptionList = pvStack_c;
  return param_1;
}

// clang-format on
