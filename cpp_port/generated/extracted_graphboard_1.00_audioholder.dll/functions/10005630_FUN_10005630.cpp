/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10005630 @ 10005630
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10005630 at 10005630
// Signature: undefined4 __thiscall FUN_10005630(void * this, int param_1)
// Entry: 10005630
// Body: 10005630 - 100058b8
// ============================================================================
undefined4 __thiscall FUN_10005630(void *this,int param_1)

{
  int *piVar1;
  HGLOBAL pvVar2;
  LPVOID lpBuffer;
  int iVar3;
  int iVar4;
  DWORD DVar5;
  BOOL BVar6;
  int iVar7;
  DWORD DVar8;
  int iVar9;
  DWORD nNumberOfBytesToWrite;
  int iStack_10;
  DWORD DStack_8;
  int iStack_4;
  
  if ((param_1 < *(int *)(*(int *)((int)this + 0x90) + 0x21c)) &&
     (piVar1 = *(int **)(*(int *)((int)this + 0x90) + 0x220), piVar1 != (int *)0x0)) {
    iStack_4 = (**(code **)(*piVar1 + 0x38))();
    iVar4 = *(int *)(*(int *)((int)this + 0x90) + 0x3c + param_1 * 4);
    iVar9 = *(int *)(iVar4 + 4);
    iVar4 = *(int *)(iVar4 + 8);
    if (iVar9 == iVar4) {
      return 1;
    }
    pvVar2 = GlobalAlloc(2,0xffff);
    lpBuffer = GlobalLock(pvVar2);
    if (lpBuffer != (LPVOID)0x0) {
      iVar3 = *(int *)((int)this + 0x90);
      iStack_10 = 0;
      if (0 < *(int *)(iVar3 + 0x21c)) {
        do {
          iVar3 = *(int *)(iVar3 + 0x3c + iStack_10 * 4);
          iVar7 = *(int *)(iVar3 + 4);
          if (iVar7 == iVar4) {
            *(int *)(iVar3 + 4) = iVar9;
            for (DVar8 = *(int *)(iVar3 + 8) - iVar7; 0 < (int)DVar8;
                DVar8 = DVar8 - nNumberOfBytesToWrite) {
              nNumberOfBytesToWrite = 0xffff;
              if ((int)DVar8 < 0x10000) {
                nNumberOfBytesToWrite = DVar8;
              }
              iVar4 = (**(code **)(**(int **)(*(int *)((int)this + 0x90) + 0x220) + 0x30))(iVar7,0);
              if ((((iVar4 != iVar7) ||
                   (DVar5 = (**(code **)(**(int **)(*(int *)((int)this + 0x90) + 0x220) + 0x3c))
                                      (lpBuffer,nNumberOfBytesToWrite),
                   DVar5 != nNumberOfBytesToWrite)) ||
                  (iVar4 = (**(code **)(**(int **)(*(int *)((int)this + 0x90) + 0x220) + 0x30))
                                     (iVar9,0), iVar4 != iVar9)) ||
                 ((BVar6 = WriteFile(*(HANDLE *)(*(int *)(*(int *)((int)this + 0x90) + 0x220) + 4),
                                     lpBuffer,nNumberOfBytesToWrite,&DStack_8,(LPOVERLAPPED)0x0),
                  BVar6 == 0 || (DStack_8 != nNumberOfBytesToWrite)))) {
                pvVar2 = GlobalHandle(lpBuffer);
                GlobalUnlock(pvVar2);
                pvVar2 = GlobalHandle(lpBuffer);
                GlobalFree(pvVar2);
                (**(code **)(**(int **)(*(int *)((int)this + 0x90) + 0x220) + 0x54))();
                piVar1 = *(int **)(*(int *)((int)this + 0x90) + 0x220);
                if (piVar1 != (int *)0x0) {
                  (**(code **)(*piVar1 + 4))(1);
                }
                *(undefined4 *)(*(int *)((int)this + 0x90) + 0x220) = 0;
                return 0;
              }
              iVar7 = iVar7 + nNumberOfBytesToWrite;
              iVar9 = iVar9 + nNumberOfBytesToWrite;
            }
            iVar3 = *(int *)(*(int *)((int)this + 0x90) + 0x3c + iStack_10 * 4);
            iStack_10 = -1;
            iVar4 = *(int *)(iVar3 + 8);
            *(int *)(iVar3 + 8) = iVar9;
          }
          iVar3 = *(int *)((int)this + 0x90);
          iStack_10 = iStack_10 + 1;
        } while (iStack_10 < *(int *)(iVar3 + 0x21c));
      }
      pvVar2 = GlobalHandle(lpBuffer);
      GlobalUnlock(pvVar2);
      pvVar2 = GlobalHandle(lpBuffer);
      GlobalFree(pvVar2);
      iVar4 = *(int *)(*(int *)((int)this + 0x90) + 0x3c + param_1 * 4);
      (**(code **)(**(int **)(*(int *)((int)this + 0x90) + 0x220) + 0x34))
                ((*(int *)(iVar4 + 4) - *(int *)(iVar4 + 8)) + iStack_4);
      *(undefined4 *)(*(int *)(*(int *)((int)this + 0x90) + 0x3c + param_1 * 4) + 4) = 0;
      *(undefined4 *)(*(int *)(*(int *)((int)this + 0x90) + 0x3c + param_1 * 4) + 8) = 0;
      return 1;
    }
  }
  return 0;
}

// clang-format on
