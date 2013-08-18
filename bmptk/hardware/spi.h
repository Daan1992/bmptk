//***************************************************************************
//
// file : bmptk/hardware/spi.h
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
namespace hardware {
  
//! bit-banged spi bus
//
//! This class represents a simple bit-banged (software-only) spi bus master.
//! 
//! The SS (slave select) line is not part of this bus, it is part of
//! a spi channel (specific to one chip on the spi bus).
//!
//! The last parameter is the (maximum) frequency of the clock.
//! For higher frequency arguments, the actual frequency will be limited
//! by the target's bmptk::current_time() calls. 
//! On an LPC1114 at 12 MHz, the real maximum frequency is ~ 50 kHz.
template< 
   class sclk_extern,
   class mosi_extern, 
   class miso_extern,
   unsigned int frequency = 1 * bmptk::MHz
> struct spi_bus_sclk_mosi_miso {

   //! identify this type as a spi bus
   enum { type = type_spi_bus };

   static_assert( 
      ( (int)sclk_extern::type == bmptk::type_port_out )
      | ( (int)sclk_extern::type == bmptk::type_pin_in_out )
      | ( (int)sclk_extern::type == bmptk::type_pin_oc ),
     "sclk must be a port_out, pin_in_out or a pin_oc"
   );
   static_assert( 
      ( (int)mosi_extern::type == bmptk::type_pin_out )
      | ( (int)mosi_extern::type == bmptk::type_pin_in_out )
      | ( (int)mosi_extern::type == bmptk::type_pin_oc ),
     "mosi must be a pin_out, pin_in_out or a pin_oc"
   );
   static_assert( 
      ( (int)miso_extern::type == bmptk::type_pin_in )
      | ( (int)miso_extern::type == bmptk::type_pin_in_out )
      | ( (int)miso_extern::type == bmptk::type_pin_oc ),
     "miso must be a pin_out, pin_in_out or a pin_oc"
   );
   
   //! the sck pin
   typedef bmptk::hardware::pin_out_from< sclk_extern > sclk;
   
   //! the mosi pin
   typedef bmptk::hardware::pin_out_from< mosi_extern > mosi;
   
   //! the miso pin
   typedef bmptk::hardware::pin_in_from< miso_extern > miso;
      
   //! wait at least a half-period since the previous call
   static void wait(){
      static bmptk::time epoch = 0 * bmptk::us;
      epoch += bmptk::s / ( 2 * frequency );
      bmptk::wait_until( epoch );
      epoch = bmptk::current_time();
   }    
   
   //! initialize the pins  
   static void init(){
      sclk::init();
      mosi::init();
      miso::init();
   }      
            
};     

//! specifies the mode of a spi channel
enum spi_mode {

   //! SPI mode 0 
   //
   //! In this mode the clock is low when idle, 
   //! and data is present before first clock edge.
   spi_mode_0 = 0, 
   
   //! SPI mode 1 
   //
   //! In this mode the clock is low when idle, 
   //! and data is presented at the first clock edge.
   spi_mode_1 = 1, 

   //! SPI mode 2
   //
   //! In this mode the clock is high when idle, 
   //! and data is present before first clock edge.
   spi_mode_2 = 2, 

   //! SPI mode 3
   //
   //! In this mode the clock is high when idle, 
   //! and data is presented at the first clock edge.
   spi_mode_3 = 3
};


//! a spi channel to a specific peripheral chip
//
//! This template represents a spi channel to one specific
//! chip or chain of chips. 
//! It is created from a spi bus (which represents the
//! sclk, miso and mosi lines, and the maximum bus frequency)
//! and the ss (slave select) line, and the spi mode.
//! The ss line is acive low.
template< 
   typename bus,
   typename ss_extern,
   int mode = spi_mode_0
> class spi_channel {
private:

   static const bool cpha = ( mode & 0x01 ) != 0;
   static const bool cpol = ( mode & 0x02 ) != 0;
   
   static_assert( 
      ( (int)bus::type == bmptk::type_spi_bus ),
      "bus must be a spi bus"
   );
   
   static_assert( 
      ( (int)ss_extern::type == bmptk::type_pin_out )
      | ( (int)ss_extern::type == bmptk::type_pin_in_out )
      | ( (int)ss_extern::type == bmptk::type_pin_oc ),
     "ss must be a pin_out, pin_in_out or a pin_oc"
   );
   
   //! the ss pin
   typedef bmptk::hardware::pin_out_from< ss_extern > ss;   
   
   static unsigned char byte_in_out(
      unsigned char out
   ){
      int in = 0;
      bus::wait();
      for( int i = 0; i < 8; i++ ){
      	
         in = in << 1;
         
         if( cpha == 0 ){
     
            bus::mosi::set( out & 0x80 );
            bus::wait();
      
            bus::sclk::set( ! cpol );
            if( bus::miso::get() ){
               in |= 0x80;
            }
            
            bus::wait();
            bus::sclk::set( cpol );
      
         } else {

            bus::sclk::set( ! cpol );
            bus::wait();
       
            bus::mosi::set( out & 0x80 );
            bus::sclk::set( cpol );
            bus::wait();
      
            if( bus::miso::get() ){
               in |= 0x80;
            }      
         }
         out = out << 1;
      }   
      return in;
   }

public:

   //! identify this type as a spi channel
   enum { type = type_spi_channel };

   //! intilialize the spi channel and the underlying spi bus
   static void init(){
      bus::init();
      ss::init();   
   }
   
   //! spi read-and-write transaction
   //
   //! This method performs an n_byte spi write-and-read transaction.
   //! The output pointer must point to the array of bytes to be written,
   //! the input pointer must point to the array of bytes to be read.
   //! Both arrays must be n_bytes long.
   //! Alternatively, one of the pointers can be 0.
   //! (In fact both can be 0, but that would be pointless.)
   static void transaction_in_out_n( 
      unsigned char *input, 
      unsigned char *output, 
      int n_bytes 
   ){
      bus::sclk::set( cpol );
      ss::set( 0 );
      	
      while( n_bytes-- ){
         unsigned char in, out = 0;
         if( output ){
            out = *output++;
         }      
         in = byte_in_out( out );
         if( input ){
           *input++ = in;
         }
      }      
      
      ss::set( 1 );
   }
   
   //! spi read-only transaction
   static void transaction_in_n( 
      unsigned char *input, 
      int n_bytes 
   ){
      transaction_in_out_n( input, 0, n_bytes );
   }
   
   //! spi write-only transaction
   static void transaction_out_n( 
      unsigned char *output, 
      int n_bytes 
   ){
      transaction_in_out_n( 0, output, n_bytes );
   }     
   
};

}; // namespace hardware        
}; // namespace bmptk
