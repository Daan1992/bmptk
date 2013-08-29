// basic blink-a-led example
//
// The bmptk::target:: provides the pin_out, 
// the bmptk::hardware:: provides a blink function template.
// Just instantiate the blink function with the pin_out and let it run.

#include "bmptk.h"
	
int main( void ){	
	bmptk::hardware::blink< bmptk::target::gpio_1_5 >();
}
