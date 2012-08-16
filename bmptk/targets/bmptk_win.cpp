#include "bmptk.h"

// not a user level include file, hence not included by bmptk.h
#include "bmptk_win_graphics.h"

using namespace bmptk;

#ifndef XSIZE
   #define XSIZE 256
#endif
#ifndef YSIZE
   #define YSIZE 192
#endif
#ifndef SCALE
   #define SCALE 4
#endif   

target_screen :: target_screen( void ):
   frame( vector( XSIZE, YSIZE ))
{
   initwindow( SCALE * XSIZE - 1, SCALE * YSIZE - 1, "wframe" );
}

void target_screen :: checked_write( 
   const vector v,
   const color c 
){
   int z = COLOR( c.red_get(), c.green_get(), c.blue_get() );
   for( int x = 0; x < SCALE; x++ ){
      for( int y = 0; y < SCALE; y++ ){
         putpixel ( SCALE * v.x_get() + x, SCALE * v.y_get() + y, z );
      }
   }      
}
