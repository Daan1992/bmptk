#include "targets/lpc1114fn28.hpp"
typedef hwcpp::lpc1114fn28< 48 * hwcpp::MHz > target;
typedef target::timing timing;

int main(){	        
   std::cout.connect< hwcpp::uart_out< target::gpio_1_7, timing > >();
   TRACE << "Hello world";
   timing::init();
   TRACE << "timing initialized";
}

