/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_10002740 @ 10002740
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10002740 at 10002740
// Signature: undefined __stdcall FUN_10002740(undefined4 param_1)
// Entry: 10002740
// Body: 10002740 - 100027bd
// ============================================================================
void FUN_10002740(undefined4 param_1)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_1000517b;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x30);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    puVar1[1] = 0;
    SetColorObject_Construct(puVar1 + 3);
    puVar1[3] = &PTR_LAB_100067dc;
    puVar1[4] = &PTR_LAB_100067cc;
    puVar1[5] = param_1;
    *puVar1 = &PTR_LAB_100067c0;
    InterlockedIncrement(&DAT_100082ac);
    FUN_100027c9();
    return;
  }
  FUN_100027c9();
  return;
}

// clang-format on
