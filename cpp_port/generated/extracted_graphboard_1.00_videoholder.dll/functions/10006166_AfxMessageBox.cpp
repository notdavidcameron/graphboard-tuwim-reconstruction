/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: AfxMessageBox @ 10006166
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: AfxMessageBox at 10006166
// Signature: int __stdcall AfxMessageBox(char * param_1, uint param_2, uint param_3)
// Entry: 10006166
// Body: 10006166 - 1000616b
// ============================================================================
int AfxMessageBox(char *param_1,uint param_2,uint param_3)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10006166. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = AfxMessageBox(param_1,param_2,param_3);
  return iVar1;
}

// clang-format on
