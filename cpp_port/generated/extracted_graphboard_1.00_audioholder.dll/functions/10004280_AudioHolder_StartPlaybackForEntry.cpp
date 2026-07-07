/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: AudioHolder_StartPlaybackForEntry @ 10004280
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: AudioHolder_StartPlaybackForEntry at 10004280
// Signature: undefined4 __thiscall AudioHolder_StartPlaybackForEntry(void * this, int param_1)
// Entry: 10004280
// Body: 10004280 - 100047a6
// ============================================================================
undefined4 __thiscall AudioHolder_StartPlaybackForEntry(void *this,int param_1)

{
  undefined4 *puVar1;
  int *piVar2;
  int *piVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  void *pvVar7;
  int **ppiVar8;
  UINT_PTR UVar9;
  int unaff_EBX;
  undefined4 unaff_EBP;
  int **ppiVar10;
  int **ppiVar11;
  uint uVar12;
  int *piStack_70;
  int *piStack_6c;
  int *piStack_68;
  undefined4 local_54;
  undefined4 uStack_50;
  int local_4c;
  uint local_48;
  uint uStack_44;
  undefined2 *puStack_40;
  undefined2 uStack_26;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined2 uStack_1c;
  undefined2 uStack_1a;
  uint uStack_14;
  
  piStack_68 = *(int **)((int)this + 0x2c);
  if (piStack_68 != (int *)0x0) {
    piStack_6c = (int *)0x10004298;
    uVar4 = (**(code **)(*piStack_68 + 0x28))();
    *(undefined4 *)((int)this + 0x38) = uVar4;
  }
  if (param_1 < 0) {
    return 0;
  }
  if (*(int *)((int)this + 0x21c) <= param_1) {
    return 0;
  }
  if (*(int *)((int)this + 0x38) == 0) {
    return 0;
  }
  if (*(int **)((int)this + 0x220) == (int *)0x0) {
    return 0;
  }
  iVar5 = *(int *)((int)this + param_1 * 4 + 0x3c);
  local_54 = 0;
  local_48 = 0;
  local_4c = 0;
  iVar6 = *(int *)(iVar5 + 0x58);
  if (iVar6 != 0) {
    *(undefined4 *)(iVar6 + 0x28) = 0;
    *(undefined4 *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 0x2c) = 0;
    iVar5 = *(int *)((int)this + param_1 * 4 + 0x3c);
    *(int *)(*(int *)(iVar5 + 0x58) + 0x30) = (*(int *)(iVar5 + 8) - *(int *)(iVar5 + 4)) + -0x2c;
    puVar1 = *(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
    if (puVar1[1] == 0) {
      if (puVar1 != (undefined4 *)0x0) {
        piStack_6c = (int *)*puVar1;
        piStack_68 = &local_4c;
        piStack_70 = (int *)0x10004324;
        (**(code **)(*piStack_6c + 0x24))();
      }
      if (local_4c != 1) {
        return 1;
      }
      piStack_68 = (int *)0x0;
      piStack_6c = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
      piStack_70 = (int *)0x1000433f;
      iVar5 = (**(code **)(*piStack_6c + 0x34))();
      if (iVar5 == 0) {
        return 1;
      }
    }
    else {
      iVar5 = *(int *)((int)this + param_1 * 4 + 0x3c);
      piStack_68 = (int *)0x0;
      piStack_6c = (int *)(*(int *)(iVar5 + 4) + 0x2c);
      piStack_70 = (int *)0x1000436d;
      iVar6 = (**(code **)(**(int **)((int)this + 0x220) + 0x30))();
      if (iVar6 == *(int *)(iVar5 + 4) + 0x2c) {
        piStack_70 = (int *)**(undefined4 **)(iVar5 + 0x58);
        (**(code **)(*piStack_70 + 0x48))();
        puVar1 = *(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
        piVar2 = (int *)*puVar1;
        iVar5 = (**(code **)(*piVar2 + 0x2c))
                          (piVar2,0,puVar1[2],&stack0xffffffa0,&stack0xffffffa4,&local_54,&uStack_50
                          );
        if ((iVar5 == 0) && (unaff_EBX != 0)) {
          iVar5 = *(int *)((int)this + param_1 * 4 + 0x3c);
          puVar1 = *(undefined4 **)(iVar5 + 0x58);
          piStack_70 = *(int **)(*(int *)(iVar5 + 0x58) + 8);
          iVar5 = (**(code **)(**(int **)((int)this + 0x220) + 0x3c))();
          if (iVar5 == puVar1[2]) {
            piVar2 = (int *)*puVar1;
            piStack_70 = (int *)0x0;
            iVar5 = (**(code **)(*piVar2 + 0x4c))(piVar2,unaff_EBP,unaff_EBX);
            if (iVar5 == 0) {
              piStack_70 = (int *)0x10004409;
              uVar4 = (**(code **)(**(int **)((int)this + 0x220) + 0x14))();
              piStack_70 = (int *)0x1;
              *(undefined4 *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 4) = uVar4
              ;
              *(undefined4 *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 0x10) = 1;
              *(undefined4 *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 0xc) = 0;
              piVar2 = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
              iVar5 = (**(code **)(*piVar2 + 0x30))(piVar2,0);
              if (iVar5 == 0) {
                return 1;
              }
            }
          }
        }
      }
    }
    piStack_70 = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
    iVar5 = (**(code **)(*piStack_70 + 8))();
    while (iVar5 != 0) {
      iVar5 = (**(code **)(*(int *)**(undefined4 **)
                                     (*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 8))();
    }
    operator_delete(*(void **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58));
    *(undefined4 *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) = 0;
    return 0;
  }
  piStack_6c = *(int **)(iVar5 + 4);
  piStack_68 = (int *)0x0;
  piStack_70 = (int *)0x100044a4;
  iVar5 = (**(code **)(**(int **)((int)this + 0x220) + 0x30))();
  if (iVar5 != *(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 4)) {
    return 0;
  }
  piStack_70 = (int *)0x2c;
  iVar5 = (**(code **)(**(int **)((int)this + 0x220) + 0x3c))();
  if (iVar5 != 0x2c) {
    return 0;
  }
  uStack_50 = 0;
  local_4c = 0;
  local_48 = 0;
  uStack_44 = 0;
  puStack_40 = (undefined2 *)0x0;
  pvVar7 = operator_new(0x34);
  *(void **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) = pvVar7;
  *(undefined4 *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 0x28) = 0;
  *(undefined4 *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 0x2c) = 0;
  iVar5 = *(int *)((int)this + param_1 * 4 + 0x3c);
  *(int *)(*(int *)(iVar5 + 0x58) + 0x30) = (*(int *)(iVar5 + 8) - *(int *)(iVar5 + 4)) + -0x2c;
  uStack_50 = 0x14;
  local_4c = 0xe2;
  iVar5 = *(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
  puStack_40 = (undefined2 *)(iVar5 + 0x14);
  *puStack_40 = 1;
  *(undefined2 *)(iVar5 + 0x16) = uStack_26;
  *(undefined4 *)(iVar5 + 0x18) = uStack_24;
  *(undefined4 *)(iVar5 + 0x1c) = uStack_20;
  *(undefined2 *)(iVar5 + 0x20) = uStack_1c;
  *(undefined2 *)(iVar5 + 0x22) = uStack_1a;
  *(undefined2 *)(iVar5 + 0x24) = 0;
  local_48 = (uint)(*(ushort *)(iVar5 + 0x22) >> 3) * (uint)*(ushort *)(iVar5 + 0x16) *
             *(int *)(iVar5 + 0x18) * 6;
  if (uStack_14 <= local_48) {
    local_48 = uStack_14;
  }
  uVar12 = 0;
  iVar5 = (**(code **)(**(int **)((int)this + 0x38) + 0xc))
                    (*(int **)((int)this + 0x38),&uStack_50,
                     *(undefined4 *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58));
  if (iVar5 != 0) goto LAB_10004784;
  piVar2 = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
  ppiVar11 = &piStack_68;
  ppiVar10 = &piStack_70;
  iVar5 = (**(code **)(*piVar2 + 0x2c))
                    (piVar2,0,unaff_EBX,&stack0xffffff8c,ppiVar10,ppiVar11,&stack0xffffff9c,0);
  if ((iVar5 == 0) && (ppiVar11 != (int **)0x0)) {
    *(uint *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 8) = uVar12 >> 1;
    if (uVar12 == uStack_44) {
      *(undefined4 *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 4) = 0;
      ppiVar8 = (int **)(**(code **)(**(int **)((int)this + 0x220) + 0x3c))(ppiVar10,ppiVar11);
      if ((ppiVar8 == ppiVar11) &&
         (piVar2 = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58),
         iVar5 = (**(code **)(*piVar2 + 0x4c))(piVar2,ppiVar10,ppiVar11,0,0), iVar5 == 0)) {
        piVar2 = *(int **)((int)this + param_1 * 4 + 0x3c);
        piVar3 = *(int **)piVar2[0x16];
        iVar5 = (**(code **)(*piVar3 + 0x30))(piVar3,0,0,*piVar2 != 0);
joined_r0x10004725:
        if (iVar5 == 0) {
          if (*(int *)((int)this + 0x230) == 0) {
            UVar9 = FUN_100065f0(*(void **)((int)this + 0x228),100);
            *(UINT_PTR *)((int)this + 0x22c) = UVar9;
          }
          *(int *)((int)this + 0x230) = *(int *)((int)this + 0x230) + 1;
          return 1;
        }
      }
    }
    else {
      iVar5 = (**(code **)(**(int **)((int)this + 0x220) + 0x3c))
                        (ppiVar10,*(undefined4 *)
                                   (*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 8));
      if (iVar5 == *(int *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 8)) {
        uVar4 = (**(code **)(**(int **)((int)this + 0x220) + 0x14))();
        *(undefined4 *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 4) = uVar4;
        piVar2 = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
        iVar5 = (**(code **)(*piVar2 + 0x4c))(piVar2,ppiVar10,ppiVar11,0,0);
        if (iVar5 == 0) {
          *(undefined4 *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 0x10) = 1;
          *(undefined4 *)(*(int *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) + 0xc) = 0;
          piVar2 = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
          iVar5 = (**(code **)(*piVar2 + 0x30))(piVar2,0,0,1);
          goto joined_r0x10004725;
        }
      }
    }
  }
  piVar2 = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
  iVar5 = (**(code **)(*piVar2 + 8))(piVar2);
  while (iVar5 != 0) {
    piVar2 = (int *)**(undefined4 **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58);
    iVar5 = (**(code **)(*piVar2 + 8))(piVar2);
  }
LAB_10004784:
  operator_delete(*(void **)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58));
  *(undefined4 *)(*(int *)((int)this + param_1 * 4 + 0x3c) + 0x58) = 0;
  return 0;
}

// clang-format on
