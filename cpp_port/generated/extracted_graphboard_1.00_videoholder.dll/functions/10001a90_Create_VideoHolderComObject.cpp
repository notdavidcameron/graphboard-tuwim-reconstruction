/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: Create_VideoHolderComObject @ 10001a90
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Create_VideoHolderComObject at 10001a90
// Signature: undefined __stdcall Create_VideoHolderComObject(void)
// Entry: 10001a90
// Body: 10001a90 - 10001b0c
// ============================================================================
void Create_VideoHolderComObject(void)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_1000698b;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x148);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    VideoHolderContext_Construct(puVar1);
    *puVar1 = &PTR_LAB_10007858;
    puVar1[1] = &PTR_LAB_1000781c;
    puVar1[2] = &PTR_LAB_100077f8;
    puVar1[3] = &PTR_LAB_100077e8;
    InterlockedIncrement(&DAT_1000a41c);
    FUN_10001b16();
    return;
  }
  FUN_10001b16();
  return;
}

// clang-format on
