/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: Create_RecorderComObject @ 10001cc0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Create_RecorderComObject at 10001cc0
// Signature: undefined __stdcall Create_RecorderComObject(void)
// Entry: 10001cc0
// Body: 10001cc0 - 10001d3c
// ============================================================================
void Create_RecorderComObject(void)

{
  undefined4 *puVar1;
  void *pvStack_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_1000598b;
  pvStack_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &pvStack_10;
  puVar1 = operator_new(0x88);
  local_8 = CONCAT31(local_8._1_3_,1);
  if (puVar1 != (undefined4 *)0x0) {
    RecorderContext_Construct(puVar1);
    *puVar1 = &PTR_LAB_100068b8;
    puVar1[1] = &PTR_LAB_10006880;
    puVar1[2] = &PTR_LAB_10006850;
    puVar1[3] = &PTR_LAB_10006840;
    InterlockedIncrement(&DAT_100083ac);
    FUN_10001d46();
    return;
  }
  FUN_10001d46();
  return;
}

// clang-format on
