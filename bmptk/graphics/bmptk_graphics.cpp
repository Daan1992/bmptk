// ==========================================================================
//
// refer to graphics.h for all information
//
// ==========================================================================

#include <cstdlib>
#include <iostream>
#include <iomanip>

#include "bmptk.h"
 
namespace bmptk {  

// ==========================================================================
//
// vector
//

vector operator * ( int n, const vector v ) {
   return vector( v * n ); }  

std::ostream & operator<<( std::ostream &s, const vector p ){
   s << std::dec << "(" << p.x_get() << "," << p.y_get() << ")";
   return s;
}


// ==========================================================================
//
// color
//

std::ostream & operator<<( std::ostream &s, const color c ){
   if( c.is_transparent() ){
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
   if( width < 1 || fg.is_transparent() ){
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
   circle_draw( f, position, bg, 1 );
   circle_draw( f, position, fg, 0 );
}

void circle::circle_draw_pixel( 
   frame &fr,
   const vector position,
   const vector v,
   const color c
) const {
   
   int start = width / 2;
   int end = (int) width - start;
   for( int x = - start; x < end; x++ ){
      for( int y = - start; y < end; y ++ ){
         if( abs( x ) + abs( y ) < (int) width ){
            drawable_draw_pixel( fr, position, v + vector( x, y ), c );   
         }   
      }
   }      
}   

void circle::circle_draw (
   frame &fr, 
   const vector position,
   const color c,
   bool fill
) const {

   // don't draw anything when the size would be 0 
   if( radius < 1 ){
      return;       
   }
   
   // http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
   
   int f = 1 - radius;
   int ddFx = 1;
   int ddFy = -2 * radius;
   int x = 0;
   int y = radius;
    
   if( fill ){
   
      // top and bottom
      drawable_draw_pixel( fr, position, vector( 0, + radius ), c );
      drawable_draw_pixel( fr, position, vector( 0, - radius ), c );

      // left and right
      line( vector( 2 * radius, 0 ), c ).draw( fr, position - vector( radius, 0 ));
      
   } else {
   
      // top and bottom
      circle_draw_pixel( fr, position, vector( 0, + radius ), c );
      circle_draw_pixel( fr, position, vector( 0, - radius ), c );

      // left and right 
      circle_draw_pixel( fr, position, vector( + radius, 0 ), c );
      circle_draw_pixel( fr, position, vector( - radius, 0 ), c );
   }   
    
   while( x < y ){
      
      // calculate next outer circle point
      if( f >= 0 ){
        y--;
        ddFy += 2;
        f += ddFy;
      }
      x++;
      ddFx += 2;
      f += ddFx;   
                    
      if( fill ){
         line( vector( 2 * x, 0 ), c ).draw( fr, position + vector( -x,  y ));
         line( vector( 2 * x, 0 ), c ).draw( fr, position + vector( -x, -y ));
         line( vector( 2 * y, 0 ), c ).draw( fr, position + vector( -y,  x ));
         line( vector( 2 * y, 0 ), c ).draw( fr, position + vector( -y, -x ));
      } else {
         circle_draw_pixel( fr, position, vector( + x, + y ), c );
         circle_draw_pixel( fr, position, vector( - x, + y ), c );
         circle_draw_pixel( fr, position, vector( + x, - y ), c );
         circle_draw_pixel( fr, position, vector( - x, - y ), c );
         circle_draw_pixel( fr, position, vector( + y, + x ), c );
         circle_draw_pixel( fr, position, vector( - y, + x ), c );
         circle_draw_pixel( fr, position, vector( + y, - x ), c );
         circle_draw_pixel( fr, position, vector( - y, - x ), c );
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
   unsigned int base = 3 * ( p.x_get() + p.y_get() * size.x_get() );    
   return color( data[ base ], data[ base + 1 ], data[ base + 2 ] );    
}

bool inline_bw_photo :: bool_read( const vector p ) const {
   unsigned int a = p.x_get() + p.y_get() * size.x_get();
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
         int char_size = fm.f->char_size( *s ).x_get() + fm.spacing.x_get();
         if(( x + char_size > size.x_get() ) && fm.wrap ){
            count++;    
            x = 0;                
         }
         x += char_size;                
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
   int spaces = line_chars(  *s, sz, fm ) - 1;
   int missing = 0;
   if( fm.h == align_far ){
      p += vector( extra, 0 );
   } 
   if( fm.h == align_centre ){
      p += vector( extra / 2, 0 );
   } 
   for( ; (**s != '\n') && (**s != '\0') ; (*s)++ ){           
      char_photo fc( *fm.f, **s, fm.fg, fm.bg );
      if( fm.h == align_fill ){    
         missing += extra; 
         int adjust = missing / std::max( spaces, 1 );
         p += vector( adjust, 0 );
         missing -= adjust * spaces;
      }
      fr.draw( p, fc );
      p += vector( fc.size.x_get() +fm.spacing.x_get(), 0 );
   }          
}

// draws the text
void text::draw( 
   frame &fx, 
   const vector fx_position
) const {

   subframe fr( fx, fx_position, fx.size_get() - fx_position, f.scale );

   vector p( 0, 0 );
   int lines = line_count( s, fr.size_get(), f ); 
   int extra = std::max( 
      0, 
      fr.size_get().y_get() - lines * f.f->font_char_size.y_get() );
   int missing = 0;
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
         int adjust = missing / lines; 
         // trace << extra << " " << missing << " " << adjust;
         p += vector( 0, adjust );
         missing -= adjust * lines;
      }
      draw_text_line( fr, p, size, f, &ss );   
      if( *ss == '\n' ){ 
         ss++;
         p = vector( 
            0,
            p.y_get() + f.f->font_char_size.y_get() + f.spacing.y_get() );         
      }   
   }               
}


// ==========================================================================
//
// frame
//

void frame::clear( const color c ){
   if( ! c.is_transparent() ){  
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

} // namespace bmptk
