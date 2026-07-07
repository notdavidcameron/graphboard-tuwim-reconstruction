/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10005f80 @ 10005f80
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10005f80 at 10005f80
// Signature: undefined __fastcall FUN_10005f80(int param_1)
// Entry: 10005f80
// Body: 10005f80 - 10006038
// ============================================================================
void __fastcall FUN_10005f80(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  HWND__ *hWnd;
  HWND__ local_10;
  void *local_c;
  undefined1 *local_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  local_8 = &LAB_10007648;
  local_c = ExceptionList;
  iVar3 = 0;
  ExceptionList = &local_c;
  local_10.unused = param_1;
  SendMessageA(*(HWND *)(param_1 + 0x27c),0x184,0,0);
  CString::CString((CString *)&local_10);
  iVar1 = *(int *)(param_1 + 0x90);
  local_4 = 0;
  if (0 < *(int *)(iVar1 + 0x21c)) {
    iVar2 = 0x3c;
    do {
      hWnd = &local_10;
      CString::Format((CString *)(*(int *)(iVar2 + iVar1) + 0xc),(char *)hWnd);
      SendMessageA(hWnd,*(UINT *)(param_1 + 0x27c),0x180,0);
      iVar1 = *(int *)(param_1 + 0x90);
      iVar3 = iVar3 + 1;
      iVar2 = iVar2 + 4;
    } while (iVar3 < *(int *)(iVar1 + 0x21c));
  }
  local_4 = 0xffffffff;
  CString::~CString((CString *)&local_10);
  ExceptionList = local_c;
  return;
}

// clang-format on
