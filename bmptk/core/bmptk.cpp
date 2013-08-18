#include "bmptk.h"

namespace bmptk {

void wait_busy_until( time t ){
   while( current_time() < t){}
}   

void wait_busy( time t ){
   wait_busy_until( current_time() + t );
}

void wait_until_default( time t ){
   wait_busy_until( t );
}

// the rtos installs a replacement that invokes the scheduler
void ( *wait_until_handler )( time t ) = wait_until_default;

void wait_until( time t ){
   (*wait_until_handler)( t );
}

void wait( time t ){
   wait_until( current_time() + t );
}

void ( *user_error_handler )( const char * ) = nullptr;

void fatal_error_detected( const char *msg ){
   if( user_error_handler ){
      user_error_handler( msg );
   }
   for(;;);   
}

void fatal_error_handler( void f( const char * ) ){
   user_error_handler = f;
}

}; // namespace bmptk