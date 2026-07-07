/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10006090 @ 10006090
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10006090 at 10006090
// Signature: undefined __fastcall FUN_10006090(int param_1)
// Entry: 10006090
// Body: 10006090 - 100060c4
// ============================================================================
void __fastcall FUN_10006090(int param_1)

{
  void *this;
  int iVar1;
  
  iVar1 = 0;
  this = *(void **)(param_1 + 0x90);
  if (0 < *(int *)((int)this + 0x21c)) {
    do {
      AudioHolder_StopPlaybackForEntry(this,iVar1);
      this = *(void **)(param_1 + 0x90);
      iVar1 = iVar1 + 1;
    } while (iVar1 < *(int *)((int)this + 0x21c));
  }
  FUN_10005440(param_1);
  return;
}

// clang-format on
