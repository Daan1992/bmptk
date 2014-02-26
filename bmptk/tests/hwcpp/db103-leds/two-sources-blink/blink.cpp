#include "project.h"

void blink(){
   hwcpp::blinking< target::gpio_1_4, timing >::blink();  
}

