/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/AudioHolder.dll
  Source function: AudioHolder_AttachBackPointer @ 10003b00
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: AudioHolder_AttachBackPointer at 10003b00
// Signature: undefined4 __stdcall AudioHolder_AttachBackPointer(int param_1)
// Entry: 10003b00
// Body: 10003b00 - 10003b1b
// ============================================================================
undefined4 AudioHolder_AttachBackPointer(int param_1)

{
  (**(code **)(*(int *)(param_1 + 4) + 0x2c))(param_1 + 4);
  *(int *)(param_1 + 0x34) = param_1;
  return 0;
}

// clang-format on
