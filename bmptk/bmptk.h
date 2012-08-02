//***************************************************************************
//
// file : bmptk/bmptk.h
//
// LICENSE (zlib license)
//
// Copyright (c) 2012 Wouter van Ooijen (wouter@voti.nl)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in a
// product, an acknowledgment in the product documentation would be
// appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
// and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
//***************************************************************************

// ==========================================================================
//
//! \mainpage
//!
//! bmptk is a C++ library for bare-metal use on 32-bit embedded systems. 
//! Bare-metal refers to the situation that there is only the application. 
//! The application has full control of the hardware, and is running 
//! without the support of a separate operating system. 
//! Operating system functionality can still be present, but must be in the 
//! form of libraries, which are statically linked into the application. 
//! bmptk contains a number of such is such a libraries.
//!
//! The graphics part of the library provides the interface to a 
//! few LCD screens, and basic graphics functions of drawing lines, 
//! circles, boxes, characters, and pictures on a graphic screen. 
//!
//! The widget part builds on the graphics part to provide a widget 
//! toolkit with event processing and objects like button, 
//! slider, and tab. 
//!
//! GraWiLi provides a command-line tool (the Python script inline.py) 
//! that can be used to translate an image (.bmp or .jpg) or font (.ttf) 
//! files to C++ code that can be included in an application. 
//! A few pre-translated images and fonts are provided.
//!
//! Using GraWiLi requires an interface to a (presumably LCD) screen, 
//! and for the widget part, a way to capture events. 
//! Interfaces are available for Windows (to quickly test an application, 
//! I use DevCpp), NintendoDS (using ??) 
//! and the FriendlyARM micro2440 board with 2.5" LCD (using ??).
//! 
//! GraWiLi is licensed under a very permissive license. 
//! Basically the only requirements are that a distribution of 
//! the (original or modified) GraWiLi sources must use the same 
//! license, and that you don't blame the author for anything. 
//! No legal requirements are put on the executable code, or on 
//! the code that you link with the library.
//!
//! \par Class overview
//! 
//!    - \ref gwlib::vector    : x,y coordinates 
//!    - \ref gwlib::color     : 3 * 8 bit RGB color
//!    - \ref gwlib::event     : postion + type of a GUI event
//! 
//! Vector and color are basic classes that implement a 2-dimensional 
//! integer coordinates, and a 3-byte (+ transparency) RGB color encoding.
//! An event is something that the user caused at a specific location on the
//! screen, typically a mouse click or a stylus touch.
//!
//!    - \ref gwlib::drawable  : things that can be drawn on a screen
//!    - \ref gwlib::line      : a line
//!    - \ref gwlib::rectangle : a rectangle (box)
//!    - \ref gwlib::circle    : a circle
//!    - \ref gwlib::photo     : a rectangle of pixels
//!    - \ref gwlib::inline_rgb_photo : a color image stored in ROM
//!    - \ref gwlib::inline_bw_photo  : a BW image stored in ROM
//!    - \ref gwlib::font      : pictures for the set of ASCII characters
//!    - \ref gwlib::format    : specified how a text is drawn
//!    - \ref gwlib::text      : an ASCII string and a format
//!
//! Drawables are things that can be drawn. A drawable specifies its shape
//! and size, but not its location (starting point), that is supplied
//! when it is drawn. A photo is a rectangular block of (readonly) pixels.
//! Inline_rgb_photo and inline_bw_photo are photo's that are stored in
//! in-line code (in ROM).
//! 
//! The inline.py tool can be used to create such an in-line representation.
//!
//!     - \ref gwlib::frame     : something you can read and write by pixel
//!     - \ref gwlib::subframe  : part of a frame
//!
//! A frame is the thing on which you can draw a drawable. 
//! Frame inhertits from photo, so it is also a drawable, so you can draw
//! the content of a frame on (a part of) another frame.
//! A subframe is a rectangular part of another frame, which can be
//! rotated, mirrored, or enlarged.
//!    
//!    - \ref gwlib::widget    : can handle events and can (re)draw itself
//!    - \ref gwlib::wframe    : frame widget (has border and background)
//!    - \ref gwlib::wtoplevel : widget that has a frame and an eventloop
//! 
//! \par GraWiLi Software License - Version 1.0 - 2011-03-16
//! 
//! Permission is hereby granted, free of charge, to any person 
//! or organization obtaining a copy of the software and accompanying 
//! documentation covered by this license (the "Software") to use, 
//! reproduce, display, distribute, execute, and transmit the Software, 
//! and to prepare derivative works of the Software, and to permit 
//! third-parties to whom the Software is furnished to do so, all subject 
//! to the following:
//! 
//! The copyright notices in the Software and this entire statement, 
//! including the above license grant, this restriction and the following 
//! disclaimer, must be included in all copies of the Software, in whole 
//! or in part, and all derivative works of the Software, unless such 
//! copies or derivative works are solely in the form of machine-executable 
//! object code generated by a source language processor.
//! 
//! THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
//! EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
//! OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND 
//! NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR ANYONE 
//! DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, 
//! WHETHER IN CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
//! CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
//! IN THE SOFTWARE.
//


#pragma once
#ifndef _BMPTK_H_
#define _BMPTK_H_

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
      const char version[] = "V00.01 ( work-in-progress @ 2012-07-19)";   
      
        //! time units
        //
        //! These units are used by all methods that require a time argument.
        //! For instance, to pass half a second, use 
        //!
        //!    500 * bmptk::ms
        //!
        //! Please do not make your code depend on the exact values, they
        //! might change in future versions!
      enum bmptk_time_units { 
         s  = 1000 * 1000, 
         ms = 1000,
         us = 1
      };    
      
      //! wait implementation
      void wait( unsigned int t );      
      
   };  
                 

   #ifdef TARGET_nds
      #include "bmptk-nds.h"
   #endif              
   #ifdef CHIP_lpc2478
      #include "lpc2478.h"
   #endif              

   // #include "pins/pins.h"

   #include "graphics.h"

#endif
