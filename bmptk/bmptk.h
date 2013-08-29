//***************************************************************************
//
// file : bmptk/bmptk.h
//
// LICENSE (MIT expat license, copy of bmptk/license.txt)
//
// Copyright (c) 2012, 2013 Wouter van Ooijen (wouter@voti.nl)
// 
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
// 
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//***************************************************************************

#ifndef _BMPTK_H
#define _BMPTK_H

// include the target-independent bmptk header files
#include "core/basics.h"	
#include "core/time.h"	
#include "core/memory.h"	
#include "core/channels.h"
#include "core/context.h"	
#include "core/rtos.h"
#include "graphics/graphics.h"
#include "graphics/font_default.h"
#include "hardware/pins.h"
#include "hardware/ports.h"
#include "hardware/demos.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "chips/pcf8574.h"
#include "chips/pcd8544.h"
#include "chips/hc595.h"
//#include "hardware/rfm73.h"
//#include "hardware/nrf24l01.h"
#include "chips/sr04.h"
            
// include the target-specific header file
#ifdef BMPTK_TARGET_nds
   #include "targets/nds/nds.h"
#endif              
#ifdef BMPTK_TARGET_win
   #include "targets/win/win.h"
#endif              
#ifdef BMPTK_CHIP_lpc810m021
   #include "targets/cortex/lpc810m021.h"
#endif              
#ifdef BMPTK_CHIP_lpc1114fn28
   #include "targets/cortex/lpc1114fn28.h"
#endif              
#ifdef BMPTK_CHIP_lpc1227fbd301
   #include "targets/cortex/lpc1227fbd301.h"
#endif              
#ifdef BMPTK_CHIP_lpc2478
   #include "targets/armv7/lpc2478.h"
#endif              
      
#endif // #ifndef _BMPTK_H
