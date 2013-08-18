//***************************************************************************
//
// file : bmptk/hardware/demos.h
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

// document everything in this file
/*! \file */

namespace bmptk {
namespace hardware {

//! blink a LED
template< typename arg_pin, int flavour = arg_pin::type >
void blink( time t = 200 * bmptk::ms ){

   static_assert( 
      ( flavour == bmptk::type_pin_out )
      | ( flavour == bmptk::type_pin_in_out )
      | ( flavour == bmptk::type_pin_oc ),
	   "blink requires a pin_out, pin_in_out or a pin_oc"
   );	
   
   typedef bmptk::hardware::pin_out_from< arg_pin > pin;
   typedef bmptk::hardware::flipable< pin >flipper;
   
   pin::init();
   for(;;){
      bmptk::wait( t );
      flipper::flip();
   }
}

//! beep
template< typename arg_pin, int flavour = arg_pin::type  >
void beep(
   unsigned long long int period   =   1 * bmptk::ms,
   unsigned long long int duration = 500 * bmptk::ms
){

   static_assert( 
      ( flavour == bmptk::type_pin_out )
      | ( flavour == bmptk::type_pin_in_out )
      | ( flavour == bmptk::type_pin_oc ),
	   "beep requires a pin_out, pin_in_out or a pin_oc"
   );
   
   typedef bmptk::hardware::pin_out_from< arg_pin > pin;
   
   pin::init();
   while( duration > period ){
      duration -= period;
      pin::set( 1 );
      bmptk::wait( duration / 2 );
      pin::set( 0 );
      bmptk::wait( duration / 2 );
   }
}

//! kitt-style display
//! [kitt]
template< typename arg_port, int flavour = arg_port::type >
void kitt( bmptk::time t = 200 * bmptk::ms ){

   static_assert( 
      ( flavour == bmptk::type_port_out )
      | ( flavour == bmptk::type_port_in_out )
      | ( flavour == bmptk::type_port_oc ),
	   "kitt requires a port_out, port_in_out or a port_oc"
   );
   
   typedef bmptk::hardware::port_out_from< arg_port > port;
   
   port::init();
   for(;;){
   
      // walk the light from the 0th pin to the highest
      for( int i = 0; i < port::n_pins(); i++ ){
         port::set( 0x01 << i );
         bmptk::wait( t );
      }   
	  
	  // walk the light back from the highest-minuus-1 to the 1st
      for( int i = port::n_pins() - 2; i > 0; i-- ){
         port::set( 0x01 << i );
         bmptk::wait( t );
      }   
   }
}	  
//! [kitt]
	  
}; // namespace hardware	  
}; // namespace bmptk; 
