/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: AfxMessageBox @ 1000484a
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: AfxMessageBox at 1000484a
// Signature: int __stdcall AfxMessageBox(char * param_1, uint param_2, uint param_3)
// Entry: 1000484a
// Body: 1000484a - 1000484f
// ============================================================================
int AfxMessageBox(char *param_1,uint param_2,uint param_3)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x1000484a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = AfxMessageBox(param_1,param_2,param_3);
  return iVar1;
}

// clang-format on
