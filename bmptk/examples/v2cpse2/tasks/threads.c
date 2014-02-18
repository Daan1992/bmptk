#include "timer.h"
#include "threads.h"

thread * first_thread = 0;

void thread_init( thread *t, void f( void )){
   t->wakeup_time = 0;
   t->next = first_thread;
   first_thread = t;

   t->stack[ STACK_SIZE - 1 ] = (int) f;
   t->sp = (int) & t->stack[ STACK_SIZE - 10 ];
}

int main_sp;
thread * current_thread = ( thread *) & main_sp;

void switch_to( thread * t );

thread * find_thread(){
   thread * t;
   for(;;){
      for( t = first_thread; t != 0; t = t->next ){
         if( current_time() >= t->wakeup_time ){
            return t;
         }
      }
   }
}

void wait( int d ){
   current_thread->wakeup_time = current_time() + d;
   switch_to( find_thread() );
}

void rtos(){
   switch_to( first_thread );
}
