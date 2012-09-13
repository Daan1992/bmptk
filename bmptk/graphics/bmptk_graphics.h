// ==========================================================================
//
// file: bmptk/graphics/bmptk-graphics.h
//
// ==========================================================================

#ifndef BMPTK_GRAPHICS_H
#define BMPTK_GRAPHICS_H

#include <iostream>
#include <limits>

namespace bmptk { 

// ==========================================================================
//
// class vector
//
//! a relative or absolute (= relative to (0,0)) location on a grid
//
//! A vector is a pair of 16-bit integer values that are the x and y 
//! coordinates of an absolute or relative location on an integer grid. 
//! A vector can be constructed from its x and y values. 
//! When no values are supplied a vector defaults to (0,0).
//!
//! Vectors are intended to represent a location, displacement, or
//! size on a garphics screen. 
//! 16 bits should be more than enough for this purpose.
//!
//! When a vector is used to identify a pixel on a screen (0,0)
//! is the top-left pixel.
//!
//! Two vectors can be added or subtracted to yield a new vector. 
//! A vector can be multiplied or divided by an integer to yield a new vector. 
//! Vectors can be compared for equality and inequality. 
//! The x_projection() and y_projection() functions return the 
//! projection of the vector on the two axises. 
//! The function is_within() tests whether a vector is within the 
//! box bounded by the origin and the argument. 
//! 
//! For debugging, a << operator is provided to print a vector.
//!
//! A vector fits in a single (32 bits) word.
//! It has no references to other objects.
//

class vector {
private:   
     
   //! returns the sign (-1, 0, 1) of the argument  
   static int sign( short int x ){ 
      return ( x > 0 ) - (x < 0); }  
      
   //! returns whether the x is within <m,0] or [0,m>    
   static bool is_within( short int x, short int m ){
      return (( x >= 0 ) && ( x < m )) || (( x <= 0 ) && ( x > m )); }

   //! the x coordinate
   short int x;
  
   //! the y coordinate
   short int y;

public:      

   //! construct a vector, the value defaults to (0,0)
   vector(): x( 0 ), y( 0 ) {}   
     
   //! construct a vector from its x and y coordinates
   vector( short int x, short int y ): x( x ), y( y ) {}   
     
   //! get the x component
   short int x_get() const { return x; }
   
   //! get the y component
   short int y_get() const { return y; }
   
   //! add two vectors by adding the x and y coordinates
   vector operator + ( const vector p ) const {
      return vector( x + p.x, y + p.y ); }          
      
   //! add a vector to an existing vector   
   vector operator += ( const vector p ){
      return vector( x += p.x, y += p.y ); }          
      
   //! return the vector unmodified   
   vector operator + ( void ) const {
      return vector( x, y ); }
   
   //! subtract two vectors by subtracting the coordinates
   vector operator - ( const vector p ) const {
      return vector( x - p.x, y - p.y ); }          
      
   //! subtract a vector from an existing vector   
   vector operator -= ( const vector p ){
      return vector( x -= p.x, y -= p.y ); }          
      
   //! returns the negative of the vector (point-mirrored in the origin)
   vector operator - ( void ) const {
      return vector( - x, - y ); }
   
   //! divides a vector by an integer by dividing the coordinates
   vector operator / ( int n ) const {
      return vector( x / n, y / n ); }          
      
   //! divides an existing vector by an integer   
   vector operator /= ( int n ){
      return vector( x /= n, y /= n ); }          
      
   //! multiplies a vector by an integer by multiplying the coordinates
   vector operator * ( int n ) const {
      return vector( x * n, y * n ); }         
      
   //! multiplies an existing vector by an integer
   vector operator *= ( int n ){
      return vector( x *= n, y *= n ); }         
      
   //! multiplying two vectors multiplies their X and Y components
   vector operator * ( const vector rhs ) const {
      return vector( x * rhs.x, y * rhs.y ); }         
      
   //! reports whether two vectors are equal  
   bool operator == ( const vector p ) const {
      return ( x == p.x ) && ( y == p.y );  }          

   //! reports whether two vectors are unequal
   bool operator != ( const vector p ) const {
      return ! ( *this == p );  }     
      
   //! returns the direction of a vector      
   //
   //! The direction of a vector is a vector that, 
   //! for both its x and y component, has the 
   //! value 1 or -1, and the same sign
   //! as the original vector.
   vector direction( void ) const {
      return vector( sign( x ), sign( y )); }           
           
   //! returns the absolute of a vector
   // 
   //! The absolute of a vector is the original vector,
   //! mirrored into the first quadrant by the x and y axises.
   //! In other words, the x and y of the absolute 
   //! are the absolutes of the original x and y.
   vector abs( void ) const {
      // std::abs is not present in DevKitPro??
      return vector( 
         ( x >= 0 ) ? x : -x,
         ( y >= 0 ) ? y : -y  ); 
   }  
      
   //! returns the projection of the vector on the x axis
   //
   //! In other words, the vector with y set to 0.   
   vector x_projection( void ) const {
      return vector( x, 0 ); }        
      
   //! returns the projection of the vector on the y axis
   //
   //! In other words, the vector with x set to 0.
   vector y_projection( void ) const {
      return vector( 0, y ); }        
      
   //! returns with x and y swapped
   //
   //! In other words, the return vector as mirrored in the y=x line.
   vector mirrored( void ) const {
      return vector( y, x ); }        
      
   //! reports whether te vector is within the box [0,p>
   //   
   //! The call a.is_within(b) returns true iff a is within 
   //! the rectangle [(0,0),b], where the boundary lines that go through (0,0) 
   //! are included, but the boundaries that go through b 
   //! are excluded.
   bool is_within( const vector p ) const {
      return is_within( x, p.x ) && is_within( y, p.y ); }
      
