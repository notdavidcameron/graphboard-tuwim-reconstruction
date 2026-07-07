/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: Read @ 1000695c
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Read at 1000695c
// Signature: uint __thiscall Read(CArchive * this, void * param_1, uint param_2)
// Entry: 1000695c
// Body: 1000695c - 10006961
// ============================================================================
uint __thiscall CArchive::Read(CArchive *this,void *param_1,uint param_2)

{
  uint uVar1;
  
                    /* WARNING: Could not recover jumptable at 0x1000695c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar1 = Read(this,param_1,param_2);
  return uVar1;
}

// clang-format on
