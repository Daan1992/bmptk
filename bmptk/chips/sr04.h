//***************************************************************************
//
// file : bmptk/hardware/sr04.h
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

#pragma once
#ifndef BMPTK_CHIPS_H
#define BMPTK_CHIPS_H

   namespace bmptk {
                   
      //! interface to an SR04 ultrasonic distance sensor
      //
      //! This sensor can be triggered to output a short ultrasonic
      //! sound burst, and detect the reflection of this burst from
      //! a nearby solid object. The datasheet claims that it can
      //! detect objects at distances of 2 .. 400 cm. My estimate
      //! is that 4 .. 100 cm is more realistic.
      //!
      //! The sensor needs a 5V supply (it won't work a 3.3 V), but
      //! its trigger input will trigger on a 3.3V pulse.
      //!
      //! The sensor's echo output will produce a pulse that starts
      //! ~ 500 us after the trigger pulse, and ends when the echo is 
      //! received. Hence the width of the pulse is proportional to the
      //! distance of the object.
      //!
      //! The get_pulse_length() method (and hence the get_distance() 
      //! method) uses busy waiting (in a tight loop, not by calling 
      //! bmptk wait function) to generate the trigger pulse and to
      //! measure the echo pulse. When the pulse does not start within
      //! 1 ms after the trigger (expected time is 500 us), or when
      //! the echo pulse is longer than 30 ms (corresponding to > 5 m)
      //! an error is returned. 
      //!
      //! To avoid problems with late reflexions of a pulse influencing a
      //! next measurement it is advised to observe a time interval of
      //! at least 60 ms between calls to the get_pulse_length()
      //! or get_distance() methods.
      //
      template< typename trigger, typename pulse >
      class sr04 {   
      public: 
	  
      	 //! constructor, supply the trigger and echo pins
         static void init(){
            trigger::init();
            trigger::set( 0 );
            pulse::init();
         }
        
         //! measure and return the echo pulse witdh 
         //
         //! The unit is the bmptk timing unit, so to convert for instance 
         //! to microseconds divide the result by bmptk::us.
         //!
         //! This method uses busy waiting. It can take up to 32 ms.
         //! When no echo pulse is seen or it is too long the error_value
         //! is returned.
         static bmptk::time pulse_width_get( 
            bmptk::time error_value = -1 * bmptk::us 
         ){
		 
         	  // when the echo line is not silent something is wrong
         	  if( pulse::get() ){
               return error_value;
            }            	
         	
            // trigger pulse to the sr04
            trigger::set( 1 );
            bmptk::wait( 10 * bmptk::us );
            trigger::set( 0 );
         
            // wait for start of echo pulse from sr04
            bmptk::time t1 = bmptk::current_time();
            while( !pulse::get() ){
               if( ( bmptk::current_time() - t1 ) > ( 1 * bmptk::ms ) ){
                  return error_value;
               }
            }
         
            // wait for end of echo pulse from sr04
            t1 = bmptk::current_time();
            while( pulse::get() ){
               if( ( bmptk::current_time() - t1 ) > ( 20 * bmptk::ms ) ){
                  return error_value;
               }
            } 
      
            // get end time
            bmptk::time t2 = bmptk::current_time();
            
            // debug
            // std::cout << t2 - t1 << " " << bmptk::us << "\n" ; 
            
            // return length of the pulse
            return t2 - t1;
         }
         
         //! measure and return the distance to a detected object
         //
         //! This method needs the speed of sound to calculate the
         //! distance from the echo pulse length it gets from 
         //! get_pulse_width(). This speed must be supplied as units 
         //! of distance per microsecond. The returned result is expressed
         //! in the same unit of distance. The default speed_of_sound 
         //! parameter value is 3430 mm/us, hence the default unit of the 
         //! returned value is mm.
         //!
         //! This method uses busy waiting. It can take up to 32 ms.
         //! When no echo pulse is seen or it is too long the error_value
         //! is returned.
         static int distance_mm_get(
            int error_value    = -1,
            int speed_of_sound = 3430  
         ){
            bmptk::time t = pulse_width_get( -1 * bmptk::us ); 
            if( t == ( -1 * bmptk::us )){ 
               return error_value; 
            }
         
            // calculate the distance in mm
            return ( t * speed_of_sound ) / ( 2 * 10000 * bmptk::us );   
         }
            
      }; 


        
   }; // namespace bmptk;         

#endif // #ifndef BMPTK_CHIPS_H