   //! the vector (0,0)   
   static vector origin(){ return vector(0,0); }
};

//! multiplies a vector by an integer by multiplying the coordinates
vector operator * ( int n, const vector v ); 

//! prints a vector
//
//! \relates vector
//! This operator prints a vector in the (\%d,\%d) format
std::ostream & operator<< ( std::ostream &s, const vector p );


// ==========================================================================
//
// class color
//
//! a color (in the 3 * 8 bit RGB format + transparency)
//
//! A color is represented by three 8-bit values (0..255) for the 
//! red, green and blue components, 
//! and an additional boolean flag to indicate transparency 
//! (in which case the rgb values are irrelevant). 
//! Transparency is recessive: when a transparent color is combined with 
//! a non-transparent color the result is a non-transparent color. 
//!
//! A color can be constructed from is three components, 
//! or from a 3-byte value (as is commonly used in for instance HTML). 
//! Two colors can be added or subtracted, or be compared for equality. 
//! A color can be multiplied or divided by an integer.
//!
//! For debugging, a << operator is provided to print a color.
//!
//! Constants are provided for some \ref basic_colors basic colors. 
//!
//! This color format is emissive: when colors are added (or multiplied
//! by an integer >1), they add up towards white. Likewise subtracting 
//! works towards black.
//!
//! A color fits in a single (32 bits) word.
//! It has no references to other objects.
//

class color {  
private:    
              
   static int clip( int x ){
      return ( x < 0 ) ? 0 : std::min( x, 0xFF ); }        

   bool transp;
   unsigned char r; 
   unsigned char g;
   unsigned char b; 
   
public:      
   
   //! constructs a color object from its three components
   //
   //! You can also supply the transparency flag, but better use
   //! the color::transparent constant.
   //! 
   //! Note that the arguments are integers, but they are clipped
   //! to the 0..255 range by the constructor.
   color( int r, int g, int b, bool t = 0 ):
      transp( t ), r( clip( r )), g( clip( g )), b( clip( b ))
   {
      if( t ){ r = g = b = 0; }
   }
      
   //! constructs a color from a three-byte value
   //
   //! For instance, color( 0xFF00FF ) is magenta.   
   color( unsigned int rgb = 0 ):
      transp( 0 ),  
      r( clip( ( rgb & 0xFF0000 ) >> 16 )), 
      g( clip( ( rgb & 0x00FF00 ) >>  8 )), 
      b( clip( ( rgb & 0x0000FF ) >>  0 )){}
      
   //! the color in RGB15 format
   unsigned int rgb15( void ) const {
      return (
           ((unsigned int)( r & 0xF8 ) >>  3 )
         | ((unsigned int)( g & 0xF8 ) <<  2 )
         | ((unsigned int)( b & 0xF8 ) <<  7 ) 
      );
   }      
   
   //! transparency indicator
   //
   //! When a color is transparent its colors are irrelevant.
   bool is_transparent() const { return transp; }
   
   //! return the red component
   //
   //! The returned component is relevant only when the
   //! color is not transparent.
   unsigned char red_get() const { return r; } 
   
   //! return the green component
   //
   //! The returned component is relevant only when the
   //! color is not transparent.
   unsigned char green_get() const { return g; }
   
   //! return the blue component
   //
   //! The returned component is relevant only when the
   //! color is not transparent.
   unsigned char blue_get() const { return b; }    

   //! adds two colors; addition works towards white
   color operator + ( const color c ) const {
      return color( 
         r + (int)c.r, 
         g + (int)c.g, 
         b + (int)c.b, 
         transp && c.transp ); }
      
   //! adds a color to an existing color   
   const color & operator += ( const color c ){
      r = clip( r + (int)c.r ); 
      g = clip( g + (int)c.g ); 
      b = clip( b + (int)c.b ); 
      transp = transp && c.transp; 
      return *this;
   }
      
   //! returns a color unmodified   
   color operator + ( void ) const {
      return color( r, g, b, transp ); }          
      
   //! subtracts two colors; subtraction works towards black      
   color operator - ( const color c ) const {
      return color( 
         r - (int)c.r, 
         g - (int)c.g, 
         b - (int)c.b, 
         transp && c.transp ); }
      
   //! subtracts a color from an existing color   
   const color & operator -= ( const color c ){
      r = clip( r - (int)c.r ); 
      g = clip( g - (int)c.g );
      b = clip( b - (int)c.b );
      transp = transp && c.transp; 
      return *this;
   }          
      
   //! returns the negative of a color   
   color operator - ( void ) const {
      return color( 
         0xFF - (int)r, 
         0xFF - (int)g, 
         0xFF - (int)b, 
         transp ); }
      
   //! divides a color by an integer; division works towards black   
   color operator / ( unsigned int n ) const {
      return color( r / n, g / n, b / n, transp ); }
      
   //! divides an exiting color by an integer   
   const color & operator /= ( unsigned int n ){
      r = clip( r / n ); 
      g = clip( g / n ); 
      b = clip( b / n ); 
      return *this;
   }
      
   //! muliplies a color by an integer; multiplication works towards white   
   color operator * ( unsigned int n ) const {
      return color( r * n, g * n, b * n, transp ); }
   
   //! multiplies an existing color by an integer 
   const color & operator *= ( unsigned int n ){
      r = clip( r * n ); 
      g = clip( g * n ); 
      b = clip( b * n ); 
      return *this;
   }
      
   //! reports whether two colors are equal   
   bool operator == ( const color c ) const {
      return transp ? c.transp : 
         ( ! c.transp ) && ( r == c.r ) && ( g == c.g ) && ( b == c.b ); }

   //! reports whether two colors are unequal
   bool operator != ( const color c ) const {
      return ! ( *this == c );  }   
   
