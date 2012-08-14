#include "bmptk.h"
#include <stdlib.h>

#include "wouter.h"
#include "next.h"
#include "bigfont.h"

using namespace bmptk;

int main( void ){
   target_screen lcd;
   lcd.clear( color::red() );
   vector margin( 8, 8 ), size, start( 10, 10 ), current;
   
   // draw some lines
   current = start;
   size = vector( 20, 20 );
   lcd.draw( current,
      line( size ));
   lcd.draw( current += ( size + margin ).x_projection(),
      line( size, color::blue() ));
    lcd.draw( current += ( size + margin ).x_projection(),
      line( size, color::purple(), 5 ));
      
   // some rectangles   
   lcd.draw( current += ( size + margin ).x_projection(), 
      rectangle( size, color::black() ));   
   lcd.draw( current += ( size + margin ).x_projection(), 
      rectangle( size, color::black(), color::purple(), 3 ));   
      
   // some rectangles woth different border reliefs   
   current = vector( start.x_get(), ( current + size + margin ).y_get() );
   size = vector( 12, 12 );
   lcd.draw( current,
      rectangle( size, color::black(), color::transparent(), 3 ));     
   lcd.draw( current += ( size + margin ).x_projection(),
      rectangle( size, color::gray(), color::blue(), 3, relief_raised ));
   lcd.draw( current += ( size + margin ).x_projection(),
      rectangle( size, color::gray(), color::blue(), 3, relief_sunken ));         
   lcd.draw( current += ( size + margin ).x_projection(),
      rectangle( size, color::blue(), color::yellow(), 3, relief_raised ));
   lcd.draw( current += ( size + margin ).x_projection(), 
      rectangle( size, color::blue(), color::yellow(), 3, relief_sunken ));   
      
   // draw some circles   
   int radius = 15;
   current = vector( start.x_get() + radius, ( current + size + margin ).y_get() + radius );
   size = vector( 2 * radius, 2 * radius );
   lcd.draw( current,
      circle( radius ));   
   lcd.draw( current += ( size + margin ).x_projection(),
      circle( radius, color::yellow(), color::green(), 5 ));   
      
   // draw a photo of me in four orientations   
   current = vector( start.x_get(), ( current + margin + size / 2 ).y_get() );
   size = wouter.size;
   { wouter.draw( lcd, current );
      current += ( size + margin ).x_projection(); }
      
   { subframe ff( lcd, current + size.x_projection(), size * vector( -1, 1 ) );
     wouter.draw( ff ); 
     current += ( size + margin ).x_projection(); }
     
   { subframe ff( lcd, current + size.y_projection() + vector( +1, -1 ), size * vector( 1, -1 ));
     wouter.draw( ff ); 
     current += ( size + margin ).x_projection(); }
     
   { subframe ff( lcd, current + size + vector( +1, -1 ), - size );
     wouter.draw( ff ); 
     current += ( size + margin ).x_projection(); }     

   // draw the next arrow
   next.draw( lcd, vector( 120, 50 ));
    
   // draw some text
   current = vector( start.x_get(), ( current + margin + size ).y_get() );
   vector text_size( 160, 28 );
   const char * mariner = 
      "IT IS an ancient Mariner,\n"
      "And he stoppeth one of three.\n"
      "'By thy long grey beard and glittering eye,\n"
      "Now wherefore stopp'st thou me?";
   { subframe ff( lcd, current, text_size );
     ff.clear( color::yellow() );
     text t( mariner, ff.size_get() );
     t.draw( ff, vector( 0, 0 ) ); } 
      
   // draw some big letters
   current = vector( 120, start.y_get() );
   text_size = vector( 120, 32 );
   { subframe ff( lcd, current, text_size );
     ff.clear( color::blue() );
     text t( "bmptk", ff.size_get() );
     format f( bigfont );
     t.f = f;
     t.draw( ff, vector( 0, 0 ) ); } 
     
   //lcd.draw( font_default );
   //lcd.draw( bigfont );
 
   for(;;);   
   return 0;
}
