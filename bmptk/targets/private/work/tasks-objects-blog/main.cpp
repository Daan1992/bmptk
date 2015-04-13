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


// =======================================================================
//
// task base class
//    - stack initialization
//    - wait() call for use in user's main
//
// =======================================================================

struct stack_space {
   int * memory;
   int * sp;   
   
   stack_space( int size_in_words, void f() ){
      memory = (int *) malloc( 4 * size_in_words );
      memory[ size_in_words - 1 ] = (int) f;
      sp = & memory[ size_in_words - 10 ];
   }   
   
};

int * main_sp;

void trampoline();

struct task {
   virtual void main() = 0;
   
   stack_space stack;

   task( int stack_size_in_words ): 
      stack{ stack_size_in_words, trampoline }
   {}
   
   time wakeup = 0;
   
   void wait( time t ){
      wakeup = now() + t;
      switch_from_to( & stack.sp, main_sp );
   }   
       
};

task * trampoline_target_task;
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
   for(;;){        
      for( task * t : list ){
         if( now() >= t->wakeup ){         
            trampoline_target_task = t;
            switch_from_to( & main_sp, t->stack.sp );         
            break;
         }
      }      
   }               
}

void wait( time t ){
   trampoline_target_task->wait( t );
}   
 

// =======================================================================
//
// user code
//
// =======================================================================

template< typename led_, time t, int size >   
struct blink : public task {

   blink(): task( size ){}
   
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
   
   //hwcpp::io::cout << sizeof( task );
   
   timing::init();
 
   // 15 OK, 14 does not work??
   schedule( { 
      // new blink< target::gpio_1_4, 200 * ms, 20 >,
      new blink< target::gpio_1_5, 300 * ms, 20 >
   } );
   
   //hwcpp::io::cout << "\n** application ended **\n";
}