   //! returns the mix of the current color and the argument   
   color mix( const color c ){
      return color( 
         ( r + c.r ) / 2, 
         ( g + c.g ) / 2, 
         ( b + c.b ) / 2,
         transp && c.transp ); }          
         
   static color blaack(){ return color( 0,       0,    0 ); }
   static color transpaarent(){ return color( 0, 0, 0, 1 ); }

   //! some basic colors
   //
   //! These static member functions return some common colors.
   //! These are not plain constants to avoid the static
   //! initialisation order problem: inline_font objects
   //! would have a problem with the balck and white parameters
   //! for their drawable subpart.

   // these 'constants' are delibreately NOT Doxygen-documented     
   static color black()       { return color( 0,       0,    0 ); }   //!< ...
   static color white()       { return color( 0xFF, 0xFF, 0xFF ); }   //!< ...
   static color red()         { return color( 0xFF,    0,    0 ); }   //!< ...
   static color green()       { return color( 0,    0xFF,    0 ); }   //!< ...
   static color blue()        { return color( 0,       0, 0xFF ); }   //!< ...
   static color gray()        { return color( 0x80, 0x80, 0x80 ); }   //!< ...
   static color yellow()      { return color( 0xFF, 0xFF,    0 ); }   //!< ...
   static color cyan()        { return color(    0, 0xFF, 0xFF ); }   //!< ...
   static color magenta()     { return color( 0xFF,    0, 0xFF ); }   //!< ...
   static color transparent() { return color( 0, 0, 0, 1 );       }   //!< ...
   static color violet()      { return color( 0xEE82EE );         }   //!< ...
   static color sienna()      { return color( 0xA0522D );         }   //!< ...
   static color purple()      { return color( 0x800080 );         }   //!< ... 
   static color pink()        { return color( 0xFFC8CB );         }   //!< ...
   static color silver()      { return color( 0xC0C0C0 );         }   //!< ...
   static color brown()       { return color( 0xA52A2A );         }   //!< ...
   static color salmon()      { return color( 0xFA8072 );         }   //!< ...

};

//! print a color
//
//! \relates color
//! This operator prints a color, either as "(transparent )",
//! or in the (\%02X,\%02X,\%02X) format.
std::ostream & operator << ( std::ostream &s, const color c );


// ==========================================================================
//
// class event
//

//! types of events
//
//! \relates event
//!
//! This type represents a user action on the screen.
//
enum event_type { 

   //! nothing happened
   //
   //! This event type is used when an event object is required,
   //! but nothing happened. 
   //! The location has no meaning.
   event_none, 
   
   //! user requests application exit
   //
   //! This event type is used when the user requests the
   //! (sub)application to terminate. 
   //! The location has no meaning. 
   event_exit, 
   
   //! user lands on the screen
   //
   //! This event type is used when the user lands on the screen:
   //! the mouse button is pressed down, or the stylus first
   //! contacts the screen. 
   //! The location indicates where this happened.   
   event_down, 
  
   //! user moves while touching the screen
   //
   //! This event type is used when the user touches a new pixel,
   //! either because the cursor or stylus first touched down on 
   //! the screen, or because the the mouse cursor or stylus 
   //! was down and moved to a different pixel.
   //! The location indicates the (new) pixel.
   //!
   //! This event type would typically be used by a freehand paint
   //! application.
   event_touch, 
  
   //! user takes off from the screen
   //
   //! This event type is used when the user has landed on the screen
   //! but now takes off (releases the mouse button, or moves the
   //! stylus from the screen).
   //! The location is the last location where the user was on
   //! the screen.
   event_up, 
  
   //! user clicks on the screen
   //
   //! This event type is used when the user clicks on the screen,
   //! which is the same as down and up in rapid succession.
   //! The down and up events will also be generated.
   //! The location is the location of the up event.
   event_click 
};

//! return the name of an event_type
//!
//! \relates event
//
const char * event_type_name( const event_type e );

//! prints the name of an event_type
//!
//! \relates event
//
std::ostream & operator<<( std::ostream &s, const event_type &e );

//! something that the users caused to happen on the screen
//
//! An event is a something that happens at a specific position on 
//! a screen. A typical event would be the user touching the screen
//! with a stylus, or clicking on a certain position with a mouse.
//!
//! When a user 
//! - touches the screen at (10,10);
//! - moves the stylus to (11,11) and to (12,12);
//! - and then removes the stylus from the screen,
//!
//! the following events will be generated in this order:
//!
//! - down:(10,10)
//! - touch:(10,10)
//! - touch:(11,11) 
//! - touch(12,12)
//! - up:(12,12)
//! - click:(12,12)
//! 
//! The last event (click) will only be generated if the down and 
//! the touch ocurred within rapid succession.
//
class event {

   //! the location where the event ocurred
   //!
   //! For some event types the location is irrelevant.
   const vector location;
   
   //! the type of the event that ocurred
   const event_type e;
   
public:

   //! creates an event from its location and type
   event( const vector p, const event_type e ):
      location( p ), e( e ) {}   
      
   //! get the location
   vector location_get() const { return location; }

   //! get the event_type
   event_type event_type_get() const { return e; }
   
};

//! prints an event
//
//! \relates event
//! Example: 
//!
//! touch:(47,120)
std::ostream & operator<<( std::ostream &s, const event &e );


// forward declaration needed because drawable
// must know its frame
class frame;


// ==========================================================================
//
// class drawable
//
//! an object that can be drawn in a frame
//
//! A drawable is an abstract object that can be drawn in a frame.
//! A drawable knows the frame the location (anchor point) within this
//1 frame where it can draw itself, and
//! it has has has a forgeground and background color
//! and a (line) width.
//! What these properties exactly mean depends on the specific drawable.
//!
//! A drawable contains two colors and one integer. 
//! It has a reference to a frame.
//! This frame must still exist when the drwabale is asked to draw itself.
//
class drawable {
protected:

