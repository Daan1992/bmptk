#include "targets/lpc1114fn28.hpp"
typedef hwcpp::lpc1114fn28< 48 * hwcpp::MHz > target;
typedef target::timing timing;

typedef hwcpp::i2c_bus_master_bb_scl_sda<
   target::gpio_0_4,
   target::gpio_0_5,
   timing::kHz< 100 >
> i2c;
   
typedef hwcpp::mcp23017< i2c, 0x07 > mcp;

typedef hwcpp::port_out_from_pins<
   mcp::gpb0,
   mcp::gpb7,
   mcp::gpb1
> leds;
   
int main(){	        
   std::cout.connect< target::uart<> >();
   std::cout << "mcp23017 kitt on 3 LEDs\n";
      hwcpp::blinking< mcp::portb::gp0 , timing >::blink();  
   hwcpp::kitt< leds, timing >();    
}

