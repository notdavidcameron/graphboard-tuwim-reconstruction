/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: VideoHolderClassFactory_Destroy @ 10001960
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: VideoHolderClassFactory_Destroy at 10001960
// Signature: undefined __fastcall VideoHolderClassFactory_Destroy(undefined4 * param_1)
// Entry: 10001960
// Body: 10001960 - 1000198f
// ============================================================================
void __fastcall VideoHolderClassFactory_Destroy(undefined4 *param_1)

{
  *param_1 = &PTR_VideoHolder_QueryInterfaceFromMap_100077a4;
  param_1[1] = 1;
  DeleteCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  DeleteCriticalSection
            ((LPCRITICAL_SECTION)((-(uint)(param_1 != (undefined4 *)0x0) & (uint)(param_1 + 1)) + 4)
            );
  return;
}

// clang-format on
