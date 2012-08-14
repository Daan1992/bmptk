#include "bmptk.h"

using namespace bmptk;

target_screen :: target_screen( void ):
   frame( vector( 255, 191 ))
{

   // set frame buffer mode 0
   videoSetMode( MODE_FB0 );

   // lcdSwap();
   lcdMainOnBottom();

   // also: MODE_5_2D ??

   // enable VRAM A for writing by the cpu and use
   // as a framebuffer by video hardware
   vramSetBankA( VRAM_A_LCD );

}

void target_screen :: checked_write( 
   const vector v,
   const color c 
){
   VRAM_A[ v.x_get() + ( v.y_get() * 256 ) ] = c.rgb15();
}