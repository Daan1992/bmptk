//#include <stdlib.h>
#include "bmptk.h"
#include "LPC11xx.h"
#include <iostream>

int holdrandbla = 0;

int rand(){
   return(((holdrandbla = holdrandbla * 214013L + 2531011L) >> 16) & 0x7fff);
}

unsigned int random_in_range( unsigned int min, unsigned int max ){
   unsigned int x = rand();
   x = x % ( max - min + 1 ); 
   return min + x;
}

bmptk::graphics::vector random_vector_within( 
   const bmptk::graphics::vector size 
){ 
   return bmptk::graphics::vector( 
      random_in_range( 0, size.x_get()), 
      random_in_range( 0, size.y_get())
   );
}



int main( void ){

   bmptk::chips::lcd5510<
      bmptk::target::gpio_1_0,
      bmptk::target::gpio_1_1,
      bmptk::target::gpio_1_2,
      bmptk::target::gpio_1_3,
      bmptk::target::gpio_1_4
   > lcd;   
   
   for(;;){ 
   
      // std::cout << "************* CLEAN ****************\n";
      lcd.clear( bmptk::graphics::color::white() ); 
      bmptk::graphics::rectangle( 
         lcd, 
         bmptk::graphics::vector::origin(), 
         lcd.size_get() - bmptk::graphics::vector::one()
      ).draw();      
      
      for( unsigned int n = 0; n < 20; n++ ){
      
         auto start = random_vector_within( lcd.size_get());
         auto end = random_vector_within( lcd.size_get());
         // std::cout << start << " " << end << "\n";
         
         bmptk::graphics::line( 
            lcd, 
            start, 
            end - start, 
            bmptk::graphics::color::black() 
         ).draw(); 
         bmptk::wait( 200 * bmptk::ms );        
      }
   }
 
   return 0;
}

