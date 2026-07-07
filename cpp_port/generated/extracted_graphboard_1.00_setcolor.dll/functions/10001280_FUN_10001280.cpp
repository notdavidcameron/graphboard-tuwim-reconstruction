/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_10001280 @ 10001280
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001280 at 10001280
// Signature: undefined __thiscall FUN_10001280(void * this, CDataExchange * param_1, char * param_2)
// Entry: 10001280
// Body: 10001280 - 1000152d
// ============================================================================
void __thiscall FUN_10001280(void *this,CDataExchange *param_1,char *param_2)

{
  int *piVar1;
  CDataExchange *pCVar2;
  int iVar3;
  CString *pCVar4;
  CString local_18 [4];
  CString local_14 [4];
  CWnd *local_10;
  CWnd *local_c;
  undefined1 *local_8;
  undefined4 local_4;
  
  pCVar2 = param_1;
  local_4 = 0xffffffff;
  local_8 = &LAB_100050b8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  DDX_Control(param_1,0xca,(CWnd *)((int)this + 0x8c));
  local_10 = (CWnd *)((int)this + 0xcc);
  DDX_Control(pCVar2,0xcd,local_10);
  DDX_Control(pCVar2,0xcb,(CWnd *)((int)this + 0x10c));
  DDX_Control(pCVar2,0xd0,(CWnd *)((int)this + 0x14c));
  DDX_Control(pCVar2,0xce,(CWnd *)((int)this + 0x18c));
  DDX_Control(pCVar2,0xc9,(CWnd *)((int)this + 0x1cc));
  DDX_Control(pCVar2,0xcc,(CWnd *)((int)this + 0x20c));
  DDX_Check(pCVar2,0xd0,(int *)((int)this + 0x24c));
  if ((*(int *)((int)this + 0x74) == 0) || (*(int *)((int)this + 0x24c) != 0)) {
    iVar3 = 0;
  }
  else {
    iVar3 = 1;
  }
  CWnd::EnableWindow((CWnd *)((int)this + 0x20c),iVar3);
  CWnd::EnableWindow((CWnd *)((int)this + 0x14c),(uint)(*(int *)((int)this + 0x74) != 0));
  if (*(int *)((int)this + 0x1ac) != 0) {
    if (*(int *)(*(int *)((int)this + 0x78) + -8) == 0) {
      CWnd::GetWindowTextA((CWnd *)((int)this + 0x18c),(CString *)((int)this + 0x78));
    }
    CString::CString((CString *)&param_1);
    local_4 = 0;
    CString::CString(local_18);
    local_4 = CONCAT31(local_4._1_3_,1);
    CString::Format((CString *)&param_1,(char *)&param_1);
    CWnd::SetWindowTextA((CWnd *)((int)this + 0x18c),param_2);
    piVar1 = *(int **)((int)this + 100);
    if (piVar1 != (int *)0x0) {
      CString::Format((CString *)
                      (uint)*(byte *)((int)piVar1 + *(int *)((int)this + 0x6c) * 4 + *piVar1 + 2),
                      (char *)local_14);
      pCVar4 = (CString *)operator+((CString *)&local_c,(CString *)((int)this + 0x7c));
      CString::operator=((CString *)&param_2,pCVar4);
      CString::~CString((CString *)&local_10);
      CWnd::SetWindowTextA((CWnd *)((int)this + 0x8c),param_2);
      CString::Format(*(CString **)((int)this + 0x6c),(char *)local_14);
      pCVar4 = (CString *)operator+((CString *)&local_c,(CString *)((int)this + 0x80));
      CString::operator=((CString *)&param_2,pCVar4);
      CString::~CString((CString *)&local_10);
      CWnd::SetWindowTextA((CWnd *)((int)this + 0x10c),param_2);
      CString::Format(local_14,(char *)local_14);
      pCVar4 = (CString *)operator+((CString *)&local_c,(CString *)((int)this + 0x84));
      CString::operator=((CString *)&param_2,pCVar4);
      CString::~CString((CString *)&local_10);
      CWnd::SetWindowTextA(local_c,param_2);
    }
    CString::~CString(local_14);
    CString::~CString((CString *)&param_2);
  }
  ExceptionList = local_8;
  return;
}

// clang-format on
