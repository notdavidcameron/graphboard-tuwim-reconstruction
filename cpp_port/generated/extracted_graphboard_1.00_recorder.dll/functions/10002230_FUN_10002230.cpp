/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10002230 @ 10002230
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10002230 at 10002230
// Signature: undefined __stdcall FUN_10002230(undefined4 param_1)
// Entry: 10002230
// Body: 10002230 - 100022c0
// ============================================================================
void FUN_10002230(undefined4 param_1)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_100059ab;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x90);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    puVar1[1] = 0;
    RecorderContext_Construct(puVar1 + 2);
    puVar1[2] = &PTR_LAB_100069b4;
    puVar1[3] = &PTR_LAB_1000697c;
    puVar1[4] = &PTR_LAB_1000694c;
    puVar1[5] = &PTR_LAB_1000693c;
    puVar1[6] = param_1;
    *puVar1 = &PTR_LAB_10006930;
    InterlockedIncrement(&DAT_100083ac);
    FUN_100022cc();
    return;
  }
  FUN_100022cc();
  return;
}

// clang-format on
