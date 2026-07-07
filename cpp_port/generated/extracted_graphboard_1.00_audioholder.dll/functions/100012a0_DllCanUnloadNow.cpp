/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: DllCanUnloadNow @ 100012a0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: DllCanUnloadNow at 100012a0
// Signature: HRESULT __stdcall DllCanUnloadNow(void)
// Entry: 100012a0
// Body: 100012a0 - 100012e8
// ============================================================================
HRESULT DllCanUnloadNow(void)

{
  AFX_MODULE_STATE *pAVar1;
  long lVar2;
  undefined4 local_8;
  int local_4;
  
                    /* 0x12a0  1  DllCanUnloadNow */
  pAVar1 = (AFX_MODULE_STATE *)FUN_10006c5b();
  AFX_MAINTAIN_STATE2::AFX_MAINTAIN_STATE2((AFX_MAINTAIN_STATE2 *)&local_8,pAVar1);
  lVar2 = AfxDllCanUnloadNow();
  if ((lVar2 == 0) && (DAT_1000b47c == 0)) {
    *(undefined4 *)(local_4 + 4) = local_8;
    return 0;
  }
  *(undefined4 *)(local_4 + 4) = local_8;
  return 1;
}

// clang-format on
