/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_100029c0 @ 100029c0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100029c0 at 100029c0
// Signature: undefined __thiscall FUN_100029c0(void * this, BYTE * param_1, LPCSTR param_2)
// Entry: 100029c0
// Body: 100029c0 - 100029e8
// ============================================================================
void __thiscall FUN_100029c0(void *this,BYTE *param_1,LPCSTR param_2)

{
  int iVar1;
  
  iVar1 = lstrlenA((LPCSTR)param_1);
  RegSetValueExA(*(HKEY *)this,param_2,0,1,param_1,iVar1 + 1);
  return;
}

// clang-format on
