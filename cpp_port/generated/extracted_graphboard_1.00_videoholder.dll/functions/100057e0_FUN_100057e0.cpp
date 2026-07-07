/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_100057e0 @ 100057e0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100057e0 at 100057e0
// Signature: undefined4 __stdcall FUN_100057e0(char * param_1, char * param_2, char * param_3)
// Entry: 100057e0
// Body: 100057e0 - 100059d5
// ============================================================================
undefined4 FUN_100057e0(char *param_1,char *param_2,char *param_3)

{
  char *pcVar1;
  int iVar2;
  HGLOBAL pvVar3;
  LPVOID pMem;
  uint uVar4;
  CString *this;
  CString *this_00;
  CFile local_2c [16];
  CFile local_1c [4];
  CFile local_18 [12];
  void *pvStack_c;
  undefined1 *local_8;
  int local_4;
  
  local_4 = 0xffffffff;
  local_8 = &LAB_10006dc0;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  CFile::CFile(local_1c);
  local_4 = 0;
  CFile::CFile(local_2c);
  pcVar1 = param_1;
  local_4._0_1_ = 1;
  iVar2 = CFile::Open(local_1c,param_1,0x1001,(CFileException *)0x0);
  if (iVar2 == 0) {
    CString::CString((CString *)&param_1);
    local_4._0_1_ = 2;
    CString::Format(this,(char *)&param_1);
    AfxMessageBox(param_2,0,0);
    CString::~CString((CString *)&param_2);
  }
  else {
    iVar2 = CFile::Open(local_2c,param_2,0,(CFileException *)0x0);
    if (iVar2 == 0) {
      CString::CString((CString *)&param_2);
      local_4._0_1_ = 3;
      CString::Format(this_00,(char *)&param_2);
      AfxMessageBox(param_3,0,0);
      CFile::Close(local_18);
      CFile::Remove(pcVar1);
      CString::~CString((CString *)&param_3);
    }
    else {
      pvVar3 = GlobalAlloc(2,0xffff);
      pMem = GlobalLock(pvVar3);
      if (pMem != (LPVOID)0x0) {
        uVar4 = CFile::Read(local_2c,pMem,0xffff);
        while (uVar4 != 0) {
          CFile::Write(local_1c,pMem,uVar4);
          uVar4 = CFile::Read(local_2c,pMem,0xffff);
        }
        pvVar3 = GlobalHandle(pMem);
        GlobalUnlock(pvVar3);
        pvVar3 = GlobalHandle(pMem);
        GlobalFree(pvVar3);
        CFile::Close(local_1c);
        CFile::Close(local_2c);
        local_4 = (uint)local_4._1_3_ << 8;
        CFile::~CFile(local_2c);
        local_4 = 0xffffffff;
        CFile::~CFile(local_1c);
        ExceptionList = pvStack_c;
        return 1;
      }
      CFile::Close(local_1c);
      CFile::Close(local_2c);
      CFile::Remove(pcVar1);
      AfxMessageBox(s_End_of_memory_1000a218,0,0);
    }
  }
  local_4 = (uint)local_4._1_3_ << 8;
  CFile::~CFile(local_2c);
  local_4 = 0xffffffff;
  CFile::~CFile(local_1c);
  ExceptionList = pvStack_c;
  return 0;
}

// clang-format on
