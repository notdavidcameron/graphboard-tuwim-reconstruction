/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10001d50 @ 10001d50
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001d50 at 10001d50
// Signature: undefined __stdcall FUN_10001d50(undefined4 param_1)
// Entry: 10001d50
// Body: 10001d50 - 10001de0
// ============================================================================
void FUN_10001d50(undefined4 param_1)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_1000720b;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x244);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    puVar1[1] = 0;
    AudioHolderContext_Construct(puVar1 + 3);
    puVar1[3] = &PTR_LAB_100087dc;
    puVar1[4] = &PTR_LAB_100087ac;
    puVar1[5] = &PTR_LAB_1000878c;
    puVar1[6] = &PTR_LAB_1000877c;
    puVar1[7] = param_1;
    *puVar1 = &PTR_LAB_10008770;
    InterlockedIncrement(&DAT_1000b47c);
    FUN_10001dec();
    return;
  }
  FUN_10001dec();
  return;
}

// clang-format on
