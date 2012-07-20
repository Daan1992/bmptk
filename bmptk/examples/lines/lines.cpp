#include "bmptk.h"
#include <stdlib.h>

using namespace bmptk;
using namespace bmptk::graphics;

unsigned int random( unsigned int min, unsigned int max ){
   unsigned int x = rand();
   x = x % ( max - min + 1 );
   return min + x;
}

vector random_vector( const vector size ){
   return vector( random( 0, size.x_get()), random( 0, size.y_get()));
}

int main( void ){
   nintendo_screen lcd;
   for(;;){
     lcd.clear( color::red );
     for( unsigned int n = 0; n < 500; n++ ){
         vector start = random_vector( lcd.size_get());
         vector end = random_vector( lcd.size_get());
         lcd.draw( start, line( end - start, color::black ));
         swiWaitForVBlank();
      }
   }
   return 0;
}