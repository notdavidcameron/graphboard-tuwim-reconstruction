/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Notes: raw Ghidra decompiler output with only file-level framing added.
*/
#include "../include/ghidra_compat.hpp"

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


// ============================================================================
// Function: FUN_10003660 at 10003660
// Signature: undefined FUN_10003660(void)
// Entry: 10003660
// Body: 10003660 - 100037b3
// ============================================================================
/* WARNING: Removing unreachable block (ram,0x100036c1) */
/* WARNING: Removing unreachable block (ram,0x100036d6) */
/* WARNING: Removing unreachable block (ram,0x100036e4) */

LSTATUS __thiscall FUN_10003660(undefined4 *param_1,LPCSTR param_2)

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
  
  puStack_8 = &LAB_1000520b;
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
      iVar2 = FUN_10003660(aCStack_10c);
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


// ============================================================================
// Function: FUN_100037c0 at 100037c0
// Signature: int __stdcall FUN_100037c0(undefined4 param_1, int * param_2, int param_3)
// Entry: 100037c0
// Body: 100037c0 - 100038ef
// ============================================================================
int FUN_100037c0(undefined4 param_1,int *param_2,int param_3)

{
  int *piVar1;
  undefined4 *puVar2;
  HRESULT HVar3;
  int iVar4;
  int *local_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_10005228;
  local_c = ExceptionList;
  local_20 = (int *)0x0;
  local_4 = 0;
  if (param_2 != (int *)0x0) {
    ExceptionList = &local_c;
    HVar3 = CoCreateInstance((IID *)&DAT_10006948,(LPUNKNOWN)0x0,1,(IID *)&DAT_10006958,&local_20);
    if (-1 < HVar3) {
      iVar4 = *param_2;
      while (iVar4 != 0) {
        puVar2 = (undefined4 *)param_2[1];
        uStack_1c = *puVar2;
        uStack_18 = puVar2[1];
        uStack_14 = puVar2[2];
        uStack_10 = puVar2[3];
        if (param_3 == 0) {
          if (*param_2 == 1) {
            (**(code **)(*local_20 + 0x18))();
          }
          else {
            (**(code **)(*local_20 + 0x20))(local_20,param_1,1,&uStack_1c);
          }
        }
        else {
          if (*param_2 == 1) {
            iVar4 = (**(code **)(*local_20 + 0x14))();
          }
          else {
            iVar4 = (**(code **)(*local_20 + 0x1c))(local_20,param_1,1,&uStack_1c);
          }
          if (iVar4 < 0) {
            local_4 = 0xffffffff;
            if (local_20 == (int *)0x0) {
              ExceptionList = local_c;
              return iVar4;
            }
            (**(code **)(*local_20 + 8))(local_20);
            ExceptionList = local_c;
            return iVar4;
          }
        }
        piVar1 = param_2 + 2;
        param_2 = param_2 + 2;
        iVar4 = *piVar1;
      }
    }
    if (local_20 != (int *)0x0) {
      local_4 = 0xffffffff;
      (**(code **)(*local_20 + 8))(local_20);
    }
  }
  ExceptionList = local_c;
  return 0;
}


// ============================================================================
// Function: FUN_100038f0 at 100038f0
// Signature: undefined __fastcall FUN_100038f0(int * param_1)
// Entry: 100038f0
// Body: 100038f0 - 100038fc
// ============================================================================
void __fastcall FUN_100038f0(int *param_1)

{
  int *piVar1;
  
  piVar1 = (int *)*param_1;
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
  }
  return;
}


