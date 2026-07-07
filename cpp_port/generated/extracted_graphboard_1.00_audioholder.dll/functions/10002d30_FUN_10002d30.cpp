/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10002d30 @ 10002d30
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10002d30 at 10002d30
// Signature: undefined __thiscall FUN_10002d30(void * this, HKEY param_1, LPCSTR param_2, REGSAM param_3)
// Entry: 10002d30
// Body: 10002d30 - 10002d79
// ============================================================================
void __thiscall FUN_10002d30(void *this,HKEY param_1,LPCSTR param_2,REGSAM param_3)

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
