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

#define rx0 10
#define ry0 10
#define rx1 200
#define ry1 150

int position_is_in_rectangle( int x, int y, int x0, int y0, int x1, int y1 ){
   // std::cout << x << ":" << y << " " << x0 << ":" << y0 << " " << x1 << ":" << y1 << "\n";
   return( x > x0 && x < x1 && y > y0 && y < y1 );
}

int main( void ){
   target_screen lcd;
   lcd.clear( color::red );
   lcd.draw( vector( rx0, ry0 ), rectangle( vector( rx1 - rx0, ry1 - ry0 )));
   for(;;){
      vector v = touched();
      if( v.x_get() >= 0 ){
         if( position_is_in_rectangle( 
            v.x_get(), v.y_get(), rx0, ry0, rx1, ry1 
         )){
            lcd.write( v, color::blue );
         }
      } 
   }
   return 0;
}

