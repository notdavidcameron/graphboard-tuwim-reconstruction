/*
  Direct C++ port generated from Ghidra MCP localhost.
  Source program: /extracted/GraphBoard 1.00/SetColor.dll
  Source function: CpuFeatures_DetectMmxSupport @ 10004280
*/
#include "../../../include/ghidra_compat.hpp"

// clang-format off

// ============================================================================
// Function: CpuFeatures_DetectMmxSupport at 10004280
// Signature: undefined4 __stdcall CpuFeatures_DetectMmxSupport(void)
// Entry: 10004280
// Body: 10004280 - 100042c5
// ============================================================================
/* WARNING: Removing unreachable block (ram,0x100042b1) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 CpuFeatures_DetectMmxSupport(void)

{
  undefined4 *puVar1;
  uint uVar2;
  undefined4 uVar3;
  byte in_CF;
  byte in_PF;
  byte in_AF;
  byte in_ZF;
  byte in_SF;
  byte in_TF;
  byte in_IF;
  byte in_OF;
  byte in_NT;
  byte in_AC;
  byte in_VIF;
  byte in_VIP;
  byte in_ID;
  uint uVar4;
  
  _DAT_10008320 = 0;
  uVar4 = (uint)(in_NT & 1) * 0x4000 | (uint)(in_OF & 1) * 0x800 | (uint)(in_IF & 1) * 0x200 |
          (uint)(in_TF & 1) * 0x100 | (uint)(in_SF & 1) * 0x80 | (uint)(in_ZF & 1) * 0x40 |
          (uint)(in_AF & 1) * 0x10 | (uint)(in_PF & 1) * 4 | (uint)(in_CF & 1) |
          (uint)(in_ID & 1) * 0x200000 | (uint)(in_VIP & 1) * 0x100000 |
          (uint)(in_VIF & 1) * 0x80000 | (uint)(in_AC & 1) * 0x40000;
  uVar2 = uVar4 ^ 0x40000;
  uVar3 = 0;
  if (((uint)((uVar2 & 0x4000) != 0) * 0x4000 | (uint)((uVar2 & 0x800) != 0) * 0x800 |
       (uint)((uVar2 & 0x200) != 0) * 0x200 | (uint)((uVar2 & 0x100) != 0) * 0x100 |
       (uint)((uVar2 & 0x80) != 0) * 0x80 | (uint)((uVar2 & 0x40) != 0) * 0x40 |
       (uint)((uVar2 & 0x10) != 0) * 0x10 | (uint)((uVar2 & 4) != 0) * 4 | (uint)((uVar2 & 1) != 0)
       | (uint)((uVar2 & 0x200000) != 0) * 0x200000 | (uint)((uVar2 & 0x40000) != 0) * 0x40000) !=
      uVar4) {
    uVar2 = uVar4 ^ 0x200000;
    uVar3 = 0;
    if (((uint)((uVar2 & 0x4000) != 0) * 0x4000 | (uint)((uVar2 & 0x800) != 0) * 0x800 |
         (uint)((uVar2 & 0x200) != 0) * 0x200 | (uint)((uVar2 & 0x100) != 0) * 0x100 |
         (uint)((uVar2 & 0x80) != 0) * 0x80 | (uint)((uVar2 & 0x40) != 0) * 0x40 |
         (uint)((uVar2 & 0x10) != 0) * 0x10 | (uint)((uVar2 & 4) != 0) * 4 |
         (uint)((uVar2 & 1) != 0) | (uint)((uVar2 & 0x200000) != 0) * 0x200000 |
        (uint)((uVar2 & 0x40000) != 0) * 0x40000) != uVar4) {
      puVar1 = (undefined4 *)cpuid_Version_info(1);
      uVar3 = *puVar1;
      if ((puVar1[2] & 0x800000) != 0) {
        _DAT_10008320 = 1;
      }
    }
  }
  return uVar3;
}

// clang-format on
