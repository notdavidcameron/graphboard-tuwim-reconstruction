/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: operator+= @ 10006172
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: operator+= at 10006172
// Signature: CString * __thiscall operator+=(CString * this, CString * param_1)
// Entry: 10006172
// Body: 10006172 - 10006177
// ============================================================================
CString * __thiscall CString::operator+=(CString *this,CString *param_1)

{
  CString *pCVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10006172. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pCVar1 = operator+=(this,param_1);
  return pCVar1;
}

// clang-format on
