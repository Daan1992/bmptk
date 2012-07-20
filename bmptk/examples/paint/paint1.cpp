#include "bmptk.h"

#include <nds.h>
#include <nds/touch.h>

using namespace bmptk;
using namespace bmptk::graphics;

int main( void ){
   struct touchPosition position;
   nintendo_screen lcd;
   lcd.clear( color::red );
   for(;;){
      scanKeys();
      int held = keysHeld();
      if( held & KEY_TOUCH ){
         touchRead( & position );
         lcd.write( vector( position.px, position.py ), color::black );
      }
      swiWaitForVBlank();
   }
   return 0;
}

