/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: AudioArchive_ReadFileRange @ 10003bf0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: AudioArchive_ReadFileRange at 10003bf0
// Signature: undefined4 __stdcall AudioArchive_ReadFileRange(int * param_1, CArchive * param_2, uint param_3)
// Entry: 10003bf0
// Body: 10003bf0 - 10003c91
// ============================================================================
undefined4 AudioArchive_ReadFileRange(int *param_1,CArchive *param_2,uint param_3)

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
    if (param_3 == 0) goto LAB_10003c64;
  }
  else {
    uVar3 = 0xffff;
  }
  do {
    while( true ) {
      uVar2 = CArchive::Read(param_2,pMem,uVar3);
      if (uVar2 != uVar3) {
        return 0;
      }
      (**(code **)(*param_1 + 0x40))(pMem,uVar3);
      param_3 = param_3 - uVar3;
      if (param_3 < 0x10000) break;
      uVar3 = 0xffff;
    }
    uVar3 = param_3;
  } while (param_3 != 0);
LAB_10003c64:
  (**(code **)(*param_1 + 0x50))();
  pvVar1 = GlobalHandle(pMem);
  GlobalUnlock(pvVar1);
  pvVar1 = GlobalHandle(pMem);
  GlobalFree(pvVar1);
  return 1;
}

// clang-format on
