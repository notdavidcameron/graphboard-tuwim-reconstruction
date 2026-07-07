/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10004d50 @ 10004d50
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10004d50 at 10004d50
// Signature: CPropertyPage * __stdcall FUN_10004d50(void)
// Entry: 10004d50
// Body: 10004d50 - 10004da9
// ============================================================================
CPropertyPage * FUN_10004d50(void)

{
  CPropertyPage *pCVar1;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10005b8a;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  pCVar1 = operator_new(0xd4);
  local_4 = 0;
  if (pCVar1 != (CPropertyPage *)0x0) {
    pCVar1 = FUN_10004dd0(pCVar1);
    ExceptionList = local_c;
    return pCVar1;
  }
  ExceptionList = local_c;
  return (CPropertyPage *)0x0;
}

// clang-format on
