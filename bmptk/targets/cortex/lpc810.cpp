#include "bmptk.h"
#include "Lpc8XX.h"

   // return the number of microseconds since startup 
   // (or rather, first call of this function) 
   // using the SysTick timer, running from the 12 MHz clock
bmptk::time bmptk::time_since_startup(){
   static unsigned long long int last_low = 0;
   static unsigned long long int high = 0;
   
   static bool init_done = false;
   if( ! init_done ){
      init_done = true;
	   
      SysTick->CTRL  = 0;         // stop the timer
      SysTick->LOAD  = 0xFFFFFF;  // use its as a 24-bit timer
      SysTick->VAL   = 0;         // clear the timer
      SysTick->CTRL  = 1;         // start the timer
   }  

   // the timer ticks down, but we want an up counter
   unsigned int low = 0xFFFFFF - ( SysTick->VAL & 0xFFFFFF );
   if( low < last_low ){
   
      // the timer rolled over, so increment the high part
      high += 0x1ULL << 24;
   }
   last_low = low;

   // return the aggregated ticks value
   return time( low | high );
}


// called when a pure virtual method is called
extern "C" void __cxa_pure_virtual( void ){
   while(1){}	
}

extern "C"
void memcpy( unsigned char *s1, unsigned char *s2, unsigned int n){
   while( n-- ){
      *s1++ = *s2++;
   }   
}
