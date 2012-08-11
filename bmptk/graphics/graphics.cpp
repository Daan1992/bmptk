// ==========================================================================
//
// refer to graphics.h for all information
//
// ==========================================================================



#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "graphics.h"
// #include "font.h"

namespace bmptk { 
namespace graphics {

// ==========================================================================
//
// vector
//

const vector vector::origin = vector( 0, 0 );

std::ostream & operator<<( std::ostream &s, const vector p ){
   s << std::dec << "(" << p.x_get() << "," << p.y_get() << ")";
   return s;
}


// ==========================================================================
//
// color
//

std::ostream & operator<<( std::ostream &s, const color c ){
   if( c == color::transparent ){
      s << "( transparent  )";
   } else {
      s << std::hex << std::setfill( '0' ) << "(" 
        << std::setw( 2 ) << c.red_get() << "," 
        << std::setw( 2 ) << c.green_get() << "," 
        << std::setw( 2 ) << c.blue_get() 
        << std::setfill( ' ' ) << ")";
   }        
   return s;
}

const color color::black( 0,       0,    0 );
const color color::white( 0xFF, 0xFF, 0xFF );
const color color::red  ( 0xFF,    0,    0 );
const color color::green( 0,    0xFF,    0 );
const color color::blue ( 0,       0, 0xFF );
const color color::gray    = ( color::black  + color::white ) / 2;
const color color::yellow  = color::red  + color::green;
const color color::cyan    = color::blue + color::green;
const color color::magenta = color::blue + color::red;
const color color::transparent( 0, 0, 0, 1 );

const color color::violet( 0xEE82EE );
const color color::sienna( 0xA0522D );
const color color::purple( 0x800080 );
const color color::pink(   0xFFC8CB );
const color color::silver( 0xC0C0C0 );
const color color::brown(  0xA52A2A );
const color color::salmon( 0xFA8072 );


// ==========================================================================
//
// event
//

  
const char * const event_name[] = {
   "none", "exit", "down", "touch", "up", "click" };
   
const char * event_type_name( const event_type e ){
   if ( e <= event_click ){
      return event_name[ e ];
   } else {
      return  "invalid";
   }
}   

std::ostream & operator<<( std::ostream &s, const event_type &e ){ 
   s << event_type_name( e );
   return s;
}

std::ostream & operator<<( std::ostream &s, const event &e ){
   s << e.location_get() << ":" << e.event_type_get();
   return s;
}


// ==========================================================================
//
// drawable
//

void drawable::drawable_draw_pixel( 
   frame &f, 
   const vector position, 
   const vector address, 
   const color c 
){
   f.write( position + address, c );   
}


// ==========================================================================
//
// line
//

void line::draw( 
   frame &f, 
   const vector position
) const {
    
   // don't bother to draw anything 
   // when the size would be 0 or the color transparent
   if( width < 1 || fg == color::transparent ){
      return;       
   }
              
   // http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
   // http://homepages.enterprise.net/murphy/thickline/index.html
   
   // int i0 = 0; int i1 = 1;
   int i0 = - ( width / 2 ); 
   // int i1 = 1;
   int i1 = width + i0;
   // i1 = width;
   
   int x0 = 0;
   int y0 = 0;
   int x1 = size.x_get();
   int y1 = size.y_get();
   int Dx = x1 - x0; 
   int Dy = y1 - y0;
   int steep = (abs(Dy) >= abs(Dx));
   vector offset( 0, 1);
   if( steep ){
      std::swap(x0, y0);
      std::swap(x1, y1);
      // recompute Dx, Dy after swap
      Dx = x1 - x0;
      Dy = y1 - y0;
      offset = vector( 1, 0 );
   }
   int xstep = 1;
   if( Dx < 0 ){
      xstep = -1;
      Dx = -Dx;
   }
   int ystep = 1;
   if( Dy < 0 ){
      ystep = -1;    
      Dy = -Dy; 
   }
   int TwoDy = 2*Dy; 
   int TwoDyTwoDx = TwoDy - 2*Dx; // 2*Dy - 2*Dx
   int E = TwoDy - Dx; //2*Dy - Dx
   int y = y0;
   int xDraw, yDraw;  
   for( int x = x0; x != x1; x += xstep ){    
      if (steep) {     
         xDraw = y;
         yDraw = x;
      } else {     
         xDraw = x;
         yDraw = y;
      }
      for( int i = i0; i < i1; i++ ){
         drawable_draw_pixel( 
           f, 
           position + offset * i,
           vector( xDraw, yDraw ), 
           fg );
      }   
      // trace << xDraw << " " << yDraw;
      if( E > 0 ){
         E += TwoDyTwoDx; //E += 2*Dy - 2*Dx;
         y = y + ystep;
      } else {
         E += TwoDy; //E += 2*Dy;
      }
   }
}


// ==========================================================================
//
// rectangle
//

const char * const shape_relief_name[] = {
   "flat", "raised", "sunken" };
   
relief flip( const relief r ){
   if( r == relief_flat ) return relief_flat;	
   if( r == relief_raised ) return relief_sunken;	
   return relief_raised;	
}   

std::ostream & operator<<( std::ostream &s, const relief &r ){ 
   if( r <= relief_raised ){
      s << shape_relief_name[ r ];
   } else {
      s << "invalid";
   }         
   return s;
}
   
void rectangle::draw( 
   frame &f, 
   const vector position
) const {
    
   // don't draw anything that should be invisible
   if( size.x_get() == 0 || size.y_get() == 0 ){
      return;       
   }

   // determine the border color(s)
   color c1 = fg, c2 = fg;
   if( border == relief_raised ){ 
      c1 = bright; 
      c2 = dark;
   }
   if( border == relief_sunken ){ 
      c2 = bright; 
      c1 = dark;
   }

   // draw the borders
   for( unsigned int i = 0; i < width; i++ ){
      vector s = size.direction() * i;  
      vector d = size - ( s * 2 );   
      f.draw( position + s,                    line( d.x_projection(),   c1 ));       
      f.draw( position + s + d.x_projection(), line( d.y_projection(),   c2 ));       
      f.draw( position + s + d,                line( - d.x_projection(), c2 ));       
      f.draw( position + s + d.y_projection(), line( - d.y_projection(), c1 ));       
   }   
   
   // draw the inner region
   vector margin = size.direction() * width;
   subframe( 
      f, 
      position + margin, 
      ( size + size.direction() ) - margin * 2 ).clear( bg ); 
}   


// ==========================================================================
//
// circle
//

void circle::draw( 
   frame &f, 
   const vector position
) const {
    
   // don't draw anything when the size would be 0 
   if( radius < 1 ){
      return;       
   }

   // http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
   
   int ff = 1 - radius;
   int ddFx = 1;
   int ddFy = -2 * radius;
   int y = radius;
 
   int radius1 = radius + 1 - width;
   int ff1 = 1 - radius1;
   int ddFx1 = 1;
   int ddFy1 = -2 * radius1;
   int y1 = radius1;
   
/*   
   // outer circle top and bottom
   drawable_draw_pixel( f, position,        0, + radius, fg );
   drawable_draw_pixel( f, position,        0, - radius, fg );
 
   // inner circle top and bottom
   drawable_draw_pixel( f, position,        0, + radius1, fg );
   drawable_draw_pixel( f, position,        0, - radius1, fg );
*/
   
   // y-axis lines
   line( 0, width, fg ).draw( f, position - vector( 0, radius ) );
   line( 0, radius1 * 2, bg ).draw( f, position - vector( 0, radius1 ) );
   line( 0, width, fg ).draw( f, position + vector( 0, radius1 ) );
 
   for( int x = 1; x < y; x++  ){
      
      // calculate next outer circle point
      if( ff >= 0 ){
        y--;
        ddFy += 2;
        ff += ddFy;
      }
      ddFx += 2;
      ff += ddFx;   
       
      // calculate next inner circle point
      if( ff1 >= 0 ){
        y1--;
        ddFy1 += 2;
        ff1 += ddFy1;
      }
      ddFx1 += 2;
      ff1 += ddFx1;          
      
      if( x <= y1 ){
            
         int w = 1 + y - y1;
         line( 0, + w, fg ).draw( f, position + vector( + x, - y ) );
         line( 0, - w, fg ).draw( f, position + vector( + x, + y ) );            

         line( 0, + w, fg ).draw( f, position + vector( - x , - y ) );
         line( 0, - w, fg ).draw( f, position + vector( - x , + y ) );            
               
         line( 0, y1 * 2, bg ).draw( f, position + vector( + x, - y1 ) );
         line( 0, y1 * 2, bg ).draw( f, position + vector( - x, - y1 ) );  
         
         // int w2 = x;
         line( 0, + w, fg ).draw( f, position + vector( + x, - y ) );
         line( 0, - w, fg ).draw( f, position + vector( + x, + y ) );            

         line( 0, + w, fg ).draw( f, position + vector( - x , - y ) );
         line( 0, - w, fg ).draw( f, position + vector( - x , + y ) ); 
               
         line( 0, x * 2, bg ).draw( f, position + vector( + y1, - x ) );
         line( 0, x * 2, bg ).draw( f, position + vector( - y1, - x ) );  
                      
      } else {
            
         int w = 1 + 2 * y;
         line( 0, w, color::gray ).draw( f, position + vector(   x, - y ) );
         line( 0, 1 + 2 * x, color::black ).draw( f, position + vector(   y, - x ) );
         // line( 0, - w, fg ).draw( f, position + vector( - x, y ) );
/*      
      drawable_draw_pixel( f, position, + x, + y, fg );
      drawable_draw_pixel( f, position, - x, + y, fg );
      drawable_draw_pixel( f, position, + x, - y, fg );
      drawable_draw_pixel( f, position, - x, - y, fg );
      drawable_draw_pixel( f, position, + y, + x, fg );
      drawable_draw_pixel( f, position, - y, + x, fg );
      drawable_draw_pixel( f, position, + y, - x, fg );
      drawable_draw_pixel( f, position, - y, - x, fg );
*/      
      }
   }
}


// ==========================================================================
//
// photo
//

void photo::draw( frame &f, const vector position ) const {
   for( int x = 0; x < size.x_get(); x++  ){
      for( int y = 0; y < size.y_get(); y++ ){
         vector a( x, y );            
         drawable_draw_pixel( f, position, a, read( a )); } } } 

         
// ==========================================================================
//
// inline_photo
//

color inline_rgb_photo :: checked_read( const vector p ) const {
   int base = 3 * ( p.x_get() + p.y_get() * size.x_get() );    
   return color( data[ base ], data[ base + 1 ], data[ base + 2 ] );    
}

bool inline_bw_photo :: bool_read( const vector p ) const {
   int a = p.x_get() + p.y_get() * size.x_get();
   return ( data[ a / 8 ] ) & ( 1 << ( a % 8 ));   
}


// ==========================================================================
//
// inline_font
//

vector inline_font :: char_size( char c ) const {
   if( ! has( c )){
      return vector( 0, font_char_size.y_get() ); 
             
   } else if( fixed ){
      return font_char_size;      
      
   } else {           
      return vector( 
         start[ 1 + (int)c ] - start[ (int)c ],
         font_char_size.y_get() );
   }
}   

bool inline_font :: read( char c, const vector p ) const {
   return bool_read( p + vector( start[ (int) c ], 0 ));
}

// ==========================================================================
//
// format
//

const char * const font_alignment_name[] = {
   "near", "far", "centre", "filled" }; 

std::ostream & operator<<( std::ostream &s, const font_alignment &a ){ 
   if( a <= align_fill ){
      s << font_alignment_name[ a ];
   } else {
      s << "invalid";
   }         
   return s;
}
   
// const format format_default( ff );


// ==========================================================================
//
// text
//

//! returns number of lines needed to draw the text 
//
//! This function returns the number of (vertical) lines rquired to draw 
//! the text s using the format fm and size. 
//! Wrapping is taken into account.
//!
//! This width is used to determine where to start drawing the 
//! text when the vertical alignment is not align_near.
int line_count( 
   const char *s, 
   const vector size, 
   const format &fm 
){
   int count = 1; 
   int x = 0;
   for( ; *s != '\0'; s++ ){
      if( *s == '\n' ){
         count++;
         x = 0;
      } else {
         int char_size = fm.f->char_size( *s ).x_get() * fm.spacing.x_get();
         if(( x + char_size > size.x_get() ) && fm.wrap ){
            count++;    
            x = char_size;                
         } else {
            x += char_size;                
         }
      }                  
   }     
   return count;
}

//! returns the length in pixels of the first formatted line from s 
//
//! This function returns the width, in pixels, of the first
//! line of characters from s, taking the size and format into account.
//!
//! This width is used to determine where to start drawing the 
//! text when the horizontal alignment is not align_near.
int line_width( 
   const char *s, 
   const vector size, 
   const format &fm 
){
   int length = 0;
   for( ; *s != '\0'; s++ ){
      if( *s == '\n' ){
         return length;
      } else {
         int char_size = fm.f->char_size( *s ).x_get() 
            + fm.spacing.x_get();
         if( length + char_size > size.x_get() ){
            return length;                 
         } else {
            length += char_size;                
         }      
      }                  
   }     
   return length;
}

//! returns the number of characters in the first formatted line from s 
int line_chars( 
   const char *s, 
   const vector size, 
   const format &fm 
){
   int length = 0;
   int chars = 0;
   for( ; *s != '\0'; s++ ){
      if( *s == '\n' ){
         return chars;
      } else {
         int char_size = fm.f->char_size( *s ).x_get() 
            + fm.spacing.x_get();
         if( length + char_size > size.x_get() ){
            return chars;                 
         } else {
            length += char_size;                
            chars++;
         }      
      }                  
   }     
   return chars;
}

//! draws a single line of text, and advances the text pointer-pointer
void draw_text_line( 
   frame &fr, 
   vector &p, 
   const vector sz, 
   const format &fm, 
   const char **s 
){   
   int width = line_width( *s, sz, fm );
   int extra = sz.x_get() - width;
   int spaces = line_chars(  *s, sz, fm ) + 1;
   int missing = 0;
   if( fm.h == align_far ){
      p += extra;
   } 
   if( fm.h == align_centre ){
      p += extra / 2;
   } 
   for( ; (**s != '\n') && (**s != '\0') ; (*s)++ ){           
      char_photo fc( *fm.f, **s, fm.fg, fm.bg );
      if( fm.h == align_fill ){    
         missing += extra; 
         int adjust = missing / spaces;
         p += vector( adjust, 0 );
         missing -= adjust * spaces;
      }
      fr.draw( p, fc );
      p += vector( fc.size.x_get() +fm.spacing.x_get(), 0 );
   }          
}

// draws the text
void text::draw( 
   frame &fr, 
   const vector position
) const {

   vector p = vector::origin;
   int spaces = 1; // line_count( s, f ) + 1; // wovo somethng wrong!!!
   int extra = std::max( 
      0, 
      f.f->font_char_size.y_get() 
         - ( spaces - 1 ) * f.f->font_char_size.y_get() );
   int missing = 0;
   // trace << spaces << " " << extra;
   if( f.v == align_far ){
      p += vector( 0, extra);
   } 
   if( f.v == align_centre ){
      p += vector( 0, extra / 2 );
   } 
   const char *ss = s;
   for( ; *ss != '\0'; ){
      if( f.v == align_fill ){  
         missing += extra;   
         int adjust = missing / spaces; 
         // trace << extra << " " << missing << " " << adjust;
         p += vector( 0, adjust );
         missing -= adjust * spaces;
      }
      draw_text_line( fr, p, size, f, &ss );   
      if( *ss == '\n' ){ 
         ss++;
         p = vector( 0, f.f->font_char_size.y_get() + f.spacing.y_get() );         
      }   
   }               
}


// ==========================================================================
//
// frame
//

void frame::clear( const color c ){
   if( c != color::transparent ){  
      vector step = size.direction() ;
      for( int x = 0; x != size.x_get(); x += step.x_get()  ){
         for( int y = 0; y != size.y_get(); y += step.y_get() ){
            write( vector( x, y ), c ); } } } }    

         
// ==========================================================================
//
// wtoplevel
//
  
void wtoplevel :: run( void ){
   redraw();  
   for(;;){   
      event e = event_get();    
      if( e.event_type_get() == event_exit ) break;
      if( e.event_type_get() != event_none ){
         // trace << e; 
         handle( e );
         if( e.event_type_get() == event_down){
            handle( event( e.location_get(), event_touch ));    
         }
         redraw();
      }
   }      
}

} // namespace bmptks
} // namespace graphics
