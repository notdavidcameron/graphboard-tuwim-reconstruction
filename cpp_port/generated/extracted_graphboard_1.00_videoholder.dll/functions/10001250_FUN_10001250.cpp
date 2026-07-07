/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10001250 @ 10001250
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001250 at 10001250
// Signature: undefined __fastcall FUN_10001250(CPropertySheet * param_1)
// Entry: 10001250
// Body: 10001250 - 100012a4
// ============================================================================
void __fastcall FUN_10001250(CPropertySheet *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_10006948;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined ***)param_1 = &PTR_LAB_10007530;
  local_4 = 0;
  VideoSettingsPage_Destroy((CPropertyPage *)(param_1 + 0x8c));
  local_4 = 0xffffffff;
  CPropertySheet::~CPropertySheet(param_1);
  ExceptionList = local_c;
  return;
}

// clang-format on
