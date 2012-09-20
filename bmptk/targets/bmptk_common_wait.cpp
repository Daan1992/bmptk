#include "bmptk.h"

void bmptk::wait_busy_until( unsigned long long int t ){
   while( bmptk::time_since_startup() < t){}
}   

void bmptk::wait_until( unsigned long long int t ){
   wait_busy_until( t );
}

void bmptk::wait_busy( unsigned long long int t ){
   wait_busy_until( bmptk::time_since_startup() + t );
}

void bmptk::wait( unsigned long long int t ){
   bmptk::wait_until( bmptk::time_since_startup() + t );
}
