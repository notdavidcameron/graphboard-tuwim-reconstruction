/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_100054d0 @ 100054d0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100054d0 at 100054d0
// Signature: undefined __fastcall FUN_100054d0(CWnd * param_1)
// Entry: 100054d0
// Body: 100054d0 - 10005640
// ============================================================================
void __fastcall FUN_100054d0(CWnd *param_1)

{
  CString *this;
  int iVar1;
  int iVar2;
  int iVar3;
  CWnd *local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10006d88;
  local_c = ExceptionList;
  if (*(int *)(*(int *)(param_1 + 0x90) + 0x118) != 0) {
    ExceptionList = &local_c;
    local_10 = param_1;
    CString::CString((CString *)&local_10,(CString *)(*(int *)(param_1 + 0x90) + 0x138));
    local_4 = 0;
    CString::operator+=((CString *)&local_10,
                        (CString *)
                        **(undefined4 **)
                          (*(int *)(param_1 + 0x90) + 0x50 + *(int *)(param_1 + 0x94) * 4));
    CFile::Remove((char *)local_10);
    this = (CString *)
           **(undefined4 **)(*(int *)(param_1 + 0x90) + 0x50 + *(int *)(param_1 + 0x94) * 4);
    if (this != (CString *)0x0) {
      CString::~CString(this);
      operator_delete(this);
    }
    operator_delete(*(void **)(*(int *)(param_1 + 0x90) + 0x50 + *(int *)(param_1 + 0x94) * 4));
    iVar1 = *(int *)(param_1 + 0x90);
    iVar3 = *(int *)(param_1 + 0x94);
    if (iVar3 < *(int *)(iVar1 + 0x118) + -1) {
      iVar2 = iVar3 * 4 + 0x50;
      do {
        iVar3 = iVar3 + 1;
        *(undefined4 *)(iVar2 + iVar1) = *(undefined4 *)(iVar2 + 4 + iVar1);
        iVar1 = *(int *)(param_1 + 0x90);
        iVar2 = iVar2 + 4;
      } while (iVar3 < *(int *)(iVar1 + 0x118) + -1);
    }
    *(int *)(*(int *)(param_1 + 0x90) + 0x118) = *(int *)(*(int *)(param_1 + 0x90) + 0x118) + -1;
    iVar1 = *(int *)(*(int *)(param_1 + 0x90) + 0x118);
    if (iVar1 <= *(int *)(param_1 + 0x94)) {
      if (iVar1 == 0) {
        *(undefined4 *)(param_1 + 0x94) = 0;
      }
      else {
        *(int *)(param_1 + 0x94) = iVar1 + -1;
      }
    }
    CWnd::UpdateData(param_1,0);
    FUN_10005d90((int)param_1);
    SendMessageA(*(HWND *)(param_1 + 700),0x186,*(WPARAM *)(param_1 + 0x94),0);
    local_4 = 0xffffffff;
    CString::~CString((CString *)&local_10);
  }
  ExceptionList = local_c;
  return;
}

// clang-format on
