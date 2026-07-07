/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: DllGetClassObject @ 10001fa0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: DllGetClassObject at 10001fa0
// Signature: HRESULT __stdcall DllGetClassObject(IID * rclsid, IID * riid, LPVOID * ppv)
// Entry: 10001fa0
// Body: 10001fa0 - 10002085
// ============================================================================
/* WARNING: Removing unreachable block (ram,0x10001fac) */

HRESULT DllGetClassObject(IID *rclsid,IID *riid,LPVOID *ppv)

{
  int *piVar1;
  ulong *puVar2;
  ulong uVar3;
  int iVar4;
  undefined4 *puVar5;
  
                    /* 0x1fa0  2  DllGetClassObject */
  puVar5 = DAT_100082a8;
  iVar4 = 0;
  if (ppv == (LPVOID *)0x0) {
    return -0x7fffbffd;
  }
  *ppv = (LPVOID)0x0;
  puVar2 = (ulong *)*puVar5;
  do {
    if (puVar2 == (ulong *)0x0) {
LAB_1000206e:
      if ((*ppv == (LPVOID)0x0) && (iVar4 == 0)) {
        iVar4 = -0x7ffbfeef;
      }
      return iVar4;
    }
    if ((((puVar5[2] != 0) && (rclsid->Data1 == *puVar2)) &&
        (uVar3._0_2_ = rclsid->Data2, uVar3._2_2_ = rclsid->Data3, uVar3 == puVar2[1])) &&
       ((*(ulong *)rclsid->Data4 == puVar2[2] && (*(ulong *)(rclsid->Data4 + 4) == puVar2[3])))) {
      piVar1 = puVar5 + 4;
      if (puVar5[4] == 0) {
        EnterCriticalSection((LPCRITICAL_SECTION)&DAT_100082e4);
        if (*piVar1 == 0) {
          iVar4 = (*(code *)puVar5[2])(puVar5[3],&DAT_10006938,piVar1);
        }
        LeaveCriticalSection((LPCRITICAL_SECTION)&DAT_100082e4);
      }
      puVar5 = (undefined4 *)*piVar1;
      if (puVar5 != (undefined4 *)0x0) {
        iVar4 = (**(code **)*puVar5)(puVar5,riid,ppv);
      }
      goto LAB_1000206e;
    }
    if (DAT_10008298 == 100) {
      puVar5 = puVar5 + 7;
    }
    else {
      puVar5 = puVar5 + 9;
    }
    puVar2 = (ulong *)*puVar5;
  } while( true );
}

// clang-format on
