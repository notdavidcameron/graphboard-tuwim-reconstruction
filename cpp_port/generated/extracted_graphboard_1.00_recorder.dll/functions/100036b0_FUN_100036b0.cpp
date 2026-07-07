/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_100036b0 @ 100036b0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100036b0 at 100036b0
// Signature: LPWSTR __stdcall FUN_100036b0(LPWSTR param_1, LPCSTR param_2, int param_3, UINT param_4)
// Entry: 100036b0
// Body: 100036b0 - 100036d9
// ============================================================================
LPWSTR FUN_100036b0(LPWSTR param_1,LPCSTR param_2,int param_3,UINT param_4)

{
  *param_1 = L'\0';
  MultiByteToWideChar(param_4,0,param_2,-1,param_1,param_3);
  return param_1;
}

// clang-format on