// ============================================================================
// Function: FUN_10003900 at 10003900
// Signature: HRESULT __stdcall FUN_10003900(int param_1, LPCWSTR param_2)
// Entry: 10003900
// Body: 10003900 - 10003af0
// ============================================================================
HRESULT FUN_10003900(int param_1,LPCWSTR param_2)

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
  
  puStack_c = &LAB_10005250;
  local_10 = ExceptionList;
  local_8 = 1;
  local_14 = (LPCWSTR)0x0;
  local_18 = (ITypeLib *)0x0;
  ExceptionList = &local_10;
  GetModuleFileNameA(*(HMODULE *)(param_1 + 0xc),local_128,0x104);
  if (param_2 != (LPCWSTR)0x0) {
    iVar1 = lstrlenW(param_2);
    FUN_10004bc0();
    lpString2 = FUN_10003b30(&stack0xfffffdd4,param_2,iVar1 * 2 + 2,0);
    lstrcatA(local_128,lpString2);
  }
  if (&stack0x00000000 == (undefined1 *)0x128) {
    szFile = (LPCOLESTR)0x0;
  }
  else {
    iVar1 = lstrlenA(local_128);
    FUN_10004bc0();
    szFile = String_MultiByteToWideChar((LPWSTR)&stack0xfffffdd4,local_128,iVar1 + 1,0);
  }
  HVar2 = LoadTypeLib(szFile,&local_18);
  if (HVar2 < 0) {
    lpString1 = (LPCSTR)0x0;
    lpsz = local_128;
    if (local_128[0] == '\0') {
LAB_100039ff:
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
      if (lpString1 == (LPCSTR)0x0) goto LAB_100039ff;
    }
    lstrcpyA(lpString1,&DAT_10008128);
    if (&stack0x00000000 == (undefined1 *)0x128) {
      szFile = (LPCOLESTR)0x0;
    }
    else {
      iVar1 = lstrlenA(local_128);
      FUN_10004bc0();
      szFile = String_MultiByteToWideChar((LPWSTR)&stack0xfffffdd4,local_128,iVar1 + 1,0);
    }
    HVar2 = LoadTypeLib(szFile,&local_18);
    if (HVar2 < 0) goto LAB_10003a66;
  }
  local_14 = SysAllocString(szFile);
LAB_10003a66:
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
    iVar1 = FUN_10003b70(local_220);
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


// ============================================================================
// Function: FUN_10003b30 at 10003b30
// Signature: LPSTR __stdcall FUN_10003b30(LPSTR param_1, LPCWSTR param_2, int param_3, UINT param_4)
// Entry: 10003b30
// Body: 10003b30 - 10003b5b
// ============================================================================
LPSTR FUN_10003b30(LPSTR param_1,LPCWSTR param_2,int param_3,UINT param_4)

{
  *param_1 = '\0';
  WideCharToMultiByte(param_4,0,param_2,-1,param_1,param_3,(LPCSTR)0x0,(LPBOOL)0x0);
  return param_1;
}


// ============================================================================
// Function: FUN_10003b60 at 10003b60
// Signature: undefined __fastcall FUN_10003b60(undefined4 * param_1)
// Entry: 10003b60
// Body: 10003b60 - 10003b69
// ============================================================================
void __fastcall FUN_10003b60(undefined4 *param_1)

{
  SysFreeString((BSTR)*param_1);
  return;
}


// ============================================================================
// Function: FUN_10003b70 at 10003b70
// Signature: int __stdcall FUN_10003b70(short * param_1)
// Entry: 10003b70
// Body: 10003b70 - 10003bab
// ============================================================================
int FUN_10003b70(short *param_1)

{
  short sVar1;
  short *psVar2;
  short *psVar3;
  
  sVar1 = *param_1;
  psVar2 = param_1;
  psVar3 = param_1;
  while (sVar1 != 0) {
    sVar1 = *psVar3;
    if (sVar1 != 0) {
      psVar3 = psVar3 + 1;
    }
    if (((sVar1 == 0x5c) || (sVar1 == 0x2f)) || (sVar1 == 0x3a)) {
      psVar2 = psVar3;
    }
    sVar1 = *psVar3;
  }
  return (int)psVar2 - (int)param_1 >> 1;
}


// ============================================================================
// Function: FUN_10003c40 at 10003c40
// Signature: HRESULT __thiscall FUN_10003c40(void * this, BSTR param_1)
// Entry: 10003c40
// Body: 10003c40 - 10003ed5
// ============================================================================
/* WARNING: Exceeded maximum restarts with more pending */

HRESULT __thiscall FUN_10003c40(void *this,BSTR param_1)

