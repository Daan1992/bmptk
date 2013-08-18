//***************************************************************************
//
// file : bmptk/core/basics.h
//
// LICENSE (MIT expat license, copy of bmptk/license.txt)
//
// Copyright (c) 2013 Wouter van Ooijen (wouter@voti.nl)
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

// ==========================================================================
// 
//! bmptk namespace
//
//! This namespace contains all bmptk declarations 
//! (except, of course, the macro's)
//
// ==========================================================================
  
// ==========================================================================
// 
//! bmptk namespace
//
//! This namespace contains all bmptk declarations 
//! (except, of course, the macro's)
//
// ==========================================================================  
namespace bmptk {

//! hardware: pins, ports
//
//! This namespace contains abstractions for I/O pins and ports, 
//! for interfaces like i2c and spi.
namespace hardware {};

//! peripheral chips
//
//! This namespace contains abstractions for various chips and modules.
namespace chips {};

//! non-preememptive rtos
//
//! This namespace provides a simple non-preemeptive (cooperative) rtos.
namespace rtos {};


// ==========================================================================
// 
// debug macro's
//
// ==========================================================================

//! file-name : line-number macro
//
//! The macro BMPTK_HERE transates to a newline, the file-name, ":",
//! and the line-number of the place where the BMPTK_HERE macro
//! appears. This is usefull for debug logging.
#define BMPTK_HERE_STR( X ) #X
#define BMPTK_HERE2( F, L ) ( "\n" F ":" BMPTK_HERE_STR( L ) " " )
#define BMPTK_HERE BMPTK_HERE2( __FILE__, __LINE__ )


//! helper macro for the bmtkp version
#define BMPTK_VERSION_STR( X ) BMPTK_HERE_STR( X )

//! bmtkp version, determined by Makefile.inc
const char version[] = BMPTK_VERSION_STR( BMPTK_VERSION );

   
//! use instead of cout; prepends source file name and line number
//
//! Printing to trace (instead of cout) prepends BMPTK_HERE and a space.
//! This provides an easy way to check if and when a certain line 
//! of code is executed, and optionally print some debugging 
//! information.       
//! The output is also flushed, which can be usefull when the
//! program will shortly crash, which might leave the output buffer
//! not written. 
//! Trace can simply be used standalone as a statement, just 'trace;'.
//! Note that trace is a macro, not an object.
//!
//! \code
//! trace << "n=" << n << "\n";
//! \endcode
//!
//! could for instance print the output line
//!
//! \code
//! main.c:20 n=15
//! \endcode
//!
//#define trace ( std::cout << BMPTK_HERE << " " << std::flush )    
#define trace ( std::cout << BMPTK_HERE << " " )    


//! bmptk class 'type' identification
//
//! This type is used by various bmptk class templates to identify
//! their 'type', which makes it easier for other templates to specialize
//! on their class parameters.
enum {
   type_none, type_pin_ad, 
   type_pin_in, type_pin_out, type_pin_in_out, type_pin_oc,
   type_port_in, type_port_out, type_port_in_out, type_port_oc,
   type_i2c_bus, 
   type_spi_bus, type_spi_channel
};


//! bmptk class 'features' identification
//
//! This type is a bit field identifying 'additiona' fatures
//! of bmptk static class types.
enum {
   has_none   = 0x0000,
   has_ad     = 0x0001, 
   has_pullup = 0x0002
};   


//! Constants for Hz, kHz, MHz, GHz
enum {
   Hz = 1,
   kHz = 1000,
   MHz = 1000 * 1000,
   GHz = 1000 * 1000 * 1000
};


//! handle a fatal error
//
//! This function is called by bmptk code when a fatal condition has
//! been detected at run time. 
//! The function that is called is determined by fatal_error_handler().
//! By default this function does nothing, but fatal_error_hander
//! The default is to loop forever (do nothing).
//!
//! The application could specify a function that makes a attempt to 
//! alert the user show the msg.
//! When the user-defined error handler returns the system will
//! loop forever (do nothing).
extern "C" void fatal_error_detected( const char *msg );

//! set the fatal error handler
//
//! This function sets the function that is called when 
//! fatal_error_detected() is called.
//! When this error hanler fucntion returns the system will
//! loop forever (do nothing).
void fatal_error_handler( void f( const char * ) );


}; // namespace bmptk; 
