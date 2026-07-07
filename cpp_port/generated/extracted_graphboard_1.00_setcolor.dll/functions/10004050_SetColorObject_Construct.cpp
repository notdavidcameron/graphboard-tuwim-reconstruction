/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: SetColorObject_Construct @ 10004050
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: SetColorObject_Construct at 10004050
// Signature: undefined4 * __fastcall SetColorObject_Construct(undefined4 * param_1)
// Entry: 10004050
// Body: 10004050 - 1000407f
// ============================================================================
undefined4 * __fastcall SetColorObject_Construct(undefined4 *param_1)

{
  *param_1 = &PTR_LAB_100068d8;
  param_1[2] = 0;
  InitializeCriticalSection((LPCRITICAL_SECTION)(param_1 + 3));
  *param_1 = &PTR_LAB_100068b8;
  param_1[1] = &PTR_LAB_100068a8;
  CpuFeatures_DetectMmxSupport();
  return param_1;
}

// clang-format on
