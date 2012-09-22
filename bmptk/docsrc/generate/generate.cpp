#include "bmptk.h"

using namespace bmptk;

void border( frame &f ){
   rectangle( 
      f, 
	  vector( 0, 0 ), 
	  f.size_get() - vector::one(), 
	  color::blue(), 
	  color::yellow() 
   ).draw();
}

void coordinates( frame & lcd ){
   lcd.clear( color::white() );
   
   subframe raw( lcd, vector( 10, 10 ), vector( 100, 100 ));
   frame_snapshot sub( raw );
   sub.clear( color :: white() );
   border( sub );
   line( sub, vector( 10,  0 ), vector(   0, 70 ) ).draw();
   line( sub, vector(  0, 10 ), vector( 100,  0 ) ).draw();
   
   line( sub, vector( 100, 90 ), vector( -30, 0 ) ).draw();
   line( sub, vector( 90, 100 ), vector( 0, -100 ) ).draw();
   
   text( "(0,0)" ).draw( sub, vector( 10, 20 ));
   line( sub, vector( 20, 20 ), vector( -15, -15 ), color::blue(), 3 ).draw();
   
   text( "(0,xmax)" ).draw( sub, vector( 50, 20 ));
   line( sub, vector( 80, 20 ), vector( 15, -15 ), color::blue(), 3 ).draw(); 
   
   text( "(xmax,ymax)" ).draw( sub, vector( 35, 75 ));
   line( sub, vector( 85, 85 ), vector( 10, 10 ), color::blue(), 3 ).draw();
   
   sub.write_to_bmp_file( "lcd-pixel-coordinates.bmp" );
}

int main( void ){
   target_screen lcd;
   coordinates( lcd );
   return 0;
}
