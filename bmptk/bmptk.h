//***************************************************************************
//
// file : bmptk/bmptk.h
//
// Copyright (c) 2012 ... 2015 Wouter van Ooijen (wouter@voti.nl)
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// This is the one header file a user should include to
// use the bmptk core facilities (which are mainly supplied by the 
// target-specific files).
//
//***************************************************************************

#ifndef _BMPTK_H_
#define _BMPTK_H_

// ==========================================================================
// 
// version
//
// ==========================================================================

//! helper macro for the bmtkp version
#define BMPTK_VERSION_STR( X ) #X

//! the bmtkp version, which is passed on the command line by Makefile.inc
const char version[] = BMPTK_VERSION_STR( BMPTK_VERSION );	


// ==========================================================================
// 
// debug macro's
//
// ==========================================================================

//! file-name : line-number macro
//
//! The macro BMPTK_HERE tranlsates to a newline, the file-name, ":",
//! and the line-number of the place where the BMPTK_HERE macro
//! appears. This can be usefull for debug logging.
#define BMPTK_HERE_STR( X ) #X
#define BMPTK_HERE2( F, L ) ( "\n" F ":" BMPTK_HERE_STR( L ) " " )
#define BMPTK_HERE BMPTK_HERE2( __FILE__, __LINE__ )

#define BMPTK_TRACE ( hwcpp::io::cout << BMPTK_HERE )

          
// ==========================================================================
// 
// include the target-specific header file(s)
//
// ==========================================================================
                        
#ifdef BMPTK_TARGET_win
   #include "targets/win/win.h"
#endif              
#ifdef BMPTK_CHIP_lpc810m021
   #include "targets/cortex/cmsis/lpc800/inc/lpc810m021.h"
#endif              
#ifdef BMPTK_CHIP_lpc1114fn28
   #include "targets/cortex/cmsis/11xx/inc/LPC11xx.h"
   #include "targets/cortex/cortex.h"
#endif                          
      
#endif // #ifndef _BMPTK_H_
