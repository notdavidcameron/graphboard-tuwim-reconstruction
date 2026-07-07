/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10005e50 @ 10005e50
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10005e50 at 10005e50
// Signature: undefined4 __fastcall FUN_10005e50(CDialog * param_1)
// Entry: 10005e50
// Body: 10005e50 - 10005e9b
// ============================================================================
undefined4 __fastcall FUN_10005e50(CDialog *param_1)

{
  CDialog::OnInitDialog(param_1);
  FUN_10005d90((int)param_1);
  SendMessageA(*(HWND *)(param_1 + 700),0x186,*(WPARAM *)(param_1 + 0x94),0);
  CWnd::GetWindowTextA((CWnd *)(param_1 + 0x9c),(CString *)(param_1 + 0x98));
  CWnd::UpdateData((CWnd *)param_1,0);
  return 1;
}

// clang-format on
