//***************************************************************************
//
// file : bmptk/hardware/hc595.h
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

//! channel to a hc595 chip
template< typename channel >
struct hc595_channel : public bmptk::hardware::port_buffered_out_channel {

   static_assert( 
      ( (int)channel::type == bmptk::type_spi_channel ),
      "channel must be an spi channel"
   );
   
   //! initialize the channel to the chip
   static void init(){
      channel::init();
   }
   
   //! set the pin son the chip
   static void set( unsigned int x ){
      unsigned char c = x;
      channel::transaction_out_n( &c, 1 ); 
   }   
};
	
//! interface for a (74)HC595 SPI output shift register
//
//! This class implements a simple interface for a (74)HC595 
//! 8-bit SPI output shift register. 
//!
//! The TPIC6C595 chip has a compatible interface, but features
//! open-drain high-current (100 mA) MOSFET outputs.
template < typename channel >
class hc595 : public bmptk::hardware::port_buffer_out< 
   hc595_channel< channel > >{};
    
}; // namespace chips
}; // namespace bmptk
