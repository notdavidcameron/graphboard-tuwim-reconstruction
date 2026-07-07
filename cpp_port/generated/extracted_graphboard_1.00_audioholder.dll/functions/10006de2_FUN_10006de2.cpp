/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10006de2 @ 10006de2
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10006de2 at 10006de2
// Signature: undefined __cdecl FUN_10006de2(_onexit_t param_1)
// Entry: 10006de2
// Body: 10006de2 - 10006e0d
// ============================================================================
void __cdecl FUN_10006de2(_onexit_t param_1)

{
  if (DAT_1000c5b0 == -1) {
    _onexit(param_1);
    return;
  }
  __dllonexit(param_1,&DAT_1000c5b0,&DAT_1000c5ac);
  return;
}

// clang-format on
