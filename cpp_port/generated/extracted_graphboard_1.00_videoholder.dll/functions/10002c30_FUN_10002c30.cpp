/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10002c30 @ 10002c30
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10002c30 at 10002c30
// Signature: undefined __thiscall FUN_10002c30(void * this, BYTE * param_1, LPCSTR param_2)
// Entry: 10002c30
// Body: 10002c30 - 10002c58
// ============================================================================
void __thiscall FUN_10002c30(void *this,BYTE *param_1,LPCSTR param_2)

{
  int iVar1;
  
  iVar1 = lstrlenA((LPCSTR)param_1);
  RegSetValueExA(*(HKEY *)this,param_2,0,1,param_1,iVar1 + 1);
  return;
}

// clang-format on
