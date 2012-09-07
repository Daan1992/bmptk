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
   
      //! bmtkp version
      const char version[] = "V01.00 ( work-in-progress @ 2012-09-06)";   
      
        //! time units
        //
        //! These units are used by all methods that require a time argument.
        //! For instance, to pass half a second, use 
        //!
        //!    500 * bmptk::ms
        //!
        //! Please do not make your code depend on the exact values, 
        //! they! might change in future versions!
      enum bmptk_time_units { 
         s  = 1000 * 1000, 
         ms = 1000,
         us = 1
      };    
      
      //! wait implementation
      void wait( unsigned int t );      
      
   };  
           
   // include the graphics part only if requested to do so              
   #ifdef BMPTK_GRAPHICS
      #include "bmptk_graphics.h"
      #include "bmptk_font_default.h"
   #endif
   
   // include the target-specific header files
   #ifdef TARGET_nds
      #include "bmptk_nds.h"
   #endif              
   #ifdef TARGET_win
      #include "bmptk_win.h"
   #endif              
   #ifdef CHIP_lpc2478
      #include "lpc2478.h"
   #endif              


#endif // #ifndef BMPTK_H
