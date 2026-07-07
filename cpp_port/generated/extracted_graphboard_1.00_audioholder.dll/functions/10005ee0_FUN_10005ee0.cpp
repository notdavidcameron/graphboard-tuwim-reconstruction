/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10005ee0 @ 10005ee0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10005ee0 at 10005ee0
// Signature: undefined __fastcall FUN_10005ee0(CDialog * param_1)
// Entry: 10005ee0
// Body: 10005ee0 - 10005f2e
// ============================================================================
void __fastcall FUN_10005ee0(CDialog *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_10007628;
  local_c = ExceptionList;
  local_4 = 0;
  ExceptionList = &local_c;
  CString::~CString((CString *)(param_1 + 0xb0));
  local_4 = 0xffffffff;
  CDialog::~CDialog(param_1);
  ExceptionList = local_c;
  return;
}

// clang-format on
