/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10001020 @ 10001020
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001020 at 10001020
// Signature: CWnd * __thiscall FUN_10001020(void * this, undefined4 param_1)
// Entry: 10001020
// Body: 10001020 - 100010bf
// ============================================================================
CWnd * __thiscall FUN_10001020(void *this,undefined4 param_1)

{
  char *pcVar1;
  undefined4 *puVar2;
  char *pcVar3;
  ulong uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  HWND__ *pHVar9;
  HMENU__ *pHVar10;
  void *pvVar11;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_100058e8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  CWnd::CWnd(this);
  pvVar11 = (void *)0x0;
  pHVar10 = (HMENU__ *)0x0;
  pHVar9 = (HWND__ *)0x0;
  iVar8 = 0;
  iVar7 = 0;
  iVar6 = 0;
  iVar5 = 0;
  uVar4 = 0x80000000;
  pcVar3 = s_Local_Wnd_10008020;
  local_4 = 0;
  *(undefined ***)this = &PTR_LAB_100063b8;
  pcVar1 = AfxRegisterWndClass(0,(HICON__ *)0x0,(HBRUSH__ *)0x0,(HICON__ *)0x0);
  iVar5 = CWnd::CreateEx(this,0,pcVar1,pcVar3,uVar4,iVar5,iVar6,iVar7,iVar8,pHVar9,pHVar10,pvVar11);
  if (iVar5 != 0) {
    *(undefined4 *)((int)this + 0x44) = param_1;
    puVar2 = (undefined4 *)((int)this + 0x48);
    for (iVar5 = 0x14; iVar5 != 0; iVar5 = iVar5 + -1) {
      *puVar2 = 0;
      puVar2 = puVar2 + 1;
    }
    ExceptionList = local_c;
    return this;
  }
  ExceptionList = local_c;
  return this;
}

// clang-format on
