/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10002d80 @ 10002d80
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10002d80 at 10002d80
// Signature: undefined FUN_10002d80(void)
// Entry: 10002d80
// Body: 10002d80 - 10002ed3
// ============================================================================
/* WARNING: Removing unreachable block (ram,0x10002de1) */
/* WARNING: Removing unreachable block (ram,0x10002df6) */
/* WARNING: Removing unreachable block (ram,0x10002e04) */

LSTATUS __thiscall FUN_10002d80(undefined4 *param_1,LPCSTR param_2)

{
  HKEY hKey;
  LSTATUS LVar1;
  int iVar2;
  HKEY local_124;
  DWORD DStack_120;
  HKEY local_11c;
  undefined4 *local_118;
  _FILETIME _Stack_114;
  CHAR aCStack_10c [256];
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_1000729b;
  pvStack_c = ExceptionList;
  local_4 = 0;
  local_11c = (HKEY)0x0;
  ExceptionList = &pvStack_c;
  local_118 = param_1;
  LVar1 = RegOpenKeyExA((HKEY)*param_1,param_2,0,0x2001f,&local_11c);
  hKey = local_11c;
  if (LVar1 == 0) {
    local_124 = local_11c;
    DStack_120 = 0x100;
    iVar2 = RegEnumKeyExA(local_11c,0,aCStack_10c,&DStack_120,(LPDWORD)0x0,(LPSTR)0x0,(LPDWORD)0x0,
                          &_Stack_114);
    while (iVar2 == 0) {
      iVar2 = FUN_10002d80(aCStack_10c);
      if (iVar2 != 0) {
        if (hKey == (HKEY)0x0) {
          ExceptionList = pvStack_c;
          return iVar2;
        }
        RegCloseKey(hKey);
        ExceptionList = pvStack_c;
        return iVar2;
      }
      DStack_120 = 0x100;
      iVar2 = RegEnumKeyExA(hKey,0,aCStack_10c,&DStack_120,(LPDWORD)0x0,(LPSTR)0x0,(LPDWORD)0x0,
                            &_Stack_114);
      param_1 = local_118;
    }
    if (hKey != (HKEY)0x0) {
      RegCloseKey(hKey);
      local_124 = (HKEY)0x0;
    }
    LVar1 = RegDeleteKeyA((HKEY)*param_1,param_2);
    if (local_124 != (HKEY)0x0) {
      RegCloseKey(local_124);
    }
  }
  ExceptionList = pvStack_c;
  return LVar1;
}

// clang-format on
