/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: operator+= @ 10006ae8
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: operator+= at 10006ae8
// Signature: CString * __thiscall operator+=(CString * this, CString * param_1)
// Entry: 10006ae8
// Body: 10006ae8 - 10006aed
// ============================================================================
CString * __thiscall CString::operator+=(CString *this,CString *param_1)

{
  CString *pCVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10006ae8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pCVar1 = operator+=(this,param_1);
  return pCVar1;
}

// clang-format on
