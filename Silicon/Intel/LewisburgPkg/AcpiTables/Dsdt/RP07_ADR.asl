/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

Method (_ADR, 0) {
  If (LNotEqual(RPA7,0)) {
    Return (RPA7)
  } Else {
    Return (0x001C0006)
  }
}