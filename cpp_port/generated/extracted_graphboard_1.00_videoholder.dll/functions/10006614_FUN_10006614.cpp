/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10006614 @ 10006614
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10006614 at 10006614
// Signature: undefined __cdecl FUN_10006614(_onexit_t param_1)
// Entry: 10006614
// Body: 10006614 - 1000663f
// ============================================================================
void __cdecl FUN_10006614(_onexit_t param_1)

{
  if (DAT_1000b568 == -1) {
    _onexit(param_1);
    return;
  }
  __dllonexit(param_1,&DAT_1000b568,&DAT_1000b564);
  return;
}

// clang-format on
