/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_100032d0 @ 100032d0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100032d0 at 100032d0
// Signature: undefined4 __stdcall FUN_100032d0(undefined4 param_1, IID * param_2, HKEY param_3, LPCSTR param_4)
// Entry: 100032d0
// Body: 100032d0 - 10003604
// ============================================================================
undefined4 FUN_100032d0(undefined4 param_1,IID *param_2,HKEY param_3,LPCSTR param_4)

{
  HKEY pHVar1;
  int iVar2;
  LSTATUS LVar3;
  code *pcVar4;
  LPCSTR pCVar5;
  CHAR local_128 [256];
  HKEY local_28;
  _FILETIME local_24;
  HKEY local_1c;
  DWORD local_18;
  HKEY local_14;
  void *local_10;
  undefined1 *puStack_c;
  uint local_8;
  
  pHVar1 = param_3;
  puStack_c = &LAB_100051f0;
  local_10 = ExceptionList;
  local_8 = 0;
  local_28 = (HKEY)0x80000000;
  pcVar4 = RegEnumKeyExA_exref;
  ExceptionList = &local_10;
  if ((param_3 != (HKEY)0x0) &&
     (ExceptionList = &local_10, iVar2 = lstrcmpiA((LPCSTR)param_3,&DAT_1000831c),
     pcVar4 = RegEnumKeyExA_exref, iVar2 != 0)) {
    param_3 = (HKEY)0x0;
    local_8 = CONCAT31(local_8._1_3_,1);
    local_14 = (HKEY)0x0;
    LVar3 = RegOpenKeyExA((HKEY)0x80000000,(LPCSTR)pHVar1,0,0x2001f,&local_14);
    if (LVar3 == 0) {
      LVar3 = FUN_100031a0(&param_3);
      pcVar4 = RegEnumKeyExA_exref;
      param_3 = local_14;
      if (LVar3 == 0) {
        local_18 = 0x100;
        iVar2 = RegEnumKeyExA(local_14,0,local_128,&local_18,(LPDWORD)0x0,(LPSTR)0x0,(LPDWORD)0x0,
                              &local_24);
        while (iVar2 == 0) {
          iVar2 = FUN_10003660(local_128);
          if (iVar2 != 0) goto LAB_100034c0;
          local_18 = 0x100;
          iVar2 = RegEnumKeyExA(param_3,0,local_128,&local_18,(LPDWORD)0x0,(LPSTR)0x0,(LPDWORD)0x0,
                                &local_24);
        }
        FUN_100031a0(&param_3);
        RegDeleteKeyA((HKEY)0x80000000,(LPCSTR)pHVar1);
LAB_100034c0:
        local_8 = local_8 & 0xffffff00;
        FUN_100031a0(&param_3);
        goto LAB_1000335f;
      }
    }
    local_8 = local_8 & 0xffffff00;
    FUN_100031a0(&param_3);
    pcVar4 = RegEnumKeyExA_exref;
  }
LAB_1000335f:
  pCVar5 = param_4;
  if ((param_4 != (LPCSTR)0x0) && (iVar2 = lstrcmpiA(param_4,&DAT_1000831c), iVar2 != 0)) {
    param_3 = (HKEY)0x0;
    local_8 = CONCAT31(local_8._1_3_,2);
    local_14 = (HKEY)0x0;
    LVar3 = RegOpenKeyExA((HKEY)0x80000000,pCVar5,0,0x2001f,&local_14);
    if (LVar3 == 0) {
      LVar3 = FUN_100031a0(&param_3);
      param_3 = local_14;
      if (LVar3 == 0) {
        param_4 = (LPCSTR)0x100;
        iVar2 = (*pcVar4)(local_14,0,local_128,&param_4,0,0,0,&local_24);
        while (iVar2 == 0) {
          iVar2 = FUN_10003660(local_128);
          if (iVar2 != 0) goto LAB_10003420;
          param_4 = (LPCSTR)0x100;
          iVar2 = (*pcVar4)(param_3,0,local_128,&param_4,0,0,0,&local_24);
        }
        FUN_100031a0(&param_3);
        RegDeleteKeyA((HKEY)0x80000000,pCVar5);
      }
    }
LAB_10003420:
    local_8 = local_8 & 0xffffff00;
    FUN_100031a0(&param_3);
  }
  StringFromCLSID(param_2,(LPOLESTR *)&local_1c);
  param_3 = local_1c;
  if (local_1c == (HKEY)0x0) {
    pCVar5 = (LPCSTR)0x0;
  }
  else {
    pCVar5 = &stack0xfffffecc;
    iVar2 = lstrlenW((LPCWSTR)local_1c);
    FUN_10004bc0();
    WideCharToMultiByte(0,0,(LPCWSTR)param_3,-1,&stack0xfffffecc,iVar2 * 2 + 2,(LPCSTR)0x0,
                        (LPBOOL)0x0);
  }
  param_3 = (HKEY)0x0;
  LVar3 = RegOpenKeyExA((HKEY)0x80000000,s_CLSID_10008120,0,0x20019,&param_3);
  if (LVar3 == 0) {
    LVar3 = RegCloseKey((HKEY)0x80000000);
    pHVar1 = param_3;
    local_28 = param_3;
    if (LVar3 == 0) {
      param_3 = (HKEY)0x0;
      local_8 = CONCAT31(local_8._1_3_,3);
      iVar2 = FUN_10003610(&param_3,pHVar1,pCVar5,0x2001f);
      if (iVar2 == 0) {
        param_4 = (LPCSTR)0x100;
        iVar2 = RegEnumKeyExA(param_3,0,local_128,(LPDWORD)&param_4,(LPDWORD)0x0,(LPSTR)0x0,
                              (LPDWORD)0x0,&local_24);
        while (iVar2 == 0) {
          iVar2 = FUN_10003660(local_128);
          if (iVar2 != 0) goto LAB_100035ca;
          param_4 = (LPCSTR)0x100;
          iVar2 = RegEnumKeyExA(param_3,0,local_128,(LPDWORD)&param_4,(LPDWORD)0x0,(LPSTR)0x0,
                                (LPDWORD)0x0,&local_24);
        }
        FUN_100031a0(&param_3);
        RegDeleteKeyA(pHVar1,pCVar5);
      }
LAB_100035ca:
      FUN_100031a0(&param_3);
    }
  }
  CoTaskMemFree(local_1c);
  if (local_28 != (HKEY)0x0) {
    RegCloseKey(local_28);
  }
  ExceptionList = local_10;
  return 0;
}

// clang-format on
