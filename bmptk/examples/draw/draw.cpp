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
   line( lcd, current, size ).draw();
   line( lcd, current += ( size + margin ).x_projection(),
      size, color::blue() ).draw();
   line( lcd, current += ( size + margin ).x_projection(),
      size, color::purple(), 5 ).draw();
      
   // some rectangles   
   rectangle( lcd, current += ( size + margin ).x_projection(), 
      size, color::black() ).draw();
   rectangle( lcd, current += ( size + margin ).x_projection(), 
      size, color::black(), color::purple(), 3 ).draw();
      
   // some rectangles woth different border reliefs   
   current = vector( start.x_get(), ( current + size + margin ).y_get() );
   size = vector( 12, 12 );
   rectangle( 
      lcd, current,
      size, 
	  color::black(), color::transparent(), 
	  3 
   ).draw();     
   rectangle( 
      lcd, current += ( size + margin ).x_projection(),
      size, 
	  color::gray(), color::blue(), 
	  3, relief_raised 
   ).draw();
   rectangle( 
      lcd, 
	  current += ( size + margin ).x_projection(),
      size, 
	  color::gray(), color::blue(), 
	  3, relief_sunken 
   ).draw();         
   rectangle( 
      lcd, 
	  current += ( size + margin ).x_projection(),
      size, 
	  color::blue(), color::yellow(), 
	  3, relief_raised 
   ).draw();
   rectangle( 
      lcd, current += ( size + margin ).x_projection(), 
      size, 
	  color::blue(), color::yellow(), 
	  3, relief_sunken 
   ).draw();   
      
   // draw some circles   
   int radius = 15;
   current = vector( 
      start.x_get() + radius, 
     ( current + size + margin ).y_get() + radius );
   size = vector( 2 * radius, 2 * radius );
   circle( lcd, current,
      radius ).draw();   
   circle( lcd, current += ( size + margin ).x_projection(),
      radius, color::yellow(), color::green(), 5 ).draw();   
      
   // draw a photo of me in four orientations   
   current = vector( start.x_get(), ( current + margin + size / 2 ).y_get() );
   size = wouter().size_get();
   { wouter().draw( lcd, current );
      current += ( size + margin ).x_projection(); }
      
   { subframe ff( lcd, 
        current + size.x_projection(), 
		size * vector( -1, 1 ) );
     wouter().draw( ff ); 
     current += ( size + margin ).x_projection(); }
     
   { subframe ff( lcd, 
       current + size.y_projection() + vector( +1, -1 ), 
	   size * vector( 1, -1 ));
     wouter().draw( ff ); 
     current += ( size + margin ).x_projection(); }
     
   { subframe ff( lcd, 
        current + size + vector( +1, -1 ), - size );
     wouter().draw( ff ); 
     current += ( size + margin ).x_projection(); }     

   // draw the next arrow
   next().draw( lcd, vector( 120, 50 )); 
    
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
     text( mariner ).draw( ff ); 
	 current += ( text_size + margin ).y_projection(); } 
      
   // show the BMPTK version 
   subframe version_frame( lcd, current, vector( 100, 12 ));
   version_frame.clear( color::blue());
   text( bmptk::version ).draw( version_frame );   
   
   // draw some big letters
   current = vector( 120, start.y_get() );
   text_size = vector( 120, 32 );
   { subframe ff( lcd, current, text_size );
     ff.clear( color::blue() );
     format f( bigfont() );
     text t( "bmptk", f );
     t.draw( ff ); } 
     
   // font_default().draw( lcd );
   // bigfont().draw( lcd );
   

 
   for(;;);   
   return 0;
}
