#include "targets/lpc1114fn28.hpp"
typedef hwcpp::lpc1114fn28< 48 * hwcpp::MHz > target;
typedef target::timing timing;

typedef hwcpp::i2c_bus_master_bb_scl_sda<
   target::scl,
   target::sda,
   timing::kHz< 100 >
> i2c;

typedef hwcpp::pcf8574a<
   i2c
> pcf8574a;   

int main(){	        
   std::cout.connect< target::uart<> >();
   std::cout << "kitt on 8 LEDs (active-low) on a PCF8574A";

   hwcpp::kitt< hwcpp::invert< pcf8574a >, timing >();
}

