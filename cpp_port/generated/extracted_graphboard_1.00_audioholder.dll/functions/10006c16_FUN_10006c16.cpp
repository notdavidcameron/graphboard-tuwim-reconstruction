/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10006c16 @ 10006c16
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10006c16 at 10006c16
// Signature: undefined __stdcall FUN_10006c16(HWND__ * param_1, uint param_2, uint param_3, long param_4)
// Entry: 10006c16
// Body: 10006c16 - 10006c5a
// ============================================================================
/* WARNING: Function: _EH_prolog replaced with injection: EH_prolog */

void FUN_10006c16(HWND__ *param_1,uint param_2,uint param_3,long param_4)

{
  undefined4 local_18;
  int local_14;
  void *local_10;
  undefined *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &DAT_1000776c;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  AFX_MAINTAIN_STATE2::AFX_MAINTAIN_STATE2
            ((AFX_MAINTAIN_STATE2 *)&local_18,(AFX_MODULE_STATE *)&DAT_1000b4f8);
  local_8 = 0;
  AfxWndProc(param_1,param_2,param_3,param_4);
  *(undefined4 *)(local_14 + 4) = local_18;
  ExceptionList = local_10;
  return;
}

// clang-format on
