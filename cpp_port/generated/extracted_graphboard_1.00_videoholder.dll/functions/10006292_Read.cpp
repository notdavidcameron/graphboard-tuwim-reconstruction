/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: Read @ 10006292
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Read at 10006292
// Signature: uint __thiscall Read(CFile * this, void * param_1, uint param_2)
// Entry: 10006292
// Body: 10006292 - 10006297
// ============================================================================
uint __thiscall CFile::Read(CFile *this,void *param_1,uint param_2)

{
  uint uVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10006292. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar1 = Read(this,param_1,param_2);
  return uVar1;
}

// clang-format on