   //! parent frame
   frame &fr;
   
   //! the position
   vector position;

   //! foreground color
   color fg;
   
   //! background color
   color bg;
   
   //! width of (for instance) lines
   unsigned int width;  
   
   //! draw one pixel on f, at position
   //
   //! This is the function that the draw function in a concrete drawable
   //! should use to draw each pixel of itself.
   //! Address is the location of the pixel that is to be drawn, c its color.
   void drawable_draw_pixel( 
      const vector address, 
      const color c 
   ) const;
         
   //! constructor, specify frame, position, fg, bg colors, (line) width
   //
   //! The default is to draw in black forgeground, with transparent 
   //! (meaning: do not draw) background, line width (when applicable)
   //! 1 pixel.
   drawable( 
      frame &fr,
      const vector position = vector::origin(),
      const color fg = color::black(), 
      const color bg = color::transparent(),
      unsigned int width = 1
   ): fr( fr ), position( position ), fg( fg ), bg( bg ), width( width ) {}
   
public:    

      //! get the parent frame
   frame &frame_get() const { return fr; }
   
   //! get the position
   const vector position_get() const { return position; }
     
   //! get the forgeround color
   color fg_get() const { return fg; }
      
   //! get the background color
   color bg_get() const { return bg; }
      
   //! get the width
   unsigned int width_get() const { return width; }
   
   //! set the position
   void position_set( const vector pos ){ position = pos; }
      
   //! set the forgeround color
   void fg_set( const color c ){ fg = c; }
      
   //! set the background color
   void bg_set( const color c ){ bg = c; }
      
   //! set the width
   void width_set( unsigned int w ){ width = w; }
   
   //! draw yourself
   virtual void draw() const = 0;
      
};


// ==========================================================================
//
// class frame
//
//! a rectangular block of writable pixels
//
//! A frame is an abstract class, representing a rectangle block of pixels.
//! Pixels are addressed by vectors.
//! By convention, (0,0) is the upper left pixel (origin).
//! Each pixel has a color, which can be written.
//!
//! The pixel operation checked_write is pure virtual. 
//! This operations must be implemented by a concrete frame.
//

class frame {
private:

protected:
    
   //! frame size
   const vector size;
   
   //! write one pixel 
   //
   //! A concrete frame class must implement this function.
   //! It can safely assume that the address is within the frame, and
   //! that the colcor is not transparent.
   virtual void checked_write( const vector p, const color c ) = 0;  
   
public:          

   //! create a frame of specified size
   frame( const vector size ): size( size ) {}
   
   //! get the size of the frame
   vector size_get() const { return size; }
            
   virtual vector translate( const vector p ) const {
      return p; }   
   
   //! whether p is within the frame   
   bool is_valid( vector p ){
      return p.is_within( size ); }   
   
   //! write one pixel, address specified by vector
   //
   //! This function checks for transparancy (which is interpreted
   //! as 'do not write') and checks wether the p is valid
   //! (writing to a pixel outside the frame is ignored).
   //! Only when both checks succeed it will call checked_write
   //! to do the actual writing.
   void write( const vector p, const color c ){
      if( is_valid( p ) && ( ! c.is_transparent() )){
         checked_write( p, c ); }}        
   
   //! fill the full frame with the indicated color
   virtual void clear( const color c = color::white() );
};


// ==========================================================================
//
// class frame_dummy
//
//! writing to this frame does nothing
//

class frame_dummy : public frame {
public:
   frame_dummy( const vector size = vector::origin() ): frame( size ){}
protected:

   //! checked_write implementation as requiredby frame
   //
   //! This method does nothing.
   //! 
   void checked_write( const vector p, const color c ){}
};


// ==========================================================================
//
// class subframe
//
//! frame that represents a rectangular part of another frame
//
//! A subframe is created by specifying the master frame, the top-left pixel
//! of the subfram within the master frame, and the direction
//! in which the subframe extends within the master frame.
//! ( top_left + direction - direction.direction() ) == 
//! bottom-right pixel of the subframe.
//! The direction can point towards all four quadrants, hence
//! the subframe can appear mirrored within the master frame.
//!
//! The default scale is 1, which causes the subframe to appear
//! 1:1 in the master frame. When the scale is 0 it does not appear,
//! and reading a pixel from the subframe will always 
//! return color::transparent.
//!
//! When the scale factor is larger than 1, the subframe will expand its 
//! use of the area in the master by the scale factor, starting from
//! the upperleft (as supplied to the subframe constructor). 
//! In other words, the size inside the subframe remains the same,
//! but the area it occupies in the master frame is larger.
//!
//! When the scale is large than 1, the topleft pixel of the subframe
//! still occupies the pixel at the original place in the master frame,
//! but also its neigbouring pixels in the direction towards the bottomright
//! pixel.
//

class subframe : public frame {
public:

   //! the master of this subframe
   frame & master;              
   
   //! location within the master of this subframes topleft pixel
   const vector top_left;
   
   //! location within the master of this subframes bottomright pixel
   const vector bottom_right;
   
   //! the scale at which this subframe appears in its master
   const unsigned int scale;
   
   //! whether the subframe coordinates are swapped
   const bool swapped;
         
   //! translate a subframe coordinate to a master frame coordinate      
   vector translate( const vector p ) const {
      vector x( 
         top_left.x_get() < bottom_right.x_get()
            ? top_left.x_get() + p.x_get() 
            : top_left.x_get() - p.x_get(),
         top_left.y_get() < bottom_right.y_get()  
            ? top_left.y_get() + p.y_get()
            : top_left.y_get() - p.y_get()
       );
	   if( swapped ){
          x = x.mirrored();
	   }       
	   return x;
	}   
      
