#include "targets/lpc1114fn28.hpp"
typedef hwcpp::lpc1114fn28< 48 * hwcpp::MHz > target;
typedef target::waiting timing;

typedef hwcpp::port_out_from_pins< 
   hwcpp::all<
      target::gpio_0_4,
      target::gpio_0_7,
      target::gpio_1_2, 
      target::gpio_1_5
   >,
   hwcpp::all<
      target::gpio_0_5,
      target::gpio_1_0, 
      target::gpio_1_3,
      target::gpio_1_8
   >,
   hwcpp::all<
      target::gpio_0_6,
      target::gpio_1_1, 
      target::gpio_1_4,
      target::gpio_1_9 
   >
> leds;

int main(){	
   hwcpp::kitt< leds, timing >();
}

