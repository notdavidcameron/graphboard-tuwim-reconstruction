/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: AudioArchive_WriteFileRange @ 10003b40
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: AudioArchive_WriteFileRange at 10003b40
// Signature: undefined4 __stdcall AudioArchive_WriteFileRange(CArchive * param_1, int * param_2, uint param_3)
// Entry: 10003b40
// Body: 10003b40 - 10003be2
// ============================================================================
undefined4 AudioArchive_WriteFileRange(CArchive *param_1,int *param_2,uint param_3)

{
  HGLOBAL pvVar1;
  LPVOID pMem;
  uint uVar2;
  uint uVar3;
  
  pvVar1 = GlobalAlloc(2,0xffff);
  pMem = GlobalLock(pvVar1);
  if (pMem == (LPVOID)0x0) {
    return 0;
  }
  if (param_3 < 0x10000) {
    uVar3 = param_3;
    if (param_3 == 0) goto LAB_10003bb4;
  }
  else {
    uVar3 = 0xffff;
  }
  do {
    while( true ) {
      uVar2 = (**(code **)(*param_2 + 0x3c))(pMem,uVar3);
      if (uVar2 != uVar3) {
        return 0;
      }
      CArchive::Write(param_1,pMem,uVar3);
      param_3 = param_3 - uVar3;
      if (param_3 < 0x10000) break;
      uVar3 = 0xffff;
    }
    uVar3 = param_3;
  } while (param_3 != 0);
LAB_10003bb4:
  CArchive::Flush(param_1);
  pvVar1 = GlobalHandle(pMem);
  GlobalUnlock(pvVar1);
  pvVar1 = GlobalHandle(pMem);
  GlobalFree(pvVar1);
  return 1;
}

// clang-format on
