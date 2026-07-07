/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: operator+= @ 1000614e
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: operator+= at 1000614e
// Signature: CString * __thiscall operator+=(CString * this, char * param_1)
// Entry: 1000614e
// Body: 1000614e - 10006153
// ============================================================================
CString * __thiscall CString::operator+=(CString *this,char *param_1)

{
  CString *pCVar1;
  
                    /* WARNING: Could not recover jumptable at 0x1000614e. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pCVar1 = operator+=(this,param_1);
  return pCVar1;
}

// clang-format on
