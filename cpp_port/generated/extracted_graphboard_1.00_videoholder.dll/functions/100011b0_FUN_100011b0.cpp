/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_100011b0 @ 100011b0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100011b0 at 100011b0
// Signature: CPropertySheet * __thiscall FUN_100011b0(void * this, uint param_1, undefined4 param_2, uint param_3)
// Entry: 100011b0
// Body: 100011b0 - 10001227
// ============================================================================
CPropertySheet * __thiscall FUN_100011b0(void *this,uint param_1,undefined4 param_2,uint param_3)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10006936;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  CPropertySheet::CPropertySheet(this,param_1,(CWnd *)0x0,param_3);
  local_4 = 0;
  FUN_10004f30((CPropertyPage *)((int)this + 0x8c));
  local_4 = CONCAT31(local_4._1_3_,1);
  *(undefined ***)this = &PTR_LAB_10007530;
  *(undefined4 *)((int)this + 0x11c) = param_2;
  CPropertySheet::AddPage(this,(CPropertyPage *)((int)this + 0x8c));
  ExceptionList = local_c;
  return this;
}

// clang-format on
