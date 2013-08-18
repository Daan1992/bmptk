//***************************************************************************
//
// file : bmptk/core/time.h
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


namespace bmptk {

//! time elapsed since some epoch
//
//! Time is expressed as the number of clock ticks elapsed since some epoch. 
//! On most targets the first call to current_time() starts the hardware 
//! timer, so the epch is this first call.
//! Time is stored as an excapsulated unsigned long long int.
//! The operators +, -, *, /, > and < are provided, as far as they make sense.
//!
//! Application code must use constants us, ms and s to express time.
//! The real unit of time is the number of us, not exceeding 1000. 
//! Hence the maximum time than is can be expressed is 
//! 18446744073 seconds, or 584 years.
//!
//! All ime opeartions are constexpr and/or inline, hence the time 
//! abstraction imposes no overhead.
class time {

   // the time expressed as 64-bit signed integer
   long long int t;
   
   // convert an integer to a time
   constexpr time( long long int n ): t( n ){}
   
   // current_time() must be able to convert an integer to a time
   friend bmptk::time bmptk::current_time();
public:

   //! create an un-initialized time object
   time(){}

   #ifndef DOXYDOC
   // to be used only by const us, hence not documented
   // return the time value for one microsecond
   static constexpr time us(){ return time( BMPTK_TICKS_PER_US ); }
   #endif
   
   //! adding two times yields a time
   constexpr time operator+( const time &rhs ) const {
      return time( t + rhs.t );
   }
   
   //! adding two times
   void operator+=( const time &rhs ){
      t += rhs.t;
   }
   
   //! substracting two times yields a time
   constexpr time operator-( const time &rhs ) const {
      return time( t - rhs.t );
   }
   
   //! substracting two times
   void operator-=( const time &rhs ){
      t -= rhs.t;
   }
   
   //! multiplying a time by an integer yields a time
   constexpr time operator*( const int n ) const {
      return time( t * n );
   }
   
   //! multiplying a time by an integer
   void operator*=( const int n ){
      t *= n;
   }
   
   //! dividing a time by an integer yields a time
   constexpr time operator/( const int n ) const {
      return time( t / n );
   }
   
   //! dividing a time by an integer
   void operator/=( const int n ){
      t /= n;
   }
   
   //! dividing a time by another time yields an integer 
   constexpr unsigned long long int operator/( const time rhs ) const {
      return t / rhs.t;
   }
   
   //! compare two times
   constexpr bool operator<( const time rhs ) const {
      return t < rhs.t;
   }
   
   //! compare two times
   constexpr bool operator<=( const time rhs ) const {
      return t <= rhs.t;
   }
   
   //! compare two times
   constexpr bool operator>( const time rhs ) const {
      return t > rhs.t;
   }
   
   //! compare two times
   constexpr bool operator>=( const time rhs ) const {
      return t >= rhs.t;
   }
   
   //! compare two times for equality
   constexpr bool operator==( const time rhs ) const {
      return t == rhs.t;
   }

   //! compare two times for inequality
   constexpr bool operator!=( const time rhs ) const {
      return t != rhs.t;
   }
};

//! multiplying an integer by a time yields a time
inline constexpr time operator*( long long int n, const time rhs ){
   return rhs * n;
}


//! unit of time : microsecond
//
//! Unit of time, to be used by methods that 
//! require a time argument. For instance, to pass half a millisecond, use 
//!
//!    500 * bmptk::us
constexpr time us( time::us() );

//! unit of time : millisecond
//
//! Unit of time, to be used by methods that 
//! require a time argument. For instance, to pass half a second, use 
//!
//!    500 * bmptk::ms
constexpr time ms( us * 1000 );

//! unit of time : microsecond
//
//! Units of time, to be are used by methods that 
//! require a time argument. For instance, to pass one hour, use 
//!
//!    60 * 60 * bmptk::s
constexpr time s( us * 1000 * 1000 );


//! return the current time
//!
//! This function returns the amount of time elaspsed 
//! since some arbitrary epoch.
//! 
//! On most targets the epoch will be the first call to this function.
//!
//! On most targets this function (and hence all the wait functions 
//! that use it) must be called at least once every x time.
//! In practice this will seldom be a problem: applications that
//! use timing tend to do so on a sub-second basis.
//! On an NDS x is 130 seconds. 
//!
//! On windows this function (and hence all timing related functions)
//! returns the time since windows started, with a resolution of 1 ms.
//! 49 Days after windows was last started it will wrap around to 0.
//! Don't blame me, I am just the piano player..
time current_time();
      
//! busy wait until a moment in time
//
//! This function waits until the specified moment in time.
//! It uses busy waiting, polling \ref current_time().
//!
//! When task switching (which can result in a longer delay) is not a 
//! problem \ref wait_until() or an RTOS time function should be used 
//! instead, to avoid locking up the other tasks.
void wait_busy_until( time t );      

//! wait until a moment in time
//
//! This function waits until the specified moment in time.
//! It uses busy waiting, polling \ref current_time().
//!
//! By default this function calls \ref wait_busy_until, but
//! when an RTOS is used it will switch to another task.
//! When the extra delay that might be caused by task switching
//! is not acceptable \ref wait_busy should be used instead.
void wait_until( time t );      

//! busy wait an amount of time
//
//! This function waits the requested amount of time before returning. 
//! It uses busy waiting, polling \ref current_time() to check
//! whether the request amount of time has elapsed.
//!
//!
//! When this function is used to measure time over 
//! multiple calls using \ref wait_busy_until should be considered
//! beacuse it can avoid the 'accumulated error' problem. 
//!
//! When task switching (which can result in a longer delay) is not a 
//! problem \ref wait() should be used instead, because it does not
//! lock up the other tasks.
void wait_busy( time t );      
  
//! wait an amount of time
//
//! This function waits at leas the requested amount of time 
//! before returning. 
//!
//! When this function is used to measure time over 
//! multiple calls using \ref wait_until should be considered
//! beacuse it can avoid the 'accumulated error' problem.   
//!
//! By default this function calls \ref wait_busy, but
//! when an RTOS is used it will switch to another task.
//! When the extra delay that might be caused by task switching
//! is not acceptable \ref wait_busy should be used instead.
void wait( time t );      


}; // namespace bmptk; 
