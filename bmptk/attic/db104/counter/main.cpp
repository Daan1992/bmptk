// DB104 counter firmware
//
// Counts on the seven-segment display.
//
// External connections functions:
//    RST  : reset
//    IN1  : count
//    IN2  : _up/down
//    IN3  : _pulsed/continuous (10Hz)
//    OUT1 : carry
//    OUT2 : _zero
//    OUT3 : not used
//
// With a suitable pull-up (1k will do) the carry output
// can be cascaded to the next counter's count input.
//
// The external connections are buffered by an ULN2003A,
// hence the corresponding LPC1114 pins are inverted!

#include "lpc1114fn28.hpp"
namespace std { ostream cout; };

namespace hwcpp {

   template< unsigned int frequency >
   struct db104 :
      public lpc1114fn28< frequency > 
   {
      typedef lpc1114fn28< frequency > chip;
      typedef typename chip::callback timing;

      typedef port_out_from_pins<
         typename chip::gpio_0_2,
         typename chip::gpio_0_3,
         typename chip::gpio_0_4,
         typename chip::gpio_0_5,
         typename chip::gpio_0_6, 
         typename chip::gpio_0_7, 
         typename chip::gpio_0_8, 
         typename chip::gpio_0_9 
      > segments_port;
   
      typedef port_out_from_pins<
         typename chip::gpio_1_9,
         typename chip::gpio_1_8,
         typename chip::gpio_1_5,
         typename chip::gpio_1_4
      > digits_port;
   
      typedef seven_segment_display<
         timing,
         invert< segments_port >,
         digits_port
      > display;   
   
      typedef pin_in_from< typename chip::gpio_1_0 > pin_count;
      typedef pin_in_from< typename chip::gpio_1_1 > pin_pulsed;
      typedef pin_in_from< typename chip::gpio_1_2 > pin_up;
      typedef pin_out_from< typename chip::gpio_0_10 > pin_carry;
      typedef pin_out_from< typename chip::gpio_0_11 > pin_zero; 	
      
      static void init(){
      
         pin_count::init();
         pin_pulsed::init();
         pin_up::init();
         pin_carry::init();
         pin_zero::init();
         
         pin_carry::set( 1 );
         
         display::init();
         timing::init();      
      }
      
   };
};   

typedef hwcpp::db104< 48 * hwcpp::MHz > target;
   	
template< class carry_pin >
static void count( int &n, bool direction ){
   if( direction ){
      n++;
      if( n > 9999 ){
         n = 0;
         carry_pin::set( 0 );
      } else {
         carry_pin::set( 1 );
      }
   } else {
      n--;
      if( n < 0 ){
         n = 9999;
         carry_pin::set( 0 );
      } else {
         carry_pin::set( 1 );
      }
   }  
}  
   
int main( void ){	
   std::cout.connect< target::uart<> >();
   TRACE << "DB104 counter application\n";
   target::init();
      
   int current_value = 0;
   const target::timing::duration interval = target::timing::duration::ms( 100 );
   bool next, last = 0;
   target::timing::moment t = target::timing::now();
      
   for(;;){
      target::pin_zero::set( current_value == 0 ); 
      target::display::set_decimal_value( current_value ); 
        
      for( int i = 0; i < 50; i++ ){  
         target::display::mux();
         target::timing::ms< 1 >::wait(); 
      }   
      
      //current_value++;
             
      if ( false ){ // if( target::pin_pulsed::get() ){
      
         // pulsed counting
         next = target::pin_count::get();
         if( ! next && ( next != last )){
            count< target::pin_carry >( 
               current_value, target::pin_up::get() );  
         }
         last = next;
         
      } else {
      
         // auto_counting 
         if( t < target::timing::now() ){
            t += interval;
            { // if( ! target::pin_pulsed::get() ){
               if( ! target::pin_count::get() ){
                  count< target::pin_carry >( 
                     current_value, target::pin_up::get() );                                                       
               } 
            }
         } 
      
      }
   }
   return 0;
}

