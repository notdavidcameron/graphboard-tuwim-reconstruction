/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_10003b70 @ 10003b70
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10003b70 at 10003b70
// Signature: int __stdcall FUN_10003b70(short * param_1)
// Entry: 10003b70
// Body: 10003b70 - 10003bab
// ============================================================================
int FUN_10003b70(short *param_1)

{
  short sVar1;
  short *psVar2;
  short *psVar3;
  
  sVar1 = *param_1;
  psVar2 = param_1;
  psVar3 = param_1;
  while (sVar1 != 0) {
    sVar1 = *psVar3;
    if (sVar1 != 0) {
      psVar3 = psVar3 + 1;
    }
    if (((sVar1 == 0x5c) || (sVar1 == 0x2f)) || (sVar1 == 0x3a)) {
      psVar2 = psVar3;
    }
    sVar1 = *psVar3;
  }
  return (int)psVar2 - (int)param_1 >> 1;
}

// clang-format on
