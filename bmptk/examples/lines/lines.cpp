#include "bmptk.h"
#include <stdlib.h>

using namespace bmptk;
using namespace graphics;

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
   for(;;){
      lcd.clear( color::red );
      for( unsigned int n = 0; n < 500; n++ ){
         vector start = random_vector_within( lcd.size_get());
         vector end = random_vector_within( lcd.size_get());
         lcd.draw( start, line( end - start, color::black ));
         #ifdef target_nds
            swiWaitForVBlank();
         #endif         
      }
   }
   return 0;
}
