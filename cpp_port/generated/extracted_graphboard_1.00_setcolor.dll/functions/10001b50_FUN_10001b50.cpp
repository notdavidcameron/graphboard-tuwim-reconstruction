/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_10001b50 @ 10001b50
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001b50 at 10001b50
// Signature: UINT __fastcall FUN_10001b50(CWnd * param_1)
// Entry: 10001b50
// Body: 10001b50 - 10001c16
// ============================================================================
UINT __fastcall FUN_10001b50(CWnd *param_1)

{
  CClientDC *this;
  CDC *this_00;
  CPalette *pCVar1;
  UINT UVar2;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_1000511a;
  local_c = ExceptionList;
  if (*(int *)(param_1 + 0x60) != 0) {
    ExceptionList = &local_c;
    this = operator_new(0x14);
    local_4 = 0;
    if (this == (CClientDC *)0x0) {
      this_00 = (CDC *)0x0;
    }
    else {
      this_00 = (CDC *)CClientDC::CClientDC(this,param_1);
    }
    local_4 = 0xffffffff;
    pCVar1 = CDC::SelectPalette(this_00,*(CPalette **)(param_1 + 0x60),0);
    UVar2 = RealizePalette(*(HDC *)(this_00 + 4));
    CDC::SelectPalette(this_00,pCVar1,0);
    if (this_00 != (CDC *)0x0) {
      (**(code **)(*(int *)this_00 + 4))(1);
    }
    if (UVar2 != 0) {
      InvalidateRect(*(HWND *)(param_1 + 0x20),(RECT *)0x0,1);
    }
    ExceptionList = local_c;
    return UVar2;
  }
  ExceptionList = &local_c;
  UVar2 = CWnd::Default(param_1);
  ExceptionList = local_c;
  return UVar2;
}

// clang-format on
