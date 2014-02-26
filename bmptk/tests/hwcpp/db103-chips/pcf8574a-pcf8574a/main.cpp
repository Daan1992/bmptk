#include "targets/lpc1114fn28.hpp"
typedef hwcpp::lpc1114fn28< 48 * hwcpp::MHz > target;
typedef target::timing timing;

typedef hwcpp::pcf8574a<
   hwcpp::i2c_bus_master_bb_scl_sda<
      target::scl,
      target::sda,
      timing::kHz< 100 >
   >   
> chip1;

typedef hwcpp::pcf8574a<
   hwcpp::i2c_bus_master_bb_scl_sda<
      chip1::p6,
      chip1::p7,
      timing::kHz< 100 >
   >   
> chip2;  

int main(){	        
   std::cout.connect< target::uart<> >();
   std::cout << "blink on a LED on a 2nd-level PCF8574A";

   hwcpp::blinking< 
      chip2::p4,
      timing 
   >::blink();	
}

