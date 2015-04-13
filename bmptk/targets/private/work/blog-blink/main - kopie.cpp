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

typedef hwcpp::lpc1114fn28< 48 * hwcpp::MHz > target;
typedef target::timing timing;

typedef long long int time;
const time us = timing::duration::ticks_per_us;
const time ms = 1000 * us;
const time s  = 1000 * ms;

time now(){ return timing::now().raw(); }
#define trace HWCPP_TRACE


// =======================================================================
//
// task 'base class': 
//    - stack initialization
//    - wait() call for use in user's main
//
// =======================================================================

int * main_sp;

template< int n >
struct task {
   static int stack[ n ];
   static int * task_sp;
   static time wakeup;
   
   static void init( void main() ){
      stack[ n - 1 ] = (int) main ;
      task_sp = & stack[ n - 10 ];
   }
       
   static void wait( time t ){
      wakeup = now() + t;
      switch_from_to( & task_sp, main_sp );
   }
};

template< int n > int   task< n >::stack[ n ];
template< int n > int * task< n >::task_sp;
template< int n > time  task< n >::wakeup = 0;


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

template< typename... tasks >
struct scheduler{
   static_assert( 
      sizeof...( tasks ) == 0, 
      "scheduler<> requires task arguments"
   );
};

template<> 
struct scheduler<>{
   static void init(){}
   static void schedule(){};
};

template< typename first_task, typename... more_tasks >
struct scheduler< first_task, more_tasks... >{

   static void init(){
      first_task::init( first_task::main );
      scheduler< more_tasks... >::init();
   };
   
   static void schedule(){
      if( now() >= first_task::wakeup ){
trace;         
         switch_from_to( & main_sp, first_task::task_sp );
trace;         
      }
trace;      
      scheduler< more_tasks... >::schedule();
   };      
   
   static void run(){
      init();
      for(;;){
trace;
         schedule();
trace; 
      }          
   }
};


// =======================================================================
//
// user code
//
// =======================================================================

template< typename led_, int n, time t >   
struct blink : public task< n > {
   using led = hwcpp::pin_out_from< led_ >;
   
   static void main(){
      led::init();
      for(;;){
         led::set( 1 );
         blink::wait( t );
         led::set( 0 );   
         blink::wait( t );
      }
   }
};

int main(){	        
   hwcpp::io::cout.connect< target::uart<> >();
   
   timing::init();
 
   scheduler< 
      blink< target::gpio_1_4, 100, 200 * ms >,
      blink< target::gpio_1_5, 101, 300 * ms >
   >::run();
   
   hwcpp::io::cout << "\n** application ended **\n";
}
