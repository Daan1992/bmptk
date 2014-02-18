// ==========================================================================
//
// File      : numeric.hpp
// Part of   : hardware library (www.voti.nl/hwlib)
// Copyright : wouter@voti.nl 2014
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

namespace hardware {

   // =======================================================================
   //
   // hardware library version
   //
   // =======================================================================
   
   struct seven_segment_digit_archetype {
      typedef void has_seven_segment_digit;
      static void set_segments( unsigned char segs );
      static void set_value( unsigned char value );
      static void set_decimal_point( bool enable );
   };
   
   template< unsigned int _n_digits >
   struct seven_segment_display_archetype {
      typedef void has_seven_segment_display;
      static constexpr unsigned int n_digits = _n_digits;
      static void set_digit_segments( unsigned int n, unsigned char segs );
      static void set_digit_value( unsigned int n, unsigned char value );
      static void set_decimal_point( unsigned int n, bool enable );      
   };
   
   template< unsigned int _n_digits >
   struct numeric_display_archteype {
      typedef void as_numeric_display;
      static constexpr unsigned int n_digits = _n_digits;
      static void set_value( unsigned int n, unsigned char points = 0 );      
   };   
   
   
   
}; 