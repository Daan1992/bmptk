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

typedef hwcpp::lpc1114fn28< 12 * hwcpp::MHz > target;
typedef target::timing timing;

typedef long long int time;
const time us = timing::duration::ticks_per_us;
const time ms = 1000 * us;
const time s  = 1000 * ms;

time now(){ 
   // return 0;
   return timing::now().raw(); 
}
//#define trace HWCPP_TRACE


// =======================================================================
//
// task 'base class': 
//    - stack initialization
//    - wait() call for use in user's main
//
// =======================================================================

template< int stack_word_size >
struct stack {
   int stack[ stack_word_size ];
   
   stack( void f() ){
      stack[ stack_word_size - 1 ] = (int) f;
      task_sp = & stack[ stack_word_size - 10 ];
   }   
};

int * main_sp;

template< int id, int ss >
struct task {
   static int stack[ ss ];
   static int * task_sp;
   static time wakeup;
   static const int marker = 0; // 0xDEADBEEF;
   
   static void init( void main() ){
      if(0) if( marker != 0 ){
         for( int i = 0; i < ss; ++i ){
            stack[ i ] = marker;
         }
      }
      stack[ ss - 1 ] = (int) main ;
      task_sp = & stack[ ss - 10 ];
   }
       
   static void wait( time t ){
      wakeup = now() + t;
      switch_from_to( & task_sp, main_sp );
   }
   
   static int stack_used(){
      for( int i = 0; i < ss; ++i ){
         if( stack[ i ] != marker ){
            return ss - i;
         }
      }
      return 0;
   }
   
   static void info(){
      hwcpp::io::cout 
         << "task " << id << " stack usage " << stack_used() << " of " << ss << "\n";
   }
};

template< int id, int ss > int   task< id, ss >::stack[ ss ];
template< int id, int ss > int * task< id, ss >::task_sp;
template< int id, int ss > time  task< id, ss >::wakeup = 0;


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
struct scheduler{};

template<> 
struct scheduler<>{
   static void init(){}
   static void schedule(){};
   static void info(){};
};

template< typename first_task, typename... more_tasks >
struct scheduler< first_task, more_tasks... >{

   static void init(){
      first_task::init( first_task::main );
      scheduler< more_tasks... >::init();
   };
   
   static void schedule(){
      if( now() >= first_task::wakeup ){         
         switch_from_to( & main_sp, first_task::task_sp );         
      } else {    
         scheduler< more_tasks... >::schedule();
      }   
   };    
   
   static void info(){
      first_task::info();
      scheduler< more_tasks... >::info();
   }  
   
   static void run(){
      int n = 0;
      init();
      for(;;){
         schedule();
         if(0) if( ++n % 10000 == 0 ){
            info();
         }
      }          
   }
};


// =======================================================================
//
// user code
//
// =======================================================================

template< int id, typename led_, int stack_size, time t >   
struct blink : public task< id, stack_size > {
   using led = hwcpp::pin_out_from< led_ >;
   
   static void __attribute__((noreturn)) main(){
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
   //hwcpp::io::cout.connect< target::uart<> >();
   
   timing::init();
 
   // 15 OK, 14 does not work??
   scheduler< 
      //blink< 1, target::gpio_1_4, 15, 200 * ms >,
      blink< 2, target::gpio_1_5, 15, 300 * ms >
   >::run();
   
   //hwcpp::io::cout << "\n** application ended **\n";
}
