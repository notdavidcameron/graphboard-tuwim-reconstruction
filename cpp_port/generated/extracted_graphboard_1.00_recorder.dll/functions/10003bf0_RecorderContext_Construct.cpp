/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: RecorderContext_Construct @ 10003bf0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: RecorderContext_Construct at 10003bf0
// Signature: undefined4 * __fastcall RecorderContext_Construct(undefined4 * param_1)
// Entry: 10003bf0
// Body: 10003bf0 - 10003c91
// ============================================================================
undefined4 * __fastcall RecorderContext_Construct(undefined4 *param_1)

{
  void *this;
  CWnd *pCVar1;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10005aea;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  param_1[1] = &PTR_LAB_10006bfc;
  param_1[2] = &PTR_LAB_10006be0;
  param_1[4] = 0;
  *param_1 = &PTR_LAB_10006b68;
  param_1[1] = &PTR_LAB_10006b30;
  param_1[2] = &PTR_LAB_10006b00;
  param_1[3] = &PTR_LAB_10006af0;
  param_1[5] = 0;
  param_1[0xb] = 0;
  param_1[6] = 0;
  param_1[0xd] = 8;
  this = operator_new(0x98);
  local_4 = 0;
  if (this == (void *)0x0) {
    pCVar1 = (CWnd *)0x0;
  }
  else {
    pCVar1 = FUN_10001020(this,param_1);
  }
  param_1[0xe] = pCVar1;
  param_1[0xf] = 0;
  param_1[0xc] = 0;
  param_1[0x17] = 0;
  param_1[0x18] = 0;
  param_1[0x16] = 0;
  ExceptionList = local_c;
  return param_1;
}

// clang-format on
