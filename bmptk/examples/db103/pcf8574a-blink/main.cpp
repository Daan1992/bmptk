#include "bmptk.h"

typedef bmptk::hardware::i2c_master< 
   bmptk::target::gpio_0_4,
   bmptk::target::gpio_0_5
> i2c_bus;  		

typedef bmptk::chips::pcf8574a< i2c_bus > chip;
   
int main( void ){	
   bmptk::hardware::blink< chip::pin_3 >();   
}