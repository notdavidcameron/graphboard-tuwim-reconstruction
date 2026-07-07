/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: FUN_10006e30 @ 10006e30
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10006e30 at 10006e30
// Signature: undefined __stdcall FUN_10006e30(void)
// Entry: 10006e30
// Body: 10006e30 - 10006e5e
// ============================================================================
/* WARNING: Unable to track spacebase fully for stack */

void FUN_10006e30(void)

{
  uint in_EAX;
  undefined1 *puVar1;
  undefined4 unaff_retaddr;
  
  puVar1 = &stack0x00000004;
  for (; 0xfff < in_EAX; in_EAX = in_EAX - 0x1000) {
    puVar1 = puVar1 + -0x1000;
  }
  *(undefined4 *)(puVar1 + (-4 - in_EAX)) = unaff_retaddr;
  return;
}

// clang-format on
