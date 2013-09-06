#include "bmptk.h"
#include "LPC11xx.h"
#include <iostream>

template< typename speaker >
class morse_beeper {
public:

   static const char * code( char c ){
      static const char * list[] = {
         "A.-",
         "B-...",
         "C-.-.",
         "D-..",
         "E.",
         "F..-.",
         "G--.",
         "H....",
         "I..",
         "J.---",
         "K-.-",
         "L.-..",
         "M--",
         "N-.",
         "O---",
         "P.--.",
         "Q--.-",
         "R.-.",
         "S...",
         "T-",
         "U..-",
         "V...-",
         "W.--",
         "X-..-",
         "Y-.--",
         "Z--..",
         "0-----",
         "1.----",
         "2..---",
         "3...--",
         "4....-",
         "5.....",
         "6-....",
         "7--...",
         "8---..",
         "9----.",
         "..-.-.-",
         ",--..--",
         "?..--..",
         ":---...",
         ""
      };
      for( int i = 0; list[ i ][0] != '\0'; i++ ){
         if ( list[ i ][ 0 ] == c ){
            return list[ i ] + 1;
         }
      }
      return "";
   }   
            
   static void beep( int d ){
      speaker::set( 1 );
      bmptk::wait( d );   
      speaker::set( 0 );
   }
   
   static void send( char c, int unit, int dash = 5 ){
      if( c == ' ' ){
         bmptk::wait( 7 * unit );
         return;
      } 
      
      const char * s = code( c );
      if( *s == '\0' ){ 
         beep( 20 * unit );
         return;
      }
      
      while( *s != '\0' ){
         if( *s++ == '.' ){
            beep( 1 * unit );
         } else {
            beep( dash * unit );
         }
         bmptk::wait( 1 * unit );
      }
      bmptk::wait( dash * unit );
   }
   
   static void send( const char *msg, int unit, int dash = 5 ){
      while( *msg != '\0' ){
         send( *msg++, unit, 5 );
         bmptk::wait( 30 * unit );
      }        
   }
   
   static void init(){
      speaker::init();
   }

};

typedef bmptk::target::gpio_1_4_out speaker;
typedef morse_beeper< speaker > beeper;

int main( void ){	

   speaker::init();
	beeper::init();
   
   speaker::set( 1 );
   bmptk::wait( 2 * bmptk::s );
   speaker::set( 0 );
   bmptk::wait( 2 * bmptk::s );
   
   beeper::send( "N52:08.355 E005:21.698", 100 * bmptk::ms );

   for(;;){}
}