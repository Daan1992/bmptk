#include "bmptk.h"
#include <iostream>   

// create the uart class
// the default baudrate is the download baudrate defined by the makefile
typedef bmptk::target::uart<> uart;

typedef bmptk::sr04< 
   bmptk::target::gpio_1_0_out,
   bmptk::target::gpio_1_1_in
> sr04; 

template< typename pin >
struct servo1 {
   static void init(){
      pin::init();
      pin::set(0);
   }
   static void pulse( int n ){
      pin::set(1);
      bmptk::wait( n * bmptk::us );
      pin::set(0);
   }
};

typedef servo1< bmptk::target::gpio_1_3_out > servo;

template< int start, int steps >
void scan(){
   int image[ steps ];
   for( int i=0; i< 20; i++ ){
      servo::pulse( 1500 - start );
      bmptk::wait( 50 * bmptk::ms ); 
   }
   for( int i = 0; i < steps; i++ ){
      for( int j = 0; j < 5; j++ ){
         servo::pulse( ( 1500 - start ) + ( i * 2 * start ) / steps );
         bmptk::wait( 50 * bmptk::ms );
      }   
      image[ i ] = sr04::distance_mm_get();
   }
   for( int i=0; i< 20; i++ ){
      servo::pulse( 1500 - start );
      bmptk::wait( 50 * bmptk::ms ); 
   }
   for( int i = 0; i < steps; i++ ){
      std::cout<< std::setw( 2 ) << i << "  " << std::setw( 4 ) << image[ i ];
      for( int j = 0; 35 * j < image[ i ]; j++ ){ std::cout << ' '; }
      std::cout << "*\n"; 
   }
}

int main( void ){	
        
   // attach cout to the uart
   std::cout.use( uart::channel() );
      
   // print our message
   std::cout << "\nHello world\n";
   
   sr04::init();
   servo::init();
   
   scan< 700, 30 >();
   
   std::cout << "done\n";
   
   // do not terminate
   if(0) for(;;){
      bmptk::wait( 1000 * bmptk::ms );
      std::cout << sr04::distance_mm_get() << "\n";
   }
}