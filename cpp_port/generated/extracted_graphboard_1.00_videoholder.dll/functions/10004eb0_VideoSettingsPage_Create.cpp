/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: VideoSettingsPage_Create @ 10004eb0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: VideoSettingsPage_Create at 10004eb0
// Signature: CPropertyPage * __stdcall VideoSettingsPage_Create(void)
// Entry: 10004eb0
// Body: 10004eb0 - 10004f09
// ============================================================================
CPropertyPage * VideoSettingsPage_Create(void)

{
  CPropertyPage *pCVar1;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_10006c0a;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  pCVar1 = operator_new(800);
  local_4 = 0;
  if (pCVar1 != (CPropertyPage *)0x0) {
    pCVar1 = FUN_10004f30(pCVar1);
    ExceptionList = local_c;
    return pCVar1;
  }
  ExceptionList = local_c;
  return (CPropertyPage *)0x0;
}

// clang-format on
