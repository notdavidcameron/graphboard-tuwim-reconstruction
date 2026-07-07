/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10006040 @ 10006040
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10006040 at 10006040
// Signature: undefined4 __fastcall FUN_10006040(CDialog * param_1)
// Entry: 10006040
// Body: 10006040 - 10006087
// ============================================================================
undefined4 __fastcall FUN_10006040(CDialog *param_1)

{
  CDialog::OnInitDialog(param_1);
  FUN_10005440((int)param_1);
  FUN_10005f80((int)param_1);
  SendMessageA(*(HWND *)(param_1 + 0x27c),0x186,*(WPARAM *)(param_1 + 0x94),0);
  SetTimer(*(HWND *)(param_1 + 0x20),1,100,(TIMERPROC)0x0);
  return 1;
}

// clang-format on
