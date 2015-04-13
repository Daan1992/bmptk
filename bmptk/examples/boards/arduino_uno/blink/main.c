// blink a LED on pin Arduino 13 = Atmeag328P PB5
//
// This is the pin that is used in the very first Arduino example.
// Some (but not all) Arduino Uno boards have a built-in LED at this pin.
//
// This is a very simple blink-a-LED application.
// It aims to show direct use of the hardware rather than
// good programming practices (like abstracting from the GPIO pin
// used and using a timer for the delay).
//
// It uses a busy-wait for the delay, which can be affected
// by the compiler and optimization settings used.
//

// includes the cmsis definitions for the chip 
#include "bmptk.h"
			
int main( void ){	

   // must be volatile to prevent optimization of the wait loops
   // note: use long, because int on an AVR is 16 bits
   volatile unsigned long int i;
    
   // make the pin an output
   DDRB |= ( 0x01 << 5 );
   
   // loop forever
   while( 1 ){
   
      // make the pin high
      PORTB |= ( 0x01 << 5 );

      // wait 
      for( i = 0; i < 100000; i++ ){ 
      }
	
      // make the pin low
      PORTB &= ~( 0x01 << 5 );

      // wait 
      for( i = 0; i <100000; i++ ){ 
      }
	
	}     
}