#include "bmptk.h"

using namespace bmptk;
using namespace graphics;

#ifdef TARGET_nds
   #include <nds.h>
   #include <nds/touch.h>
   vector touched( void ){
      struct touchPosition position;
      swiWaitForVBlank();
      scanKeys();
      int held = keysHeld();   
      if( held & KEY_TOUCH ){
         touchRead( & position );
         return vector( position.px, position.py );
      } else {
         return vector( -1, -1 );
      }      
    }   
#endif

#ifdef TARGET_win
   #include "bmptk-win-graphics.h"
   vector touched( void ){
      int x, y;  
      static bool down = 0;
      getmouseclick( WM_LBUTTONDOWN, x, y );
      if( x != -1 ){
         down = 1;   
         return vector( x, y );
      }   
      getmouseclick( WM_LBUTTONUP, x, y );
      if( x != -1 ){
         down = 0;   
         return vector( -1, -1 );
      }   
      if( down ){
         getmouseclick( WM_MOUSEMOVE, x, y );
         if( x != -1 ){  
         return vector( x, y );
      }   
   }
   return vector( -1, -1 );
}
#endif   

int main( void ){

   target_screen lcd;
   lcd.clear( color::red() );
   for(;;){
      vector v = touched();
      if( v.x_get() >= 0 ){
         lcd.write( v, color::black() );
      }  
   }
   return 0;
}

