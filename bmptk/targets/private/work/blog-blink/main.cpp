// =======================================================================
//
// featherweight multitasking demo
//
// =======================================================================
   
#ifndef BMPTK_TARGET_db103
   #error "this is a DB103 project"
#endif   

#include "targets/lpc1114fn28.hpp"
#include "switch_from_to.hpp"
#include <initializer_list>
#include <stdlib.h>

typedef hwcpp::lpc1114fn28< 12 * hwcpp::MHz > target;
typedef target::timing timing;

typedef long long int time;
const time us = timing::duration::ticks_per_us;
const time ms = 1000 * us;
const time s  = 1000 * ms;

time now(){ return timing::now().raw(); }

void* operator new(size_t size){ return malloc( size ); }

#define trace HWCPP_TRACE

void wait( time t ){
   time wakeup = now() + t;
   while( now() < wakeup ){}
} 

using led = hwcpp::pin_out_from< target::gpio_1_4 >;

int main(){	          
   timing::init();
   led::init();
   for(;;){
      led::set( 1 );
      wait( 200 * ms );
      led::set( 0 );   
      wait( 200 * ms );
   }
}
