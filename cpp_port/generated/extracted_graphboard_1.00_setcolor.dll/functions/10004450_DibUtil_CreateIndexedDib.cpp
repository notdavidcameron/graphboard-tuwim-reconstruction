/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: DibUtil_CreateIndexedDib @ 10004450
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: DibUtil_CreateIndexedDib at 10004450
// Signature: undefined4 * __stdcall DibUtil_CreateIndexedDib(int param_1, int param_2, int param_3)
// Entry: 10004450
// Body: 10004450 - 100045a8
// ============================================================================
undefined4 * DibUtil_CreateIndexedDib(int param_1,int param_2,int param_3)

{
  HGLOBAL hMem;
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  
  iVar3 = (((int)(param_1 * param_2 + (param_1 * param_2 >> 0x1f & 7U)) >> 3) + 3U & 0xfffffffc) *
          param_3;
  hMem = GlobalAlloc(0x42,iVar3 + 0x428);
  puVar1 = GlobalLock(hMem);
  if (puVar1 == (undefined4 *)0x0) {
    return (undefined4 *)0x0;
  }
  *puVar1 = 0x28;
  puVar1[1] = param_2;
  puVar1[2] = param_3;
  *(undefined2 *)(puVar1 + 3) = 1;
  *(short *)((int)puVar1 + 0xe) = (short)param_1;
  puVar1[4] = 0;
  puVar1[5] = iVar3;
  puVar1[6] = 0;
  puVar1[7] = 0;
  puVar1[8] = 0;
  puVar1[9] = 0;
  if (param_1 == 4) {
    puVar1[8] = 0x10;
  }
  else if (param_1 == 8) {
    puVar1[8] = 0x100;
  }
  iVar3 = 0;
  puVar2 = puVar1 + 10;
  if (0 < (int)(puVar1[8] + ((int)puVar1[8] >> 0x1f & 0xfU)) >> 4) {
    do {
      *puVar2 = 0;
      puVar2[1] = 0x800000;
      puVar2[2] = 0x8000;
      puVar2[3] = 0x808000;
      puVar2[4] = 0x80;
      puVar2[5] = 0x800080;
      puVar2[6] = 0x8080;
      puVar2[7] = 0xc0c0c0;
      puVar2[8] = 0x808080;
      puVar2[9] = 0xff0000;
      puVar2[10] = 0xff00;
      puVar2[0xb] = 0xffff00;
      puVar2[0xc] = 0xff;
      puVar2[0xd] = 0xff00ff;
      puVar2[0xe] = 0xffff;
      puVar2[0xf] = 0xffffff;
      puVar2 = puVar2 + 0x10;
      iVar3 = iVar3 + 1;
    } while (iVar3 < (int)(puVar1[8] + ((int)puVar1[8] >> 0x1f & 0xfU)) >> 4);
  }
  return puVar1;
}

// clang-format on
