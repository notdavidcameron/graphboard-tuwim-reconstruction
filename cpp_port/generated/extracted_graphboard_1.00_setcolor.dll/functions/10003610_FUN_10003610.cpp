/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_10003610 @ 10003610
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10003610 at 10003610
// Signature: undefined __thiscall FUN_10003610(void * this, HKEY param_1, LPCSTR param_2, REGSAM param_3)
// Entry: 10003610
// Body: 10003610 - 10003659
// ============================================================================
void __thiscall FUN_10003610(void *this,HKEY param_1,LPCSTR param_2,REGSAM param_3)

{
  LSTATUS LVar1;
  HKEY local_4;
  
  local_4 = (HKEY)0x0;
  LVar1 = RegOpenKeyExA(param_1,param_2,0,param_3,&local_4);
  if (LVar1 == 0) {
    if (*(HKEY *)this != (HKEY)0x0) {
      RegCloseKey(*(HKEY *)this);
      *(undefined4 *)this = 0;
    }
    *(HKEY *)this = local_4;
  }
  return;
}

// clang-format on
