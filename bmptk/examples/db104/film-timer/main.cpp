// DB104 film timer firmware
//
// This application counts from the initial value of 20.00 
// (20 minutes, 0 seconds) down to 0.00 at a rate of 0.01 per second.
// Counting starts when IN1 is made high.
// When 0.00 is reached OUT1 is pulled low for 0.5 second
// and the application resets to 20.00 (and the downcounting will
// resume when IN1 is made high).
// RST can be made high at any time to reset the application to 20.00 .
//
// External connections functions:
//    RST  : reset
//    IN1  : make high to start counting down
//    OUT1 : pulled low for 0.5 second when 0.00 is reached

#include "db104.hpp"
typedef hwcpp::db104<> target;
namespace std { ostream cout; };

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

typedef hwcpp::pin_in_from< target::pin_in1 >  pin_start;
typedef hwcpp::invert< hwcpp::pin_out_from< target::pin_out1 >> pin_pulse;
   
int main( void ){	
   //std::cout.connect< target::uart<> >();
   //std::count << "DB104 next film timer\n";
   target::init();
   pin_start::init();
   pin_pulse::init();
      
   constexpr int start_value = 2000;   
   constexpr auto interval = target::timing::duration::s( 1 );
   constexpr char decimal_points = 0x04;

   for(;;){
      bool counting = false;
      target::timing::moment t = target::timing::now();     
      int current_value = start_value;       
      
      target::display::set_decimal_value( current_value, decimal_points ); 
      target::display::mux();
      target::timing::ms< 1 >::wait(); 
      
      pin_pulse::set( 0 );
      counting |= pin_start::get();
      
      while( counting ){
         target::display::set_decimal_value( current_value, decimal_points ); 
        
         for( int i = 0; i < 50; i++ ){  
            target::display::mux();
            target::timing::ms< 1 >::wait(); 
         }   
      
         if( t < target::timing::now() ){
            t += interval;
            current_value--;
            if( current_value % 100 == 99 ){
               current_value = 100 * ( current_value / 100 ) + 59;
            }
         }
      
         if( current_value == 0 ){
            counting = false;
            pin_pulse::set( 1 );
            for( int i = 0; i < 500; i++ ){  
               target::display::mux();
               target::timing::ms< 1 >::wait(); 
            }          
         }
      }
            
   }
   return 0;
}

