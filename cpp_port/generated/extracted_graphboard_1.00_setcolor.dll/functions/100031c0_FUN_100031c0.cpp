/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_100031c0 @ 100031c0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100031c0 at 100031c0
// Signature: undefined __thiscall FUN_100031c0(void * this, HKEY param_1, LPCSTR param_2, LPSTR param_3, DWORD param_4, REGSAM param_5, LPSECURITY_ATTRIBUTES param_6, undefined4 * param_7)
// Entry: 100031c0
// Body: 100031c0 - 1000322b
// ============================================================================
void __thiscall
FUN_100031c0(void *this,HKEY param_1,LPCSTR param_2,LPSTR param_3,DWORD param_4,REGSAM param_5,
            LPSECURITY_ATTRIBUTES param_6,undefined4 *param_7)

{
  LSTATUS LVar1;
  HKEY local_4;
  
  local_4 = (HKEY)0x0;
  LVar1 = RegCreateKeyExA(param_1,param_2,0,param_3,param_4,param_5,param_6,&local_4,
                          (LPDWORD)&param_6);
  if (param_7 != (undefined4 *)0x0) {
    *param_7 = param_6;
  }
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
