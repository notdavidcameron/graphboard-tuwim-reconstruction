/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_10002a70 @ 10002a70
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10002a70 at 10002a70
// Signature: undefined __fastcall FUN_10002a70(undefined4 * param_1)
// Entry: 10002a70
// Body: 10002a70 - 10002aaf
// ============================================================================
void __fastcall FUN_10002a70(undefined4 *param_1)

{
  *param_1 = &PTR_LAB_100067c0;
  param_1[1] = 1;
  InterlockedDecrement(&DAT_100082ac);
  if (param_1 != (undefined4 *)0xfffffff4) {
    DeleteCriticalSection((LPCRITICAL_SECTION)(param_1 + 6));
    return;
  }
  DeleteCriticalSection((LPCRITICAL_SECTION)0x4);
  return;
}

// clang-format on
