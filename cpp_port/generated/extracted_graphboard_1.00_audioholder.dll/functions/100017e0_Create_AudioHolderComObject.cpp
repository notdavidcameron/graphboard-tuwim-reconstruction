/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: Create_AudioHolderComObject @ 100017e0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Create_AudioHolderComObject at 100017e0
// Signature: undefined __stdcall Create_AudioHolderComObject(void)
// Entry: 100017e0
// Body: 100017e0 - 1000185c
// ============================================================================
void Create_AudioHolderComObject(void)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_100071eb;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x238);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    AudioHolderContext_Construct(puVar1);
    *puVar1 = &PTR_LAB_100086f8;
    puVar1[1] = &PTR_LAB_100086c8;
    puVar1[2] = &PTR_LAB_100086a8;
    puVar1[3] = &PTR_LAB_10008698;
    InterlockedIncrement(&DAT_1000b47c);
    FUN_10001866();
    return;
  }
  FUN_10001866();
  return;
}

// clang-format on
