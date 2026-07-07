/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10003020 @ 10003020
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10003020 at 10003020
// Signature: HRESULT __stdcall FUN_10003020(int param_1, LPCWSTR param_2)
// Entry: 10003020
// Body: 10003020 - 10003210
// ============================================================================
HRESULT FUN_10003020(int param_1,LPCWSTR param_2)

{
  int iVar1;
  LPSTR lpString2;
  HRESULT HVar2;
  LPCSTR lpsz;
  uint uVar3;
  uint uVar4;
  LPWSTR szFile;
  LPCSTR lpString1;
  LPCWSTR pWVar5;
  OLECHAR *pOVar6;
  OLECHAR local_220 [124];
  char local_128 [272];
  ITypeLib *local_18;
  LPCWSTR local_14;
  void *local_10;
  undefined1 *puStack_c;
  uint local_8;
  
  puStack_c = &LAB_100072e0;
  local_10 = ExceptionList;
  local_8 = 1;
  local_14 = (LPCWSTR)0x0;
  local_18 = (ITypeLib *)0x0;
  ExceptionList = &local_10;
  GetModuleFileNameA(*(HMODULE *)(param_1 + 0xc),local_128,0x104);
  if (param_2 != (LPCWSTR)0x0) {
    iVar1 = lstrlenW(param_2);
    FUN_10006e30();
    lpString2 = FUN_10003250(&stack0xfffffdd4,param_2,iVar1 * 2 + 2,0);
    lstrcatA(local_128,lpString2);
  }
  if (&stack0x00000000 == (undefined1 *)0x128) {
    szFile = (LPCOLESTR)0x0;
  }
  else {
    iVar1 = lstrlenA(local_128);
    FUN_10006e30();
    szFile = FUN_10003220((LPWSTR)&stack0xfffffdd4,local_128,iVar1 + 1,0);
  }
  HVar2 = LoadTypeLib(szFile,&local_18);
  if (HVar2 < 0) {
    lpString1 = (LPCSTR)0x0;
    lpsz = local_128;
    if (local_128[0] == '\0') {
LAB_1000311f:
      lpString1 = lpsz;
    }
    else {
      do {
        if (local_128[0] == '.') {
          lpString1 = lpsz;
        }
        lpsz = CharNextA(lpsz);
        local_128[0] = *lpsz;
      } while (local_128[0] != '\0');
      if (lpString1 == (LPCSTR)0x0) goto LAB_1000311f;
    }
    lstrcpyA(lpString1,&DAT_1000b178);
    if (&stack0x00000000 == (undefined1 *)0x128) {
      szFile = (LPCOLESTR)0x0;
    }
    else {
      iVar1 = lstrlenA(local_128);
      FUN_10006e30();
      szFile = FUN_10003220((LPWSTR)&stack0xfffffdd4,local_128,iVar1 + 1,0);
    }
    HVar2 = LoadTypeLib(szFile,&local_18);
    if (HVar2 < 0) goto LAB_10003186;
  }
  local_14 = SysAllocString(szFile);
LAB_10003186:
  if (-1 < HVar2) {
    iVar1 = lstrlenW(local_14);
    uVar4 = iVar1 * 2 + 2;
    pWVar5 = local_14;
    pOVar6 = local_220;
    for (uVar3 = uVar4 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
      *(undefined4 *)pOVar6 = *(undefined4 *)pWVar5;
      pWVar5 = pWVar5 + 2;
      pOVar6 = pOVar6 + 2;
    }
    for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
      *(char *)pOVar6 = (char)*pWVar5;
      pWVar5 = (LPCWSTR)((int)pWVar5 + 1);
      pOVar6 = (OLECHAR *)((int)pOVar6 + 1);
    }
    iVar1 = FUN_10003280(local_220);
    pWVar5 = local_14;
    local_220[iVar1] = L'\0';
    HVar2 = RegisterTypeLib(local_18,pWVar5,local_220);
  }
  local_8 = local_8 & 0xffffff00;
  if (local_18 != (ITypeLib *)0x0) {
    (*local_18->lpVtbl->Release)(local_18);
  }
  SysFreeString(local_14);
  ExceptionList = local_10;
  return HVar2;
}

// clang-format on
