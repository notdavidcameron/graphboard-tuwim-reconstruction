/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10004f90 @ 10004f90
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10004f90 at 10004f90
// Signature: undefined __fastcall FUN_10004f90(CPropertyPage * param_1)
// Entry: 10004f90
// Body: 10004f90 - 10004fba
// ============================================================================
void __fastcall FUN_10004f90(CPropertyPage *param_1)

{
  LRESULT LVar1;
  
  LVar1 = SendMessageA(*(HWND *)(param_1 + 0xb4),0x147,0,0);
  *(LRESULT *)(*(int *)(param_1 + 0x90) + 0x34) = LVar1;
  CPropertyPage::OnOK(param_1);
  return;
}

// clang-format on