{
  ITypeInfo *pIVar1;
  ITypeInfo *pIVar2;
  HRESULT HVar3;
  BSTR pOVar4;
  int iVar5;
  UINT UVar6;
  ITypeInfo *unaff_EBX;
  UINT index;
  ITypeInfo *This;
  uint uVar7;
  int iVar8;
  FUNCDESC *pFuncDesc;
  ITypeInfo *pIStack_28;
  TYPEATTR *pTStack_24;
  ITypeInfo *pIStack_20;
  HRESULT local_1c;
  TYPEATTR *pTStack_18;
  ITypeInfo *pIStack_14;
  ITypeLib *pIStack_10;
  void *local_c;
  undefined *puStack_8;
  int iStack_4;
  
  iStack_4 = 0xffffffff;
  puStack_8 = &DAT_1000528b;
  local_c = ExceptionList;
  if (*(int *)((int)this + 0xc) == 0) {
    local_1c = -0x7fffbffb;
    ExceptionList = &local_c;
    EnterCriticalSection((LPCRITICAL_SECTION)&DAT_100082b4);
    if ((*(int *)((int)this + 0xc) == 0) &&
       (local_1c = LoadRegTypeLib(*(GUID **)((int)this + 4),*(WORD *)((int)this + 8),
                                  *(WORD *)((int)this + 10),(LCID)param_1,&pIStack_10),
       -1 < local_1c)) {
      pIStack_28 = (ITypeInfo *)0x0;
      iStack_4 = 0;
      local_1c = (*pIStack_10->lpVtbl->GetTypeInfoOfGuid)(pIStack_10,*(GUID **)this,&pIStack_28);
      pIVar1 = pIStack_28;
      if (-1 < local_1c) {
        pIStack_14 = pIStack_28;
        if (pIStack_28 != (ITypeInfo *)0x0) {
          (*pIStack_28->lpVtbl->AddRef)(pIStack_28);
        }
        pIStack_20 = (ITypeInfo *)0x0;
        iStack_4 = CONCAT31(iStack_4._1_3_,2);
        HVar3 = (*pIStack_28->lpVtbl->QueryInterface)(pIStack_28,(IID *)&DAT_10008130,&pIStack_20);
        This = pIVar1;
        pIVar2 = pIStack_20;
        if (-1 < HVar3) {
          if (unaff_EBX != (ITypeInfo *)0x0) {
            (*unaff_EBX->lpVtbl->AddRef)(unaff_EBX);
          }
          This = unaff_EBX;
          pIVar2 = unaff_EBX;
          if (pIVar1 != (ITypeInfo *)0x0) {
            (*pIVar1->lpVtbl->Release)(pIVar1);
            pIVar2 = unaff_EBX;
          }
        }
        pIStack_20 = pIVar2;
        HVar3 = (*This->lpVtbl->GetTypeAttr)(This,&pTStack_24);
        if (-1 < HVar3) {
          uVar7 = (uint)pTStack_18->cFuncs;
          *(uint *)((int)this + 0x18) = uVar7;
          if (uVar7 == 0) {
            pOVar4 = (BSTR)0x0;
          }
          else {
            param_1 = operator_new(uVar7 * 0xc + 4);
            iStack_4._0_1_ = 3;
            if (param_1 == (BSTR)0x0) {
              pOVar4 = (BSTR)0x0;
            }
            else {
              pOVar4 = param_1 + 2;
              *(uint *)param_1 = uVar7;
              FUN_10004ce3(pOVar4,0xc,uVar7,&LAB_10003fe0);
            }
            iStack_4 = CONCAT31(iStack_4._1_3_,2);
          }
          index = 0;
          *(BSTR *)((int)this + 0x14) = pOVar4;
          if (0 < *(int *)((int)this + 0x18)) {
            iVar8 = 0;
            do {
              HVar3 = (*This->lpVtbl->GetFuncDesc)(This,index,(FUNCDESC **)&pTStack_24);
              if (-1 < HVar3) {
                param_1 = (BSTR)0x0;
                pFuncDesc = (FUNCDESC *)0x0;
                iStack_4 = CONCAT31(iStack_4._1_3_,4);
                iVar5 = (*This->lpVtbl->GetDocumentation)
                                  (This,(pTStack_24->guid).Data1,&param_1,0,0);
                pIVar1 = pIStack_14;
                if (-1 < iVar5) {
                  pIStack_14 = (ITypeInfo *)0x0;
                  *(ITypeInfo **)(iVar8 + *(int *)((int)this + 0x14)) = pIVar1;
                  UVar6 = SysStringLen(*(BSTR *)(iVar8 + *(int *)((int)this + 0x14)));
                  *(UINT *)(iVar8 + 4 + *(int *)((int)this + 0x14)) = UVar6;
                  *(MEMBERID *)(iVar8 + 8 + *(int *)((int)this + 0x14)) = pFuncDesc->memid;
                }
                (*This->lpVtbl->ReleaseFuncDesc)(This,pFuncDesc);
                iStack_4 = CONCAT31(iStack_4._1_3_,2);
                SysFreeString(param_1);
              }
              index = index + 1;
              iVar8 = iVar8 + 0xc;
            } while ((int)index < *(int *)((int)this + 0x18));
          }
          (*This->lpVtbl->ReleaseTypeAttr)(This,pTStack_18);
        }
        *(ITypeInfo **)((int)this + 0xc) = This;
        pIStack_14 = (ITypeInfo *)0x0;
        iStack_4._0_1_ = 1;
        if (pIStack_20 != (ITypeInfo *)0x0) {
          (*pIStack_20->lpVtbl->Release)(pIStack_20);
        }
        iStack_4 = (uint)iStack_4._1_3_ << 8;
      }
      (*pIStack_10->lpVtbl->Release)(pIStack_10);
      iStack_4 = 0xffffffff;
      if (pIStack_28 != (ITypeInfo *)0x0) {
        (*pIStack_28->lpVtbl->Release)(pIStack_28);
      }
    }
    LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_100082b4);
    ComTypeInfo_AddDispatchMapEntry(0x10008298,FUN_10003f80,this);
    ExceptionList = local_c;
    return local_1c;
  }
  return 0;
}


