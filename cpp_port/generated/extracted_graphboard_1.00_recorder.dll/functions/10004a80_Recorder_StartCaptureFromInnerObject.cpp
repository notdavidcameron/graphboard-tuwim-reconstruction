/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/Recorder.dll
  Source function: Recorder_StartCaptureFromInnerObject @ 10004a80
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: Recorder_StartCaptureFromInnerObject at 10004a80
// Signature: bool __stdcall Recorder_StartCaptureFromInnerObject(int * param_1)
// Entry: 10004a80
// Body: 10004a80 - 10004aa2
// ============================================================================
bool Recorder_StartCaptureFromInnerObject(int *param_1)

{
  bool bVar1;
  undefined3 extraout_var;
  
  (**(code **)(*param_1 + 0x2c))(param_1);
  param_1[0xf] = (int)param_1;
  bVar1 = Recorder_StartCapture((int)(param_1 + -1));
  return (bool)('\x01' - (CONCAT31(extraout_var,bVar1) != 0));
}

// clang-format on
