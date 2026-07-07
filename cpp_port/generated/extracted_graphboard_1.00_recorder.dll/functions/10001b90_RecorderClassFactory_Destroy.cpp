/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: RecorderClassFactory_Destroy @ 10001b90
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: RecorderClassFactory_Destroy at 10001b90
// Signature: undefined __fastcall RecorderClassFactory_Destroy(undefined4 * param_1)
// Entry: 10001b90
// Body: 10001b90 - 10001bbf
// ============================================================================
void __fastcall RecorderClassFactory_Destroy(undefined4 *param_1)

{
  *param_1 = &PTR_Recorder_QueryInterfaceFromMap_100067fc;
  param_1[1] = 1;
  DeleteCriticalSection((LPCRITICAL_SECTION)(param_1 + 9));
  DeleteCriticalSection
            ((LPCRITICAL_SECTION)((-(uint)(param_1 != (undefined4 *)0x0) & (uint)(param_1 + 1)) + 4)
            );
  return;
}

// clang-format on
