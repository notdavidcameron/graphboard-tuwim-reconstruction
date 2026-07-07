/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10006d88 @ 10006d88
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10006d88 at 10006d88
// Signature: undefined4 __stdcall FUN_10006d88(undefined4 param_1, int param_2)
// Entry: 10006d88
// Body: 10006d88 - 10006dd8
// ============================================================================
undefined4 FUN_10006d88(undefined4 param_1,int param_2)

{
  HLOCAL hMem;
  _AFX_THREAD_STATE *p_Var1;
  AFX_MODULE_STATE *pAVar2;
  
  if (param_2 == 1) {
    hMem = LocalAlloc(0,0x2000);
    if (hMem == (HLOCAL)0x0) {
      return 0;
    }
    LocalFree(hMem);
    p_Var1 = AfxGetThreadState();
    pAVar2 = AfxSetModuleState((AFX_MODULE_STATE *)&DAT_1000b4f8);
    *(AFX_MODULE_STATE **)(p_Var1 + 8) = pAVar2;
  }
  else if (param_2 == 0) {
    p_Var1 = AfxGetThreadState();
    AfxSetModuleState(*(AFX_MODULE_STATE **)(p_Var1 + 8));
  }
  return 1;
}

// clang-format on
