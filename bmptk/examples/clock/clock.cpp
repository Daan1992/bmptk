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
   line( f, centre, radial( m, radius - 10 ), color::black(), 2 ).draw();
   line( f, centre, radial( h, radius / 2 ), color::black(), 4 ).draw();
   circle( f, centre, 3, color::black(), color::black()).draw();
   f.flush();
}

void draw_digital_clock( frame &f, unsigned long long int t ){
   t = t / bmptk::s;
   int s = t % 60;
   t = t / 60;
   int m = t % 60;
   t = t / 60;
   int h = t % 12;   
   
   std::ostringstream tt;
   tt 
      << dec << setw(2) << setfill('0') << h    
      << ":" << dec << setw(2) << m 
      << ":" << dec << setw(2) << s;
   std::string strx =  tt.str();
   const char* ct = strx.c_str();      
   
   f.clear( color::red() );
   text( ct ).draw( f );   
   f.flush();
}

class fat : public frame {
public:
   frame &f;
   fat( frame &fr ): frame( fr.size_get()), f( fr ){}
   void checked_write( const vector p, const color c ){
      for( int dx = -1; dx < 2; dx++ ){
	     for( int dy = -1; dy < 2; dy++ ){
	        f.write( p + vector( dx, dy ), c );
		 }
      } 
   }
   void clear( const color c = color::white() ){
       f.clear( c ); 
   }
};

class shadow : public frame {
public:
   frame &f;
   color bg;
   shadow( frame &fr ): frame( fr.size_get()), f( fr ){}
   void checked_write( const vector p, const color c ){
      f.write( p, c );
      f.write( p + vector( 2,2 ), c.mixed_with( bg ) );
   }
   void clear( const color c = color::white() ){
     bg = c;
     f.clear( c ); 
   }
};

int main( void ){

   target_screen lcd;
   subframe a1( lcd, vector( 10, 10 ), vector( 100, 100 ) );
   subframe a2( lcd, vector( 120, 10 ), vector( 100, 100 ) );
   subframe d( lcd, vector( 11, 120 ), vector( 38, 11 ) );
   frame_buffer a1_buffered( a1 );
   shadow f2( a2 );
   frame_tee a( a1_buffered, f2 );
   for(;;){
      unsigned long long int t = time_since_startup();
      draw_analog_clock( a, t );
      draw_digital_clock( d,t );
	  wait( 1000 * ms );
   }
   
   return 0;
}
