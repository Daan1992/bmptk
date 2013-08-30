// show a kitt-style display on 12 LEDs

#include "bmptk.h"

typedef bmptk::hardware::port_out_from_pins<
   bmptk::target::gpio_0_4,
   bmptk::target::gpio_0_5,
   bmptk::target::gpio_0_6,
   bmptk::target::gpio_0_7,

   bmptk::target::gpio_1_0,
   bmptk::target::gpio_1_1,
   bmptk::target::gpio_1_2,
   bmptk::target::gpio_1_3,

   bmptk::target::gpio_1_4,
   bmptk::target::gpio_1_5,
   bmptk::target::gpio_1_8,
   bmptk::target::gpio_1_9
> leds;
		
int main( void ){	
   bmptk::hardware::kitt< leds >( 50 * bmptk::ms );	      
}
