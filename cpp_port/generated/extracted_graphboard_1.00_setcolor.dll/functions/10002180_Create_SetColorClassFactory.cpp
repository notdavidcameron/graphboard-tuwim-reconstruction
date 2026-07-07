/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: Create_SetColorClassFactory @ 10002180
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Create_SetColorClassFactory at 10002180
// Signature: undefined __stdcall Create_SetColorClassFactory(void)
// Entry: 10002180
// Body: 10002180 - 100021ea
// ============================================================================
void Create_SetColorClassFactory(void)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_1000513b;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x3c);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    SetColorRefCountedState_Construct(puVar1 + 1);
    *puVar1 = &PTR_LAB_10006760;
    InitializeCriticalSection((LPCRITICAL_SECTION)(puVar1 + 9));
    *puVar1 = &PTR_SetColor_QueryInterfaceFromMap_1000674c;
    FUN_100021f4();
    return;
  }
  FUN_100021f4();
  return;
}

// clang-format on