// ============================================================================
// Function: FUN_10003f80 at 10003f80
// Signature: undefined __stdcall FUN_10003f80(Editor * param_1)
// Entry: 10003f80
// Body: 10003f80 - 10003fd4
// ============================================================================
void FUN_10003f80(Editor *param_1)

{
  int *piVar1;
  void *pvVar2;
  
  piVar1 = (param_1->secondaryDispatch).vtable;
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
  }
  pvVar2 = param_1->invalidateTarget;
  (param_1->secondaryDispatch).vtable = (void *)0x0;
  if (pvVar2 != (void *)0x0) {
    FUN_10004bef(pvVar2,0xc,*(int *)((int)pvVar2 + -4),FUN_10003b60);
    operator_delete((void *)((int)pvVar2 + -4));
    param_1->invalidateTarget = (void *)0x0;
    return;
  }
  param_1->invalidateTarget = (void *)0x0;
  return;
}


// ============================================================================
// Function: FUN_10004938 at 10004938
// Signature: undefined4 * __stdcall FUN_10004938(void)
// Entry: 10004938
// Body: 10004938 - 1000495f
// ============================================================================
undefined4 * FUN_10004938(void)

{
  AFX_MODULE_STATE::AFX_MODULE_STATE((AFX_MODULE_STATE *)&DAT_10008330,1,FUN_10004996,0x600);
  DAT_10008330 = &PTR_FUN_100068fc;
  return &DAT_10008330;
}


// ============================================================================
// Function: FUN_10004960 at 10004960
// Signature: AFX_MODULE_STATE * __thiscall FUN_10004960(void * this, byte param_1)
// Entry: 10004960
// Body: 10004960 - 1000497a
// ============================================================================
AFX_MODULE_STATE * __thiscall FUN_10004960(void *this,byte param_1)

{
  AFX_MODULE_STATE::~AFX_MODULE_STATE(this);
  if ((param_1 & 1) != 0) {
    CNoTrackObject::operator_delete(this);
  }
  return this;
}


// ============================================================================
// Function: FUN_10004980 at 10004980
// Signature: undefined __stdcall FUN_10004980(void)
// Entry: 10004980
// Body: 10004980 - 1000498b
// ============================================================================
void FUN_10004980(void)

{
  FUN_10004b9c((_onexit_t)&LAB_1000498c);
  return;
}


// ============================================================================
// Function: FUN_10004996 at 10004996
// Signature: undefined __stdcall FUN_10004996(HWND__ * param_1, uint param_2, uint param_3, long param_4)
// Entry: 10004996
// Body: 10004996 - 100049da
// ============================================================================
/* WARNING: Function: _EH_prolog replaced with injection: EH_prolog */

void FUN_10004996(HWND__ *param_1,uint param_2,uint param_3,long param_4)

{
  undefined4 local_18;
  int local_14;
  void *local_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &DAT_10005348;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  AFX_MAINTAIN_STATE2::AFX_MAINTAIN_STATE2
            ((AFX_MAINTAIN_STATE2 *)&local_18,(AFX_MODULE_STATE *)&DAT_10008330);
  local_8 = 0;
  AfxWndProc(param_1,param_2,param_3,param_4);
  *(undefined4 *)(local_14 + 4) = local_18;
  ExceptionList = local_10;
  return;
}


