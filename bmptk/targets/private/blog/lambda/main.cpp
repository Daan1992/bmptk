// =======================================================================
//
// featherweight multitasking demo
//
// =======================================================================

// #include "targets/lpc1114fn28.hpp"

#include <iostream>
#include <functional>
#include <array> 
#include <algorithm> 

int twice( int x ){ return 2 * x; } 

template< typename T, typename I >
void apply1( I start, I end, T (*f)(T) ){
   for( auto p = start; p != end; p++ ){
      *p = f( *p );
   }
}

template< typename T, typename F >
void apply2( T *start, T *end, F f ){
   apply1( start, end, (T (*)(T)) f );
}

template< typename T, typename F >
void apply22( T *start, T *end, F f ){
   volatile static int n = 0;
   for( auto p = start; p != end; p++ ){
      *p = f( *p ) + n;
      n += 7;  
   }
}

void apply3( int *start, int *end, int (*f)(int) ){
   for( auto p = start; p != end; p++ ){
      *p = f( *p );
   }
}

void apply4( int *start, int *end, const std::function< int(int) > & f ){
   for( auto p = start; p != end; p++ ){
      *p = f( *p );
   }
}

std::array< int, 5 > a = { 1, 2, 3, 4, 5 };

void foo( int v, int & n ){
   apply4( 
      a.begin(), a.end(),  
      [ &n, v ]( int x ){ ++n; return x + v; } 
   );
}  

int five(){ return 5; }

void gg(){
   // volatile std::array< int, 5 > a = { 1, 2, 3, 4, 5 };   
   apply1( 
      a.begin(), a.end(),  
      five // twice 
   );
   
   apply2( 
      a.begin(), a.end(),  
      five // twice 
   );
   
   for( auto x : a ){
      std::cout << x << "\n";
   }   
   
   apply2(
      a.begin(), a.end(), 
      []( int x ){ return 2 * x; } 
   );
   apply2(
      a.begin(), a.end(), 
      []( int x ){ return 3 * x; } 
   );
}


// void apply( const std::function< int(int) > & g ){ (void) g( 3 ); }

void apply( int (*g)(int) ){ (void) g( 3 ); }

// template< typename T> void apply( const T & g ){ (void) g( 3 ); }

int f( int x ){
   //if( x == 9 ) throw x; 
   return 3 * x; 
}

int main(){	         
   gg();
   volatile int x = a[2];
   (void)x;
   
   std::array< int, 5 > aa;
   aa[ 3 ]++; 
   // try { (void)a; } catch (int x ){};
   //(void)a.at(6);
   apply( [](int x ){ return 2 * x; } );
   // apply( f );
}
