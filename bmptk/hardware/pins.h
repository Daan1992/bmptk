//***************************************************************************
//
// file : bmptk/hardware/pins.h
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
// pin interface classes
//
// ==========================================================================

//! input pin interface 
//
//! This class defines the interface of an input pin.
//!
//! This class is to be used as parent class for
//! classes that provide the pin_in interface. 
struct pin_in {

   //! identify this type as a type_pin_in
   enum { type = type_pin_in };
   
   //! this type has no additional fatures
   enum { features = has_none };

   //! initialize the pin as input
   //
   //! This method initializes the pin as input pin.
   //! This might involve configuring the pin as gpio, and
   //! disabling analog or other special functions.
   static void init();
   
   //! get the current level on the pin
   //
   //! This method returns the current level on the input pin.
   //! When the level is a logic one (high voltage, Vdd) the
   //! returned value is true, otherwise (logic zero, low level, Vss)
   //! the value false is returned.
   //!
   //! The init() method must be called first.
   static bool get();
};

//! output pin interface
//
//! This class defines the interface of an output pin.
//!
//! This class is to be used as parent class for
//! classes that provide the pin_out interface. 
struct pin_out {

   //! identify this type as a type_pin_out
   enum { type = type_pin_out };
   
      //! this type has no additional fatures
   enum { features = has_none };

   //! initialize the pin as output
   //
   //! This method initializes the pin as output pin.
   //! This might involve configuring the pin as gpio, and
   //! disabling analog or other special functions.
   static void init();
   
   //! set the current level on the pin
   //
   //! This method sets the current level output by the pin.
   //! When the value is true a logic one (high voltage, Vdd) will appear
   //! on the pin, when the value if false a the pin will 
   //! output a logic zero (low voltage, Vss).
   //!
   //! The init() method must be called first.
   static void set( bool x );
};

//! input-output pin interface
//
//! This class defines the interface of a pin
//! that can be configured for use as either input or output.
//!
//! This class is to be used as parent class for
//! classes that provide the pin_in_out interface. 
struct pin_in_out {

   //! identify this type as a type_pin_in_out
   enum { type = type_pin_in_out };
   
      //! this type has no additional fatures
   enum { features = has_none };
   
   //! initialize the pin
   //
   //! This method initializes the pin as an input/output pin.
   //! This might involve configuring the pin as gpio, and
   //! disabling analog or other special functions.   
   //! Note that it does *not* involve setting the pin to
   //! a known direction, but in general microcontroller
   //! pins will default to input after a reset.
   static void init();
   
   //! set the direction of the pin to input
   //
   //! This method call sets the direction of the pin to input.
   //! The init() method must have been called first.
   static void direction_set_input();
   
   //! set the direction of the pin to output
   //
   //! This method call sets the direction of the pin to output.
   //! The init() method must have been called first.
   static void direction_set_output();
     
   //! @copydoc pin_in::get()
   //! After calling init(), the direction must be set to input
   //! by calling direction_set_input().
   static bool get();
   
   //! @copydoc pin_out::set()
   //! After calling init(), the direction must be set to output
   //! by calling direction_set_output().
   //!
   //! Note that a set() call on a pin that is not yet set to output
   //! might not have any effect.
   static void set( bool x );
};

//! open_collector input-output pin interface
//
//! This class defines the interface of a pin
//! that can only sink (not source) current, 
//! and can be an input too. Such a pin is typically
//! used on a multi-drop bus with a pull-up resistor.
//!
//! This class is to be used as parent class for
//! classes that provide the pin_in_out interface. 
struct pin_oc {

   //! identify this type as a type_pin_oc
   enum { type = type_pin_oc };
   
      //! this type has no additional fatures
   enum { features = has_none };

   //! initialize the pin
   //
   //! This method initializes the pin as an open-collector/input pin.
   //! This might involve configuring the pin as gpio, and
   //! disabling analog or other special functions.   
   //! Note that it does *not* involve setting the pin to
   //! a known level, but in general microcontroller
   //! pins will default to input after a reset,
   //! which corresponds to a high level for an open-collector
   //! pin with an external pull-up resistor.
   static void init();
   
   //! @copydoc pin_out::set()
   //!
   //! Setting the pin to 0 causes the pin to be pulled low.
   //! Setting a pin to 1 causes the pin to float.
   static void set( bool x ); 
   
