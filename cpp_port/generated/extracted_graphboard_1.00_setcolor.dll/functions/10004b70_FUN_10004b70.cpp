/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_10004b70 @ 10004b70
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10004b70 at 10004b70
// Signature: undefined __cdecl FUN_10004b70(_onexit_t param_1)
// Entry: 10004b70
// Body: 10004b70 - 10004b9b
// ============================================================================
void __cdecl FUN_10004b70(_onexit_t param_1)

{
  if (DAT_100093e8 == -1) {
    _onexit(param_1);
    return;
  }
  __dllonexit(param_1,&DAT_100093e8,&DAT_100093e4);
  return;
}

// clang-format on
