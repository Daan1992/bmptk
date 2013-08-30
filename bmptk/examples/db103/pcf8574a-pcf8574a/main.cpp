#include "bmptk.h"

typedef bmptk::hardware::i2c_master< 
   bmptk::target::gpio_0_4,
   bmptk::target::gpio_0_5
> i2c_bus_1;  		

typedef bmptk::chips::pcf8574a< i2c_bus_1 > chip_1;
   
typedef bmptk::hardware::i2c_master< 
   chip_1::pin_6,
   chip_1::pin_7
> i2c_bus_2;  		

typedef bmptk::chips::pcf8574a< i2c_bus_2 > chip_2;
   
int main( void ){	
   bmptk::hardware::blink< 
      bmptk::hardware::pin_out_from< 
         chip_2::pin_4 >>(); 
}