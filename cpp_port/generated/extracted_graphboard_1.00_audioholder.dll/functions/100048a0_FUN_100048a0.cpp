/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_100048a0 @ 100048a0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100048a0 at 100048a0
// Signature: undefined __fastcall FUN_100048a0(void * param_1)
// Entry: 100048a0
// Body: 100048a0 - 10004c4a
// ============================================================================
void __fastcall FUN_100048a0(void *param_1)

{
  void *pvVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  uint uVar5;
  int iVar6;
  undefined4 uVar7;
  uint uVar8;
  void *unaff_EBX;
  int *piVar9;
  undefined4 *puVar10;
  void *local_28;
  int iStack_24;
  int iStack_20;
  int iStack_1c;
  int iStack_18;
  int iStack_14;
  undefined4 uStack_10;
  int iStack_c;
  undefined1 auStack_8 [8];
  
  local_28 = param_1;
  iVar2 = (**(code **)(**(int **)((int)param_1 + 0x2c) + 0x28))(*(int **)((int)param_1 + 0x2c));
  if ((iVar2 == 0) || (iStack_24 = 0, *(int *)((int)param_1 + 0x21c) < 1)) {
    return;
  }
  piVar9 = (int *)((int)param_1 + 0x3c);
  do {
    iVar2 = iStack_24;
    piVar4 = *(int **)(*piVar9 + 0x58);
    if ((piVar4 != (int *)0x0) && (*piVar4 != 0)) {
      if (piVar4[1] == 0) {
        iVar3 = (**(code **)(*(int *)*piVar4 + 0x24))((int *)*piVar4,&iStack_c);
        if (iVar3 == 0) {
          iVar3 = (**(code **)(*(int *)**(undefined4 **)(*piVar9 + 0x58) + 0x10))
                            ((int *)**(undefined4 **)(*piVar9 + 0x58),&local_28,auStack_8);
          if (iVar3 != 0) goto LAB_10004c39;
          if ((iStack_c == 0) ||
             (iVar3 = *(int *)(*piVar9 + 0x58),
             *(void **)(iVar3 + 0x30) <=
             (void *)((int)local_28 + *(int *)(iVar3 + 0x2c) * *(int *)(iVar3 + 8) * 2))) {
            AudioHolder_StopPlaybackForEntry(param_1,iVar2);
          }
        }
      }
      else {
        iStack_1c = 0;
        iStack_20 = 0;
        piVar4 = (int *)**(undefined4 **)(*piVar9 + 0x58);
        iVar3 = (**(code **)(*piVar4 + 0x10))(piVar4,&local_28,auStack_8);
        if (iVar3 != 0) goto LAB_10004c39;
        piVar4 = (int *)*piVar9;
        if (*piVar4 == 0) {
          if (local_28 < *(void **)(piVar4[0x16] + 0x28)) {
            piVar4 = (int *)(piVar4[0x16] + 0x2c);
            *piVar4 = *piVar4 + 1;
          }
          *(void **)(*(int *)(*piVar9 + 0x58) + 0x28) = local_28;
          piVar4 = (int *)*piVar9;
          iVar3 = piVar4[0x16];
          if (*(void **)(iVar3 + 0x30) <=
              (void *)((int)local_28 + *(int *)(iVar3 + 0x2c) * *(int *)(iVar3 + 8) * 2)) {
            AudioHolder_StopPlaybackForEntry(param_1,iVar2);
            goto LAB_10004bbf;
          }
        }
        puVar10 = (undefined4 *)piVar4[0x16];
        pvVar1 = (void *)puVar10[2];
        if (((local_28 < pvVar1) && (puVar10[4] != 0)) || ((pvVar1 < local_28 && (puVar10[3] != 0)))
           ) {
          piVar4 = (int *)*puVar10;
          if (puVar10[4] == 0) {
            iVar3 = (**(code **)(*piVar4 + 0x2c))
                              (piVar4,0,pvVar1,&iStack_1c,&uStack_10,&iStack_20,&iStack_14,0);
            if (iVar3 != 0) goto LAB_10004c39;
            *(undefined4 *)(*(int *)(*piVar9 + 0x58) + 0xc) = 0;
            *(undefined4 *)(*(int *)(*piVar9 + 0x58) + 0x10) = 1;
          }
          else {
            iVar3 = (**(code **)(*piVar4 + 0x2c))(piVar4,pvVar1);
            if (iVar3 != 0) goto LAB_10004c39;
            *(undefined4 *)(*(int *)(*piVar9 + 0x58) + 0x10) = 0;
            *(undefined4 *)(*(int *)(*piVar9 + 0x58) + 0xc) = 1;
          }
          iVar2 = *(int *)(*piVar9 + 0x58);
          iVar3 = *(int *)(iVar2 + 4);
          uVar8 = *(uint *)(iVar2 + 8);
          uVar5 = *(int *)(*piVar9 + 8) - iVar3;
          if (uVar5 <= uVar8) {
            uVar8 = uVar5;
          }
          iVar6 = iStack_1c;
          if (iStack_1c == 0) {
            iVar6 = iStack_20;
          }
          iStack_18 = iVar6;
          iVar3 = (**(code **)(**(int **)((int)param_1 + 0x220) + 0x30))(iVar3,0);
          iVar2 = iStack_24;
          if ((iVar3 != *(int *)(*(int *)(*piVar9 + 0x58) + 4)) ||
             (uVar5 = (**(code **)(**(int **)((int)param_1 + 0x220) + 0x3c))(iVar6,uVar8),
             iVar2 = iStack_24, uVar5 != uVar8)) goto LAB_10004c39;
          puVar10 = (undefined4 *)((int *)*piVar9)[0x16];
          uVar5 = puVar10[2];
          param_1 = unaff_EBX;
          if (uVar8 != uVar5) {
            if (*(int *)*piVar9 == 0) {
              if (*(short *)((int)puVar10 + 0x22) == 8) {
                puVar10 = (undefined4 *)(uVar8 + iVar6);
                if (puVar10 < (undefined4 *)(uVar5 + iVar6)) {
                  uVar5 = (int)(uVar5 + iVar6) - (int)puVar10;
                  for (uVar8 = uVar5 >> 2; uVar8 != 0; uVar8 = uVar8 - 1) {
                    *puVar10 = 0x7f7f7f7f;
                    puVar10 = puVar10 + 1;
                  }
                  for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
                    *(undefined1 *)puVar10 = 0x7f;
                    puVar10 = (undefined4 *)((int)puVar10 + 1);
                  }
                }
              }
              else if (*(short *)((int)puVar10 + 0x22) == 0x10) {
                puVar10 = (undefined4 *)(uVar8 + iVar6);
                if (puVar10 < (undefined4 *)(uVar5 + iVar6)) {
                  uVar8 = (int)(uVar5 + iVar6) + (1 - (int)puVar10);
                  for (uVar5 = uVar8 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
                    *puVar10 = 0;
                    puVar10 = puVar10 + 1;
                  }
                  for (uVar8 = (uint)((uVar8 >> 1 & 1) != 0); uVar8 != 0; uVar8 = uVar8 - 1) {
                    *(undefined2 *)puVar10 = 0;
                    puVar10 = (undefined4 *)((int)puVar10 + 2);
                  }
                }
              }
              puVar10 = *(undefined4 **)(*piVar9 + 0x58);
              if (puVar10[4] == 0) {
                *(undefined4 *)
                 (*(int *)(*(int *)((int)unaff_EBX + iStack_24 * 4 + 0x3c) + 0x58) + 4) = 0;
                piVar9 = (int *)**(undefined4 **)
                                  (*(int *)((int)unaff_EBX + iStack_24 * 4 + 0x3c) + 0x58);
                (**(code **)(*piVar9 + 0x4c))(piVar9,iStack_1c,uStack_10,iStack_20,iStack_14);
                piVar9 = (int *)**(undefined4 **)
                                  (*(int *)((int)unaff_EBX + iVar2 * 4 + 0x3c) + 0x58);
                iVar3 = (**(code **)(*piVar9 + 0x30))(piVar9,0,0,0);
                iVar2 = iStack_24;
                if (iVar3 == 0) {
                  return;
                }
                goto LAB_10004c39;
              }
            }
            else {
              iVar3 = *piVar9;
              iVar6 = (**(code **)(**(int **)((int)unaff_EBX + 0x220) + 0x30))
                                (*(int *)(iVar3 + 4) + 0x2c,0);
              iVar2 = iStack_24;
              if (iVar6 != *(int *)(iVar3 + 4) + 0x2c) {
LAB_10004c39:
                AudioHolder_StopPlaybackForEntry(param_1,iVar2);
                return;
              }
              puVar10 = *(undefined4 **)(*piVar9 + 0x58);
              iVar3 = (**(code **)(**(int **)((int)unaff_EBX + 0x220) + 0x3c))
                                (iStack_18 + uVar8,*(int *)(*(int *)(iVar3 + 0x58) + 8) - uVar8);
              iVar2 = iStack_24;
              if (iVar3 != puVar10[2] - uVar8) goto LAB_10004c39;
            }
          }
          iVar2 = iStack_14;
          (**(code **)(*(int *)*puVar10 + 0x4c))((int *)*puVar10,iStack_1c,uStack_10,iStack_20);
          uVar7 = (**(code **)(**(int **)(iVar2 + 0x220) + 0x14))();
          *(undefined4 *)(*(int *)(*piVar9 + 0x58) + 4) = uVar7;
          iVar2 = iStack_24;
        }
      }
    }
LAB_10004bbf:
    iStack_24 = iVar2 + 1;
    piVar9 = piVar9 + 1;
    if (*(int *)((int)param_1 + 0x21c) <= iStack_24) {
      return;
    }
  } while( true );
}

// clang-format on
