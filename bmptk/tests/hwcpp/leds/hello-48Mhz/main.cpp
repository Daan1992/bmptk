#include "targets/lpc1114fn28.hpp"
typedef hwcpp::lpc1114fn28< 48 * hwcpp::MHz > target;
typedef target::waiting timing;
namespace std { ostream cout; };

int main(){	        
   std::cout.connect< target::uart<> >();
   TRACE << "Hello world";
   timing::init();
   TRACE << "timing initialized";
}

