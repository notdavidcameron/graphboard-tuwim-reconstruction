/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: ShowWindow @ 10004850
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: ShowWindow at 10004850
// Signature: int __thiscall ShowWindow(CWnd * this, int param_1)
// Entry: 10004850
// Body: 10004850 - 10004855
// ============================================================================
int __thiscall CWnd::ShowWindow(CWnd *this,int param_1)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10004850. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = ShowWindow(this,param_1);
  return iVar1;
}

// clang-format on
