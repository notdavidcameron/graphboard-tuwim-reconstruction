/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: RecorderContext_Destroy @ 10003ca0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: RecorderContext_Destroy at 10003ca0
// Signature: undefined __fastcall RecorderContext_Destroy(char * param_1)
// Entry: 10003ca0
// Body: 10003ca0 - 10003d6a
// ============================================================================
void __fastcall RecorderContext_Destroy(char *param_1)

{
  int iVar1;
  char *local_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_10005b08;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  *(undefined ***)param_1 = &PTR_LAB_10006b68;
  *(undefined ***)(param_1 + 8) = &PTR_LAB_10006b00;
  *(undefined ***)(param_1 + 4) = &PTR_LAB_10006b30;
  *(undefined ***)(param_1 + 0xc) = &PTR_LAB_10006af0;
  local_10 = param_1;
  FUN_10004ab0((int)(param_1 + 4));
  if (*(int **)(param_1 + 0x38) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0x38) + 4))(1);
  }
  if (*(int **)(param_1 + 0x30) != (int *)0x0) {
    (**(code **)(**(int **)(param_1 + 0x30) + 0x20))(&local_10);
    puStack_8 = (undefined1 *)0x0;
    iVar1 = (**(code **)(**(int **)(param_1 + 0x30) + 0x38))();
    (**(code **)(**(int **)(param_1 + 0x30) + 0x54))();
    if (*(int **)(param_1 + 0x30) != (int *)0x0) {
      (**(code **)(**(int **)(param_1 + 0x30) + 4))(1);
    }
    param_1[0x30] = '\0';
    param_1[0x31] = '\0';
    param_1[0x32] = '\0';
    param_1[0x33] = '\0';
    if (iVar1 == 0) {
      CFile::Remove(local_10);
    }
    uStack_4 = 0xffffffff;
    CString::~CString((CString *)&local_10);
  }
  if (*(void **)(param_1 + 0x3c) != (void *)0x0) {
    operator_delete(*(void **)(param_1 + 0x3c));
  }
  ExceptionList = pvStack_c;
  return;
}

// clang-format on
