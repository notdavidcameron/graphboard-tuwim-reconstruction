/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_10001d46 @ 10001d46
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001d46 at 10001d46
// Signature: int __stdcall FUN_10001d46(void)
// Entry: 10001d46
// Body: 10001d46 - 10001dcb
// ============================================================================
int FUN_10001d46(void)

{
  int iVar1;
  int unaff_EBP;
  char *unaff_ESI;
  
  *(undefined4 *)(unaff_EBP + -4) = 0xffffffff;
  if (unaff_ESI != (char *)0x0) {
    iVar1 = (*(code *)**(undefined4 **)unaff_ESI)();
    if (iVar1 != 0) {
      *(undefined ***)unaff_ESI = &PTR_LAB_100068b8;
      *(undefined ***)(unaff_ESI + 4) = &PTR_LAB_10006880;
      *(undefined ***)(unaff_ESI + 8) = &PTR_LAB_10006850;
      *(undefined ***)(unaff_ESI + 0xc) = &PTR_LAB_10006840;
      unaff_ESI[0x10] = '\x01';
      unaff_ESI[0x11] = '\0';
      unaff_ESI[0x12] = '\0';
      unaff_ESI[0x13] = '\0';
      InterlockedDecrement(&DAT_100083ac);
      RecorderContext_Destroy(unaff_ESI);
      operator_delete(unaff_ESI);
    }
    ExceptionList = *(void **)(unaff_EBP + -0xc);
    return iVar1;
  }
  ExceptionList = *(void **)(unaff_EBP + -0xc);
  return *(int *)(unaff_EBP + -0x18);
}

// clang-format on
