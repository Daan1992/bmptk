// show a kitt-style display on 10 LEDs
// The pins are kept in the input state.
// A pin is made 'weakly high' by enabling its pullup.
// Note that this makes the LEDs light up only very faintly.

#include "bmptk.h"

template< typename pin > struct pin_out_by_pullup : 
   public bmptk::hardware::pin_out 
{
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

typedef bmptk::hardware::port_out_from_pins<
   pin_out_by_pullup< bmptk::target::gpio_0_6 >,
   pin_out_by_pullup< bmptk::target::gpio_0_7 >,

   pin_out_by_pullup< bmptk::target::gpio_1_0 >,
   pin_out_by_pullup< bmptk::target::gpio_1_1 >,
   pin_out_by_pullup< bmptk::target::gpio_1_2 >,
   pin_out_by_pullup< bmptk::target::gpio_1_3 >,

   pin_out_by_pullup< bmptk::target::gpio_1_4 >,
   pin_out_by_pullup< bmptk::target::gpio_1_5 >,
   pin_out_by_pullup< bmptk::target::gpio_1_8 >,
   pin_out_by_pullup< bmptk::target::gpio_1_9 >
> leds;
		
int main( void ){	
   // pul the i2c pins low to disable thier LEDs
   bmptk::target::gpio_0_4::init();
   bmptk::target::gpio_0_5::init();
   bmptk::target::gpio_0_4::set(0);
   bmptk::target::gpio_0_5::set(0);
   
   bmptk::hardware::kitt< leds >( 50 * bmptk::ms );	      
}
