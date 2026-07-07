/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: AudioHolderClassFactory_Destroy @ 100016b0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: AudioHolderClassFactory_Destroy at 100016b0
// Signature: undefined __fastcall AudioHolderClassFactory_Destroy(undefined4 * param_1)
// Entry: 100016b0
// Body: 100016b0 - 100016df
// ============================================================================
void __fastcall AudioHolderClassFactory_Destroy(undefined4 *param_1)

{
  *param_1 = &PTR_AudioHolder_QueryInterfaceFromMap_10008654;
  param_1[1] = 1;
  DeleteCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  DeleteCriticalSection
            ((LPCRITICAL_SECTION)((-(uint)(param_1 != (undefined4 *)0x0) & (uint)(param_1 + 1)) + 4)
            );
  return;
}

// clang-format on
