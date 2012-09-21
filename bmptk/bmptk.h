//***************************************************************************
//
// file : bmptk/bmptk.h
//
// LICENSE (MIT expat license, copy of license.txt)
//
// Copyright (c) 2012 Wouter van Ooijen (wouter@voti.nl)
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
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE..
//
//***************************************************************************

#pragma once
#ifndef BMPTK_H
#define BMPTK_H

   // =======================================================================
   // 
   //! debug macro's
   //
   // =======================================================================

      //! file-name : line-number macro
      //
      //! The macro BMPTK_HERE transates to a newline, the file-name, ":",
      //! and the line-number of the place where the BMPTK_HERE macro
      //! appears. This is usefull for debug logging.
   #define BMPTK_HERE_STR( X ) #X
   #define BMPTK_HERE2( F, L ) ( "\n" F ":" BMPTK_HERE_STR( L ))
   #define BMPTK_HERE BMPTK_HERE2( __FILE__, __LINE__ )

      //! use instead of cout; prepends source file name and line number
      //
      //! Printing to trace (instead of cout) prepends BMPTK_HERE and a
      //! space, which makes it easy to locate the line in your sources.
      //! The output is also flushed, which can be usefull when the
      //! program will shortly crash, which might leave the output buffer
      //! not written. 
      //! Trace can simply be used standalone as a statement, just 'trace;'.
      //! Note that trace is a macro, not an object.
   #define trace ( std::cout << BMPTK_HERE << " " << std::flush )


   // =======================================================================
   // 
   //! bmptk namespace
   //
   //! This namespace contains all bmptk declarations.
   //
   // =======================================================================
   
   namespace bmptk {
   
      //! bmtkp version, determined by Makefile.inc
      const char version[] = BMPTK_VERSION;   
      
        //! time units
        //
        //! These units are used by all methods that require a time argument.
        //! For instance, to pass half a second, use 
        //!
        //!    500 * bmptk::ms
        //!
        //! Please do not make your code depend on the exact values, 
        //! they! might change in future versions!
      enum time_units { 
         s  = 1000ULL * 1000ULL, 
         ms = 1000ULL,
         us = 1ULL
      };    
	  
	  //! the elapsed time since startup
	  //!
	  //! This function returns the amount of time
	  //! (in \ref time_units) since startup.
	  //! 
	  //! On some targets the ime-since-startup only starts
	  //! running with the first call to this function.
	  //!
	  //! On most targets this function (and hence all the wait functions 
	  //! that rely use it) must be called at least once every x time.
	  //! In practice this will seldom be a problem: applications that
	  //! use timing tend to do so on a sub-second basis.
	  //! On an NDS x is 130 seconds. 
	  //!
	  //! On windows this function (and hence all timing related functions)
	  //! returns the time since windows started, with a resolution of 1 ms.
	  //! 49 Days after windows was last started it will wrap around to 0.
	  //! Don't blame me, I am just the piano player..
	  unsigned long long int time_since_startup();
      
      //! busy wait until a moment in time
	  //
	  //! This function waits until the specified moment in time
	  //! (in \ref time_units since startup)
	  //! It uses busy waiting, polling \ref time_since_startup().
	  //!
	  //! When task switching (which can result in a longer delay) is not a 
	  //! problem \ref wait_until() or an RTOS time function should be used 
	  //! instead, to avoid locking up the other tasks.
      void wait_busy_until( unsigned long long int t );      
 
      //! wait until a moment in time
	  //
	  //! This function waits until the specified moment in time
	  //! (in \ref time_units since startup)
	  //! It uses busy waiting, polling \ref time_since_startup().
	  //!
	  //! By default this function calls \ref wait_busy_until, but
	  //! when an RTOS is used it will switch to another task.
	  //! When the extra delay that might be caused by task switching
	  //! is not acceptable \ref wait_busy should be used instead.
      void wait_until( unsigned long long int t );      
 
      //! busy wait an amount of time
	  //
	  //! This function waits the requested amount of time 
	  //! (in \ref time_units) before returning. 
	  //! It uses busy waiting, polling \ref time_since_startup() to check
	  //! whether the request amount of time has elapsed.
	  //!
	  //!
	  //! When this function is used to measure time over 
	  //! multiple calls using \ref wait_busy_until should be considered
	  //! beacuse it can avoid the 'accumulated error' problem.	
	  //!
	  //! When task switching (which can result in a longer delay) is not a 
	  //! problem \ref wait() should be used instead, because it does not
	  //! lock up the other tasks.
      void wait_busy( unsigned long long int t );      
      
      //! wait an amount of time
	  //!
 	  //! This function waits at leas the requested amount of time 
	  //! (in \ref time_units) before returning. 
	  //!
	  //! When this function is used to measure time over 
	  //! multiple calls using \ref wait_until should be considered
	  //! beacuse it can avoid the 'accumulated error' problem.	  
	  //!
	  //! By default this function calls \ref wait_busy, but
	  //! when an RTOS is used it will switch to another task.
	  //! When the extra delay that might be caused by task switching
	  //! is not acceptable \ref wait_busy should be used instead.
      void wait( unsigned long long int t );      
      
   };  
           
   // include the graphics part only if requested to do so              
   #ifdef BMPTK_GRAPHICS
      #include "bmptk_graphics.h"
      #include "bmptk_font_default.h"
   #endif
   
   // include the target-specific header files
   #ifdef BMPTK_TARGET_nds
      #include "bmptk_nds.h"
   #endif              
   #ifdef BMPTK_TARGET_win
      #include "bmptk_win.h"
   #endif              
   #ifdef BMPTK_CHIP_lpc2478
      #include "lpc2478.h"
   #endif              

#endif // #ifndef BMPTK_H
