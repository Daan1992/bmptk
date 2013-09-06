#include "bmptk.h"
#include "LPC11xx.h"
#include <iostream>

template< 
   typename c1, typename c2, typename c3, typename c4, typename c5,
   typename r1, typename r2, typename r3, typename r4, 
   typename r5, typename r6, typename r7, 
   
class matrix {
private:
   bmptk::output_pin &c1, &c2, &c3, &c4, &c5;
   bmptk::output_pin &r1, &r2, &r3, &r4, &r5, &r6, &r7;
   int row;
   unsigned long long int timestamp_last;
public:
   bool pixels[ 5 ][ 7 ];
   
   void poll(){
      unsigned long long int timestamp_next = bmptk::time_since_startup();
      if( timestamp_next - timestamp_last < 1 * bmptk::ms ){
         return;
      }
      timestamp_last = timestamp_next;
      c1.set( pixels[ 0 ][ row ] );
      c2.set( pixels[ 1 ][ row ] );
      c3.set( pixels[ 2 ][ row ] );
      c4.set( pixels[ 3 ][ row ] );
      c5.set( pixels[ 4 ][ row ] );
      r1.set( row != 0 );
      r2.set( row != 1 );
      r3.set( row != 2 );
      r4.set( row != 3 );
      r5.set( row != 4 );
      r6.set( row != 5 );
      r7.set( row != 6 );
      if( ++row >= 7 ){ row = 0; }
   }
   
   matrix(
      bmptk::output_pin &c1,
      bmptk::output_pin &c2,
      bmptk::output_pin &c3,
      bmptk::output_pin &c4,
      bmptk::output_pin &c5,
      bmptk::output_pin &r1,
      bmptk::output_pin &r2,
      bmptk::output_pin &r3,
      bmptk::output_pin &r4,
      bmptk::output_pin &r5,
      bmptk::output_pin &r6,
      bmptk::output_pin &r7
   ):
      c1( c1 ), c2( c2 ), c3( c3 ), c4( c4 ), c5( c5 ),
      r1( r1 ), r2( r2 ), r3( r3 ), r4( r4 ), r5( r5 ), r6( r6 ), r7( r7 ),
      row( 0 ), timestamp_last( 0 )
   {
      c1.direction_set_output();
      c2.direction_set_output();
      c3.direction_set_output();
      c4.direction_set_output();
      c5.direction_set_output();
      r1.direction_set_output();
      r2.direction_set_output();
      r3.direction_set_output();
      r4.direction_set_output();
      r5.direction_set_output();
      r6.direction_set_output();
      r7.direction_set_output();
      for( int i = 0; i < 5; i++ ){
         for( int j = 0; j < 7; j++ ){
            pixels[ i ][ j ] = false;
         }
      }
      poll();      
   }      
      
};



int main( void ){	

   std::cout << "\n5 x 7 LED matrix display\n";
   
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);     //enable IOCON
	LPC_IOCON->R_PIO1_0 |= 0x01;
	LPC_IOCON->R_PIO1_1 |= 0x01;
	LPC_IOCON->R_PIO1_2 |= 0x01;
	LPC_IOCON->SWDIO_PIO1_3 |= 0x01;

	auto c1  = bmptk::chip::pin( 1, 9 );
	auto c2  = bmptk::chip::pin( 1, 8 );  
	auto c3  = bmptk::chip::pin( 1, 5 );
	auto c4  = bmptk::chip::pin( 1, 4 );  
	auto c5  = bmptk::chip::pin( 1, 3 );  
	auto r1  = bmptk::chip::pin( 1, 2 );
	auto r2  = bmptk::chip::pin( 1, 1 );  
	auto r3  = bmptk::chip::pin( 1, 0 );
	auto r4  = bmptk::chip::pin( 0, 7 );  
	auto r5  = bmptk::chip::pin( 0, 6 );  
	auto r6  = bmptk::chip::pin( 0, 5 );
	auto r7  = bmptk::chip::pin( 0, 4 );  
	
	matrix m( c1, c2, c3, c4, c5, r1, r2, r3, r4, r5, r6, r7 );

   for(;;){
      m.pixels[ 0 ][ 0 ] = true;
      m.pixels[ 1 ][ 1 ] = true;
      m.pixels[ 2 ][ 2 ] = true;
      m.pixels[ 3 ][ 3 ] = true;
      m.pixels[ 4 ][ 4 ] = true;
      m.pixels[ 3 ][ 5 ] = true;
      m.pixels[ 2 ][ 6 ] = true;
      m.poll();
   }      
   
}