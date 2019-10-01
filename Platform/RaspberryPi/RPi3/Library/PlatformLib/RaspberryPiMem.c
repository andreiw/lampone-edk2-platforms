/** @file
 *
 *  Copyright (c) 2017-2018, Andrey Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) 2014, Linaro Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <IndustryStandard/Bcm2836.h>
#include <Library/PcdLib.h>

extern UINT64 mSystemMemoryEnd;
extern UINT64 mGPUMemoryBase;
extern UINT64 mGPUMemoryLength;

#define VariablesSize (FixedPcdGet32(PcdFlashNvStorageVariableSize) +   \
                       FixedPcdGet32(PcdFlashNvStorageFtwWorkingSize) + \
                       FixedPcdGet32(PcdFlashNvStorageFtwSpareSize) +  \
                       FixedPcdGet32(PcdNvStorageEventLogSize))

#define VariablesBase (FixedPcdGet64(PcdFdBaseAddress) + \
                       FixedPcdGet32(PcdFdSize) - \
                       VariablesSize)

#define ATFBase (FixedPcdGet64(PcdFdBaseAddress) + FixedPcdGet32(PcdFdSize))

STATIC ARM_MEMORY_REGION_DESCRIPTOR RaspberryPiMemoryRegionDescriptor[] = {
  {
    /* 0 - Firmware Volume. */
    FixedPcdGet64 (PcdFdBaseAddress), FixedPcdGet64 (PcdFdBaseAddress),
    FixedPcdGet32 (PcdFdSize) - VariablesSize,
    ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK
  },
  {
    /* 1 - Variables Volume. */
    VariablesBase, VariablesBase,
    VariablesSize,
    ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK
  },
  {
    /* 2 - ATF reserved RAM. */
    ATFBase, ATFBase,
    FixedPcdGet64 (PcdSystemMemoryBase) - ATFBase,
    ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK
  },
  {
    /* 3 - System RAM. */
    FixedPcdGet64 (PcdSystemMemoryBase), FixedPcdGet64 (PcdSystemMemoryBase),
    0,
    ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK
  },
  {
    /* 4 - Reserved GPU RAM. */
    0,
    0,
    0,
    ARM_MEMORY_REGION_ATTRIBUTE_DEVICE
  },
  {
    /* 5 - More RAM. */
    0x40000000,
    0x40000000,
    BCM2836_SOC_REGISTERS - 0x40000000,
    ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK
  },
  {
    /* 6 - SOC registers. */
    BCM2836_SOC_REGISTERS,
    BCM2836_SOC_REGISTERS,
    BCM2836_SOC_REGISTER_LENGTH,
    ARM_MEMORY_REGION_ATTRIBUTE_DEVICE
  },
  {
  }
};

/**
  Return the Virtual Memory Map of your platform

  This Virtual Memory Map is used by MemoryInitPei Module to initialize the MMU
  on your platform.

  @param[out]   VirtualMemoryMap    Array of ARM_MEMORY_REGION_DESCRIPTOR
                                    describing a Physical-to-Virtual Memory
                                    mapping. This array must be ended by a
                                    zero-filled entry

**/
VOID
ArmPlatformGetVirtualMemoryMap (
  IN ARM_MEMORY_REGION_DESCRIPTOR** VirtualMemoryMap
  )
{
  RaspberryPiMemoryRegionDescriptor[3].Length = mSystemMemoryEnd + 1 -
    FixedPcdGet64 (PcdSystemMemoryBase);

  RaspberryPiMemoryRegionDescriptor[4].PhysicalBase =
    RaspberryPiMemoryRegionDescriptor[3].PhysicalBase +
    RaspberryPiMemoryRegionDescriptor[3].Length;

  RaspberryPiMemoryRegionDescriptor[4].VirtualBase =
    RaspberryPiMemoryRegionDescriptor[4].PhysicalBase;

  RaspberryPiMemoryRegionDescriptor[4].Length =
    RaspberryPiMemoryRegionDescriptor[5].PhysicalBase -
    RaspberryPiMemoryRegionDescriptor[4].PhysicalBase;

  DEBUG ((DEBUG_INFO, "FD:\n"
    "\tPhysicalBase: 0x%lX\n"
    "\tVirtualBase: 0x%lX\n"
    "\tLength: 0x%lX\n",
    RaspberryPiMemoryRegionDescriptor[0].PhysicalBase,
    RaspberryPiMemoryRegionDescriptor[0].VirtualBase,
    RaspberryPiMemoryRegionDescriptor[0].Length +
    RaspberryPiMemoryRegionDescriptor[1].Length));

  DEBUG ((DEBUG_INFO, "Variables (part of FD):\n"
    "\tPhysicalBase: 0x%lX\n"
    "\tVirtualBase: 0x%lX\n"
    "\tLength: 0x%lX\n",
    RaspberryPiMemoryRegionDescriptor[1].PhysicalBase,
    RaspberryPiMemoryRegionDescriptor[1].VirtualBase,
    RaspberryPiMemoryRegionDescriptor[1].Length));

  DEBUG ((DEBUG_INFO, "ATF RAM:\n"
    "\tPhysicalBase: 0x%lX\n"
    "\tVirtualBase: 0x%lX\n"
    "\tLength: 0x%lX\n",
    RaspberryPiMemoryRegionDescriptor[2].PhysicalBase,
    RaspberryPiMemoryRegionDescriptor[2].VirtualBase,
    RaspberryPiMemoryRegionDescriptor[2].Length));

  DEBUG ((DEBUG_INFO, "System RAM:\n"
    "\tPhysicalBase: 0x%lX\n"
    "\tVirtualBase: 0x%lX\n"
    "\tLength: 0x%lX\n",
    RaspberryPiMemoryRegionDescriptor[3].PhysicalBase,
    RaspberryPiMemoryRegionDescriptor[3].VirtualBase,
    RaspberryPiMemoryRegionDescriptor[3].Length));

  DEBUG ((DEBUG_INFO, "GPU Reserved:\n"
    "\tPhysicalBase: 0x%lX\n"
    "\tVirtualBase: 0x%lX\n"
    "\tLength: 0x%lX\n",
    RaspberryPiMemoryRegionDescriptor[4].PhysicalBase,
    RaspberryPiMemoryRegionDescriptor[4].VirtualBase,
    RaspberryPiMemoryRegionDescriptor[4].Length));

  DEBUG ((DEBUG_INFO, "More RAM:\n"
    "\tPhysicalBase: 0x%lX\n"
    "\tVirtualBase: 0x%lX\n"
    "\tLength: 0x%lX\n",
    RaspberryPiMemoryRegionDescriptor[5].PhysicalBase,
    RaspberryPiMemoryRegionDescriptor[5].VirtualBase,
    RaspberryPiMemoryRegionDescriptor[5].Length));

  DEBUG ((DEBUG_INFO, "SoC reserved:\n"
    "\tPhysicalBase: 0x%lX\n"
    "\tVirtualBase: 0x%lX\n"
    "\tLength: 0x%lX\n",
    RaspberryPiMemoryRegionDescriptor[6].PhysicalBase,
    RaspberryPiMemoryRegionDescriptor[6].VirtualBase,
    RaspberryPiMemoryRegionDescriptor[6].Length));

  *VirtualMemoryMap = RaspberryPiMemoryRegionDescriptor;
}
