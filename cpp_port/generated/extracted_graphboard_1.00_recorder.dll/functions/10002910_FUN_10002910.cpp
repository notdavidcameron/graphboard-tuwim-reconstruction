/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10002910 @ 10002910
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10002910 at 10002910
// Signature: IID * __stdcall FUN_10002910(int param_1, IID * param_2, BYTE * param_3, BYTE * param_4, HKEY param_5, uint param_6)
// Entry: 10002910
// Body: 10002910 - 10002d2a
// ============================================================================
IID * FUN_10002910(int param_1,IID *param_2,BYTE *param_3,BYTE *param_4,HKEY param_5,uint param_6)

{
  uint uVar1;
  LPCWSTR lpWideCharStr;
  uint uVar2;
  int iVar3;
  IID *pIVar4;
  HMODULE pHVar5;
  char *pcVar6;
  DWORD DVar7;
  BYTE *pBVar8;
  BYTE local_324 [260];
  BYTE local_220 [260];
  BYTE local_11c [256];
  LPCWSTR local_1c;
  HKEY local_18;
  BYTE *local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_100059f0;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  LoadStringA(*(HINSTANCE *)(param_1 + 4),(UINT)param_5,(LPSTR)local_11c,0x100);
  GetModuleFileNameA(*(HMODULE *)(param_1 + 4),(LPSTR)local_220,0x104);
  StringFromCLSID(param_2,&local_1c);
  lpWideCharStr = local_1c;
  if (local_1c == (LPCWSTR)0x0) {
    local_14 = (BYTE *)0x0;
  }
  else {
    iVar3 = lstrlenW(local_1c);
    FUN_10005640();
    WideCharToMultiByte(0,0,lpWideCharStr,-1,&stack0xfffffcd0,iVar3 * 2 + 2,(LPCSTR)0x0,(LPBOOL)0x0)
    ;
    local_14 = &stack0xfffffcd0;
  }
  pBVar8 = local_14;
  param_2 = (IID *)0x0;
  local_8 = 0;
  uVar2 = FUN_10002d70(&param_2,(HKEY)0x80000000,(LPCSTR)param_3,(LPSTR)0x0,0,2,
                       (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
  if (uVar2 == 0) {
    iVar3 = lstrlenA((LPCSTR)local_11c);
    RegSetValueExA((HKEY)param_2,(LPCSTR)0x0,0,1,local_11c,iVar3 + 1);
    FUN_10002de0(&param_2,s_CLSID_10008180,pBVar8,(LPCSTR)0x0);
    local_8 = 0xffffffff;
    FUN_10002d50(&param_2);
  }
  else {
    pIVar4 = (IID *)(uVar2 & 0xffff | 0x80070000);
    local_8 = 0xffffffff;
    FUN_10002d50(&param_2);
    param_2 = pIVar4;
    if (pIVar4 != (IID *)0x0) goto LAB_10002ab8;
  }
  param_2 = (IID *)0x0;
  local_8 = 1;
  uVar2 = FUN_10002d70(&param_2,(HKEY)0x80000000,(LPCSTR)param_4,(LPSTR)0x0,0,2,
                       (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
  if (uVar2 == 0) {
    FUN_10002e50(&param_2,local_11c,(LPCSTR)0x0);
    FUN_10002de0(&param_2,s_CLSID_10008180,pBVar8,(LPCSTR)0x0);
    local_8 = 0xffffffff;
    FUN_10002d50(&param_2);
    param_2 = (IID *)0x0;
  }
  else {
    local_8 = 0xffffffff;
    FUN_10002d50(&param_2);
    param_2 = (IID *)(uVar2 & 0xffff | 0x80070000);
  }
LAB_10002ab8:
  uVar2 = 0;
  if (param_2 == (IID *)0x0) {
    param_5 = (HKEY)0x0;
    local_8 = 2;
    local_18 = (HKEY)0x0;
    uVar2 = RegOpenKeyExA((HKEY)0x80000000,s_CLSID_10008180,0,0x2001f,&local_18);
    if (uVar2 == 0) {
      uVar2 = 0;
      if (param_5 != (HKEY)0x0) {
        uVar2 = RegCloseKey(param_5);
      }
      param_5 = local_18;
    }
    if (uVar2 == 0) {
      local_18 = (HKEY)0x0;
      uVar2 = RegCreateKeyExA(param_5,(LPCSTR)local_14,0,(LPSTR)0x0,0,0x2001f,
                              (LPSECURITY_ATTRIBUTES)0x0,&local_18,(LPDWORD)&local_14);
      if (uVar2 == 0) {
        uVar2 = FUN_10002d50(&param_5);
        param_5 = local_18;
      }
      if (uVar2 == 0) {
        iVar3 = lstrlenA((LPCSTR)local_11c);
        RegSetValueExA(param_5,(LPCSTR)0x0,0,1,local_11c,iVar3 + 1);
        local_14 = (BYTE *)0x0;
        local_8 = CONCAT31(local_8._1_3_,3);
        iVar3 = FUN_10002d70(&local_14,param_5,"ProgID",(LPSTR)0x0,0,0xf003f,
                             (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
        if (iVar3 == 0) {
          FUN_10002e50(&local_14,param_3,(LPCSTR)0x0);
        }
        local_8._0_1_ = 2;
        FUN_10002d50(&local_14);
        param_3 = (BYTE *)0x0;
        local_8 = CONCAT31(local_8._1_3_,4);
        iVar3 = FUN_10002d70(&param_3,param_5,"VersionIndependentProgID",(LPSTR)0x0,0,0xf003f,
                             (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
        if (iVar3 == 0) {
          FUN_10002e50(&param_3,param_4,(LPCSTR)0x0);
        }
        local_8 = CONCAT31(local_8._1_3_,2);
        FUN_10002d50(&param_3);
        if ((*(int *)(param_1 + 4) == 0) ||
           (pHVar5 = GetModuleHandleA((LPCSTR)0x0), uVar1 = param_6,
           *(HMODULE *)(param_1 + 4) == pHVar5)) {
          DVar7 = GetShortPathNameA((LPCSTR)local_220,(LPSTR)local_324,0x104);
          if (DVar7 == 0x104) {
            local_8 = 0xffffffff;
            FUN_10002d50(&param_5);
            ExceptionList = local_10;
            return (IID *)0x8007000e;
          }
          if ((DVar7 == 0) || (pBVar8 = local_324, DVar7 == 0x57)) {
            pBVar8 = local_220;
          }
          param_3 = (BYTE *)0x0;
          local_8 = CONCAT31(local_8._1_3_,5);
          iVar3 = FUN_10002d70(&param_3,param_5,"LocalServer32",(LPSTR)0x0,0,0xf003f,
                               (LPSECURITY_ATTRIBUTES)0x0,(undefined4 *)0x0);
          if (iVar3 == 0) {
            FUN_10002e50(&param_3,pBVar8,(LPCSTR)0x0);
          }
          local_8 = CONCAT31(local_8._1_3_,2);
          FUN_10002d50(&param_3);
        }
        else {
          pcVar6 = "AUTPRX32.DLL";
          if ((param_6 & 4) == 0) {
            pcVar6 = (char *)local_220;
          }
          FUN_10002de0(&param_5,"InprocServer32",(BYTE *)pcVar6,(LPCSTR)0x0);
          if ((uVar1 & 2) == 0) {
            pBVar8 = (BYTE *)(-(uint)((uVar1 & 1) != 0) & 0x10006a98);
          }
          else {
            pBVar8 = "both";
          }
          if (pBVar8 != (BYTE *)0x0) {
            FUN_10002de0(&param_5,"InprocServer32",pBVar8,"ThreadingModel");
          }
        }
      }
    }
    if (param_5 != (HKEY)0x0) {
      RegCloseKey(param_5);
    }
  }
  CoTaskMemFree(local_1c);
  if (uVar2 != 0) {
    param_2 = (IID *)(uVar2 & 0xffff | 0x80070000);
  }
  ExceptionList = local_10;
  return param_2;
}

// clang-format on
