/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10005650 @ 10005650
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10005650 at 10005650
// Signature: undefined __fastcall FUN_10005650(CWnd * param_1)
// Entry: 10005650
// Body: 10005650 - 100056a0
// ============================================================================
void __fastcall FUN_10005650(CWnd *param_1)

{
  LRESULT LVar1;
  
  LVar1 = SendMessageA(*(HWND *)(param_1 + 700),0x188,0,0);
  if (*(int *)(param_1 + 0x94) != LVar1) {
    CWnd::UpdateData(param_1,1);
    LVar1 = SendMessageA(*(HWND *)(param_1 + 700),0x188,0,0);
    *(LRESULT *)(param_1 + 0x94) = LVar1;
    CWnd::UpdateData(param_1,0);
  }
  return;
}

// clang-format on
