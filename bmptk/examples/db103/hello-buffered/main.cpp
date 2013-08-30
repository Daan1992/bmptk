#include "bmptk.h"
#include <iostream>

// create the uart class
// the default baudrate is the download baudrate defined by the makefile
typedef bmptk::target::uart<> uart;

int main( void ){	
   
   // create a 100 char buffer object on top of the uart
   bmptk::output_channel_polled_buffer< char, 100 > buffer( uart::channel() );
   
   // attach cout to the buffer
   std::cout.use( buffer );
   
   // these characters will be put in the buffer
   std::cout << "\nHello world\n";
   
   // this char will be printed before the buffer is printed
   uart::put( '*' );

   // this will acually print the buffer
   for(;;){
      buffer.poll();
   }
}