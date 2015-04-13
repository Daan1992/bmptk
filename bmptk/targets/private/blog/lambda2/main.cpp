// =======================================================================
//
// featherweight multitasking demo
//
// =======================================================================

// #include "targets/lpc1114fn28.hpp"

// #include <iostream>
#include <functional>
#include <array> 
#include <algorithm> 
#include "header.hpp"
 
// int twice( int x ){ return 2 * x; }  

void apply( const std::function< void(int) > & g ){ g( 3 ); }   
// void apply( int (*g)(int) ){ (void) g( 3 ); }

// template< typename T> void apply( const T & g ){ (void) g( 3 ); }


 
int main(){	         
   // apply( [](int x ){ return 2 * x; } );
}