   //! @copydoc pin_in::get()
   //!
   //! Note that after calling set(0) the pin will in all probability
   //! read as 0, so this call only makes realy sense after a set(1) call.
   static bool get();
};


//! interface for configuring a pull-up pin
//
//! This is the mix-in interface for a pin that can have a configurable
//! (weak) pull-up.
//!
//! This class is to be used as mix-in parent class for
//! classes that provide the pin_in_out interface. 
class pin_configurable_pullup {
public:

   //! this type has a configurable pull-up
   enum { features = has_pullup };

   //! enable the pullup
   static void pullup_enable();
   
   //! disable the pullup
   static void pullup_disable();
};

//! interface for an anlog (A/D) input pin
//
//! This is the interface for a pin that can function as a
//! digital (Analog-to_digital converter) input.
//!
//! This class is to be used as parent class for
//! classes that provide the pin_in_out interface. 
class pin_ad {
   
      //! this type has an A/D converter
   enum { features = has_ad };

   //! initialize the pin as A/D pin
   //
   //! This method must be called before the get() method
   //! is called.
   static void ad_init();
   
   //! convert the signal and return the result
   //
   //! This method starts the A/D converter, waits for
   //! the rusult and returns it. 
   //! The ad_init() method must have been called first.
   static unsigned int ad_get();
   
   //! return the number of significant bits
   static unsigned int ad_bits();
   
   //! return the maximum A/D value
   static unsigned int ad_max();
};


// ==========================================================================
//
// create a pin from another pin
//
// ==========================================================================

#ifdef DOXYDOC

   //! create a pin_in_out from another pin
   //
   //! This template creates a pin_in_out from the argument pin.
   //! The argument pin must be a pin_in_out or a pin_oc.
   //!
   //! The pin_in_out_from, pin_in_from, pin_out_from and pin_oc_from templates 
   //! are used by template algorithms to convert
   //! their arguments to the flavour of pin that they expect.
   template< typename pin > struct pin_in_out_from : public pin_in_out {};

#else

//! fallback when no specfic match is found
template< typename pin, int flavour = pin::type >
struct pin_in_out_from {
   static_assert( 
      ( flavour == bmptk::type_pin_in_out )
      | ( flavour == bmptk::type_pin_oc ),
	   "pin_in_out_from requires a pin_in_out or a pin_oc"
   );	       
};

//! create a pin_in_out from a pin_in_out
template< typename pin >
struct pin_in_out_from< pin, type_pin_in_out > : public pin {};

//! create a pin_in_out from a pin_oc
template< typename pin >
struct pin_in_out_from< pin, type_pin_oc > : public pin {

   //! \copydoc pin_in_out::direction_set_input
   static void direction_set_input(){
      // setting an open-collector pin high will
	  // make it float
      pin::set( 1 );
   }
   
   //! \copydoc pin_in_out::direction_set_output
   static void direction_set_output(){
      // no need to do anything, setting the pin high
	  // or low will do the real work
   }
        
};

#endif

#ifdef DOXYDOC

   //! create a pin_oc from another pin
   //
   //! This template creates a pin_oc from the argument pin.
   //! The argument pin must be a pin_in_out or a pin_oc.
   //!
   //! The pin_in_out_from, pin_in_from, pin_out_from and pin_oc_from templates 
   //! are used by template algorithms to convert
   //! their arguments to the flavour of pin that they expect.
   template< typename pin > struct pin_oc_from : public pin_oc {};
   
#else

//! fallback when no specfic match is found
//! [pin_oc_from default]
template< typename pin, int flavour = pin::type >
struct pin_oc_from {
   static_assert( 
      ( flavour == bmptk::type_pin_oc )
      | ( flavour == bmptk::type_pin_in_out ),
	   "pin_oc_from requires a pin_oc or a pin_in_out"
   );	       
};
//! [pin_oc_from default]

//! create a pin_oc from a pin_oc
//! [pin_oc_from pin_oc]
template< typename pin >
class pin_oc_from< pin, type_pin_oc > : public pin {};
//! [pin_oc_from pin_oc]

//! create a pin_oc from a pin_in_out
//! [pin_oc_from pin_in_out]
template< typename pin >
struct pin_oc_from< pin, type_pin_in_out > : public pin_oc {

   // don't inherit from the pin, because that would inherit 
   // direction_set_input and direction_set_output

