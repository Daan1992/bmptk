//***************************************************************************
//
// file : bmptk.h
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


#ifndef _BMPTK_H_
#define _BMPTK_H_

   //! bmptk namespace
   //
   //! This namespace contains all bmptk declarations.
   namespace bmptk {
   	
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

   #include "pins/pins.h"
   #include "targets/target.h"

#endif
