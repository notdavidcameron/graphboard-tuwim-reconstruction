/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: SetColorClassFactory_Destroy @ 10002340
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: SetColorClassFactory_Destroy at 10002340
// Signature: undefined __fastcall SetColorClassFactory_Destroy(undefined4 * param_1)
// Entry: 10002340
// Body: 10002340 - 1000236f
// ============================================================================
void __fastcall SetColorClassFactory_Destroy(undefined4 *param_1)

{
  *param_1 = &PTR_SetColor_QueryInterfaceFromMap_1000674c;
  param_1[1] = 1;
  DeleteCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  DeleteCriticalSection
            ((LPCRITICAL_SECTION)((-(uint)(param_1 != (undefined4 *)0x0) & (uint)(param_1 + 1)) + 4)
            );
  return;
}

// clang-format on
