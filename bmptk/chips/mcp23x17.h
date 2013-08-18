//***************************************************************************
//
// file : bmptk/hardware/mcp23x17.h
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


	
//! high-level for an mcp23x17 chip
//
//! This class implements a high-level interface for an
//! mcp27016 (i2c) or mcp27s16 (spi) chip.
template < typename interface
class mcp27x16 {  
private:

   static unsigned char port_buffer_a, port_buffer_b;
     
   static void port_flush_a(){
      channel::transaction_out_n( &port_buffer, 1 );     
   }
         
   template< int p >
   class single_pin : public bmptk::hardware::pin_out {
   public:   
   
      static void init(){
	     channel::init();   
	   }
	  
      static void set( bool x ){
	     if( x ){
		    port_buffer |= ( 0x01 << p );
         } else {
		    port_buffer &= ~ ( 0x01 << p );
         }		 
		 port_flush();
	  }
 
   };
   
public:
	
   //! initialize
   static void init(){
      channel::init();
   }
   
   //! return the number of pins in this port (8)
   static int n_pins(){ 
      return 8; 
   }
   
   //! set the port value
   static void set( unsigned char x ){
      port_buffer = x;
      port_flush();
   }     
   
   //! pin 0, output-only
   typedef single_pin< 0 > pin_0;

   //! pin 1, output-only
   typedef single_pin< 1 > pin_1;

   //! pin 2, output-only
   typedef single_pin< 2 > pin_2;

   //! pin 3, output-only
   typedef single_pin< 3 > pin_3;

   //! pin 4, output-only
   typedef single_pin< 4 > pin_4;

   //! pin 5, output-only
   typedef single_pin< 5 > pin_5;

   //! pin 6, output-only
   typedef single_pin< 6 > pin_6;

   //! pin 7, output-only
   typedef single_pin< 7 > pin_7;
     
}; 

template < typename channel > 
unsigned char hc595< channel >::port_buffer;
    
}; // namespace chips
}; // namespace bmptk