// ============================================================================
// Function: FUN_100049db at 100049db
// Signature: undefined4 * __stdcall FUN_100049db(void)
// Entry: 100049db
// Body: 100049db - 100049e0
// ============================================================================
undefined4 * FUN_100049db(void)

{
  return &DAT_10008330;
}


// ============================================================================
// Function: FUN_100049e1 at 100049e1
// Signature: undefined4 __stdcall FUN_100049e1(HINSTANCE__ * param_1, int param_2)
// Entry: 100049e1
// Body: 100049e1 - 10004b07
// ============================================================================
int FUN_100049e1(HINSTANCE__ *param_1,int param_2)

{
  undefined4 uVar1;
  int *piVar2;
  _AFX_THREAD_STATE *p_Var3;
  int iVar4;
  AFX_MODULE_STATE *pAVar5;
  CDynLinkLibrary *this;
  undefined4 local_c;
  int local_8;
  
  if (param_2 != 1) {
    if (param_2 == 0) {
      pAVar5 = AfxSetModuleState((AFX_MODULE_STATE *)&DAT_10008330);
      p_Var3 = AfxGetThreadState();
      *(AFX_MODULE_STATE **)(p_Var3 + 8) = pAVar5;
      pAVar5 = AfxGetModuleState();
      if (*(int **)(pAVar5 + 4) != (int *)0x0) {
        (**(code **)(**(int **)(pAVar5 + 4) + 0x70))();
      }
      AfxLockTempMaps();
      AfxUnlockTempMaps(-1);
      AfxWinTerm();
      AfxTermExtensionModule((AFX_EXTENSION_MODULE *)&DAT_100093c0,1);
    }
    else if (param_2 == 3) {
      AFX_MAINTAIN_STATE2::AFX_MAINTAIN_STATE2
                ((AFX_MAINTAIN_STATE2 *)&local_c,(AFX_MODULE_STATE *)&DAT_10008330);
      AfxLockTempMaps();
      AfxUnlockTempMaps(-1);
      AfxTermThread(param_1);
      *(undefined4 *)(local_8 + 4) = local_c;
    }
    return 1;
  }
  param_2 = 0;
  AfxCoreInitModule();
  p_Var3 = AfxGetThreadState();
  uVar1 = *(undefined4 *)(p_Var3 + 8);
  iVar4 = AfxWinInit(param_1,(HINSTANCE__ *)0x0,&DAT_1000831c,0);
  if (iVar4 != 0) {
    pAVar5 = AfxGetModuleState();
    piVar2 = *(int **)(pAVar5 + 4);
    if ((piVar2 == (int *)0x0) || (iVar4 = (**(code **)(*piVar2 + 0x58))(), iVar4 != 0)) {
      *(undefined4 *)(p_Var3 + 8) = uVar1;
      AfxInitExtensionModule((AFX_EXTENSION_MODULE *)&DAT_100093c0,param_1);
      this = operator_new(0x40);
      if (this != (CDynLinkLibrary *)0x0) {
        CDynLinkLibrary::CDynLinkLibrary(this,(AFX_EXTENSION_MODULE *)&DAT_100093c0,0);
      }
      param_2 = 1;
      goto LAB_10004a6d;
    }
    (**(code **)(*piVar2 + 0x70))();
  }
  AfxWinTerm();
LAB_10004a6d:
  *(undefined4 *)(p_Var3 + 8) = uVar1;
  p_Var3 = AfxGetThreadState();
  AfxSetModuleState(*(AFX_MODULE_STATE **)(p_Var3 + 8));
  return param_2;
}


// ============================================================================
// Function: FUN_10004b08 at 10004b08
// Signature: undefined4 __stdcall FUN_10004b08(undefined4 param_1, int param_2)
// Entry: 10004b08
// Body: 10004b08 - 10004b58
// ============================================================================
undefined4 FUN_10004b08(undefined4 param_1,int param_2)

