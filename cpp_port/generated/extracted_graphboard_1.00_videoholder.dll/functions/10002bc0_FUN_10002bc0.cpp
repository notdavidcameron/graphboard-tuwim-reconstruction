/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10002bc0 @ 10002bc0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10002bc0 at 10002bc0
// Signature: LSTATUS __thiscall FUN_10002bc0(void * this, LPCSTR param_1, BYTE * param_2, LPCSTR param_3)
// Entry: 10002bc0
// Body: 10002bc0 - 10002c2a
// ============================================================================
LSTATUS __thiscall FUN_10002bc0(void *this,LPCSTR param_1,BYTE *param_2,LPCSTR param_3)

{
  HKEY hKey;
  BYTE *lpData;
  HKEY hKey_00;
  LSTATUS LVar1;
  int iVar2;
  HKEY local_4;
  
  local_4 = (HKEY)0x0;
  LVar1 = RegCreateKeyExA(*(HKEY *)this,param_1,0,(LPSTR)0x0,0,0xf003f,(LPSECURITY_ATTRIBUTES)0x0,
                          &local_4,(LPDWORD)&param_1);
  hKey_00 = local_4;
  lpData = param_2;
  hKey = (HKEY)0x0;
  if (LVar1 == 0) {
    iVar2 = lstrlenA((LPCSTR)param_2);
    LVar1 = RegSetValueExA(hKey_00,param_3,0,1,lpData,iVar2 + 1);
    hKey = hKey_00;
  }
  if (hKey != (HKEY)0x0) {
    RegCloseKey(hKey);
  }
  return LVar1;
}

// clang-format on
