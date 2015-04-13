// =======================================================================
//
// featherweight multitasking demo
//
// =======================================================================
   
#ifndef BMPTK_TARGET_db103
   #error "this is a DB103 project"
#endif   

#include "targets/lpc1114fn28.hpp"

int operator "" _ms( unsigned long long int x ){ return x; }

class time {
   /*
   namespace dummy {
   static int operator "" _ms( unsigned long long int x ){ return x; }
   };
   */
};

int main(){	          
   auto x = 10_ms;
   (void)x;
}
