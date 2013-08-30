#include "bmptk.h"

unsigned long long int bmptk::time_since_startup(){
   static unsigned long long int last_low = 0;
   static unsigned long long int high = 0;
   
   static bool init = true;
   if( init ){
      init = false;
	   
      //
   }
	
   unsigned int low = 0; // read_cascaded_timers();
   if( low < last_low ){
      // the counter rolled over, increment the high word
      high += 0x1ULL << 32;
   }
   last_low = low;
   
   // the timer ticks at 33.514 MHz, divide by 33.5 
   // to get a reasonable approximation of 1 Mhz
   return (( low | high ) / 67 ) * 2;
}

// ???
void *__dso_handle;
