#include "bmptk.h"

#include <nds.h>
#include <nds/touch.h>

using namespace bmptk;
using namespace graphics;

#define rx0 10
#define ry0 10
#define rx1 200
#define ry1 150

int position_is_in_rectangle( int x, int y, int x0, int y0, int x1, int y1 ){
   return( x > x0 && x < x1 && y > y0 && y < y1 );
}

int main( void ){
   struct touchPosition position;
   target_screen lcd;
   lcd.clear( color::red );
   lcd.draw( vector( rx0, ry0 ), rectangle( vector( rx1, ry1 )));
   for(;;){
      scanKeys();
      int held = keysHeld();
      if( held & KEY_TOUCH ){
         touchRead( & position );
         if( position_is_in_rectangle( 
            position.px, position.py, rx0, ry0, rx1, ry1 
         )){
            lcd.write( vector( position.px, position.py ), color::blue );
         }
      }
      swiWaitForVBlank();
   }
   return 0;
}

