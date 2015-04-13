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

#define trace HWCPP_TRACE


// =======================================================================
//
// task base class
//    - stack initialization
//    - wait() call for use in user's main
//
// =======================================================================


void trampoline();

struct task {
   int ss;
   int * stack;
   int * task_sp;
   time wakeup = 0;
   const int marker = 0; // 0xDEADBEEF;
   static int * main_sp;
   
   task( int ss ): ss{ ss }{
      stack = (int*) malloc( 4 * ss );
      if( marker != 0 ){
         for( int i = 0; i < ss; ++i ){
            stack[ i ] = marker;
         }
      }
      stack[ ss - 1 ] = (int) trampoline;
      task_sp = & stack[ ss - 10 ];
   }
   
   virtual void main() = 0;
       
   void wait( time t ){
      wakeup = now() + t;
      switch_from_to( & task_sp, main_sp );
   }
   
   int stack_used(){
      for( int i = 0; i < ss; ++i ){
         if( stack[ i ] != marker ){
            return ss - i;
         }
      }
      return 0;
   }
   
   void info(){
      hwcpp::io::cout 
         << "stack usage " << stack_used() << " of " << ss << "\n";
   }
};

int * task::main_sp;

task *trampoline_target_task;
void trampoline(){
   trampoline_target_task->main();
}


// =======================================================================
//
// scheduler::run
//    first initialize the tasks
//    then keep scheduling the tasks
//
// scheduler::schedule
//    if it is runnable: switch to the first task
//       (it will switch back inside wait())
//    then schedule the remaining tasks
//    
//
// =======================================================================

void schedule( std::initializer_list< task * > list ){
      int n = 0;
      for(;;){
         if(0) if( ++n % 10000 == 0 ){
            //info();
         }         
         for( task * t : list ){
            if( now() >= t->wakeup ){         
               trampoline_target_task = t;
               switch_from_to( & task::main_sp, t->task_sp );         
               break;
            }
         }      
      }               
   }

// =======================================================================
//
// user code
//
// =======================================================================

template< typename led_, time t >   
struct blink : public task {

   blink( int size ): task( size ){}
   
   using led = hwcpp::pin_out_from< led_ >;
   
   void main() override {
      led::init();
      for(;;){
         led::set( 1 );
         wait( t );
         led::set( 0 );   
         wait( t );
      }
   }
};



int main(){	        
   //hwcpp::io::cout.connect< target::uart<> >();
   
   blink< target::gpio_1_4, 200 * ms > t1 ( 20 );
   blink< target::gpio_1_5, 300 * ms > t2 ( 20 );
   timing::init();
 
   // 15 OK, 14 does not work??
   schedule( { &t1, &t2 } );
   
   //hwcpp::io::cout << "\n** application ended **\n";
}
