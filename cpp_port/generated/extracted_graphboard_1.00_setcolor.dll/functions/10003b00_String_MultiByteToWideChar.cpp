/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: String_MultiByteToWideChar @ 10003b00
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: String_MultiByteToWideChar at 10003b00
// Signature: LPWSTR __stdcall String_MultiByteToWideChar(LPWSTR param_1, LPCSTR param_2, int param_3, UINT param_4)
// Entry: 10003b00
// Body: 10003b00 - 10003b29
// ============================================================================
LPWSTR String_MultiByteToWideChar(LPWSTR param_1,LPCSTR param_2,int param_3,UINT param_4)

{
  *param_1 = L'\0';
  MultiByteToWideChar(param_4,0,param_2,-1,param_1,param_3);
  return param_1;
}

// clang-format on
