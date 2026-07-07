/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: Recorder_StartPlaybackPreview @ 100045e0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Recorder_StartPlaybackPreview at 100045e0
// Signature: undefined4 __fastcall Recorder_StartPlaybackPreview(int param_1)
// Entry: 100045e0
// Body: 100045e0 - 100047e9
// ============================================================================
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __fastcall Recorder_StartPlaybackPreview(int param_1)

{
  undefined4 *puVar1;
  uint uVar2;
  undefined4 uVar3;
  int iVar4;
  int **ppiVar5;
  UINT_PTR UVar6;
  undefined1 *puVar7;
  int **ppiVar8;
  int *piStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_28;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  uint uStack_18;
  undefined4 uStack_14;
  undefined2 *puStack_10;
  
  if (*(int **)(param_1 + 0x30) == (int *)0x0) {
    return 0;
  }
  if (*(int *)(param_1 + 0x14) == 0) {
    return 0;
  }
  uStack_3c = 0x10004603;
  (**(code **)(**(int **)(param_1 + 0x30) + 0x50))();
  uStack_3c = 0;
  uStack_40 = 0;
  piStack_44 = (int *)0x1000460d;
  (**(code **)(**(int **)(param_1 + 0x30) + 0x30))();
  piStack_44 = (int *)0x10004615;
  uVar2 = (**(code **)(**(int **)(param_1 + 0x30) + 0x38))();
  uStack_20 = 0;
  *(uint *)(param_1 + 0x74) = uVar2;
  piStack_44 = *(int **)(param_1 + 0x14);
  *(float *)(param_1 + 0x80) = (float)uVar2 * _DAT_10006478;
  uVar3 = (**(code **)(*piStack_44 + 0x28))();
  *(undefined4 *)(param_1 + 0x5c) = uVar3;
  uStack_18 = 0;
  uStack_14 = 0;
  *(undefined4 *)(param_1 + 0x78) = 0;
  *(undefined4 *)(param_1 + 0x7c) = 0;
  *(undefined4 *)(param_1 + 0x84) = 0;
  puStack_10 = (undefined2 *)0x0;
  uStack_20 = 0x14;
  uStack_1c = 0xe2;
  Recorder_BuildWaveFormatFromPreset((undefined2 *)(param_1 + 0x44),*(uint *)(param_1 + 0x34));
  uStack_18 = (uint)(*(ushort *)(param_1 + 0x52) >> 3) * (uint)*(ushort *)(param_1 + 0x46) *
              *(int *)(param_1 + 0x48) * 4;
  if (*(uint *)(param_1 + 0x74) <= uStack_18) {
    uStack_18 = *(uint *)(param_1 + 0x74);
  }
  puVar1 = (undefined4 *)(param_1 + 0x60);
  uVar2 = 0;
  puStack_10 = (undefined2 *)(param_1 + 0x44);
  iVar4 = (**(code **)(**(int **)(param_1 + 0x5c) + 0xc))
                    (*(int **)(param_1 + 0x5c),&uStack_20,puVar1);
  if (iVar4 != 0) {
    return 0;
  }
  ppiVar8 = &piStack_44;
  puVar7 = &stack0xffffffb8;
  iVar4 = (**(code **)(*(int *)*puVar1 + 0x2c))
                    ((int *)*puVar1,0,uStack_28,puVar7,ppiVar8,&uStack_40,&uStack_3c,0);
  if ((iVar4 != 0) || (ppiVar8 == (int **)0x0)) goto LAB_100047c9;
  if (uVar2 == *(uint *)(param_1 + 0x74)) {
    *(undefined4 *)(param_1 + 100) = 0;
    ppiVar5 = (int **)(**(code **)(**(int **)(param_1 + 0x30) + 0x3c))(puVar7,ppiVar8);
    if ((ppiVar5 != ppiVar8) ||
       (iVar4 = (**(code **)(*(int *)*puVar1 + 0x4c))((int *)*puVar1,puVar7,ppiVar8,0,0), iVar4 != 0
       )) goto LAB_100047c9;
    iVar4 = (**(code **)(*(int *)*puVar1 + 0x30))((int *)*puVar1,0,0,0);
  }
  else {
    *(uint *)(param_1 + 0x68) = uVar2 >> 1;
    iVar4 = (**(code **)(**(int **)(param_1 + 0x30) + 0x3c))(puVar7,uVar2 >> 1);
    if (iVar4 != *(int *)(param_1 + 0x68)) goto LAB_100047c9;
    uVar3 = (**(code **)(**(int **)(param_1 + 0x30) + 0x14))();
    *(undefined4 *)(param_1 + 100) = uVar3;
    iVar4 = (**(code **)(*(int *)*puVar1 + 0x4c))((int *)*puVar1,puVar7,ppiVar8,0,0);
    if (iVar4 != 0) goto LAB_100047c9;
    *(undefined4 *)(param_1 + 0x70) = 1;
    *(undefined4 *)(param_1 + 0x6c) = 0;
    iVar4 = (**(code **)(*(int *)*puVar1 + 0x30))((int *)*puVar1,0,0,1);
  }
  if (iVar4 == 0) {
    if (*(int *)(param_1 + 0x58) == 0) {
      UVar6 = FUN_10001140(*(void **)(param_1 + 0x38),100);
      *(UINT_PTR *)(param_1 + 0x58) = UVar6;
    }
    return 1;
  }
LAB_100047c9:
  iVar4 = (**(code **)(*(int *)*puVar1 + 8))((int *)*puVar1);
  while (iVar4 != 0) {
    iVar4 = (**(code **)(*(int *)*puVar1 + 8))((int *)*puVar1);
  }
  return 0;
}

// clang-format on
