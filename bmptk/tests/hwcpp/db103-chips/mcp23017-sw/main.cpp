#include "targets/lpc1114fn28.hpp"
typedef hwcpp::lpc1114fn28< 48 * hwcpp::MHz > target;
typedef target::timing timing;

typedef hwcpp::i2c_bus_master_bb_scl_sda<
   target::gpio_0_4,
   target::gpio_0_5,
   timing::kHz< 100 >
> i2c;
   
typedef hwcpp::mcp23017< i2c, 0x07 > mcp;
  
int main(){	        
   std::cout.connect< target::uart<> >();
   std::cout << "mcp23017 log level on port A\n";
   int old = 0x01;
   timing::init();
   mcp::porta::init();
   mcp::porta::direction_set_input();
   mcp::porta::pullup_enable();
   for(;;){
      int current = mcp::porta::get();
      if( current != old ){
         old = current;
         std::cout 
            << std::setfill( '0' ) << std::setw( 8 ) << std::bin 
            << old << "\n";
      }
   }   
}

