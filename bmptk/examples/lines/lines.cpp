// #include <stdlib.h>
#include "bmptk.h"

using namespace bmptk;

unsigned int random_in_range( unsigned int min, unsigned int max ){
   unsigned int x = rand();
   x = x % ( max - min + 1 );
   return min + x;
}

vector random_vector_within( const vector size ){
   return vector( 
      random_in_range( 0, size.x_get()), 
      random_in_range( 0, size.y_get())
   );
}

int main( void ){
   target_screen lcd;
   #ifdef TARGET_nds
      target_top_screen lcd2;
   #else
      frame_dummy lcd2( vector( 192,256 ));   
   #endif      
   for(;;){
      lcd.clear( color::red() );
      lcd2.clear( color::blue() );
      for( unsigned int n = 0; n < 500; n++ ){
         vector start = random_vector_within( lcd.size_get());
         vector end = random_vector_within( lcd.size_get());
         if( n % 2 ){
            lcd.draw( start, line( end - start, color::blue() ));
         } else {
            lcd2.draw( start, line( end - start, color::red() ));
         }         
         #ifdef TARGET_nds
            swiWaitForVBlank();
         #endif         
      }
   }
   return 0;
}
