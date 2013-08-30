// show a 'wiggle' pattern (= alternating 0xAAA and 0x555) on 12 LEDs

#include "bmptk.h"

typedef bmptk::hardware::tee<
                            bmptk::target::gpio_0_4,
   bmptk::hardware::invert< bmptk::target::gpio_0_5 >,
                            bmptk::target::gpio_0_6,
   bmptk::hardware::invert< bmptk::target::gpio_0_7 >,

                            bmptk::target::gpio_1_0,
   bmptk::hardware::invert< bmptk::target::gpio_1_1 >,
                            bmptk::target::gpio_1_2,
   bmptk::hardware::invert< bmptk::target::gpio_1_3 >,

                            bmptk::target::gpio_1_4,
   bmptk::hardware::invert< bmptk::target::gpio_1_5 >,
                            bmptk::target::gpio_1_8,                         
   bmptk::hardware::invert< bmptk::target::gpio_1_9 >
> leds;
	
int main( void ){	
   bmptk::hardware::blink< leds >();
}
