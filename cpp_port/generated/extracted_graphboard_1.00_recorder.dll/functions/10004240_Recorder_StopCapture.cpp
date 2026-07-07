/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: Recorder_StopCapture @ 10004240
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Recorder_StopCapture at 10004240
// Signature: undefined4 __fastcall Recorder_StopCapture(int param_1)
// Entry: 10004240
// Body: 10004240 - 100042f3
// ============================================================================
undefined4 __fastcall Recorder_StopCapture(int param_1)

{
  HWAVEIN hwi;
  LPWAVEHDR pwh;
  HGLOBAL hMem;
  int iVar1;
  
  if ((*(int *)(param_1 + 0x3c) != 0) &&
     (hwi = *(HWAVEIN *)(*(int *)(param_1 + 0x3c) + 0x24), hwi != (HWAVEIN)0x0)) {
    waveInStop(hwi);
    waveInReset(*(HWAVEIN *)(*(int *)(param_1 + 0x3c) + 0x24));
    iVar1 = 0;
    do {
      pwh = *(LPWAVEHDR *)(iVar1 + *(int *)(param_1 + 0x3c));
      if (pwh != (LPWAVEHDR)0x0) {
        waveInUnprepareHeader(*(HWAVEIN *)(*(int *)(param_1 + 0x3c) + 0x24),pwh,0x20);
        hMem = GlobalHandle(*(LPCVOID *)(iVar1 + *(int *)(param_1 + 0x3c)));
        GlobalFree(hMem);
        *(undefined4 *)(iVar1 + *(int *)(param_1 + 0x3c)) = 0;
      }
      iVar1 = iVar1 + 4;
    } while (iVar1 < 0x10);
    waveInClose(*(HWAVEIN *)(*(int *)(param_1 + 0x3c) + 0x24));
    operator_delete(*(void **)(param_1 + 0x3c));
    *(undefined4 *)(param_1 + 0x3c) = 0;
    (**(code **)(*(int *)(param_1 + 8) + 0x20))(param_1 + 8);
    (**(code **)(**(int **)(param_1 + 0x14) + 0x2c))(*(int **)(param_1 + 0x14));
    return 1;
  }
  return 0;
}

// clang-format on
