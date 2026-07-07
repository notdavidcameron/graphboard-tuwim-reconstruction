/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: Recorder_BuildWaveFormatFromPreset @ 10004030
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Recorder_BuildWaveFormatFromPreset at 10004030
// Signature: undefined __stdcall Recorder_BuildWaveFormatFromPreset(undefined2 * param_1, uint param_2)
// Entry: 10004030
// Body: 10004030 - 100040cf
// ============================================================================
void Recorder_BuildWaveFormatFromPreset(undefined2 *param_1,uint param_2)

{
  uint uVar1;
  bool bVar2;
  
  uVar1 = param_2 & 0x80000001;
  bVar2 = uVar1 == 0;
  *param_1 = 1;
  param_1[1] = 2;
  if ((int)uVar1 < 0) {
    bVar2 = (uVar1 - 1 | 0xfffffffe) == 0xffffffff;
  }
  if (bVar2) {
    param_1[1] = 1;
  }
  param_1[7] = 0x10;
  param_1[6] = param_1[1] * 2;
  uVar1 = param_2 & 0x80000003;
  if ((int)uVar1 < 0) {
    uVar1 = (uVar1 - 1 | 0xfffffffc) + 1;
  }
  if ((int)uVar1 < 2) {
    param_1[6] = param_1[1];
    param_1[7] = 8;
  }
  *(undefined4 *)(param_1 + 2) = 0xac44;
  if ((int)param_2 < 4) {
    *(undefined4 *)(param_1 + 2) = 8000;
  }
  else if ((int)param_2 < 8) {
    *(undefined4 *)(param_1 + 2) = 0x2b11;
  }
  else if ((int)param_2 < 0xc) {
    *(undefined4 *)(param_1 + 2) = 0x5622;
  }
  param_1[8] = 0;
  *(uint *)(param_1 + 4) = (uint)(ushort)param_1[6] * *(int *)(param_1 + 2);
  return;
}

// clang-format on
