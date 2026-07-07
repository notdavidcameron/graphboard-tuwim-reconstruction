/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10002e50 @ 10002e50
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10002e50 at 10002e50
// Signature: undefined __thiscall FUN_10002e50(void * this, BYTE * param_1, LPCSTR param_2)
// Entry: 10002e50
// Body: 10002e50 - 10002e78
// ============================================================================
void __thiscall FUN_10002e50(void *this,BYTE *param_1,LPCSTR param_2)

{
  int iVar1;
  
  iVar1 = lstrlenA((LPCSTR)param_1);
  RegSetValueExA(*(HKEY *)this,param_2,0,1,param_1,iVar1 + 1);
  return;
}

// clang-format on
