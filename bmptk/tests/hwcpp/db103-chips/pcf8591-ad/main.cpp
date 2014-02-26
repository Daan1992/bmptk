#include "targets/lpc1114fn28.hpp"
typedef hwcpp::lpc1114fn28< 48 * hwcpp::MHz > target;
typedef target::timing timing;

typedef hwcpp::i2c_bus_master_bb_scl_sda<
   target::gpio_0_4,
   target::gpio_0_5,
   timing::kHz< 100 >
> i2c;
   
typedef hwcpp::pcf8591< i2c > chip;
   
int main(){	        
   std::cout.connect< target::uart<> >();
   std::cout << "read the 4 A/D channels of a PCF8591\n";
   
   timing::init();
   
   chip::ain0::ad_init();
   chip::ain1::ad_init();
   chip::ain2::ad_init();
   chip::ain3::ad_init();
   
   for(;;){     
      timing::ms< 500 >::wait();
      std::cout << std::dec << std::setw( 3 )
         << chip::ain0::ad_get() << "  "
         << chip::ain1::ad_get() << "  "
         << chip::ain2::ad_get() << "  "
         << chip::ain3::ad_get() << "\n";
   }   
}

