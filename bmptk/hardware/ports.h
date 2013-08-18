//***************************************************************************
//
// file : bmptk/hardware/ports.h
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

// ==========================================================================
//
// port interface classes
//
// ==========================================================================

//! input port interface 
//
//! This class defines the interface of an input port,
//! which consists (at least conceptually) of 0..32 input pins
//! (each as defined by the pin_in interface).
//!
//! This class is to be used as parent class for
//! classes that provide the port_in interface. 
struct port_in {

   //! identify this type as a type_port_in
   enum { type = type_port_in };
   
      //! this type has no additional fatures
   enum { features = has_none };

   //! initialize the port as input
   //
   //! This method initializes all pins of the port as input pins.
   //! This might involve configuring the pins as gpio, and
   //! disabling analog or other special functions.   
   static void init();
   
   //! the number of pins in the port, can be 0..32.
   static int n_pins();
   
   //! get the current values of the pins in the port
   //
   //! This method returns the current level on the pins that make
   //! up the port, encoded in the bits in the returned value.
   //! The value pin of pin N is returned in the Nth bit.
   //!   
   //! When the level is a logic one (high voltage, Vdd) the
   //! bit in the returned value is 1, otherwise 
   //! (logic zero, low level, Vss)
   //! the bit is 0.
   //!
   //! The init() method must be called first.   
   static unsigned int get();
};

//! output port interface 
//
//! This class defines the interface of an output port,
//! which consists (at least conceptually) of 0..32 output pins
//! (each as defined by the pin_out interface).
//!
//! This class is to be used as parent class for
//! classes that provide the port_out interface. 
struct port_out {

   //! identify this type as a type_port_out
   enum { type = type_port_out };
   
      //! this type has no additional fatures
   enum { features = has_none };

   //! initialize the port as output
   //
   //! This method initializes all pins of the port as output pins.
   //! This might involve configuring the pins as gpio, and
   //! disabling analog or other special functions. 
   static void init();
   
   //! the number of pins in the port, can be 0..32.   
   static int n_pins();
   
   //! set the curret level on the pins in the port
   //
   //! This method sets the current level output by each pin that makes
   //! up the port, encoded in the bits in parameter value.
   //! The value pin of pin N is determined by the Nth bit.
   //!   
   //! When the value is true a logic one (high voltage, Vdd) will appear
   //! on the pin, when the value if false a the pin will 
   //! output a logic zero (low voltage, Vss).
   //!
   //! The init() method must be called first.    
   static void set( unsigned int d );
};

//! input-output port interface 
//
//! This class defines the interface of a port,
//! that can be used as input and as output,
//! which consists (at least conceptually) of 0..32 output pins
//! (each as defined by the pin_out interface).
//!
//! This class is to be used as parent class for
//! classes that provide the port_in_out interface. 
struct port_in_out {

   //! identify this type as a type_port_in_out
   enum { type = type_port_in_out };
   
      //! this type has no additional fatures
   enum { features = has_none };

   //! @copydoc port_in::init()
   //
   //! Note that this method does not set the direction, for that
   //! must call set_direction(d), set_direction_input() or
   //! set_direction_output().
   static void init();
   
   //! @copydoc port_in::n_pins()   
   static int n_pins();
   
   //! set the direction of the port to input
   //
   //! This method call sets the direction of the pin in the port to input.
   //! The init() method must have been called first.
   static void direction_set_input();
   
   //! set the direction of the port to output
   //
   //! This method call sets the direction of the pin in the port to output.
   //! The init() method must have been called first.
   static void direction_set_output();
   
   //! @copydoc port_out::set
   //! After that, set_direction_output() (or set_direction(output)) 
   //! must have been called, after the last set_direction_input()
   //! (or set_direction(input)) call.
   static void set( unsigned int d );
   
   //! @copydoc port_in::get
   //! After that, set_direction_input() (or the equivalent 
   //! set_direction(input)) 
   //! must have been called, after the last set_direction_output()
   //! (or set_direction(output)) call.
   static unsigned int get();
};

//! open-collector port interface 
//
//! This class defines the interface of a port
//! which consists (at least conceptually) of 0..32 
//! open-collector pins
//! (each as defined by the pin_oc interface).
//!
//! This class is to be used as parent class for
//! classes that provide the port_oc interface. 
struct port_oc {

