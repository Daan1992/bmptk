#include "targets/lpc1114fn28.hpp"
typedef hwcpp::lpc1114fn28< 48 * hwcpp::MHz > target;
typedef target::timing timing;

typedef hwcpp::i2c_bus_master_bb_scl_sda<
   target::gpio_0_4,
   target::gpio_0_5,
   timing::kHz< 100 >
> i2c;
   
typedef hwcpp::pcf8591< i2c > chip;

constexpr float sin( float x ){
   return x - ( x*x*x / 6 ) + ( x*x*x*x*x / 120 ) - ( x*x*x*x*x*x*x / 5040 );
}

constexpr unsigned char sinus( int x ){
   return (unsigned char ) 128 * ( 1 + sin( ( ( x - 50 ) * 2 * 3.14 ) / 100.0 ));
}

#define SINUS( n ) \
   sinus( n + 0 ), sinus( n + 1 ), \
   sinus( n + 2 ), sinus( n + 3 ), \
   sinus( n + 4 ), sinus( n + 5 ), \
   sinus( n + 6 ), sinus( n + 7 ), \
   sinus( n + 8 ), sinus( n + 9 )
   
constexpr unsigned char list[ 100 ] = {
   SINUS(  0 ), SINUS( 10 ), SINUS( 20 ), SINUS( 30 ), SINUS( 40 ),
   SINUS( 50 ), SINUS( 60 ), SINUS( 70 ), SINUS( 80 ), SINUS( 90 )
};
   
int main(){	        
   std::cout.connect< target::uart<> >();
   std::cout << "write a 100-step sine to the d/a output of a PCF8591\n";
   
   timing::init();
   chip::aout::da_init();
   
   for(;;){     
      for( int i = 0; i < 100; i++ ){
         // timing::ms< 1 >::wait();
         chip::aout::da_set( list[ i ] );
      }   
   }   
}

