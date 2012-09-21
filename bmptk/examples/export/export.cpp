// #include <stdlib.h>
#include "bmptk.h"
#include <string>
#include <iostream> 
#include <sstream> 
#include <iomanip>
#include <math.h>

using namespace bmptk;
using namespace std;

vector radial( int n, int len ){
   return vector( 
      (int) ( len * sin( n * 6.28 / 60 )),
      (int) ( len * - cos( n * 6.28 / 60 ))      
   );   
}

void draw_analog_clock( frame &f, unsigned long long int t ){
   t = t / bmptk::s;
   int s = t % 60;
   t = t / 60;
   int m = t % 60;
   t = t / 60;
   int h = t % 12;   
   
   vector centre = f.size_get() / 2;
   int radius = f.size_get().x_get() / 2 - 2;
   
   f.clear( color::white() );
   circle( f, centre, radius, color::black(), color::transparent()).draw();
   line( f, centre, radial( s, radius - 3 )).draw();
   line( f, centre, radial( m, radius * 2 / 3 ), color::black(), 2 ).draw();
   line( f, centre, radial( 5 * h, radius / 2 ), color::black(), 4 ).draw();
   circle( f, centre, 3, color::black(), color::black()).draw();
}



int main( void ){

   target_screen lcd;
   lcd.clear( color:: gray() );
   subframe sub( lcd, vector( 10, 10 ), vector( 100, 100 ) ); 
   frame_buffer buf( sub );
   draw_analog_clock( 
      buf,  
      bmptk::s * ( 5 + ( 60ULL ) * 10 + ( 60 * 60ULL ) * 15 )
   );
   buf.write_to_bmp_file( "clock.bmp" );
   buf.flush();
   wait( 3 * s );
   for(;;);
   return 0;
}

