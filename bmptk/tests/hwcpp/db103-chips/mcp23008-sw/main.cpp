#include "targets/lpc1114fn28.hpp"
typedef hwcpp::lpc1114fn28< 48 * hwcpp::MHz > target;
typedef target::timing timing;

typedef hwcpp::i2c_bus_master_bb_scl_sda<
   target::gpio_0_4,
   target::gpio_0_5,
   timing::kHz< 100 >
> i2c;
   
typedef hwcpp::mcp23008< i2c, 0x07 > mcp8;

template< class pin > struct use_pullups {

   static void init(){ 
      pin::init(); 
      pin::direction_set_input();  
   }
   
   static void set( bool x ){
      if( x ){
         pin::pullup_enable();
      } else {
         pin::pullup_disable();
      }
   }
};

typedef hwcpp::pin_in_from< mcp8::gp2 > in1;
typedef hwcpp::pin_in_from< mcp8::gp3 > in2;
typedef use_pullups< mcp8::gp0 > out1;
typedef use_pullups< mcp8::gp4 > out2;
   
int main(){	        
   std::cout.connect< target::uart<> >();
   std::cout << "mcp23008 copy switches to LEDs - using the weak pullups\n";
   std::cout << "(LEDs light up VERY VERY dim)\n";
   
   timing::init();
   
   in1::init();
   in2::init();
   out1::init();
   out2::init();

   mcp8::gp2::pullup_enable();
   mcp8::gp3::pullup_enable();
   
   for(;;){
      out1::set( in1::get() );
      out2::set( in2::get() );
   }      
}

