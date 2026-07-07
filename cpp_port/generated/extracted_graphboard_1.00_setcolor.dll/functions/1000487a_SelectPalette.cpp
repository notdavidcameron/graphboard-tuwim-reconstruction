/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: SelectPalette @ 1000487a
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: SelectPalette at 1000487a
// Signature: CPalette * __thiscall SelectPalette(CDC * this, CPalette * param_1, int param_2)
// Entry: 1000487a
// Body: 1000487a - 1000487f
// ============================================================================
CPalette * __thiscall CDC::SelectPalette(CDC *this,CPalette *param_1,int param_2)

{
  CPalette *pCVar1;
  
                    /* WARNING: Could not recover jumptable at 0x1000487a. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  pCVar1 = SelectPalette(this,param_1,param_2);
  return pCVar1;
}

// clang-format on
