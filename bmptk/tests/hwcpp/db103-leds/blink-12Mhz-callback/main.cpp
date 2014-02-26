#include "targets/lpc1114fn28.hpp"
typedef hwcpp::lpc1114fn28< 12 * hwcpp::MHz > target;
typedef target::callback timing;

int main(){	        
   hwcpp::blinking< target::gpio_1_9, timing >::blink();  
}

