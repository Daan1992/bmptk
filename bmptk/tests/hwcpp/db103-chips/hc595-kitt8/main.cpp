#include "targets/lpc1114fn28.hpp"
typedef hwcpp::lpc1114fn28< 48 * hwcpp::MHz > target;
typedef target::timing timing;

typedef hwcpp::hc595<  
   hwcpp::spi_channel_bb<
      hwcpp::spi_bus_sclk_mosi_miso_bb<
         target::gpio_1_2,
         target::gpio_1_0,
         hwcpp::pin_in_out_dummy,
         timing::MHz< 1 >
      >,
      hwcpp::invert< target::gpio_1_1 >
   >      
> chip; 
 

int main(){	        
   std::cout.connect< target::uart<> >();
   std::cout << "kitt on 8 LEDs on an HC595\n";

   hwcpp::kitt< chip, timing >();
}

