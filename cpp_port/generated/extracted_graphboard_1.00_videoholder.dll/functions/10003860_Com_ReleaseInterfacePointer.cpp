/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: Com_ReleaseInterfacePointer @ 10003860
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Com_ReleaseInterfacePointer at 10003860
// Signature: undefined __fastcall Com_ReleaseInterfacePointer(int * param_1)
// Entry: 10003860
// Body: 10003860 - 1000386c
// ============================================================================
void __fastcall Com_ReleaseInterfacePointer(int *param_1)

{
  int *piVar1;
  
  piVar1 = (int *)*param_1;
  if (piVar1 != (int *)0x0) {
    (**(code **)(*piVar1 + 8))(piVar1);
  }
  return;
}

// clang-format on