   //! \copydoc pin_oc::set
   static void set( bool x ){
      // to make a pin_in_out behave like a pin_oc
      if( x ){
	  
	     // make it float when it is set high
	     pin::direction_set_input();
		 
      } else {
	  
	     // make it output and low when it is set low
         pin::direction_set_output();
         pin::set( 0 );
      }		 
   }  	  
   
   //! \copydoc pin_oc::get
   static bool get(){
      return pin::get();
   }	  
   
};
//! [pin_oc_from pin_in_out]


#endif

#ifdef DOXYDOC

   //! create a pin_out from another pin
   //
   //! This template creates a pin_out from the argument pin.
   //! The argument pin must be a pin_in_out, pin_out or a pin_oc.
   //!
   //! The pin_in_out_from, pin_in_from, pin_out_from and pin_oc_from templates 
   //! are used by template algorithms to convert
   //! their arguments to the flavour of pin that they expect.
   template< typename pin > struct pin_out_from : public pin_out {};

#else

//! fallback when no specfic match is found
template< typename pin, int flavour = pin::type >
struct pin_out_from {
   static_assert( 
      ( flavour == bmptk::type_pin_out )
      | ( flavour == bmptk::type_pin_in_out )
      | ( flavour == bmptk::type_pin_oc ),
	   "pin_out_from requires a pin_out, pin_in_out or a pin_oc"
   );	       
};

//! create a pin_out from a pin_out
template< class pin >
struct pin_out_from< pin, type_pin_out > : public pin {};

//! create a pin_out from a pin_in_out
template< class pin >
struct pin_out_from< pin, type_pin_in_out > : public pin_out {

   // don't inherit from the pin, because that would inherit 
   // get() and direction_set_input and direction_set_output

   //! \copydoc pin_in_out::init
   static void init(){
      pin::init();
	  pin::direction_set_output();
   }
    
   //! \copydoc pin_in_out::set
   static void set( bool x ){
      pin::set( x );
   }
};

//! create a pin_out from a pin_oc
template< class pin >
struct pin_out_from< pin, type_pin_oc > : public pin_out {

   // don't inherit from the pin, because that would inherit get()

   //! \copydoc pin_in_out::init
   static void init(){
      pin::init();
   }	  
   
   //! \copydoc pin_in_out::set
   static void set( bool x ){
      pin::set( x );
   }
};

#endif
	
	
#ifdef DOXYDOC

   //! create a pin_in from another pin
   //
   //! This template creates a pin_in from the argument pin.
   //! The argument pin must be a pin_in, pin_in_out or a pin_oc.
   //!
   //! The pin_in_out_from, pin_in_from, pin_out_from and pin_oc_from templates 
   //! are used by template algorithms to convert
   //! their arguments to the flavour of pin that they expect.
   template< typename pin > struct pin_in_from : public pin_in {};

#else

//! fallback when no specfic match is found
template< typename pin, int flavour = pin::type >
struct pin_in_from {
   static_assert( 
      ( flavour == bmptk::type_pin_in )
      | ( flavour == bmptk::type_pin_in_out )
      | ( flavour == bmptk::type_pin_oc ),
	   "pin_in_from requires a pin_out, pin_in_out or a pin_oc"
   );	       
};

//! create a pin_in from a pin_in
template< class pin >
struct pin_out_from< pin, type_pin_in > : public pin {};

//! create a pin_in from a pin_in_out
template< class pin >
struct pin_in_from< pin, type_pin_in_out > : public pin_in {

   // don't inherit from the pin, because that would inherit 
   // get() and direction_set_input and direction_set_output

   //! \copydoc pin_in::init
   static void init(){
      pin::init();
	  pin::direction_set_input();
   }
      
   //! \copydoc pin_in::get
   static bool get(){
      return pin::get();	  
   }   
};
	
//! create a pin_in from a pin_oc
template< class pin >
struct pin_in_from< pin, type_pin_oc > : public pin_in {

   // don't inherit from the pin, because that would inherit set()

   //! \copydoc pin_in::init
   static void init(){
      pin::init();
	  pin::set( 1 );
   }
      
   //! \copydoc pin_in::get
   static bool get(){
      return pin::get();	  
   }   
};

#endif


// ==========================================================================
//
// invert a pin (or port, in ports.h)
//
// ==========================================================================

