/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10002000 @ 10002000
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10002000 at 10002000
// Signature: undefined __stdcall FUN_10002000(undefined4 param_1)
// Entry: 10002000
// Body: 10002000 - 10002090
// ============================================================================
void FUN_10002000(undefined4 param_1)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_100069ab;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x154);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    puVar1[1] = 0;
    VideoHolderContext_Construct(puVar1 + 3);
    puVar1[3] = &PTR_LAB_1000794c;
    puVar1[4] = &PTR_LAB_10007910;
    puVar1[5] = &PTR_LAB_100078ec;
    puVar1[6] = &PTR_LAB_100078dc;
    puVar1[7] = param_1;
    *puVar1 = &PTR_LAB_100078d0;
    InterlockedIncrement(&DAT_1000a41c);
    FUN_1000209c();
    return;
  }
  FUN_1000209c();
  return;
}

// clang-format on