{
  HLOCAL hMem;
  _AFX_THREAD_STATE *p_Var1;
  AFX_MODULE_STATE *pAVar2;
  
  if (param_2 == 1) {
    hMem = LocalAlloc(0,0x2000);
    if (hMem == (HLOCAL)0x0) {
      return 0;
    }
    LocalFree(hMem);
    p_Var1 = AfxGetThreadState();
    pAVar2 = AfxSetModuleState((AFX_MODULE_STATE *)&DAT_10008330);
    *(AFX_MODULE_STATE **)(p_Var1 + 8) = pAVar2;
  }
  else if (param_2 == 0) {
    p_Var1 = AfxGetThreadState();
    AfxSetModuleState(*(AFX_MODULE_STATE **)(p_Var1 + 8));
  }
  return 1;
}


// ============================================================================
// Function: FUN_10004b59 at 10004b59
// Signature: undefined __fastcall FUN_10004b59(undefined4 * param_1)
// Entry: 10004b59
// Body: 10004b59 - 10004b61
// ============================================================================
void __fastcall FUN_10004b59(undefined4 *param_1)

{
  *(undefined4 *)(param_1[1] + 4) = *param_1;
  return;
}


// ============================================================================
// Function: FUN_10004b70 at 10004b70
// Signature: undefined __cdecl FUN_10004b70(_onexit_t param_1)
// Entry: 10004b70
// Body: 10004b70 - 10004b9b
// ============================================================================
void __cdecl FUN_10004b70(_onexit_t param_1)

{
  if (DAT_100093e8 == -1) {
    _onexit(param_1);
    return;
  }
  __dllonexit(param_1,&DAT_100093e8,&DAT_100093e4);
  return;
}


// ============================================================================
// Function: FUN_10004b9c at 10004b9c
// Signature: int __cdecl FUN_10004b9c(_onexit_t param_1)
// Entry: 10004b9c
// Body: 10004b9c - 10004bad
// ============================================================================
int __cdecl FUN_10004b9c(_onexit_t param_1)

{
  int iVar1;
  
  iVar1 = FUN_10004b70(param_1);
  return (iVar1 != 0) - 1;
}


// ============================================================================
// Function: FUN_10004bc0 at 10004bc0
// Signature: undefined __stdcall FUN_10004bc0(void)
// Entry: 10004bc0
// Body: 10004bc0 - 10004bee
// ============================================================================
/* WARNING: Unable to track spacebase fully for stack */

void FUN_10004bc0(void)

{
  uint in_EAX;
  undefined1 *puVar1;
  undefined4 unaff_retaddr;
  
  puVar1 = &stack0x00000004;
  for (; 0xfff < in_EAX; in_EAX = in_EAX - 0x1000) {
    puVar1 = puVar1 + -0x1000;
  }
  *(undefined4 *)(puVar1 + (-4 - in_EAX)) = unaff_retaddr;
  return;
}


// ============================================================================
// Function: FUN_10004bef at 10004bef
// Signature: undefined __stdcall FUN_10004bef(undefined4 param_1, undefined4 param_2, int param_3, undefined * param_4)
// Entry: 10004bef
// Body: 10004bef - 10004c56
// ============================================================================
void FUN_10004bef(undefined4 param_1,undefined4 param_2,int param_3,undefined *param_4)

{
  void *local_14;
  undefined *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_10006900;
  puStack_10 = &DAT_10004ed6;
  local_14 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_14;
  while( true ) {
    param_3 = param_3 + -1;
    if (param_3 < 0) break;
    (*(code *)param_4)();
  }
  local_8 = 0xffffffff;
  FUN_10004c57();
  ExceptionList = local_14;
  return;
}


// ============================================================================
// Function: FUN_10004c57 at 10004c57
// Signature: undefined __stdcall FUN_10004c57(void)
// Entry: 10004c57
// Body: 10004c57 - 10004c6e
// ============================================================================
void FUN_10004c57(void)

{
  int unaff_EBP;
  
  if (*(int *)(unaff_EBP + -0x1c) == 0) {
    FUN_10004c6f(*(undefined4 *)(unaff_EBP + 8),*(undefined4 *)(unaff_EBP + 0xc),
                 *(int *)(unaff_EBP + 0x10),*(undefined **)(unaff_EBP + 0x14));
  }
  return;
}


// ============================================================================
// Function: FUN_10004c6f at 10004c6f
// Signature: undefined __stdcall FUN_10004c6f(undefined4 param_1, undefined4 param_2, int param_3, undefined * param_4)
// Entry: 10004c6f
// Body: 10004c6f - 10004ccc
// ============================================================================
void FUN_10004c6f(undefined4 param_1,undefined4 param_2,int param_3,undefined *param_4)

