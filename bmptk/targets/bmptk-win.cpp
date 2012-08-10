#include "bmptk.h"

#include "bmptk-win-graphics.h"

using namespace bmptk;
using namespace bmptk::graphics;

target_screen :: target_screen( void ):
   frame( vector( 255, 191 ))
{
   initwindow( 255, 191, "wframe");
}

void target_screen :: checked_write( 
   const vector v,
   const color c 
){
   int z = COLOR( c.red_get(), c.green_get(), c.blue_get() );
   putpixel ( v.x_get(), v.y_get(), z );
}
