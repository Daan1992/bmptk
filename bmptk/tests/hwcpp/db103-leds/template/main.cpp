#include "targets/lpc1114fn28.hpp"
typedef hwcpp::lpc1114fn28< 12 * hwcpp::MHz > target;
typedef target::callback timing;
namespace std { ostream cout; };

typedef hwcpp::all <
   target::gpio_0_4,
   target::gpio_0_5,
   target::gpio_0_6,
   target::gpio_0_7
> a1;

typedef hwcpp::all <
   target::gpio_0_4,
   target::gpio_0_5,
   target::gpio_0_6,
   target::gpio_0_7
> a2;


typedef hwcpp::all < 
   target::gpio_1_0,
   target::gpio_1_1,
   target::gpio_1_2,
   target::gpio_1_3
> a3;

typedef hwcpp::all< a1, a2, a3 > aa;

typedef hwcpp::port_out_from_pins <
   target::gpio_0_4,
   target::gpio_0_5,
   target::gpio_0_6,
   target::gpio_0_7
> p1;

typedef hwcpp::port_out_from_pins < 
   target::gpio_1_0,
   target::gpio_1_1,
   target::gpio_1_2,
   target::gpio_1_3
> p2;

typedef hwcpp::port_out_from_pins <
   target::gpio_1_4,
   target::gpio_1_5,
   target::gpio_1_6,
   target::gpio_1_7
> p3;

typedef hwcpp::port_out_from_pins< 
   target::gpio_0_4,
   target::gpio_0_5,
   target::gpio_0_6,
   target::gpio_0_7,
   target::gpio_1_0, 
   target::gpio_1_1, 
   target::gpio_1_2, 
   target::gpio_1_3,
   target::gpio_1_4,
   target::gpio_1_5,
   target::gpio_1_8,
   target::gpio_1_9 
> pa;

int main(){	        
   std::cout.connect< target::uart<> >();
   TRACE << "Hello world";
   timing::init();
   TRACE;
    
   // basic blink-a-led 
   // hwcpp::blinking< p3::pin< 0 >, timing >::blink();  
   
   // basic kitt
   hwcpp::kitt< pa, timing >();
}

