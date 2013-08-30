#include "bmptk.h"
#include <iostream>

// create the uart class
// the default baudrate is the download baudrate defined by the makefile
typedef bmptk::target::uart<> uart;

int main( void ){	

   // attach cout to the uart
   std::cout.use( uart::channel() );
      
   // print our message
   std::cout << "\nHello world\n";
   
   bmptk::wait( 500 * bmptk::ms );
     
   std::cout << "\ncontext (stack) info: " << bmptk::main_context << "\n";
   
   // do not terminate
   for(;;);
}