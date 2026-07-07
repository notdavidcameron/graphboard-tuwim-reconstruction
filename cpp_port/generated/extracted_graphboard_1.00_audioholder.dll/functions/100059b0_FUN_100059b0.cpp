/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_100059b0 @ 100059b0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100059b0 at 100059b0
// Signature: undefined __fastcall FUN_100059b0(CWnd * param_1)
// Entry: 100059b0
// Body: 100059b0 - 10005a00
// ============================================================================
void __fastcall FUN_100059b0(CWnd *param_1)

{
  LRESULT LVar1;
  
  LVar1 = SendMessageA(*(HWND *)(param_1 + 0x27c),0x188,0,0);
  if (*(int *)(param_1 + 0x94) != LVar1) {
    CWnd::UpdateData(param_1,1);
    LVar1 = SendMessageA(*(HWND *)(param_1 + 0x27c),0x188,0,0);
    *(LRESULT *)(param_1 + 0x94) = LVar1;
    CWnd::UpdateData(param_1,0);
  }
  return;
}

// clang-format on
