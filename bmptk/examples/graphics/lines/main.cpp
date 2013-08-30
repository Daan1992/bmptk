//#include <stdlib.h>
#include "bmptk.h"
#include <iostream>
using namespace bmptk;
using namespace std;

int holdrandbla = 0;

int rand(){
   return(((holdrandbla = holdrandbla * 214013L + 2531011L) >> 16) & 0x7fff);
}

unsigned int random_in_range( unsigned int min, unsigned int max ){
   unsigned int x = rand();
   x = x % ( max - min + 1 ); 
   return min + x;
}

bmptk::graphics::vector random_vector_within( const bmptk::graphics::vector size ){ 
   return bmptk::graphics::vector( 
      random_in_range( 0, size.x_get()), 
      random_in_range( 0, size.y_get())
   );
}

int main(){
   bmptk::graphics::target_screen lcd;  
   #ifdef BMPTK_TARGET_nds
      bmptk::graphics::target_top_screen lcd2;
   #else
      bmptk::graphics::frame_dummy lcd2( bmptk::graphics::vector( 192,256 ));    
   #endif      
   for(;;){ 
      lcd.clear( bmptk::graphics::color::red() ); 
      //lcd2.clear( color::blue() ); 
      for( unsigned int n = 0; n < 20; n++ ){
         auto start = random_vector_within( lcd.size_get());
         auto end = random_vector_within( lcd.size_get());
         if( n % 2 ){
            bmptk::graphics::line( 
               lcd, start, end - start, bmptk::graphics::color::black() 
            ).draw();
         } else {
            bmptk::graphics::line( 
               lcd2, start, end - start, bmptk::graphics::color::red() 
            ).draw();
         }         
         wait( 100 * ms );        
      }
   }
 
   return 0;
}
