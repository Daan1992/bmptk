#include "timer.h"
#include "uart-hw.h"
#include "threads.h"

void uart_write_string( const char *s ){
   while( *s != '\0' ){ 
      uart_write_char( *s++ ); 
   }   
} 

thread t1;
void f1(){
   for(;;){
      uart_write_string( "====== thread1 ======\n" );
      wait( 1000 * 1000 );
   }      
}
		
thread t2;
void f2(){
   for(;;){
      uart_write_string( "   ====== thread2 ======\n" );
      wait( 800 * 1000 );
   }      
}
		
int main( void ){	
   timer_init();
   uart_init(); 
   
   thread_init( &t1, f1 );
   thread_init( &t2, f2 );
   rtos();
   uart_write_string( "back in main" );   
   
   return 0;
}