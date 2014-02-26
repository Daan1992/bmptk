#include "targets/lpc1114fn28.hpp"
typedef hwcpp::lpc1114fn28< 48 * hwcpp::MHz > target;
typedef target::timing timing;

typedef hwcpp::i2c_bus_master_bb_scl_sda<
   target::gpio_0_4,
   target::gpio_0_5,
   timing::kHz< 100 >
> i2c;
   
typedef hwcpp::mcp23008< i2c, 0x07 > mcp8;

typedef hwcpp::port_out_from_pins<
   mcp8::gp0,
   mcp8::gp1,
   mcp8::gp4,
   mcp8::gp7
> leds;
   
int main(){	        
   std::cout.connect< target::uart<> >();
   std::cout << "mcp23008 kitt on 4 LEDs\n";
   
   // hwcpp::blinking< mcp8::gp1, timing >::blink();
   hwcpp::kitt< leds, timing >();    
}