#ifdef DOXYDOC

   //! invert the polarity of a pin or port
   //
   //! This class template is used to create a pin or port class
   //! based on an underlying pin or port class. 
   //! The created pin or port behaves the same as the underlying pin or
   //! port, except that any value written (set()) to the created pin or port 
   //! is inverted (true becomes false, false becomes true) before
   //! it is written to the underlying pin or port, and
   //! any value that is read (get()) is likewise inverted before it is
   //! returned.
   //!
   //! The typename template argument must be a pin or port class.
   //! The created pin or port is of the same flavour.
   //! 
   //! Example (examples/db103/wiggle/main.cpp):
   //! @include examples/db103/wiggle/main.cpp
   //! This examples creates a display that alternatively lights the
   //! even and then the odd numbered LEDs.
   //! This is achieved by feeding the odd numbered LEDs the 
   //! inverted signal.
   template< typename pin_or_port > class invert : public pin_or_port {};

#else

//! basic invert, when no specific match is found
template< class t, int flavour = t::type >
struct invert {
   static_assert( 
      // dummy condition
      flavour == bmptk::type_pin_in,
	   "invert requires a pin or port"
   );	
};
	
//! invert a pin_in 
template< class pin >
struct invert< pin, type_pin_in > : public pin {

   //! \copydoc pin_in::get
   static bool get(){
      return ! pin::get();	  
   }  
};	

//! invert a pin_out
template< class pin >
struct invert< pin, type_pin_out > : public pin {

   //! \copydoc pin_out::set
   static void set( bool x ){
      pin::set( ! x );	  
   }   
};	

//! invert a pin_in_out
template< class pin >
struct invert< pin, type_pin_in_out > : public pin {

   //! \copydoc pin_in_out::set
   static void set( bool x ){
      pin::set( ! x );	  
   }  

   //! \copydoc pin_in_out::get
   static bool get(){
      return ! pin::get();	  
   }  
};	

//! invert a pin_oc
template< class pin >
struct invert< pin, type_pin_oc > : public pin {
 
   //! \copydoc pin_oc::set
   static void set( bool x ){
      pin::set( ! x );	  
   }   

   //! \copydoc pin_oc::get
   static bool get(){
      return ! pin::get();	  
   } 
};	

#endif

// ==========================================================================
//
// flipable
//
// ==========================================================================

//! add the flip() method to a pin class
template< class pin, int flavour = pin::type >
struct flipable : public pin {
private:

   static_assert( 
      ( flavour == bmptk::type_pin_out )
      | ( flavour == bmptk::type_pin_in_out )
      | ( flavour == bmptk::type_pin_oc ),
	   "flipable requires a pin_out, pin_in_out or a pin_oc"
   );

   static bool value;
   
public:   

   //! \copydoc pin_out::set
   static void set( bool x ){
      value = x;
	  pin::set( x );
   }

   //! flip (invert) the value fo the pin
   //
   //! This method writes the flipped (inverted) value
   //! to the pin. 
   //! Both flip() and set() save the last value, so
   //! it can be inverted by flip().
   static void flip(){
      value = ! value;
      pin::set( value );
   }
};

template< class pin, int flavour > bool flipable< pin, flavour >::value;


// ==========================================================================
//
// detect a rising_edge
//
// ==========================================================================

//! detect a rising edge on a pin that can be converted to pin_in
template< class arg_pin, int flavour = arg_pin::type >
struct rising_edge : public pin_in {
private:

   static_assert( 
      ( flavour == bmptk::type_pin_in )
      | ( flavour == bmptk::type_pin_in_out )
      | ( flavour == bmptk::type_pin_oc ),
	   "flipable requires a pin_in, pin_in_out or a pin_oc"
   );

   typedef pin_in_from< arg_pin > pin;
   
   static bool last;
   
public:   

   //! \copydoc pin_in::init
   static void init(){
      pin::init();
   }	  
   
   //! get (detect) a rising edge
   //
   //! This method will return 1 if and only if the
   //! underlying pin now reads 1 and did read 0 on the previos call.
   //! In other words: it retruns 1 only on a rising edge on the 
   //! underlying pin.
   static bool get(){
	  if( last ){
	     if( ! pin::get() ){
		    last = false;
         }		 
      }	else {	
	     if( pin::get() ){
		    last = true;
			return true;
		 }
	  }
      return false;
   }   
};

template< class pin, int flavour > bool rising_edge< pin, flavour >::last = 0;


// ==========================================================================
//
// debounce
//
// ==========================================================================

