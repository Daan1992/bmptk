#include "bmptk.h"
// #include <iostream>

using namespace bmptk;


void test_format_write( 
   frame &fr, 
   vector &anchor, 
   const vector size, 
   const vector outer_margin,
   const char *line, 
   const vector spacing, 
   font_alignment h, 
   font_alignment v 
){ 
   // if( ( h != align_far )) return;
   vector inner_margin( 2, 2 );      
   rectangle( fr, anchor, size, color::blue() ).draw();
   subframe sub( 
      fr, 
	  anchor + inner_margin, 
	  size + vector( 1, 1 ) - 2 * inner_margin );
   sub.clear( color::green());
 
   format ff( font_default() );
   ff.h = h;
   ff.v = v;
   ff.spacing = spacing;
   text( line, ff ).draw( sub );
   anchor += ( size + outer_margin ).x_projection();
}

int main( void ){
   target_screen lcd;
   lcd.clear( color::white() );
   
   const char *t = "Hello nice\nworld";
   vector s( 55, 31 );
   vector outer_margin( 2, 2 );
   vector sp( 0, 0 );   
   
   format fm;
   fm.scale = 2;   
   vector tx( 20,  4 );   
   text( "near",  fm ).draw( 
      lcd, 
	  tx );
   text( "centre", fm ).draw(
      lcd,
      ( tx += ( s + outer_margin ).x_projection() ) - vector( 5, 0 ) );
   text( "far",   fm ).draw(
      lcd,
      tx += ( s + outer_margin ).x_projection() );
   text( "fill",  fm ).draw(
      lcd,
	  tx += ( s + outer_margin ).x_projection() );
   
   vector d0( 20, 20 ), d = d0;  
   text( "Ne" ).draw( lcd, d + vector( -12, 10 ));        
   test_format_write( 
      lcd, d, s, outer_margin, t, sp, align_near,   align_near );
   test_format_write( 
      lcd, d, s, outer_margin, t, sp, align_centre, align_near );
   test_format_write( 
      lcd, d, s, outer_margin, t, sp, align_far,    align_near );
   test_format_write( 
      lcd, d, s, outer_margin, t, sp, align_fill,   align_near );
    
   d = vector( d0.x_get(), d.y_get() + s.y_get() + outer_margin.y_get() );
   text( "Ce" ).draw( lcd, d + vector( -12, 10 ));
   test_format_write( 
      lcd, d, s, outer_margin, t, sp, align_near,   align_centre );
   test_format_write( 
      lcd, d, s, outer_margin, t, sp, align_centre, align_centre );
   test_format_write( 
      lcd, d, s, outer_margin, t, sp, align_far,    align_centre );
   test_format_write( 
      lcd, d, s, outer_margin, t, sp, align_fill,   align_centre );
      
   d = vector( d0.x_get(), d.y_get() + s.y_get() + outer_margin.y_get() );  
   text( "Fa" ).draw( lcd, d + vector( -12, 10 ));
 
   test_format_write( 
      lcd, d, s, outer_margin, t, sp, align_near,   align_far );
   test_format_write( 
      lcd, d, s, outer_margin, t, sp, align_centre, align_far );
   test_format_write( 
      lcd, d, s, outer_margin, t, sp, align_far,    align_far );
   test_format_write( 
      lcd, d, s, outer_margin, t, sp, align_fill,   align_far );
       
   d = vector( d0.x_get(), d.y_get() + s.y_get() + outer_margin.y_get() );   
   text( "Fi" ).draw( lcd, d + vector( -12, 10 ));
   test_format_write( 
      lcd, d, s, outer_margin, t, sp, align_near,   align_fill );
   test_format_write( 
      lcd, d, s, outer_margin, t, sp, align_centre, align_fill );
   test_format_write( 
      lcd, d, s, outer_margin, t, sp, align_far,    align_fill );
   test_format_write( 
      lcd, d, s, outer_margin, t, sp, align_fill,   align_fill );

   d = vector( d0.x_get(), d.y_get() + s.y_get() + outer_margin.y_get() );   
   sp = vector( 1, 0 );      
   text( "+1" ).draw( lcd, d + vector( -12, 10 ));
   test_format_write( 
      lcd, d, s, outer_margin, t, sp, align_near,   align_fill );
   test_format_write( 
      lcd, d, s, outer_margin, t, sp, align_centre, align_fill );
   test_format_write( 
      lcd, d, s, outer_margin, t, sp, align_far,    align_fill );
   test_format_write( 
      lcd, d, s, outer_margin, t, sp, align_fill,   align_fill );
      
   for(;;);   
   return 0;
}
