//***************************************************************************
//
// file : bmptk/chips/pcf8574.h
//
// LICENSE (MIT expat license, copy of license.txt)
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
namespace chips {
	
//! channel for a pcf8547 or pcf8547a I2C 8 bit I/O extender
template < typename bus, int base, int address >
struct pcf8574x_channel {  

   static_assert( 
      ( (int)bus::type == bmptk::type_i2c_bus ),
      "bus must be an i2c bus"
   );
   
   //! initialize the i2c bus
   static void init(){
      bus::init();   	
   }
   
   //! write a byte to the chip
   static void set( unsigned int x  ){
      bus::put_start();
      bus::put_byte( base + ( address << 1 ) );
      bus::get_ack();
      bus::put_byte( x );
      bus::get_ack();
      bus::put_stop();      
   }
   
   //! get a byte from the chip
   static unsigned int get(){
      bus::put_start();
      bus::put_byte( base + (( address << 1 ) | 0x01 ));
      bus::get_ack();
      unsigned int result = bus::get_byte();
      bus::put_stop();      
      return result;
   }
     
}; 

//! interface for a pcf8547 I2C 8 bit I/O extender
template< typename bus, int address = 0 > 
class pcf8574 : public bmptk::hardware::port_buffer_oc<
   pcf8574x_channel< bus, 0x40, address > >{};
 
//! interface for a pcf8547a I2C 8 bit I/O extender
template< typename bus, int address = 0 > 
class pcf8574a : public bmptk::hardware::port_buffer_oc<
   pcf8574x_channel< bus, 0x70, address > >{};
    
}; // namespace chips
}; // namespace bmptk