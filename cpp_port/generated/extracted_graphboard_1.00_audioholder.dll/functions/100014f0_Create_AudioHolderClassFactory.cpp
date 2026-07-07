/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: Create_AudioHolderClassFactory @ 100014f0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Create_AudioHolderClassFactory at 100014f0
// Signature: undefined __stdcall Create_AudioHolderClassFactory(void)
// Entry: 100014f0
// Body: 100014f0 - 1000155a
// ============================================================================
void Create_AudioHolderClassFactory(void)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_100071cb;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x3c);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    AudioHolderRefCountedState_Construct(puVar1 + 1);
    *puVar1 = &PTR_LAB_10008668;
    InitializeCriticalSection((LPCRITICAL_SECTION)(puVar1 + 9));
    *puVar1 = &PTR_AudioHolder_QueryInterfaceFromMap_10008654;
    FUN_10001564();
    return;
  }
  FUN_10001564();
  return;
}

// clang-format on