{
  void *local_14;
  undefined *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_10006910;
  puStack_10 = &DAT_10004ed6;
  local_14 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_14;
  while( true ) {
    param_3 = param_3 + -1;
    if (param_3 < 0) break;
    (*(code *)param_4)();
  }
  ExceptionList = local_14;
  return;
}


// ============================================================================
// Function: FUN_10004ccd at 10004ccd
// Signature: undefined4 __cdecl FUN_10004ccd(undefined4 * param_1)
// Entry: 10004ccd
// Body: 10004ccd - 10004ce2
// ============================================================================
undefined4 __cdecl FUN_10004ccd(undefined4 *param_1)

{
  if (*(int *)*param_1 != -0x1f928c9d) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  terminate();
}


// ============================================================================
// Function: FUN_10004ce3 at 10004ce3
// Signature: undefined __stdcall FUN_10004ce3(undefined4 param_1, undefined4 param_2, int param_3, undefined * param_4)
// Entry: 10004ce3
// Body: 10004ce3 - 10004d4c
// ============================================================================
void FUN_10004ce3(undefined4 param_1,undefined4 param_2,int param_3,undefined *param_4)

{
  int local_20;
  void *local_14;
  undefined *puStack_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  puStack_c = &DAT_10006920;
  puStack_10 = &DAT_10004ed6;
  local_14 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_14;
  for (local_20 = 0; local_20 < param_3; local_20 = local_20 + 1) {
    (*(code *)param_4)();
  }
  local_8 = 0xffffffff;
  FUN_10004d4d();
  ExceptionList = local_14;
  return;
}


// ============================================================================
// Function: FUN_10004d4d at 10004d4d
// Signature: undefined __stdcall FUN_10004d4d(void)
// Entry: 10004d4d
// Body: 10004d4d - 10004d64
// ============================================================================
void FUN_10004d4d(void)

{
  int unaff_EBP;
  
  if (*(int *)(unaff_EBP + -0x20) == 0) {
    FUN_10004c6f(*(undefined4 *)(unaff_EBP + 8),*(undefined4 *)(unaff_EBP + 0xc),
                 *(int *)(unaff_EBP + -0x1c),*(undefined **)(unaff_EBP + 0x18));
  }
  return;
}


// ============================================================================
// Function: FUN_10004d65 at 10004d65
// Signature: undefined4 __stdcall FUN_10004d65(undefined4 param_1, int param_2)
// Entry: 10004d65
// Body: 10004d65 - 10004e0f
// ============================================================================
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_10004d65(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  undefined4 *_Memory;
  undefined4 *puVar2;
  
  if (param_2 == 0) {
    if (0 < DAT_100093dc) {
      DAT_100093dc = DAT_100093dc + -1;
      goto LAB_10004d7b;
    }
LAB_10004da3:
    uVar1 = 0;
  }
  else {
LAB_10004d7b:
    _DAT_100093e0 = *(undefined4 *)_adjust_fdiv_exref;
    if (param_2 == 1) {
      DAT_100093e8 = malloc(0x80);
      if (DAT_100093e8 == (undefined4 *)0x0) goto LAB_10004da3;
      *DAT_100093e8 = 0;
      DAT_100093e4 = DAT_100093e8;
      initterm(&DAT_10008000,&DAT_1000800c);
      DAT_100093dc = DAT_100093dc + 1;
    }
    else if ((param_2 == 0) &&
            (_Memory = DAT_100093e8, puVar2 = DAT_100093e4, DAT_100093e8 != (undefined4 *)0x0)) {
      while (puVar2 = puVar2 + -1, _Memory <= puVar2) {
        if ((code *)*puVar2 != (code *)0x0) {
          (*(code *)*puVar2)();
          _Memory = DAT_100093e8;
        }
      }
      free(_Memory);
      DAT_100093e8 = (undefined4 *)0x0;
    }
    uVar1 = 1;
  }
  return uVar1;
}


// ============================================================================
// Function: FUN_10004ead at 10004ead
// Signature: type_info * __thiscall FUN_10004ead(void * this, byte param_1)
// Entry: 10004ead
// Body: 10004ead - 10004ec8
// ============================================================================
type_info * __thiscall FUN_10004ead(void *this,byte param_1)

{
  type_info::~type_info(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