   //! create a subframe, endpoints specified as vectors
   subframe( 
      frame &f, 
      vector top_left, 
      vector direction,
      unsigned int scale = 1,
	  bool swapped = false
   ):
      frame( direction.abs() ),
      master( f ), 
      top_left( top_left ), 
      bottom_right( top_left + direction - direction.direction() ),
      scale( scale ),
	  swapped( swapped ){}
  
protected:

   //! checked_write implementation as requiredby frame
   //
   //! When scale == 0 checked_write will do nothing, hence the
   //! subframe will not appear in the master frame.
   //! 
   void checked_write( const vector p, const color c ){
      if( is_valid( p )){
         vector q = swapped ?  p.mirrored() : p;
         for( unsigned int x = 0; x < scale; x++ ){
            for( unsigned int y = 0; y < scale; y++ ){       
               master.write( translate( q * scale + vector( x, y )), c ); 
            }}}}
                          
};


// ==========================================================================
//
// class line
//
//! a drawable line object
//
//! A line object is a drawable that represents a line.
//! When it draws itself it will use the fg of its drawable part
//! for the line, the bg of its drawable part will not be used.
//! The line will be drawn using the specified width, by drawing
//! adjacent pixels.
//! The size of a line is its lenghth in the x and y directions,
//! \b not its endpoint (it is one pixel beyond the end).
//

class line : public drawable {
public:    

   //! the length of the line; can be read or written
   //
   //! The position of a drawable is specified when it is drawn, 
   //! it is not part of the drawable. You can think of the line
   //! starting at the origin. The size is the length in the
   //! x and y directions (not the endpoint!).
   vector size;
   
   //! constructs a line from its frame, position, size, color, and width
   line( 
      frame &frame,
      const vector position,
      const vector size, 
      const color fg = color::black(), 
      int width = 1 
   ):
      drawable( frame, position, fg, color::transparent(), width ), size( size ){}
      
        
   //! draw the line on f, at position, and to scale   
   void draw() const;
};    


// ==========================================================================
//
// class rectangle

//! appearance of the border of a rectangle
//!   
//! \relates rectangle
enum relief {  

   //! the border is drawn flat, using the fg color
   relief_flat,
 
   //! the border is drawn raised, using the bright and shadow colors
   relief_raised, 
   
   //! the border is drawn raised, using the shadow and bright colors
   relief_sunken
}; 
   
//! returns the inverse of the supplied relief 
//!   
//! \relates rectangle
relief flip( const relief r );   

//! prints a relief value
//!   
//! \relates rectangle
std::ostream & operator<<( std::ostream &s, const relief &r );

//
//! a drawable rectangle object
//
//! A rectangle object is a drawable that represents a rectangle.
//! By default, when it draws itself it will use the fg of its 
//! drawable part for its boundary lines, which will be filled 
//! to the width by drawing extra lines within the outer boundary.
//! The remaining area within will be filled using the bg.
//!
//! When the relief is raised, the top and left boundary will be draw
//! using the bright color, and the bottom and right boundary will be
//! drawn using the dark color. When the relief is sunken, this
//! is reversed. The initial relief is flat.
//

class rectangle : public drawable {
public:

   //! bright border color; can be read or written; intitally fg
   //
   //! This is the color used for the bright parts of teh border
   //! when its relief is raised or sunken.
   color bright;
   
   //! dark border color; can be read or written; intially fg
   //
   //! This is the color used for the dark parts of the border
   //! when its relief is raised or sunken.
   color dark;
   
   //! the relief; can be read or written; initially flat
   //
   //! The relief used to draw the border.
   relief border;
   
   //! the position of the far corner; can be read or written
   //
   //! The position of a drawable is specified when it is drawn, 
   //! it is not part of the drawable. You can think of the origin
   //! being the near corner.
   vector size;
   
   //! constructs a rectangle from its far corner, bg, fg, width and relief
   rectangle(
      frame &frame,
      const vector position,
      const vector size, 
      const color fg     = color::black(),
      const color bg     = color::transparent(),
      int width          = 1,
      relief rel         = relief_flat
   ):
      drawable( frame, position, fg, bg, width ), 
      bright( fg ), 
      dark( fg / 4 ), 
      border( rel ),
      size( size ){}       

   //! draw the rectangle on f, at position
   void draw() const;
};  
    
    
// ==========================================================================
//
// class circle
//
//! a drawable circle object
//
//! A circle object is a drawable that represents a circle.
//! When it draws itself it will use the fg of its drawable part
//! for its boundary line, which will be filled to the width
//! by drawing extra circles within the outer boundary.
//! The remaining area within will be filled using the bg.
//

class circle : public drawable {
private:

   void circle_draw(
      const color c,
      bool fill
   ) const;
   
   void circle_draw_pixel(  
      const vector v,
      const color c
   ) const;

public:    

   //! the radius of the circle
   //!
   //! The position of a drawable is specified when it is drawn, 
   //! it is not part of the drawable. You can think of the origin
   //! being the centre.
   unsigned int radius; 
   
   //! create a circle from a radius, fg and bg colcors, and line width
   circle( 
      frame &frame,
      const vector position,
      unsigned int radius,
      const color fg = color::black(),
      const color bg = color::transparent(),
      unsigned int width = 1
   ):
      drawable( frame, position, fg, bg, width ), radius( radius ){}        
      
   //! draw the circle on f, at position
   void draw() const; 
};    


// ==========================================================================
//
// class image
//
//! a recangular block that can be drawn at a position in a frame
//
//! An image can draw itself in a frame.
//
class image {
public:
	   
