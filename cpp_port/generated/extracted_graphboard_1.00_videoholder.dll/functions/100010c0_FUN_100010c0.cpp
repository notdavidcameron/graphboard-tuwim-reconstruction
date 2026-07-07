/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_100010c0 @ 100010c0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100010c0 at 100010c0
// Signature: bool __fastcall FUN_100010c0(CWnd * param_1)
// Entry: 100010c0
// Body: 100010c0 - 10001110
// ============================================================================
bool __fastcall FUN_100010c0(CWnd *param_1)

{
  HWND__ *pHVar1;
  char *pcVar2;
  char *pcVar3;
  ulong uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  HMENU__ *pHVar9;
  void *pvVar10;
  
  if (*(int *)(param_1 + 0x20) != 0) {
    (**(code **)(*(int *)param_1 + 0x60))();
  }
  pvVar10 = (void *)0x0;
  pHVar9 = *(HMENU__ **)(*(int *)(param_1 + 0x40) + 0x2c);
  pHVar1 = (HWND__ *)(**(code **)(*(int *)pHVar9 + 0x18))(pHVar9,0,0);
  iVar8 = 0;
  iVar7 = 0;
  iVar6 = 0;
  iVar5 = 0;
  uVar4 = 0x80000000;
  pcVar3 = s_Video_parent_Wnd_1000a020;
  pcVar2 = AfxRegisterWndClass(0,(HICON__ *)0x0,(HBRUSH__ *)0x0,(HICON__ *)0x0);
  iVar5 = CWnd::CreateEx(param_1,0,pcVar2,pcVar3,uVar4,iVar5,iVar6,iVar7,iVar8,pHVar1,pHVar9,pvVar10
                        );
  return iVar5 != 0;
}

// clang-format on
