// blink-a-led example for gpio0_0
//
// Gpio0_0 is the reset pin, hence an LPC1114 running this application
// can only be reset by cycling the power. This application can be used
// to test the power-cycling function of the DB100: if a new application
// can be loaded over this one, the power-cycle function works.

#include "bmptk.h"
	
int main( void ){	
	bmptk::hardware::blink< 
      bmptk::target::gpio_0_0
   >();
}