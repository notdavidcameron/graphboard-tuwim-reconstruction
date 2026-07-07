/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_100028d0 @ 100028d0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100028d0 at 100028d0
// Signature: undefined __fastcall FUN_100028d0(undefined4 * param_1)
// Entry: 100028d0
// Body: 100028d0 - 10002907
// ============================================================================
void __fastcall FUN_100028d0(undefined4 *param_1)

{
  *param_1 = &PTR_LAB_100067a0;
  param_1[1] = &PTR_LAB_10006790;
  param_1[2] = 1;
  InterlockedDecrement(&DAT_100082ac);
  DeleteCriticalSection
            ((LPCRITICAL_SECTION)((-(uint)(param_1 != (undefined4 *)0x0) & (uint)(param_1 + 2)) + 4)
            );
  return;
}

// clang-format on
