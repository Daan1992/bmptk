#include "bmptk.h"
#include "graphics.h"

#include <nds.h>
#include <nds/touch.h>

using namespace bmptk;

class nintendo_screen : public frame {
public:
   nintendo_screen( void );
   void checked_write( const vector v, const color c );
   color checked_read( const vector v ) const;
};

nintendo_screen :: nintendo_screen( void ):
   frame( 255, 191 )
{

if(0){
   // set frame buffer mode 0
   videoSetMode( MODE_FB0 );

   // lcdSwap();
   lcdMainOnTop();

   // also: MODE_5_2D ??

   // enable VRAM A for writing by the cpu and use
   // as a framebuffer by video hardware
   vramSetBankA( VRAM_A_LCD );
} else {
   // set frame buffer mode 0
   videoSetMode( MODE_FB0 );

   // lcdSwap();
   lcdMainOnBottom();

   // also: MODE_5_2D ??

   // enable VRAM A for writing by the cpu and use
   // as a framebuffer by video hardware
   vramSetBankA( VRAM_A_LCD );
  } 
}

void nintendo_screen :: checked_write( 
   const vector v,
   const color c 
){
   VRAM_A[ v.x + ( v.y * 256 ) ] = c.rgb15();
}

color nintendo_screen :: checked_read( const vector v ) const {
   return color :: black;
}

int main( void ){
   struct touchPosition position;
   nintendo_screen lcd;
   lcd.clear( color::red );
   for(;;){
      scanKeys();
      int held = keysHeld();
      if( held & KEY_TOUCH ){
         touchRead( & position );
         lcd.write( position.px, position.py, color::black );
      }
      swiWaitForVBlank();
   }
   return 0;
}