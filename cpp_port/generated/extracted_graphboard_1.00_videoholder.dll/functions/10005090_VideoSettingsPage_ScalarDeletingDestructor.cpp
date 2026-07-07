/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/VideoHolder.dll
  Source function: VideoSettingsPage_ScalarDeletingDestructor @ 10005090
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: VideoSettingsPage_ScalarDeletingDestructor at 10005090
// Signature: CPropertyPage * __thiscall VideoSettingsPage_ScalarDeletingDestructor(void * this, byte param_1)
// Entry: 10005090
// Body: 10005090 - 100050ad
// ============================================================================
CPropertyPage * __thiscall VideoSettingsPage_ScalarDeletingDestructor(void *this,byte param_1)

{
  VideoSettingsPage_Destroy(this);
  if ((param_1 & 1) != 0) {
    operator_delete(this);
  }
  return this;
}

// clang-format on
