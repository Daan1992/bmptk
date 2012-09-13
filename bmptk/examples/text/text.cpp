#include "bmptk.h"
#include "mariner.h"

using namespace bmptk;

int main( void ){
   target_screen lcd;
   lcd.clear( color::red() );
   
   sheet< 1024 > t( lcd );
   t.clear();
   for( const char *p = mariner; p!= '\0'; p++ ){
      t.add( *p );
      #ifdef TARGET_nds
         swiWaitForVBlank();
      #endif       
   }
 
   for(;;);   
   return 0;
}
