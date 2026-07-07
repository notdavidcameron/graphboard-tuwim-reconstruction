/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: ComTypeInfo_AddDispatchMapEntry @ 10003ee0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: ComTypeInfo_AddDispatchMapEntry at 10003ee0
// Signature: undefined4 __stdcall ComTypeInfo_AddDispatchMapEntry(int param_1, undefined4 param_2, undefined4 param_3)
// Entry: 10003ee0
// Body: 10003ee0 - 10003f76
// ============================================================================
undefined4 ComTypeInfo_AddDispatchMapEntry(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_100052a0;
  local_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_10;
  puVar1 = operator_new(0xc);
  if (puVar1 == (undefined4 *)0x0) {
    ExceptionList = local_10;
    return 0x8007000e;
  }
  *puVar1 = param_2;
  puVar1[1] = param_3;
  EnterCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x1c));
  puVar1[2] = *(undefined4 *)(param_1 + 0x80);
  *(undefined4 **)(param_1 + 0x80) = puVar1;
  LeaveCriticalSection((LPCRITICAL_SECTION)(param_1 + 0x1c));
  ExceptionList = local_10;
  return 0;
}

// clang-format on