   //! identify this type as a type_port_oc
   enum { type = type_port_oc };
   
      //! this type has no additional fatures
   enum { features = has_none };

   //! @copydoc port_in::init()
   static void init();
   
   //! @copydoc port_in::n_pins()   
   static int n_pins();
     
   //! @copydoc port_out::set
   //!
   //! Setting the pin to 0 causes the pin to be pulled low.
   //! Setting a pin to 1 causes the pin to float.
   static void set( unsigned int d );
   
   //! @copydoc port_in::get
   //!
   //! Note that after setting a pin low(0) the pin will in all probability
   //! read as 0, so this call only makes realy sense for pins
   //! set high (1) call.
   static unsigned int get();
};


// ==========================================================================
//
// combine pins into a port_out
//
// ==========================================================================

//! create a port_out from a list of pins
//
//! This class template accepts a list of pins and combines
//! them into a port_out. The first pint in the list becomes
//! pin 0 in the port, etc.
//! 
//! The pins must be acceptable to pin_out_from.
//!
//! Example (examples/db103/kitt/main.cpp):
//! @include examples/db103/kitt/main.cpp
template< typename... > class port_out_from_pins : public port_out {};

#ifndef DOXYDOC

template<> class port_out_from_pins<> : public port_out {
public: 
   static void init(){}
   static void set( unsigned int x ){}
   static int n_pins(){ return 0; }
};

template< typename arg_pin, typename... tail_args > 
struct port_out_from_pins< arg_pin, tail_args... > : public port_out {

   static_assert( 
      ( (int)arg_pin::type == bmptk::type_pin_out )
      | ( (int)arg_pin::type == bmptk::type_pin_in_out )
      | ( (int)arg_pin::type == bmptk::type_pin_oc ),
	   "port_out_from_pins requires pin_out, pin_in_out or a pin_oc"
   );

   typedef port_out_from_pins< tail_args... > tail;
   typedef pin_out_from< arg_pin > pin;
   
   static void init(){ 
      pin::init();
      tail::init(); 
   }
   
   static void set( unsigned int x ){
      pin::set( x & 0x01 );
      tail::set( x >> 1 );
   }
   
   static int n_pins(){ 
      return tail::n_pins() + 1;
   }
};

#endif


//! create a port_in from a list of pins
//
//! This class template accepts a list of pins and combines
//! them into a port_in. The first pin in the list becomes
//! pin 0 in the port, etc.
//! 
//! The pins must be acceptable to pin_in_from.
template< typename... > class port_in_from_pins : public port_in {};

#ifndef DOXYDOC

template<> class port_in_from_pins<> : public port_in {
public: 
   static void init(){}
   static unsigned int get(){ return 0; }
   static int n_pins(){ return 0; }
};

template< typename arg_pin, typename... tail_args > 
class port_in_from_pins< arg_pin, tail_args... > : public port_in {
public:

   typedef port_in_from_pins< tail_args... > tail;
   typedef pin_in_from< arg_pin > pin;
   
   static void init(){ 
      pin::init();
      tail::init(); 
   }
   
   static unsigned int get(){
      return 
	     ( tail::get() << 1 )
		 | pin::get() ? 0x01 : 0x00;
   }
   
   static int n_pins(){ 
      return tail::n_pins() + 1;
   }
};

#endif


//! create a port_in_out from a list of pins
//
//! This class template accepts a list of pins and combines
//! them into a port_in_out. The first pin in the list becomes
//! pin 0 in the port, etc.
//! 
//! The pins must be acceptable to pin_in_out_from.
template< typename... > class port_in_out_from_pins : public port_in_out {};

#ifndef DOXYDOC

template<> class port_in_out_from_pins<> : public port_in_out {
public: 
   static void init(){}
   static unsigned int get(){ return 0; }
   static void set( unsigned int x ){}
   static int n_pins(){ return 0; }
   static void direction_set_input() {}
   static void direction_set_output() {}
};

template< typename arg_pin, typename... tail_args > 
class port_in_out_from_pins< arg_pin, tail_args... > : public port_in_out {
public:

