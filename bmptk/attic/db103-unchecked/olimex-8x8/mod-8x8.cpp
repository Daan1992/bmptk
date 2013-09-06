#include "bmptk.h"
#include "LPC11xx.h"

class olimex_led_matrix {
private:

   bmptk::output_pin &data;
   bmptk::output_pin &sck;
   bmptk::output_pin &latch;
   bool pixels [8][8];
   int col;
   
public:   

   olimex_led_matrix( 
      bmptk::output_pin &data, 
	   bmptk::output_pin &sck, 
      bmptk::output_pin &latch
   ):
      data( data ), 
      sck( sck ), 
      latch( latch ),
      col( 0 )
   {
      data.direction_set_output();
      sck.direction_set_output();
      latch.direction_set_output();
   }   
   
   bool & pixel( int x, int y ){
      if( ( x >= 0 ) && ( x < 8 ) && ( y >= 0 ) && ( y < 8 ) ){
	     return pixels[ x ][ y ];
	  }
   }
   
   void bit_out( bool x ){
	   sck.set( 0 );
	   bmptk::wait( 1 * bmptk::us );
	   data.set( x );
	   bmptk::wait( 1 * bmptk::us );
      sck.set( 1 );
	   bmptk::wait( 1 * bmptk::us );
   }
   
   void poll(){
	  for( int x = 0; x < 8; x++ ){
	     bit_out( pixels[ x ][ col ] );
	  }
	  for( int x = 0; x < 8; x++ ){
         bit_out( x == col );
	  }
	   bmptk::wait( 1 * bmptk::us );
	   latch.set( 0 );
	   bmptk::wait( 1 * bmptk::us );
 	   latch.set( 1 );
	   bmptk::wait( 1 * bmptk::us );
	   col = ( col == 8 ) ? 0 : col + 1;
   }
   
};

void show( olimex_led_matrix &matrix, int t = 200 * bmptk::ms ){
   while( t > 0 ){
      matrix.poll();
	   bmptk::wait( 1 * bmptk::ms );
	   t -= 1 * bmptk::ms;
   }   
}



int main( void ){	

   auto data  = bmptk::chip::pin( 1, 4 );
   auto latch = bmptk::chip::pin( 1, 5 );
   auto sck   = bmptk::chip::pin( 1, 8 );
   olimex_led_matrix matrix( data, sck, latch );
   
   for(;;){
   
      for( int x = 0; x < 8; x++ ){
         for( int y = 0; y < 8; y++ ){
	        matrix.pixel( x, y ) = x >= y;
	     }
      }
      show( matrix );
      
      for( int x = 0; x < 8; x++ ){
         for( int y = 0; y < 8; y++ ){
	        matrix.pixel( x, y ) = x <= y;
	     }
      }
      show( matrix );
      
   }
}