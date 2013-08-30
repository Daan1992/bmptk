#include "bmptk.h"

#define LOW_TIMER 0
#define HIGH_TIMER (LOW_TIMER+1)

unsigned int read_cascaded_timers(){
   unsigned int high =  TIMER_DATA( HIGH_TIMER );
   unsigned int low = TIMER_DATA( LOW_TIMER );
   
   if( high != TIMER_DATA( HIGH_TIMER )){
     // The high timer incremented while we tried to read the low timer.
	  // Read the timers again, no overflow will happen again anytime soon
	  high = TIMER_DATA( HIGH_TIMER );
	  low = TIMER_DATA( LOW_TIMER );
   }	  
   
   return low + ( high << 16 );
}

bmptk::time bmptk::current_time(){
   static unsigned long long int last_low = 0;
   static unsigned long long int high = 0;
   
   static bool init = true;
   if( init ){
      init = false;
	   
      TIMER_CR( LOW_TIMER ) = 0;
      TIMER_DATA( LOW_TIMER ) = 0;
      TIMER_CR( LOW_TIMER ) = TIMER_DIV_1 | TIMER_ENABLE;
    
      TIMER_CR( HIGH_TIMER ) = 0;
      TIMER_DATA( HIGH_TIMER ) = 0;
      TIMER_CR( HIGH_TIMER ) = TIMER_CASCADE | TIMER_DIV_1 | TIMER_ENABLE;
   }
	
   unsigned int low = read_cascaded_timers();
   if( low < last_low ){
      // the counter rolled over, increment the high word
      high += 0x1ULL << 32;
   }
   last_low = low;
   
   // the timer ticks at 33.514 MHz, multiply by 2 to get 67 ticks per us
   return time( ( low | high ) * 2 );
}


/*
void bmptk::fatal_error_detected( const char *msg  ){
   bmptk::graphics::target_screen lcd;
   lcd.clear( bmptk::graphics::color::blue());
   bmptk::graphics::text( msg ).draw( lcd );
   for(;;);
}
*/

bmptk::graphics::target_screen::target_screen( void ):
   frame( vector( 256, 192 ))
{
   videoSetMode(MODE_5_2D);
   vramSetBankA(VRAM_A_MAIN_BG);
   bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);
   lcdMainOnBottom();
}

void bmptk::graphics::target_screen::checked_write( 
   const vector v,
   const color c 
){
   if( c.is_transparent() ){ return; }
   BG_GFX[ v.x_get() + ( v.y_get() * 256 ) ] = 0x8000 | c.rgb15_get();
}

bmptk::graphics::target_top_screen::target_top_screen( void ):
   frame( vector( 256, 192 ))
{    
   videoSetModeSub(MODE_5_2D);
   vramSetBankC(VRAM_C_SUB_BG);
   bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);
   lcdMainOnBottom();
}

void bmptk::graphics::target_top_screen::checked_write( 
   const vector v,
   const color c 
){
   BG_GFX_SUB[ v.x_get() + ( v.y_get() * 256 ) ] = 0x8000 | c.rgb15_get();
}