   typedef port_in_out_from_pins< tail_args... > tail;
   typedef pin_in_out_from< arg_pin > pin;
   
   static void init(){ 
      pin::init();
      tail::init(); 
   }
   
   static void set( unsigned int x ){
      pin::set( x & 0x01 );
      tail::set( x >> 1 );
   }
   
   static unsigned int get(){
      return 
	     ( tail::get() << 1 )
		 | pin::get() ? 0x01 : 0x00;
   }
   
   static void direction_set_input() {
      pin::direction_set_input();
	   tail::direction_set_input();
   }
   
   static void direction_set_output() {
      pin::direction_set_output();
	   tail::direction_set_output();
   }
   
   static int n_pins(){ 
      return tail::n_pins() + 1;
   }
};

#endif


//! create a port_oc from a list of pins
//
//! This class template accepts a list of pins and combines
//! them into a port_oc. The first pin in the list becomes
//! pin 0 in the port, etc.
//! 
//! The pins must be acceptable to pin_oc_from.
template< typename... > class port_oc_from_pins : public port_oc {};

#ifndef DOXYDOC

template<> class port_oc_from_pins<> : public port_oc {
public: 
   static void init(){}
   static unsigned int get(){ return 0; }
   static void set( unsigned int x ){}
   static int n_pins(){ return 0; }
};

template< typename arg_pin, typename... tail_args > 
class port_oc_from_pins< arg_pin, tail_args... > : public port_oc {
public:

   typedef port_oc_from_pins< tail_args... > tail;
   typedef pin_oc_from< arg_pin > pin;
   
   static void init(){ 
      pin::init();
      tail::init(); 
   }
   
   static void set( unsigned int x ){
      pin::set( x & 0x01 );
      tail::set( x >> 1 );
   }
   
   static unsigned int get(){
      return 
	     ( tail::get() << 1 )
		 | pin::get() ? 0x01 : 0x00;
   }
      
   static int n_pins(){ 
      return tail::n_pins() + 1;
   }
};

#endif

template< typename port >
class port_out_from : public port {};

// ==========================================================================
//
// 8-bit buffered ports
//
// ==========================================================================

//! chip interface for a buffered output-only port
class port_buffered_out_channel {

   //! initialize the channel
   static void init();
   
   //! write a new value to the pins of the chip
   static void set( unsigned int x );
};

//! template for a buffered 8-bit output_only port
//
//! This template must be instantiated with a port_buffered_out_channel
//! that implements the access to the chip that has the pins.
template< typename channel >
class port_buffer_out : public port_out {

   static unsigned int port_out_buffer;
     
   static void port_out_flush(){
      channel::set( port_out_buffer );   
   }

   template< int p >
   class single_pin : public bmptk::hardware::pin_out {
   public:   
   
      static void init(){
         channel::init();   
	   }
	  
      static void set( bool x ){
	     if( x ){
           port_out_buffer |= ( 0x01 << p );
        } else {
		     port_out_buffer &= ~ ( 0x01 << p );
        }		 
		  port_out_flush();
	  }
   };
   
public:   

   //! @copydoc bmptk::hardware::port_out::init
   static void init(){
      channel::init();
   }

   //! @copydoc bmptk::hardware::port_out::set  
   static void set( unsigned int x ){
      port_out_buffer = x;
      port_out_flush();
   }
      
