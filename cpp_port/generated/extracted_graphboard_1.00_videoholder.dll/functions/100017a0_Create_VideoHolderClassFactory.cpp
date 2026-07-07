/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: Create_VideoHolderClassFactory @ 100017a0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Create_VideoHolderClassFactory at 100017a0
// Signature: undefined __stdcall Create_VideoHolderClassFactory(void)
// Entry: 100017a0
// Body: 100017a0 - 1000180a
// ============================================================================
void Create_VideoHolderClassFactory(void)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_1000696b;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x3c);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    VideoHolderRefCountedState_Construct(puVar1 + 1);
    *puVar1 = &PTR_LAB_100077b8;
    InitializeCriticalSection((LPCRITICAL_SECTION)(puVar1 + 9));
    *puVar1 = &PTR_VideoHolder_QueryInterfaceFromMap_100077a4;
    FUN_10001814();
    return;
  }
  FUN_10001814();
  return;
}

// clang-format on
