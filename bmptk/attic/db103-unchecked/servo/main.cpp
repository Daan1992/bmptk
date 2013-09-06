#include "bmptk.h"
#include <iostream>   

// create the uart class
// the default baudrate is the download baudrate defined by the makefile
typedef bmptk::target::uart<> uart;

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

int main( void ){	
        
   // attach cout to the uart
   std::cout.use( uart::channel() );
      
   // print our message
   std::cout << "\nHello world\n";
   
   servo::init();
   
   for(;;){
      bmptk::wait( 50 * bmptk::ms );
      servo::pulse( 2500 );
   }
}