//! debounce a pin_in
//
//! This template debounces a pin by keeping at least intreval time
//! between reading the underlying pin. In other words: when the
//! pin has been read less than interval time ago, it is not read
//! again but the old value is returned.
template< class arg_pin >
struct debounce : public pin_in {
private:

   static_assert( 
      ( (int)arg_pin::type == bmptk::type_pin_in )
      | ( (int)arg_pin::type == bmptk::type_pin_in_out )
      | ( (int)arg_pin::type == bmptk::type_pin_oc ),
	   "flipable requires a pin_in, pin_in_out or a pin_oc"
   );

   typedef pin_in_from< arg_pin > pin;
   
public:   

   //! \copydoc pin_in::init
   static void init(){
      pin::init();
   }	  
   
   //! get the debounced state of a pine
   //
   //! If the previous reading of the underlying pin has been more than
   //! interval ago, this method will read and return the pin value.
   //! If not, it will rerturn the last value read from the pin.
   //!
   //! The default interval of 50 ms will effectively debounce all
   //! switches and buttons, while still being unnoticable by humans.   
   static bool get( bmptk::time interval = 50 * bmptk::ms ){
      static bool last_value;
      static bmptk::time last_time = -1 * bmptk::s;
      bmptk::time now = bmptk::current_time();
	   if( now >= last_time + interval ){
	      last_value = pin::get();
         last_time = now;
      }         
      return last_value;
   }   
};


// ==========================================================================
//
// combine pins tee-wise: write to the master to write to them all
//
// ==========================================================================

#ifdef DOXYDOC

   //! one out_pin that represents any number of pin_out's
   //
   //! This class template is used to construct a single out_pin class
   //! from any number of specified downstream out_pin classes,
   //! or from all pins in an out_port.
   //! Calling init() or set() on the tee class causes the corresponding
   //! method to be called for all downstream out_pin classes
   //! (or for the underlying out_port).
   //! Hence the effect is that one out_pin can represent a bunch of out_pin's,
   //! as if they were fitted together using tee fittings.
   //!
   //! A tee with zero parameters is a kind of dummy point: you can 
   //! cann init() and set() on it, but to no effect at all.
   //!
   //! The typename template arguments must be classes that are descendants
   //! of pin_out, or a single class that is a decendant of port_out.
   //! 
   //! Example (examples/db103/wiggle/main.cpp):
   //! @include examples/db103/wiggle/main.cpp
   template< typename... > class tee : public pin_out {};
   template< typename port_out > class tee : public pin_out {};

#else

template< typename... > class tee;

template<>
class tee<> : public pin_out {
public:
   static void init(){}
   static void set( bool b ){}   
};

template< typename arg_pin, typename... tail_args >
struct tee< arg_pin, tail_args... > : public pin_out {

   static_assert( 
      ( (int)arg_pin::type == bmptk::type_pin_out )
      | ( (int)arg_pin::type == bmptk::type_pin_in_out )
      | ( (int)arg_pin::type  == bmptk::type_pin_oc ),
	   "flipable requires a pin_out, pin_in_out or a pin_oc"
   );
   
   typedef pin_out_from< arg_pin > pin;

   typedef tee< tail_args... > tail;

   static void init(){
      pin::init();
      tail::init();
   }
   
   static void set( bool b ){
      pin::set( b );
      tail::set( b );   
   }   
};

//! a do-nothing pin_in_out
//
//! This type can be used as dummy in places where a pin_in_out is required
//! but the operations on it must be ignores.
struct pin_in_out_dummy : pin_in_out {
   static void init(){}
   static void direction_set_input(){}
   static void direction_set_output(){}
   static bool get(){ return 0; }
   static void set( bool x ){}
};


/*
// ==========================================================================
//
// combine two pins with a logic function
//
// ==========================================================================

template < typename a, typename b > struct logic_xor : public pin_out {
   static void init(){ a::init(); b::init(); }	  
   static bool get(){ return a::get() ^ b::get(); }
};   

*/


/*
template < 
   typename port
> class tee < 
   port,
   std::enable_if< std::is_base_of< port_out, port >::value, int >
> : public pin_out {
public:

   static_assert(
      std::is_base_of< port_out, port>::value, 
      "port must be a descendant of port_out"
   );

   static void init(){
      port::init();
   }
   
   static void set( bool b ){
      port::set( b ? -1 : 0 );
   }
   
};
*/



#endif


	
}; // namespace hardware	  
}; // namespace bmptk; 
