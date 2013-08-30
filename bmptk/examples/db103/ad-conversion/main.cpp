#include "bmptk.h"
#include <iostream>

typedef bmptk::target::uart<> uart;
typedef bmptk::target::gpio_1_0 pin;

int main( void ){	
        
   // attach cout to the uart
   std::cout.use( uart::channel() );
      
   // print our message
   std::cout << "\nAD conversion\n";
   
   pin::ad_init();
   for(;;){
      static int n = 0;
      unsigned int ad = pin::ad_get();
      std::cout 
         << std::setw( 6 ) << n++ 
         << "   " << std::setw( 4 )<< ad 
         << "   " << std::setw( 4 )<< std::hex << std::setfill( '0' ) << ad 
         << std::setfill( ' ' ) << "\n";
      bmptk::wait( 500 * bmptk::ms );
   }
}