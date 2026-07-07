/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_100031c0 @ 100031c0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100031c0 at 100031c0
// Signature: undefined __thiscall FUN_100031c0(void * this, HKEY param_1, LPCSTR param_2, REGSAM param_3)
// Entry: 100031c0
// Body: 100031c0 - 10003209
// ============================================================================
void __thiscall FUN_100031c0(void *this,HKEY param_1,LPCSTR param_2,REGSAM param_3)

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
