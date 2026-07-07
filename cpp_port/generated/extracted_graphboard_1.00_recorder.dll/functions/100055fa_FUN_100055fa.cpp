/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: FUN_100055fa @ 100055fa
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_100055fa at 100055fa
// Signature: undefined __cdecl FUN_100055fa(_onexit_t param_1)
// Entry: 100055fa
// Body: 100055fa - 10005625
// ============================================================================
void __cdecl FUN_100055fa(_onexit_t param_1)

{
  if (DAT_100094e0 == -1) {
    _onexit(param_1);
    return;
  }
  __dllonexit(param_1,&DAT_100094e0,&DAT_100094dc);
  return;
}

// clang-format on
