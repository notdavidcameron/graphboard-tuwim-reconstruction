/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10003220 @ 10003220
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10003220 at 10003220
// Signature: LPWSTR __stdcall FUN_10003220(LPWSTR param_1, LPCSTR param_2, int param_3, UINT param_4)
// Entry: 10003220
// Body: 10003220 - 10003249
// ============================================================================
LPWSTR FUN_10003220(LPWSTR param_1,LPCSTR param_2,int param_3,UINT param_4)

{
  *param_1 = L'\0';
  MultiByteToWideChar(param_4,0,param_2,-1,param_1,param_3);
  return param_1;
}

// clang-format on
