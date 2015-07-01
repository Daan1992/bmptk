#include "LPC810lib.h"

#define LED 2

void candles(){
   int i;
   int a[ 6 ];
   for( i = 0; i < 6; ++i ){
      a[ i ] = 0;
      pin_configure_as_input( i );
   }
   for(;;){
      for( i = 0; i < 6; ++i ){
         if( ! a[ i ] ){
            if( ! pin_get( i )){
               a[ i ] = 1;
               pin_configure_as_output( i );
               pin_set( i, 0 );
            }   
         }            
      }
   }
}

void kitt( void ){
   int i;
   for(;;){
      for( i = 0; i < 6; ++i ){
         pin_configure_as_output( i );
         pin_set( i, 0 );   
         wait_us( 200 * 1000 );      
         pin_set( i, 1 );   
      }
      for( i = 4; i > 0; --i ){
         pin_set( i, 0 );   
         wait_us( 200 * 1000 );      
         pin_set( i, 1 );   
      }         
   }
}

void beep_1khz( void ){
   pin_configure_as_output( LED );
   while( 1 ){
      pin_set( LED, 1 );   
      wait_us( 500 );
      pin_set( LED, 0 );   
      wait_us( 500 );
	}     
}

void blink(){
   pin_configure_as_output( LED );
   while( 1 ){
      pin_set( LED, 1 );   
      wait_us( 200 * 1000 );
      pin_set( LED, 0 );   
      wait_us( 200 * 1000 );
	} 
}
		
int main( void ){	
   blink();
}