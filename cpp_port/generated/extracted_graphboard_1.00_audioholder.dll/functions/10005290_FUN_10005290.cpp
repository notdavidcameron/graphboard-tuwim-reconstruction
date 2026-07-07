/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10005290 @ 10005290
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10005290 at 10005290
// Signature: undefined __thiscall FUN_10005290(void * this, CDataExchange * param_1)
// Entry: 10005290
// Body: 10005290 - 1000541d
// ============================================================================
void __thiscall FUN_10005290(void *this,CDataExchange *param_1)

{
  CDataExchange *pCVar1;
  CString *pCVar2;
  void *local_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  pCVar1 = param_1;
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10007590;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  local_10 = this;
  DDX_Control(param_1,0xd4,(CWnd *)((int)this + 0x9c));
  DDX_Control(pCVar1,0xd0,(CWnd *)((int)this + 0xdc));
  DDX_Control(pCVar1,0xce,(CWnd *)((int)this + 0x11c));
  DDX_Control(pCVar1,0xc9,(CWnd *)((int)this + 0x15c));
  DDX_Control(pCVar1,0xcc,(CWnd *)((int)this + 0x19c));
  DDX_Control(pCVar1,0x10f,(CWnd *)((int)this + 0x1dc));
  DDX_Control(pCVar1,0x121,(CWnd *)((int)this + 0x21c));
  DDX_Control(pCVar1,0x11e,(CWnd *)((int)this + 0x25c));
  DDX_Control(pCVar1,0xcd,(CWnd *)((int)this + 0x29c));
  DDX_Control(pCVar1,0xd2,(CWnd *)((int)this + 0x2dc));
  DDX_Control(pCVar1,0xd3,(CWnd *)((int)this + 0x31c));
  if (*(int *)(*(int *)((int)this + 0x90) + 0x21c) != 0) {
    DDX_Check(pCVar1,0xd2,
              *(int **)(*(int *)((int)this + 0x90) + 0x3c + *(int *)((int)this + 0x94) * 4));
  }
  FUN_10005440((int)this);
  CString::CString((CString *)&param_1,s_Data_file_path__1000b25c);
  local_4 = 0;
  pCVar2 = (CString *)
           (**(code **)(**(int **)(*(int *)((int)this + 0x90) + 0x220) + 0x20))(&local_10);
  puStack_8._0_1_ = 1;
  CString::operator+=((CString *)&stack0x00000000,pCVar2);
  puStack_8 = (undefined1 *)((uint)puStack_8._1_3_ << 8);
  CString::~CString((CString *)&stack0xffffffec);
  DDX_Text(pCVar1,0xc9,(CString *)&stack0x00000000);
  puStack_8 = (undefined1 *)0xffffffff;
  CString::~CString((CString *)&stack0x00000000);
  ExceptionList = local_10;
  return;
}

// clang-format on
