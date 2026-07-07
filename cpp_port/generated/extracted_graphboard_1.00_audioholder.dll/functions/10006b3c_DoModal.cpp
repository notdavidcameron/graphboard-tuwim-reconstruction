/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: DoModal @ 10006b3c
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: DoModal at 10006b3c
// Signature: int __thiscall DoModal(CFileDialog * this)
// Entry: 10006b3c
// Body: 10006b3c - 10006b41
// ============================================================================
int __thiscall CFileDialog::DoModal(CFileDialog *this)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10006b3c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = DoModal(this);
  return iVar1;
}

// clang-format on
