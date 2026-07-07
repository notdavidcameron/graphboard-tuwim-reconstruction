/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10003af0 @ 10003af0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10003af0 at 10003af0
// Signature: undefined __fastcall FUN_10003af0(undefined4 * param_1)
// Entry: 10003af0
// Body: 10003af0 - 10003bc8
// ============================================================================
void __fastcall FUN_10003af0(undefined4 *param_1)

{
  CString *this;
  undefined4 *puVar1;
  int iVar2;
  void *pvStack_c;
  undefined1 *puStack_8;
  uint local_4;
  
  puStack_8 = &LAB_10006b45;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  *param_1 = &PTR_LAB_10007af8;
  param_1[1] = &PTR_LAB_10007abc;
  param_1[2] = &PTR_LAB_10007a98;
  param_1[3] = &PTR_LAB_10007a88;
  local_4 = 1;
  SendMessageA((HWND)param_1[0x12],0x10,0,0);
  iVar2 = 0;
  if (0 < (int)param_1[0x46]) {
    puVar1 = param_1 + 0x14;
    do {
      this = *(CString **)*puVar1;
      if (this != (CString *)0x0) {
        CString::~CString(this);
        operator_delete(this);
      }
      operator_delete((void *)*puVar1);
      iVar2 = iVar2 + 1;
      puVar1 = puVar1 + 1;
    } while (iVar2 < (int)param_1[0x46]);
  }
  if ((int *)param_1[0x13] != (int *)0x0) {
    (**(code **)(*(int *)param_1[0x13] + 4))(1);
  }
  local_4 = local_4 & 0xffffff00;
  CString::~CString((CString *)(param_1 + 0x4e));
  if (param_1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = param_1 + 4;
  }
  DeleteCriticalSection((LPCRITICAL_SECTION)(puVar1 + 1));
  ExceptionList = pvStack_c;
  return;
}

// clang-format on
