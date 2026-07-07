/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: DllCanUnloadNow @ 10001570
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: DllCanUnloadNow at 10001570
// Signature: HRESULT __stdcall DllCanUnloadNow(void)
// Entry: 10001570
// Body: 10001570 - 100015b8
// ============================================================================
HRESULT DllCanUnloadNow(void)

{
  AFX_MODULE_STATE *pAVar1;
  long lVar2;
  undefined4 local_8;
  int local_4;
  
                    /* 0x1570  1  DllCanUnloadNow */
  pAVar1 = (AFX_MODULE_STATE *)FUN_10006393();
  AFX_MAINTAIN_STATE2::AFX_MAINTAIN_STATE2((AFX_MAINTAIN_STATE2 *)&local_8,pAVar1);
  lVar2 = AfxDllCanUnloadNow();
  if ((lVar2 == 0) && (DAT_1000a41c == 0)) {
    *(undefined4 *)(local_4 + 4) = local_8;
    return 0;
  }
  *(undefined4 *)(local_4 + 4) = local_8;
  return 1;
}

// clang-format on
