/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_100067b0 @ 100067b0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100067b0 at 100067b0
// Signature: undefined __fastcall FUN_100067b0(CPropertySheet * param_1)
// Entry: 100067b0
// Body: 100067b0 - 10006804
// ============================================================================
void __fastcall FUN_100067b0(CPropertySheet *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_10007758;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined ***)param_1 = &PTR_LAB_10009050;
  local_4 = 0;
  FUN_10005190((CPropertyPage *)(param_1 + 0x8c));
  local_4 = 0xffffffff;
  CPropertySheet::~CPropertySheet(param_1);
  ExceptionList = local_c;
  return;
}

// clang-format on
