/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: FUN_10001020 @ 10001020
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: FUN_10001020 at 10001020
// Signature: CWnd * __thiscall FUN_10001020(void * this, undefined4 param_1)
// Entry: 10001020
// Body: 10001020 - 1000103a
// ============================================================================
CWnd * __thiscall FUN_10001020(void *this,undefined4 param_1)

{
  CWnd::CWnd(this);
  *(undefined ***)this = &PTR_LAB_10007438;
  *(undefined4 *)((int)this + 0x40) = param_1;
  return this;
}

// clang-format on
