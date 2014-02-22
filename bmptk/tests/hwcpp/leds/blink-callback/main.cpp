#include "targets/lpc1114fn28.hpp"
typedef hwcpp::lpc1114fn28< 48 * hwcpp::MHz > target;
typedef target::callback timing;
namespace std { ostream cout; };

typedef hwcpp::all<
   target::gpio_0_4,
   hwcpp::invert< target::gpio_0_5 >,
   target::gpio_0_6,
   hwcpp::invert< target::gpio_0_7 >,
   target::gpio_1_0, 
   hwcpp::invert< target::gpio_1_1 >, 
   target::gpio_1_2, 
   hwcpp::invert< target::gpio_1_3 >,
   target::gpio_1_4,
   hwcpp::invert< target::gpio_1_5 >,
   target::gpio_1_8,
   hwcpp::invert< target::gpio_1_9 > 
> leds;

struct my_timer : public timing::timer<> { 
   void function() override {
      TRACE << "========= timer callback called";  
       start( timing::duration::ms( 1000 ));
   }
};
   
struct my_clock : public timing::clock<> { 
   my_clock( const timing::duration x ): timing::clock<>( x ){}
   void function() override {
      TRACE << "============== clock callback called";  
   }
};
   
int main(){	      
   std::cout.connect< target::uart<> >();
   TRACE << "Callback test";
   timing::init();
   TRACE << "timing initialized"; 
	
   my_timer t1;
   my_clock t2( timing::duration::ms( 2200 ));
      
   t1.cancel();
   t1.start( timing::duration::ms( 1000 ));    
   for(;;){ 
      timing::wait( timing::duration::ms( 500 ));
   }
   
}

// extern "C" void __cxa_pure_virtual() { while (1); }

