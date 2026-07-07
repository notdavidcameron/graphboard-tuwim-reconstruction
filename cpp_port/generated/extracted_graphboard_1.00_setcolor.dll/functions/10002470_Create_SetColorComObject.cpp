/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: Create_SetColorComObject @ 10002470
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Create_SetColorComObject at 10002470
// Signature: undefined __stdcall Create_SetColorComObject(void)
// Entry: 10002470
// Body: 10002470 - 100024db
// ============================================================================
void Create_SetColorComObject(void)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_1000515b;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x24);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    SetColorObject_Construct(puVar1);
    *puVar1 = &PTR_LAB_100067a0;
    puVar1[1] = &PTR_LAB_10006790;
    InterlockedIncrement(&DAT_100082ac);
    FUN_100024e5();
    return;
  }
  FUN_100024e5();
  return;
}

// clang-format on