   //! return the number of pins in this port (8)
   static int n_pins(){ 
      return 8; 
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
unsigned int port_buffer_out< channel >::port_out_buffer;

//! chip interface for a buffered open-collector port
class port_buffered_oc_channel {

   //! initialize the channel
   static void init();
   
   //! write a new value to the pins of the chip
   static void set( unsigned int x );
   
   //! get the value of the pins of the chip
   static unsigned int get();
};

//! template for a buffered 8-bit open-collector port
//
//! This template must be instantiated with a port_buffered_oc_channel
//! that implements the access to the chip that has the pins.
template< typename channel >
class port_buffer_oc : public port_oc {

   static unsigned int port_in_buffer;
   static unsigned int port_out_buffer;
     
   static void port_out_flush(){
      channel::set( port_out_buffer );  
   }

   static void port_in_refresh(){
      port_in_buffer = channel::get();   
   }

   template< int p >
   class single_pin : public bmptk::hardware::pin_oc {
   public:   
   
      static void init(){
         channel::init();   
	   }
	  
      static void set( bool x ){
	     if( x ){
           port_out_buffer |= ( 0x01 << p );
        } else {
		     port_out_buffer &= ~ ( 0x01 << p );
        }		 
		  port_out_flush();
	  }
 
     static bool get(){
        port_in_refresh();
	     return port_in_buffer &( 0x01 << p );
	  } 
   };
   
public:   

   //! @copydoc bmptk::hardware::port_oc::init
   static void init(){
      channel::init();
   }

   //! @copydoc bmptk::hardware::port_oc::set  
   static void set( unsigned int x ){
      port_out_buffer = x;
      port_out_flush();
   }
      
   //! return the number of pins in this port (8)
   static int n_pins(){ 
      return 8; 
   }
   
   //! @copydoc bmptk::hardware::port_oc::get
   static unsigned int get(){ 
      port_in_refresh();
      return port_in_buffer;
   }
   
   //! pin 0, open-collector/input
   typedef single_pin< 0 > pin_0;

   //! pin 1, open-collector/input
   typedef single_pin< 1 > pin_1;

   //! pin 2, open-collector/input
   typedef single_pin< 2 > pin_2;

   //! pin 3, open-collector/input
   typedef single_pin< 3 > pin_3;

   //! pin 4, open-collector/input
   typedef single_pin< 4 > pin_4;

   //! pin 5, open-collector/input
   typedef single_pin< 5 > pin_5;

   //! pin 6, open-collector/input
   typedef single_pin< 6 > pin_6;

   //! pin 7, open-collector/input
   typedef single_pin< 7 > pin_7;
};

template < typename channel > 
unsigned int port_buffer_oc< channel >::port_out_buffer;
template < typename channel > 
unsigned int port_buffer_oc< channel >::port_in_buffer;

//! chip interface for a buffered input port
class port_buffered_in_channel {

   //! initialize the channel
   static void init();
   
   //! get the value of the pins of the chip
   static unsigned int get();
};

//! template for a buffered 8-bit input port
//
//! This template must be instantiated with a port_bufferedinc_channel
//! that implements the access to the chip that has the pins.
template< typename channel >
class port_buffer_in : public port_in {

   static unsigned int port_in_buffer;
     
   static void port_in_refresh(){
      port_in_buffer = channel::get();   
   }

   template< int p >
   class single_pin : public bmptk::hardware::pin_in {
   public:   
   
      static void init(){
         channel::init();   
	   }
 
      static bool get(){
         port_in_refresh();
	      return port_in_buffer &( 0x01 << p );
	   } 
   };
   
public:   

   //! @copydoc bmptk::hardware::port_oc::init
   static void init(){
      channel::init();
   }
     
   //! return the number of pins in this port (8)
   static int n_pins(){ 
      return 8; 
   }
   
   //! @copydoc bmptk::hardware::port_oc::get
   static unsigned int get(){ 
      port_in_refresh();
      return port_in_buffer;
   }
   
   //! pin 0, input_only
   typedef single_pin< 0 > pin_0;

   //! pin 1, input_only
   typedef single_pin< 1 > pin_1;

   //! pin 2, input_only
   typedef single_pin< 2 > pin_2;

   //! pin 3, input_only
   typedef single_pin< 3 > pin_3;

   //! pin 4,input_only
   typedef single_pin< 4 > pin_4;

   //! pin 5, input_only
   typedef single_pin< 5 > pin_5;

   //! pin 6, input_only
   typedef single_pin< 6 > pin_6;

   //! pin 7, input_only
   typedef single_pin< 7 > pin_7;
};

template < typename channel > 
unsigned int port_buffer_in< channel >::port_in_buffer;

//! chip interface for a buffered input-output port
class port_buffered_in_out_channel {

   //! initialize the channel
   static void init();
   
   //! write a new value to the pins of the chip
   static void set( unsigned int x );
   
   //! get the value of the pins of the chip
   static unsigned int get();
   
   //! set the direction of the pins
   //
   //! This call sets the direction of each pin.
   //! Each bit determines the direction of 1 bit, 0 = input, 1 = output.
   static unsigned direction( unsigned int x );
};

//! template for a buffered 8-bit input-output port
//
//! This template must be instantiated with a port_buffered_in_out_channel
//! that implements the access to the chip that has the pins.
template< typename channel >
class port_buffer_in_out : public port_in_out {

   static unsigned int port_in_buffer;
   static unsigned int port_out_buffer;
   static unsigned int port_direction_buffer;
     
   static void port_out_flush(){
      channel::set( port_out_buffer );  
   }

   static void port_in_refresh(){
      port_in_buffer = channel::get();   
   }

   static void port_direction_flush(){
      channel::direction( port_direction_buffer );    
   }

   template< int p >
   class single_pin : public bmptk::hardware::pin_in_out {
   public:   
   
      static void init(){
         channel::init();
	   }
	  
      static void set( bool x ){
	     if( x ){
           port_out_buffer |= ( 0x01 << p );
        } else {
		     port_out_buffer &= ~ ( 0x01 << p );
        }		 
		  port_out_flush();
	  }
 
     static bool get(){
        port_in_refresh();
	     return port_in_buffer &( 0x01 << p );
	  } 
     
      static void direction_set_input(){
         port_direction_buffer &= ( 0x01 << p ) ^ -1;
         port_direction_flush();
      }      
   
      static void direction_set_output(){
         port_direction_buffer |= ( 0x01 << p );
         port_direction_flush();
      }   
   };
   
public:   

   //! @copydoc port_in_out::init
   static void init(){
      channel::init();
   }

   //! @copydoc bmptk::hardware::port_in_out::set
   static void set( unsigned int x ){
      port_out_buffer = x;
      port_out_flush();
   }
      
   //! return the number of pins in this port (8)
   static int n_pins(){ 
      return 8; 
   }
   
   //! @copydoc bmptk::hardware::port_in_out::get
   static unsigned int get(){ 
      port_in_refresh();
      return port_in_buffer;
   }
   
   //! @copydoc bmptk::hardware::port_in_out::direction_set_input
   static void direction_set_input(){
      port_direction_buffer = 0;
      port_direction_flush();
   }      
   
   //! @copydoc bmptk::hardware::port_in_out::direction_set_output
   static void direction_set_output(){
      port_direction_buffer = 0 ^ -1;
      port_direction_flush();
   }      
   
   //! pin 0, input-output
   typedef single_pin< 0 > pin_0;

   //! pin 1, input-output
   typedef single_pin< 1 > pin_1;

   //! pin 2, input-output
   typedef single_pin< 2 > pin_2;

   //! pin 3, input-output
   typedef single_pin< 3 > pin_3;

   //! pin 4, input-output
   typedef single_pin< 4 > pin_4;

   //! pin 5, input-output
   typedef single_pin< 5 > pin_5;

   //! pin 6, input-output
   typedef single_pin< 6 > pin_6;

   //! pin 7, input-output
   typedef single_pin< 7 > pin_7;
};

template < typename channel > 
unsigned int port_buffer_in_out< channel >::port_out_buffer;
template < typename channel > 
unsigned int port_buffer_in_out< channel >::port_in_buffer;
template < typename channel > 
unsigned int port_buffer_in_out< channel >::port_direction_buffer = 0;


// ==========================================================================
//
// invert a port
//
// ==========================================================================

#ifndef DOXYDOC

template< class port >
struct invert< port, type_port_out > : public port {
   static void set( unsigned int x ){ port::set( x ^ -1 ); }
};

template< class port >
struct invert< port, type_port_in > : public port {
   static unsigned int get(){ return port::get() ^ -1; }
};

template< class port >
struct invert< port, type_port_in_out > : public port {
   static void set( unsigned int x ){ port::set( x ^ -1 ); }
   static unsigned int get(){ return port::get() ^ -1; }
};
template< class port >

struct invert< port, type_port_oc > : public port {
   static void set( unsigned int x ){ port::set( x ^ -1 ); }
   static unsigned int get(){ return port::get() ^ -1; }
};

#endif

	  
}; // namespace hardware	  
}; // namespace bmptk; 
