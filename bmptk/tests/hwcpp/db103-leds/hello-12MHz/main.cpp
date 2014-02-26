#include "targets/lpc1114fn28.hpp"
typedef hwcpp::lpc1114fn28< 12 * hwcpp::MHz > target;
typedef target::callback timing;

int main(){	        
   std::cout.connect< target::uart<> >();
   TRACE << "Hello world";
   timing::init();
   TRACE << "timing initialized";
}