   //! draw the picture in the frame at the position
   virtual void draw( 
      frame &frame, 
      const vector position = vector::origin() ) const = 0;      
};


// ==========================================================================
//
// class image_pixels
//
//! an image that can be read pixel by pixel
//
//! An image_pixels is an image, hence it can draw itself in a frame.
//!
//! Additionally, each pixel of the image_pixels can be read.
//! A concrete child class must implement a \ref checked_read() 
//! function that returns the color of the requested pixel. 
//! The image_pixels class itself provides a read() function that first 
//! checks whether the requested pixel is within the area (otherwise 
//! it returns color::transparent) and if so, calls \ref checked_read() 
//! to do the real work. 
//
class image_pixels : public image {
protected:	

   //! the size of the (rectangular) block occupied by the objecy	
   vector size;
	
   //! get the color of the specified pixel    
   //
   //! This function must be provided by a concrete photo class
   //! that depends on the 
   //! It can assume that the requested pixel is valid (within
   //! the photo).
   virtual color checked_read( const vector p ) const = 0;

public:

   //! constructor, specify the size
   image_pixels( vector size ): size( size ){}	
	      
   //! get the size of the picture	   
   vector size_get() const { return size; }	
   
     //! report whether p is within the image   
   bool is_valid( const vector p ) const {
      return p.is_within( size ); }  
   
   //! draw the picture in the frame at the position
   //!
   //! The default draw method reads each pixel and writes it in the frame.
   //! A concrete image class might provide a more efficient draw method,
   //! that draws only the pixels that are not transparent.
   virtual void draw( 
      frame &frame, 
      const vector position = vector::origin() ) const;
      
   //! read one pixel, address specified by vector
   //
   //! This function will return color::transparent
   //! when the requested pixel is outside the image.
   //! Otherwise it will call \ref protected_read.
   color read( const vector p ) const {
      if( is_valid( p )){
         return checked_read( p );
      } else {
         return color::transparent();
      }               
   }       
};


// ==========================================================================
//
// class inline_rgb_photo
//
//! a color photo stored as inline constant data (in ROM)
//
//! The class inline_rgb_photo is a concrete child of the photo class.
//! It stores the colors of the pixels in an array of constant data. 
//! Inline_rgb_photo uses 3 bytes of red, green, and blue per pixel
//! (\ref inline_bw_photo uses 1 bit per pixel). 
//! The inline.py tool can be used to convert a .jpg or .bmp file into the 
//! .h and .cpp file for an inline_rgb_photo.
//! This is likely to be the only way you will instatiate this class.
//

class inline_rgb_photo : public image_pixels {
private:

   //! the pixel data     
   const unsigned char *data;  
   
   //! read one pixel
   color checked_read( const vector p ) const;
     
public:     
   
   //! create an inline_rgb_photo object   
   inline_rgb_photo( vector size, const unsigned char *data ):
      image_pixels( size ), data( data ){}        

};

// ==========================================================================
//
// class inline_bw_photo
//
//! a black-and-wite photo stored as inline constant data (in ROM)
//
//! The class inline_bw_photo is a concrete child of the photo class.
//! It stores the colors of the pixels in an array of constant data. 
//! Inline_wb_photo uses 1 bit per pixel
//! (\ref inline_rgb_photo uses 3 bytes per pixel). 
//! The inline.py tool can be used to convert a .jpg or .bmp file into the 
//! .h and .cpp file for an inline_bw_photo.
//! This is likely to be the only way you will instantiate this class.
//!
//! Each pixel can either be on or off. An 'on' pixel returns the
//! fg colcor (default: color::black), an off pixel returns the 
//! bg colcor (deafult: color::white).
//!
//! The inline_bw_photo class can be used instead of \ref inline_rgb_photo
//! to store a picture in a more compact form. It is also used by the
//! library to store fonts.
//

class inline_bw_photo : public image_pixels {
private:

   //! the pixel data     
   const unsigned char *data;  

   color fg, bg;

public:        
    
   //! read one pixel, return bool
   bool bool_read( const vector p ) const; 
   
   //! create an inline_bw_photo object   
   inline_bw_photo( 
   
      //! the size of the photo
      const vector size,   
      
      //! the pixel data
      //
      //! The data contains one bit per pixel, x address varies fastest.
      //! No filling except (when needed) the last byte.
      //! The bits within a byte are stored LSB first.
      const unsigned char *data 
   ):
      image_pixels( size ), 
	  data( data )
   { 
      fg = color::black(); 
      bg = color::white(); 
   }        
      
private:
        
   //! read one pixel, return color
   color checked_read( const vector p ) const {
      return bool_read( p ) ? fg : bg; }                    
};


// ==========================================================================
//
// class font
//
//! the photo's (bitmaps) of characters
//
//! A font is an abstract class that holds the pixel-level black and white 
//! representation of a set of characters, according to some font rules.
//! You can read the pixels for a specific character.
//!
//! The common way to use a font is to use it to construct
//! a \ref text object, which can subsequently be drawn.
//!
//! A concrete font class must provide the has() and read() functions.
//

class font {
public:    
    
   //! true iff the font is fixed-width (all characters are the same width)
   bool const fixed;
   
   //! true iif the font is proportional (width of the characters varies)
   bool const proportional;
   
   //! the size of one character, x is valid for fixed fonts only 
   const vector font_char_size;
   
   //! create a font, specify its fixedness and character size
   font( bool fixed, const vector char_size ):
      fixed( fixed ), proportional( ! fixed ), font_char_size( char_size ){}     
    
   //! reports whether the font has the char
   //
   //! To reduce ROM size, a font can contain a subset of the ASCII
   //! characters.
   virtual bool has( char c ) const = 0;
       
