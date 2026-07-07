/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10005440 @ 10005440
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10005440 at 10005440
// Signature: undefined __fastcall FUN_10005440(int param_1)
// Entry: 10005440
// Body: 10005440 - 1000562f
// ============================================================================
void __fastcall FUN_10005440(int param_1)

{
  int iVar1;
  
  if ((*(int *)(*(int *)(param_1 + 0x90) + 0x21c) == 0) ||
     (*(int *)(*(int *)(*(int *)(param_1 + 0x90) + 0x3c + *(int *)(param_1 + 0x94) * 4) + 0x58) != 0
     )) {
    iVar1 = 0;
  }
  else {
    iVar1 = 1;
  }
  CWnd::EnableWindow((CWnd *)(param_1 + 0x9c),iVar1);
  if ((*(int *)(*(int *)(param_1 + 0x90) + 0x21c) == 0) ||
     (*(int *)(*(int *)(*(int *)(param_1 + 0x90) + 0x3c + *(int *)(param_1 + 0x94) * 4) + 0x58) == 0
     )) {
    iVar1 = 0;
  }
  else {
    iVar1 = 1;
  }
  CWnd::EnableWindow((CWnd *)(param_1 + 0xdc),iVar1);
  if ((*(int *)(*(int *)(param_1 + 0x90) + 0x21c) == 0) ||
     (*(int *)(*(int *)(*(int *)(param_1 + 0x90) + 0x3c + *(int *)(param_1 + 0x94) * 4) + 0x58) != 0
     )) {
    iVar1 = 0;
  }
  else {
    iVar1 = 1;
  }
  CWnd::EnableWindow((CWnd *)(param_1 + 0x11c),iVar1);
  CWnd::EnableWindow((CWnd *)(param_1 + 0x19c),
                     (uint)(*(int *)(*(int *)(param_1 + 0x90) + 0x21c) < 0x77));
  iVar1 = *(int *)(*(int *)(param_1 + 0x90) + 0x21c);
  if (((iVar1 == 0) || (iVar1 + -1 <= *(int *)(param_1 + 0x94))) ||
     (*(int *)(*(int *)(param_1 + 0x90) + 0x230) != 0)) {
    iVar1 = 0;
  }
  else {
    iVar1 = 1;
  }
  CWnd::EnableWindow((CWnd *)(param_1 + 0x1dc),iVar1);
  if (((*(int *)(*(int *)(param_1 + 0x90) + 0x21c) == 0) || (*(int *)(param_1 + 0x94) == 0)) ||
     (*(int *)(*(int *)(param_1 + 0x90) + 0x230) != 0)) {
    iVar1 = 0;
  }
  else {
    iVar1 = 1;
  }
  CWnd::EnableWindow((CWnd *)(param_1 + 0x21c),iVar1);
  CWnd::EnableWindow((CWnd *)(param_1 + 0x25c),*(int *)(*(int *)(param_1 + 0x90) + 0x21c));
  if ((*(int *)(*(int *)(param_1 + 0x90) + 0x21c) == 0) ||
     (*(int *)(*(int *)(param_1 + 0x90) + 0x230) != 0)) {
    iVar1 = 0;
  }
  else {
    iVar1 = 1;
  }
  CWnd::EnableWindow((CWnd *)(param_1 + 0x29c),iVar1);
  if ((*(int *)(*(int *)(param_1 + 0x90) + 0x21c) == 0) ||
     (*(int *)(*(int *)(*(int *)(param_1 + 0x90) + 0x3c + *(int *)(param_1 + 0x94) * 4) + 0x58) != 0
     )) {
    iVar1 = 0;
  }
  else {
    iVar1 = 1;
  }
  CWnd::EnableWindow((CWnd *)(param_1 + 0x2dc),iVar1);
  if ((*(int *)(*(int *)(param_1 + 0x90) + 0x21c) != 0) &&
     (*(int *)(*(int *)(param_1 + 0x90) + 0x230) != 0)) {
    CWnd::EnableWindow((CWnd *)(param_1 + 0x31c),1);
    return;
  }
  CWnd::EnableWindow((CWnd *)(param_1 + 0x31c),0);
  return;
}

// clang-format on
