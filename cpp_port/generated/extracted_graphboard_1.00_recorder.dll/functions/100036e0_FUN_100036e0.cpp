/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_100036e0 @ 100036e0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100036e0 at 100036e0
// Signature: LPSTR __stdcall FUN_100036e0(LPSTR param_1, LPCWSTR param_2, int param_3, UINT param_4)
// Entry: 100036e0
// Body: 100036e0 - 1000370b
// ============================================================================
LPSTR FUN_100036e0(LPSTR param_1,LPCWSTR param_2,int param_3,UINT param_4)

{
  *param_1 = '\0';
  WideCharToMultiByte(param_4,0,param_2,-1,param_1,param_3,(LPCSTR)0x0,(LPBOOL)0x0);
  return param_1;
}

// clang-format on
