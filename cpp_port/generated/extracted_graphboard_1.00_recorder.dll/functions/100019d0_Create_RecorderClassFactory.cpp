/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: Create_RecorderClassFactory @ 100019d0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Create_RecorderClassFactory at 100019d0
// Signature: undefined __stdcall Create_RecorderClassFactory(void)
// Entry: 100019d0
// Body: 100019d0 - 10001a3a
// ============================================================================
void Create_RecorderClassFactory(void)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_1000596b;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x3c);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    RecorderRefCountedState_Construct(puVar1 + 1);
    *puVar1 = &PTR_LAB_10006810;
    InitializeCriticalSection((LPCRITICAL_SECTION)(puVar1 + 9));
    *puVar1 = &PTR_Recorder_QueryInterfaceFromMap_100067fc;
    FUN_10001a44();
    return;
  }
  FUN_10001a44();
  return;
}

// clang-format on