   //! read pixel p of characters c    
   virtual bool read( char c, const vector p ) const = 0;
   
   //! reports te size of character c
   //
   //! This function must report (0,0) for a character that is 
   //! not in the set (for which \ref has is false).
   virtual vector char_size( char c ) const = 0;
    
};


// ==========================================================================
//
// class char_photo
//
//! a photo of a single char from a font
//
//! A char photo is the photo of one character from a \ref font.
//! It is a photo, so it can be drawn.
//! Temporary char_photo objects are used by \ref text to draw a
//! text in a specified front. 
//!
//! Unless you want to drawn characters in a way that is not provided
//! by \ref text it is unlikely that you will ever use this class.
//! 
//! A char_photo contains a reference to its font object.
//! Using a char_photo object when the font object is no longer available
//! will cause unpredictable results. 
//! When used with an \ref inline_font, which is a global const 
//! (readonly) object, so it can not become unavailable
//

class char_photo : public image_pixels {   
public:    
       
   //! the font     
   const font &f;  
    
   //! the character 
   const char c;
   
   //! the foreground colcor (default: black)
   const color fg;
   
   //! the background colcor (default: transparent)
   const color bg;

   //! instatiate a char_photo, specfy font, char, forgeground and background
   char_photo( 
      const font &f,
      char c, 
      const color fg = color::black(), 
      const color bg = color::transparent()
   ):
      image_pixels( f.char_size( c )),
      f( f ),
      c( c ), 
      fg( fg ), 
      bg( bg )
   {}
   
   //! read one pixel, return color
   color checked_read( const vector p ) const {
      return f.read( c, p ) ? fg : bg; }      
         
};


// ==========================================================================
//
// class inline_font
//
//! a font stored as inline constant data (in ROM)
//
//! An inline_font is a photo of a line of characters, stored 
//! in in-line data (in ROM).
//! As it is a photo, you can draw it in a frame to see how the
//! characters look.
//!
//! The common way to use a font is to use it to construct
//! a \ref text object, which can subsequently be drawn.
//!
//! The inline.py tool can be used to convert a .jpg or .bmp file into the 
//! .h and .cpp file for an inline_font.
//! This is likely to be the only way you will instantiate this class.
//

         // font type
         // picture is a picture of the characters in the font, 
         //    stored as one long line
         // when list is NULL the characters 0x20 .. 0x7F are supported,
         //    otherwise it is a (0-terminated) list of characters
         // for a fixed-width font x_size is the width of a char,
         //    and start is NULL
         // for a proportional font x_size is 0, and
         //    start points to an array with the x-offsets of each
         //    character. The first offset is of course 0, but
         //    still stored, and there are 0x61 offsets (not 0x60!)

class inline_font : public font, public inline_bw_photo {
public: 
    
   //! x offset within the photo of all ASCII characters
   const int *start; 
   
   //! instantiate an inline font
   inline_font( 
   
      //! true iif the font has the same size for each character
      bool fixed,
      
      //! the size of a character, x is valid for fixed fonts only
      const vector char_size,
      
      //! the x offset in the photo of each char, -1 for missing chars
      const int *start,
      
      //! the size of the photo
      const vector size,
            
      //! the photo pixels, see \ref inline_bw_photo
      const unsigned char *data
   ):         
      font( fixed, char_size ), 
      inline_bw_photo( size, data ),
      start( start )
   {}        
           
   //! report whether the font has a photo for char c        
   bool has( char c ) const { 
      if( ( c < ' ' ) || ( c >= 127 ) ) return false;
      return start[ (int) c - 32 ] >= 0; 
   }
      
   //! report the size of the photo for char c   
   vector char_size( char c ) const;
   
   //! read one pixel, return true iff it is foreground
   bool read( char c, const vector p ) const;
};


// ==========================================================================
//
// class format

//! the alignment of a text
//!   
//! \relates format
enum font_alignment { 
	
	 //! the text is put nearest to the near edge
   align_near, 
   
	 //! the text is put nearest to the far edge
   align_far, 
   
	 //! the text is centered between the edges   
   align_centre, 
   
   //! the text is space-filled to occupy the space between the edges
   align_fill 
};

//! prints an alignment
//!   
//! \relates format
std::ostream & operator<<( std::ostream &s, const font_alignment &a );

//! returns the default font
const inline_font & font_default();

//
//! specifies how a text is rendered on a screen 
//
//! A format specifies how an ASCII text is shown on a screen.
//! A format specifies the following aspects:
//!
//! - the font to be used
//! - the horizontal alignment
//! - the vertical alignment
//! - whether a line is chopped at the right edge or wrapped to the next line
//! - the foreground colcor to be used
//! - the background color to be used
//! - the margins between the text and the edges
//! - the scale factor
//!
//! These aspects are all stored in public attributes and can
//! be read and written.
//

class format {
public:
   const font *f;
   font_alignment h;
   font_alignment v;
   bool wrap;
   unsigned int scale;
   vector spacing;
   vector top_left_margin;
   vector bottom_right_margin;
   color fg;
   color bg;
   
   format( 
      const font &f = font_default(),
      font_alignment h = align_near, 
      font_alignment v = align_near, 
      bool wrap = 0,
      unsigned int scale = 1,
      vector spacing = vector::origin(),
      vector top_left_margin = vector( 2,2 ),
      vector bottom_right_margin = vector( 2,2 ),
      color fg = color::black(),
      color bg = color::transparent()
   ): 
      f( &f ),         
      h( h ), 
      v( v ), 
      wrap( wrap ),
      scale( scale ),
      spacing( spacing ),
      top_left_margin( top_left_margin ),
      bottom_right_margin( bottom_right_margin ),
      fg( fg ),
      bg( bg )
   {}
   
};    

//! print a format
std::ostream & operator<<( std::ostream &s, const format &f );


// ==========================================================================
//
// class text
//
//! a formatted text
//
//! This class is used to draw a formatted text in a frame.
//! A format object contains a \b copy of its \ref format, and a
//! \b pointer to its text string. Hence the character string is not copied, 
//! so it must still be available when the text object is drawn.
//!
//
class text : public image {
public:    
	
