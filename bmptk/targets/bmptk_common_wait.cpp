#include "bmptk.h"

void bmptk::wait_busy_until( bmptk::time t ){
   while( bmptk::current_time() < t){}
}   

void bmptk::wait_until( time t ){
   wait_busy_until( t );
}

void bmptk::wait_busy( time t ){
   wait_busy_until( bmptk::current_time() + t );
}

void bmptk::wait( time t ){
   bmptk::wait_until( bmptk::current_time() + t );
}
