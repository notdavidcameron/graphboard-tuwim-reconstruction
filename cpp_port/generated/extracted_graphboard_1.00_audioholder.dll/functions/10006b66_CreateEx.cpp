/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: CreateEx @ 10006b66
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: CreateEx at 10006b66
// Signature: int __thiscall CreateEx(CWnd * this, ulong param_1, char * param_2, char * param_3, ulong param_4, int param_5, int param_6, int param_7, int param_8, HWND__ * param_9, HMENU__ * param_10, void * param_11)
// Entry: 10006b66
// Body: 10006b66 - 10006b6b
// ============================================================================
int __thiscall
CWnd::CreateEx(CWnd *this,ulong param_1,char *param_2,char *param_3,ulong param_4,int param_5,
              int param_6,int param_7,int param_8,HWND__ *param_9,HMENU__ *param_10,void *param_11)

{
  int iVar1;
  
                    /* WARNING: Could not recover jumptable at 0x10006b66. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  iVar1 = CreateEx(this,param_1,param_2,param_3,param_4,param_5,param_6,param_7,param_8,param_9,
                   param_10,param_11);
  return iVar1;
}

// clang-format on
