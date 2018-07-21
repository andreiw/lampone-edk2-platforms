/** @file
  Copyright (c) 2015 - 2018, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _EEPROM_DATA_FV_LIB_
#define _EEPROM_DATA_FV_LIB_

#include <Uefi.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/EepromDataLib.h>
#include <Library/MemoryAllocationLib.h>

EFI_STATUS
EFIAPI
GetImage (
  IN  EFI_GUID            *NameGuid,
  OUT VOID               **Buffer,
  OUT UINTN               *Size
  );

#endif // _EEPROM_DATA_FV_LIB_
