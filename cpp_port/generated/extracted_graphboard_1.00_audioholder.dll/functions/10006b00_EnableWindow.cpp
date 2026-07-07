/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: EnableWindow @ 10006b00
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: EnableWindow at 10006b00
// Signature: int __thiscall EnableWindow(CWnd * this, int param_1)
// Entry: 10006b00
// Body: 10006b00 - 10006b05
// ============================================================================
int __thiscall CWnd::EnableWindow(CWnd *this,int param_1)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10006b00. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = EnableWindow(this,param_1);
  return iVar1;
}

// clang-format on
