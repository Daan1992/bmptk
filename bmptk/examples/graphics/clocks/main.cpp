#include "bmptk.h"
#include <stdlib.h>
#include <string>
#include <iostream> 
#include <sstream> 
#include <iomanip>
#include <math.h>

using namespace bmptk;
using namespace graphics;
using namespace std;

vector radial( int n, int len ){
   return vector( 
      (int) ( len * sin( n * 6.28 / 60 )),
      (int) ( len * - cos( n * 6.28 / 60 ))      
   );   
}

void draw_analog_clock( frame &f, bmptk::time tt ){
   unsigned long long int t = tt / bmptk::s;
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
   line( f, centre, radial( h, radius / 2 ), color::black(), 4 ).draw();
   circle( f, centre, 3, color::black(), color::black()).draw();
   f.flush();
}

void draw_digital_clock( frame &f, bmptk::time bt ){
   unsigned long long int t = bt / bmptk::s; 
   int s = t % 60;
   t = t / 60;
   int m = t % 60;
   t = t / 60;
   int h = t % 12;   
   
   std::ostringstream tt;
   tt << setfill('0')
      << dec << setw(2) << h    
      << ":" 
	  << dec << setw(2) << m 
      << ":" 
	  << dec << setw(2) << s;
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
   void flush( void ){
      f.flush();
   }
};

class shadow : public frame {
public:
   frame &f;
   color bg;
   shadow( frame &fr ): frame( fr.size_get()), f( fr ){}
   void checked_write( const vector p, const color c ){
      f.write( p, c );
      f.write( p + vector( 2, 2 ), c.mixed_with( bg ) );
   }
   void clear( const color c = color::white() ){
      bg = c;
      f.clear( c ); 
   }
   void flush( void ){
      f.flush();
   }
};

class fog : public frame {
public:
   frame &f;
   fog( frame &fr ): frame( fr.size_get()), f( fr ){}
   void checked_write( const vector p, const color c ){
      f.write( p, c.mixed_with( color::gray() ) );
   }
   void clear( const color c = color::white() ){
      f.clear( c.mixed_with( color::gray() )); 
   }
   void flush( void ){
      f.flush();
   }
};

class invert : public frame {
public:
   frame &f;
   invert( frame &fr ): frame( fr.size_get()), f( fr ){}
   void checked_write( const vector p, const color c ){
      f.write( p, - c );
   }
   void clear( const color c = color::white() ){
      f.clear( - c ); 
   }
   void flush( void ){
      f.flush();
   }
};

color negative( color c ){ return - c; }

void fattener( frame &f, vector p, color c, color bg ){ 
   for( int dx = -1; dx < 2; dx++ ){
      for( int dy = -1; dy < 2; dy++ ){
         f.write( p + vector( dx, dy ), c );
      }
   } 
}

void shadower( frame &f, vector p, color c, color bg ){ 
   f.write( p, c );
   f.write( p + vector( 2, 2 ), c.mixed_with( bg ) );
}

void border( frame &f, color c = color::red() ){
   for( int x = 0; x < f.size_get().x_get(); x++ ){
      f.write( vector( x, 0 ), c );
      f.write( vector( x, f.size_get().y_get() - 1 ), c );
   }
   for( int y = 0; y < f.size_get().y_get(); y++ ){
      f.write( vector( 0, y ), c );
      f.write( vector( f.size_get().x_get() - 1, y ), c );
   }
}

int main( void ){

   target_screen lcd;
   lcd.clear( color:: gray() );
   border( lcd );
   
   subframe d( lcd, vector( 11, 4 ), vector( 38, 11 ) );
   
   subframe a01( lcd, vector( 10, 20 ), vector( 70, 70 ) ); 
   subframe a02( lcd, vector( 90, 20 ), vector( 70, 70 ) );
   subframe a03( lcd, vector( 170, 20 ), vector( 70, 70 ) );
   subframe a01t( lcd, vector( 10, 90 ), vector( 70, 12 ) );
   subframe a02t( lcd, vector( 90, 90 ), vector( 70, 12 ) );
   subframe a03t( lcd, vector( 170, 90 ), vector( 70, 12 ) );

   subframe a11( lcd, vector( 10, 110 ), vector( 50, 50 ) );
   subframe a12( lcd, vector( 70, 110 ), vector( 50, 50 ) );
   subframe a13( lcd, vector( 130, 110 ), vector( 50, 50 ) );
   subframe a14( lcd, vector( 190, 110 ), vector( 50, 50 ) );
   subframe a11t( lcd, vector( 10, 160 ), vector( 50, 12 ) );
   subframe a12t( lcd, vector( 70, 160 ), vector( 50, 12 ) );
   subframe a13t( lcd, vector( 130, 160 ), vector( 50, 12 ) );
   subframe a14t( lcd, vector( 190, 160 ), vector( 50, 12 ) );
   
   
   frame_buffer a02_buffered( a02 );
   
   frame_buffer a03_buffered( a03 ); 
   // negative f2( a03_buffered );)
   frame_filter a03_negative( a03_buffered, negative ); 
   // fat a03_fat( a03_negative );
   frame_filter a03_fat( a03_negative, fattener );
   // shadow a03_nice( a03_fat );
   frame_filter a03_nice( a03_fat, shadower );
   
   text( "normal" ).draw( a01t );
   text( "buffered" ).draw( a02t );
   text( "b+n+f+sh" ).draw( a03t );
   
   frame_filter a12_negative( a12, negative ); 
   frame_filter a13_fat( a13, fattener );
   frame_filter a14_shadow( a14, shadower );
   
   frame_tee a0( a01, a02_buffered, a03_nice );
   frame_tee a1_raw( a11, a12_negative, a13_fat, a14_shadow );
   //frame_buffer a1( a1_raw );
   
   text( "normal" ).draw( a11t );
   text( "negative" ).draw( a12t );
   text( "fat" ).draw( a13t );
   text( "shadow" ).draw( a14t );
    
   for(;;){
      bmptk::time t = bmptk::current_time();
      draw_analog_clock( a0, t );
      draw_analog_clock( a1_raw, t );
      
      draw_digital_clock( d,t );
	  wait( 1000 * ms );
   }
   
   return 0;
}

