//***************************************************************************
//
// file : bmptk/hardware/i2c.h
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
	
//! i2c interface class
class i2c_interface {  
public:

   //! identify this type as an i2c bus
   enum { type = type_i2c_bus };

	 //! initialize the i2c bus
   static void init();   

   //! output an I2C start condition
   static void put_start();
   
   //! output an I2C stop condition
   static void put_stop();
     
   //! get an acknowledge
   //
   //! This method gets (receives) an acknowledge from the slave.
   //! The method returns true when an ack was received, false
   //! when no ack (equivalent to a nack) was received.
   //! Note that this is opposite to the polarity on the bus,
   //! where a low level represents an ack.
   static bool get_ack();
   
   //! output an ack
   static void put_ack();
   
   //! output a nack 
   static void put_nack();
   
   //! output a byte, 8 bits, MSB first
   static void put_byte( unsigned char x );
   
   //! get (receive) a byte, 8 bits, MSB first
   static unsigned char get_byte();
   
   //! multi-byte write transaction
   //
   //! This method writes n bytes from *data over the i2c bus 
   //! to the the 7-bit i2c address 'address'.
   static void write( unsigned char address, unsigned const char *data, int n ){
      put_start();
      put_byte( address << 1 );
      for( int i = 0; i < n; i++ ){
         get_ack();
         put_byte( data[ i ] );
      }               
      get_ack();
      put_stop();
   }           
   
   //! multi-byte read transaction
   //
   //! This method reads n bytes over the i2c bus from the 7-bit i2c
   //! address 'address' to *data.
   static void read( unsigned char address, unsigned char *data, int n ){
      put_start();
      put_byte( ( address << 1 ) | 0x01 );    
      for( int i = 0; i < n; i++ ){
         get_ack();
         data[ i ] = get_byte();
      }               
      put_stop();
   }           
   
};
                   
//! simple I2C bus master 
//
//! This class implements a simple bit-banged (software-only) 
//! I2C bus master. It does not support clock stretching.
//!
//! The last parameter is the (maximum) frequency of the clock.
//! For higher frequency arguments, the actual frequency will be limited
//! by the target's bmptk::current_time() calls. 
//! On an LPC1114 at 12 MHz, the real maximum frequency is ~ 50 kHz.
template< 
   class extern_scl, 
   class extern_sda,
   unsigned int frequency = 1 * bmptk::MHz 
>	  
class i2c_master : public i2c_interface {
private: 

   static_assert( 
      ( (int)extern_scl::type == bmptk::type_pin_out )
      | ( (int)extern_scl::type == bmptk::type_pin_in_out )
      | ( (int)extern_scl::type == bmptk::type_pin_oc ),
	   "scl must be a pin_out, pin_in_out or a pin_oc"
   );
   static_assert( 
      ( (int)extern_sda::type == bmptk::type_pin_out )
      | ( (int)extern_sda::type == bmptk::type_pin_in_out )
      | ( (int)extern_sda::type == bmptk::type_pin_oc ),
	   "sda must be a pin_out, pin_in_out or a pin_oc"
   );
   
   typedef bmptk::hardware::pin_oc_from< extern_scl > scl;
   typedef bmptk::hardware::pin_oc_from< extern_sda > sda;
   	
   //! wait at least a half-period since the previous call
   static void delay_half_period(){
      static bmptk::time epoch = 0 * bmptk::us;
      epoch += bmptk::s / ( 2 * frequency );
      bmptk::wait_until( epoch );
      epoch = bmptk::current_time();
   }  	
   
   //! output a single bit
   static void put_bit( bool x ){
      scl::set( 0 );
      delay_half_period();
      sda::set( x );
      scl::set( 1 );
      delay_half_period();
   }
   
   //! get (receive) a single bit
   static bool get_bit(){
      scl::set( 0 );
      delay_half_period();  
      sda::set( 1 );
      scl::set( 1 );
      delay_half_period();
      bool result = sda::get();
      delay_half_period();     
      return result;
   }    
   
public:

   //! initialize the i2c pins	
   static void init(){
      scl::init();
      sda::init();
      scl::set( 1 );   		
      sda::set( 1 );
   }      

   //! output an i2c start condition
   static void put_start(){
      sda::set( 0 );
      delay_half_period();  
      scl::set( 0 );
      delay_half_period();  
   }

   //! output an i2c stop condition
   static void put_stop(){
      scl::set( 0 );
      delay_half_period();    
      sda::set( 0 );
      delay_half_period();    
      scl::set( 1 );
      delay_half_period();    
      sda::set( 1 );
      delay_half_period();    
   }
      
   //! receive and return an ack 	  
   static bool get_ack(){
      bool ack = ! get_bit(); 
      return ack;
   } 
   
   //! output an ack
   static void put_ack(){
      put_bit( 0 );
   }

   //! output a nack
   static void put_nack(){
      put_bit( 1 );
   }
   
   //! output a byte
   static void put_byte( unsigned char x ){
      for( int i = 0; i < 8; i++ ){
         put_bit( ( x & 0x80 ) != 0 );
         x = x << 1;
      }         
   }
   
   //! receive and return a byte (without ack or nack)
   static unsigned char get_byte(){
      unsigned char result = 0;
      for( int i = 0; i < 8; i++ ){
         result = result << 1;
         if( get_bit() ){
            result |= 0x01;
         } 
      }   
      return result;     
   }        
   
};     


}; // namespace hardware        
}; // namespace bmptk