/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: VideoSettingsPage_UpdateControls @ 100053b0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: VideoSettingsPage_UpdateControls at 100053b0
// Signature: undefined __fastcall VideoSettingsPage_UpdateControls(int param_1)
// Entry: 100053b0
// Body: 100053b0 - 100054c3
// ============================================================================
void __fastcall VideoSettingsPage_UpdateControls(int param_1)

{
  int iVar1;
  
  CWnd::EnableWindow((CWnd *)(param_1 + 0x11c),*(int *)(*(int *)(param_1 + 0x90) + 0x118));
  CWnd::EnableWindow((CWnd *)(param_1 + 0x15c),*(int *)(*(int *)(param_1 + 0x90) + 0x118));
  CWnd::EnableWindow((CWnd *)(param_1 + 0x19c),
                     (uint)(*(int *)(*(int *)(param_1 + 0x90) + 0x118) < 0x31));
  iVar1 = *(int *)(*(int *)(param_1 + 0x90) + 0x118);
  if ((iVar1 == 0) || (iVar1 + -1 <= *(int *)(param_1 + 0x94))) {
    iVar1 = 0;
  }
  else {
    iVar1 = 1;
  }
  CWnd::EnableWindow((CWnd *)(param_1 + 0x1dc),iVar1);
  if ((*(int *)(*(int *)(param_1 + 0x90) + 0x118) == 0) || (*(int *)(param_1 + 0x94) == 0)) {
    iVar1 = 0;
  }
  else {
    iVar1 = 1;
  }
  CWnd::EnableWindow((CWnd *)(param_1 + 0x21c),iVar1);
  CWnd::EnableWindow((CWnd *)(param_1 + 0x25c),*(int *)(*(int *)(param_1 + 0x90) + 0x118));
  CWnd::EnableWindow((CWnd *)(param_1 + 0x29c),*(int *)(*(int *)(param_1 + 0x90) + 0x118));
  CWnd::EnableWindow((CWnd *)(param_1 + 0x2dc),*(int *)(*(int *)(param_1 + 0x90) + 0x118));
  CWnd::EnableWindow((CWnd *)(param_1 + 0xdc),*(int *)(*(int *)(param_1 + 0x90) + 0x118));
  return;
}

// clang-format on
