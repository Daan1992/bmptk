/***************************************************************************
 **
 **
 **    Master inlude file
 **
 **    Used with ARM IAR C/C++ Compiler
 **
 **    (c) Copyright IAR Systems 2007
 **
 **    $Revision: 30870 $
 **
 ***************************************************************************/

#ifndef __INCLUDES_H
#define __INCLUDES_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
// #include <intrinsics.h>
#include <assert.h>
// #include <nxp/iolpc2478.h>

#include "arm_comm.h"
#include "board.h"

// SPP +
#include "bmp.h"
#include "cursor_arrow.h"
#include "My_Functions.h"
/*
#include "Console_Terminal.h"
#include "Console_Functions.h"
#include "efs.h"
#include "ls.h"
*/

#include "uart.h"
// SPP -

/*
#include "usb_cnfg.h"
#include "usb_desc.h"
#include "usb_hw.h"
#include "usb_t9.h"
#include "usb_hooks.h"
#include "usb_dev_desc.h"
#include "usb_hooks.h"
#include "usb_buffer.h"

#include "scsi_ll.h"
#include "scsi.h"
#include "scsi_cmd.h"

#include "disk.h"
#include "lun.h"


#include "sd_dsk_desc.h"
#include "sd_card_mode.h"
*/

#include "sys.h"
#include "sdram_32M_16bit_drv.h"
#include "drv_glcd.h"
#include "drv_touch_scr.h"


#define MAX_PATH_LENGTH 128
extern char GlobalPath[MAX_PATH_LENGTH];

#endif  // __INCLUDES_H
