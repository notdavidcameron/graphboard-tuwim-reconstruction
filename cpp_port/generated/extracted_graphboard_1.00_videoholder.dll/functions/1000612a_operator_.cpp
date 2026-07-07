/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: operator>> @ 1000612a
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: operator>> at 1000612a
// Signature: CArchive * __stdcall operator>>(CArchive * param_1, CString * param_2)
// Entry: 1000612a
// Body: 1000612a - 1000612f
// ============================================================================
CArchive * operator>>(CArchive *param_1,CString *param_2)

{
  CArchive *pCVar1;
  
                    /* WARNING: Could not recover jumptable at 0x1000612a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pCVar1 = operator>>(param_1,param_2);
  return pCVar1;
}

// clang-format on
