/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: Recorder_StartCapture @ 100040d0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Recorder_StartCapture at 100040d0
// Signature: bool __fastcall Recorder_StartCapture(int param_1)
// Entry: 100040d0
// Body: 100040d0 - 10004233
// ============================================================================
bool __fastcall Recorder_StartCapture(int param_1)

{
  int *piVar1;
  void *pvVar2;
  MMRESULT MVar3;
  int iVar4;
  HGLOBAL hMem;
  LPVOID pvVar5;
  int iVar6;
  
  piVar1 = *(int **)(param_1 + 0x14);
  if ((piVar1 != (int *)0x0) && (*(int *)(param_1 + 0x3c) == 0)) {
    (**(code **)(*piVar1 + 0x30))(piVar1);
    pvVar2 = operator_new(0x2c);
    *(void **)(param_1 + 0x3c) = pvVar2;
    Recorder_BuildWaveFormatFromPreset((undefined2 *)((int)pvVar2 + 0x10),*(uint *)(param_1 + 0x34))
    ;
    *(uint *)(*(int *)(param_1 + 0x3c) + 0x28) = *(uint *)(*(int *)(param_1 + 0x3c) + 0x18) / 10;
    *(undefined4 *)(*(int *)(param_1 + 0x3c) + 0x24) = 0;
    MVar3 = waveInOpen((LPHWAVEIN)(*(int *)(param_1 + 0x3c) + 0x24),0xffffffff,
                       (LPCWAVEFORMATEX)(*(int *)(param_1 + 0x3c) + 0x10),
                       *(DWORD_PTR *)(*(int *)(param_1 + 0x38) + 0x20),0,0x10000);
    iVar6 = 0;
    if (MVar3 == 0) {
      do {
        iVar4 = iVar6 + 4;
        *(undefined4 *)(iVar6 + *(int *)(param_1 + 0x3c)) = 0;
        iVar6 = iVar4;
      } while (iVar4 < 0x10);
      iVar6 = 0;
      while( true ) {
        hMem = GlobalAlloc(0x2002,*(int *)(*(int *)(param_1 + 0x3c) + 0x28) + 0x20);
        pvVar5 = GlobalLock(hMem);
        *(LPVOID *)(iVar6 + *(int *)(param_1 + 0x3c)) = pvVar5;
        piVar1 = *(int **)(iVar6 + *(int *)(param_1 + 0x3c));
        if (piVar1 == (int *)0x0) break;
        *piVar1 = (int)(piVar1 + 8);
        *(undefined4 *)(*(int *)(iVar6 + *(int *)(param_1 + 0x3c)) + 4) =
             *(undefined4 *)(*(int *)(param_1 + 0x3c) + 0x28);
        *(undefined4 *)(*(int *)(iVar6 + *(int *)(param_1 + 0x3c)) + 8) = 0;
        *(undefined4 *)(*(int *)(iVar6 + *(int *)(param_1 + 0x3c)) + 0xc) = 0;
        *(undefined4 *)(*(int *)(iVar6 + *(int *)(param_1 + 0x3c)) + 0x10) = 0;
        *(undefined4 *)(*(int *)(iVar6 + *(int *)(param_1 + 0x3c)) + 0x14) = 0;
        MVar3 = waveInPrepareHeader(*(HWAVEIN *)(*(int *)(param_1 + 0x3c) + 0x24),
                                    *(LPWAVEHDR *)(iVar6 + *(int *)(param_1 + 0x3c)),0x20);
        if (MVar3 != 0) {
          return false;
        }
        MVar3 = waveInAddBuffer(*(HWAVEIN *)(*(int *)(param_1 + 0x3c) + 0x24),
                                *(LPWAVEHDR *)(iVar6 + *(int *)(param_1 + 0x3c)),0x20);
        if (MVar3 != 0) {
          return false;
        }
        iVar6 = iVar6 + 4;
        if (0xf < iVar6) {
          if (*(int **)(param_1 + 0x30) != (int *)0x0) {
            (**(code **)(**(int **)(param_1 + 0x30) + 0x34))(0);
            (**(code **)(**(int **)(param_1 + 0x30) + 0x30))(0,0);
          }
          MVar3 = waveInStart(*(HWAVEIN *)(*(int *)(param_1 + 0x3c) + 0x24));
          return (bool)('\x01' - (MVar3 != 0));
        }
      }
    }
  }
  return false;
}

// clang-format on
