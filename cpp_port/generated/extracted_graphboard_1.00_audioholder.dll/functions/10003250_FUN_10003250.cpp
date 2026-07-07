/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10003250 @ 10003250
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10003250 at 10003250
// Signature: LPSTR __stdcall FUN_10003250(LPSTR param_1, LPCWSTR param_2, int param_3, UINT param_4)
// Entry: 10003250
// Body: 10003250 - 1000327b
// ============================================================================
LPSTR FUN_10003250(LPSTR param_1,LPCWSTR param_2,int param_3,UINT param_4)

{
  *param_1 = '\0';
  WideCharToMultiByte(param_4,0,param_2,-1,param_1,param_3,(LPCSTR)0x0,(LPBOOL)0x0);
  return param_1;
}

// clang-format on
