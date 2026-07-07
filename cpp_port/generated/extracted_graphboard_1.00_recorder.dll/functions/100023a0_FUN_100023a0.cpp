/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_100023a0 @ 100023a0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100023a0 at 100023a0
// Signature: undefined __fastcall FUN_100023a0(char * param_1)
// Entry: 100023a0
// Body: 100023a0 - 100023d8
// ============================================================================
void __fastcall FUN_100023a0(char *param_1)

{
  *(undefined ***)param_1 = &PTR_LAB_100068b8;
  *(undefined ***)(param_1 + 4) = &PTR_LAB_10006880;
  *(undefined ***)(param_1 + 8) = &PTR_LAB_10006850;
  *(undefined ***)(param_1 + 0xc) = &PTR_LAB_10006840;
  param_1[0x10] = '\x01';
  param_1[0x11] = '\0';
  param_1[0x12] = '\0';
  param_1[0x13] = '\0';
  InterlockedDecrement(&DAT_100083ac);
  RecorderContext_Destroy(param_1);
  return;
}

// clang-format on