	 //! the ASCII text string; can be changed
   const char *s;
     
   //! the format used to draw the text; can be changed
   format f;    
   
   //! instatiate a text object
   text( 
      //! pointer to the 0-terminated character string
      //
      //! This string is not copied, only the address is stored.
      //! You can change the content of the string, or even the
      //! pointer (the \ref s attribute), as you see fit.
      //! Note: the string must still be availabe when the text is drawn
      const char *s,
           
      //! the format used to draw the text
      //
      //! This format is copied. You can change the 
      //! copy (the \ref f attribute) as you see fit.
      const format f = format()
   ): s( s ), f( f ){}        
      
   //! draw the text on f, at position
   //
   //! The text is drawn according to the current format 
   //! (the \ref f attribute), and the text that is drawn is the 
   //! current string (whatever the \ref s attribute points to).
   void draw( frame &f, const vector position = vector::origin() ) const; 
};   

// ==========================================================================
//
// A sheet is a graphical object, ultimately to be shown in a frame,
// that can show ASCII text
//

#ifdef nono
class sheet_base : public drawable {
protected:
   const char * buffer;
   unsigned int size;   
   sheet_base( ){}
   void add( char c );
   void clear(){}
   void draw(){}
} 

template< unsigned int charsize > class sheet : public sheet_base {
   char buffer[ charsize + 1 ];
public:
   sheet( ): sheet_base( , ){}
}



// ==========================================================================
//
// A widget is a graphical object, ultimately to be shown in a frame,
// that can handle events in some way, and can draw itself.
//

class widget {
protected:
   widget * children, * next_child;
public:   
   const widget *toplevel, *parent;
   frame *inner;
   bool changed;
   
   widget( widget * toplevel, widget * parent, frame *f ): 
      children( 0 ), next_child( 0 ), 
      toplevel( toplevel ), parent( parent ), 
      inner( f ), changed( 1 ) 
   {}
      
   widget( widget * parent, frame *f ): 
      children( 0 ), next_child( 0 ),
      toplevel( parent->toplevel ), parent( parent ), 
      inner( f ), changed( 1 )
   {
      parent->add( this ); }
      
   void add( widget * w ){
      w->next_child = children; 
      children = w; }           
         
   void redraw( bool forced = 0 ){
      if( forced ){
         changed = 1;
      }         
      if( changed ){   
         draw();
      }
      for( widget *w = children; w != 0; w = w->next_child ){    
         w->redraw( changed ); 
      }
      changed = 0;
   }         
         
   virtual void handle( const event &e ){
      for( widget *w = children; w != 0; w = w->next_child ){    
         event ew( w->inner->translate( 
            e.location_get() ), 
            e.event_type_get() );
         if( ew.location_get().is_within( w->inner->size_get() )){   
            w->handle( ew ); 
         }   
      }           
   };    
   
   virtual void draw( void ){};         
};


// ==========================================================================
//
// A wframe is a widget that has a border around its inner frame.
// This frame has a fixed width, but the colors and the relief can be
// changed.
//

class wframe : public widget {
private:
   subframe subf;
public:
   const vector size;
   vector origin;
   relief border;
   color bg;
   
   wframe( 
      widget &parent, 
      const vector size,
      const vector &origin = vector( 0, 0 ), 
      const color &bg = color::red(),
      const relief border = relief_raised
   ):
      widget( &parent, &subf ),
      subf( *parent.inner, origin, origin + size ),
      size( size ),
      origin( origin ),
      border( border ),
      bg( bg ) 
   {}
      
   void handle( const event &e ){
      //trace << e;  
      if( e.event_type_get() == event_down ){
         bg = color( bg.red_get(), bg.green_get(), bg.blue_get() ^ 0xFFFF );   
         border = flip( border );
         //trace << border;
         changed = 1;
      }                 
   };
   
   void draw( void ){
      subf.clear( bg );
   }
};


// ==========================================================================
//
// A wpaint is a widget that paints wherever a touch event occurs.
//

class wpaint : public widget {
private:
   subframe subf;
public:
   const vector size;
   vector origin;
   relief border;
   color bg, fg;
   
   wpaint( 
      widget &parent, 
      const vector size,
      const vector origin = vector( 0, 0 ), 
      const color &bg = color::red(),
      const color &fg = color::blue()
   ):
      widget( &parent, &subf ),
      subf( *parent.inner, origin, origin + size ),
      size( size ),
      origin( origin ),
      bg( bg ), fg( fg )
   {}
      
   void handle( const event &e ){
      if( e.event_type_get() == event_touch ){
        inner->write( e.location_get(), fg );      
      }                  
   };
   
   void draw( void ){
      subf.clear( bg );
   }
};


// ==========================================================================
//
// A wtoplevel is a widget that has an associated graphics frame
// and a way to get the events that occur within that frame.
//
// It has a run method that keeps getting and handling events
// (untill it gets an event_exit).
//

class wtoplevel : public widget {
public:
   color bg;
    
   wtoplevel( frame &f, const color &bg  ):
      widget( this, this, &f ), bg( bg ) {}
      
   virtual event event_get( void ) = 0;
   
   void draw( void ){
      inner->clear( bg ); }
   
   void run( void );
};
#endif

} // namespace bmptk

#endif // #ifdef BMPTK_GRAPHICS_H
