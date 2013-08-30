#include "bmptk.h"

typedef bmptk::hardware::spi_bus_sclk_mosi_miso< 
   bmptk::target::gpio_1_2,
   bmptk::target::gpio_1_0,
   bmptk::hardware::pin_in_out_dummy
> bus;  		

typedef bmptk::chips::hc595< 
   bmptk::hardware::spi_channel <
      bus,
      bmptk::target::gpio_1_1,
      bmptk::hardware::spi_mode_0
         >> chip;
   
int main( void ){	
   bmptk::hardware::kitt< chip >(50 * bmptk::ms);   
}