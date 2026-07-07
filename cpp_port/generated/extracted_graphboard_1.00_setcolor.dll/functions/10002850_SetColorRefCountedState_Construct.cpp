/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: SetColorRefCountedState_Construct @ 10002850
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: SetColorRefCountedState_Construct at 10002850
// Signature: undefined4 * __fastcall SetColorRefCountedState_Construct(undefined4 * param_1)
// Entry: 10002850
// Body: 10002850 - 10002866
// ============================================================================
undefined4 * __fastcall SetColorRefCountedState_Construct(undefined4 *param_1)

{
  *param_1 = 0;
  InitializeCriticalSection((LPCRITICAL_SECTION)(param_1 + 1));
  return param_1;
}

// clang-format on
