// beep on pin PIO1_5
//
// This is a very simple beep application.
// It aims to show direct use of the hardware rather than
// good programming practices (like abstracting from the GPIO pin
// used and using a timer for the delay).
//
// It uses the CMSIS header file to access the direction
// and value registers for pin GPIO1_5.
// It uses a busy-wait for the delay, which can be affected
// by the compiler and optimization settings used.
//
// Note that after a reset the GPIO1_5 pin is automatically
// configured as GPIO, hence there is no need to configure
// the pin. Some other pins are NOT configured to GPIO by a reset,
// hence blinking on such a pin requires the additional step of
// configuring the pin as GPIO.
//

#include "bmptk.h"

//! beep on pin PIO1_5
//
//! This function will beep on pin PIO1_5 for n periods.
//! The length of each phase (half-period) is determined by a
//! busy for loop that iterates x times.
void beep( int n, int x ){
   volatile int i;

   LPC_GPIO1->DIR = LPC_GPIO1->DIR | ( 0x1 << 5 );
   
   while( n > 0 ){

      LPC_GPIO1->DATA = LPC_GPIO1->DATA | ( 0x1 << 5 );

      for( i = 0; i < x; i++ );
	
      LPC_GPIO1->DATA = LPC_GPIO1->DATA & ~ ( 0x1 << 5 );
 
      for( i = 0; i < x; i++ );
      
      n--;
   }
}
			
int main( void ){	
   int i;
   for( i= 0; i < 3; i++ ){
      beep( 500, 500 );
      beep( 400, 800 );
   }   
   return 0;
}