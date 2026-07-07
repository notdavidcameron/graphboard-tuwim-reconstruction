/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: Open @ 10006b30
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Open at 10006b30
// Signature: int __thiscall Open(CFile * this, char * param_1, uint param_2, CFileException * param_3)
// Entry: 10006b30
// Body: 10006b30 - 10006b35
// ============================================================================
int __thiscall CFile::Open(CFile *this,char *param_1,uint param_2,CFileException *param_3)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10006b30. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = Open(this,param_1,param_2,param_3);
  return iVar1;
}

// clang-format on
