/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: FUN_10004bc0 @ 10004bc0
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10004bc0 at 10004bc0
// Signature: undefined __stdcall FUN_10004bc0(void)
// Entry: 10004bc0
// Body: 10004bc0 - 10004bee
// ============================================================================
/* WARNING: Unable to track spacebase fully for stack */

void FUN_10004bc0(void)

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
