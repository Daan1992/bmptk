// ==========================================================================
//
// file: bmptk/graphics/graphics.h
//
// This file contains its own documentation in Doxygen format.
//
//! \mainpage
//!
//! \par Graphics
//!
//! GraWiLi is a C++ graphics and widgets library for bare-metal use on 
//! 32-bit embedded systems. 
//! Bare-metal refers to the situation that there is only the application. 
//! The application has full control of the hardware, and is running 
//! without the support of a separate operating system. 
//! Operating system functionality can still be present, but must be in the 
//! form of libraries, which are statically linked into the application. 
//! GraWiLi is such a library.
//!
//! The graphics part of the library provides the interface to a 
//! few LCD screens, and basic graphics functions of drawing lines, 
//! circles, boxes, characters, and pictures on a graphic screen. 
//!
//! The widget part builds on the graphics part to provide a widget 
//! toolkit with event processing and objects like button, 
//! slider, and tab. 
//!
//! GraWiLi provides a command-line tool (the Python script inline.py) 
//! that can be used to translate an image (.bmp or .jpg) or font (.ttf) 
//! files to C++ code that can be included in an application. 
//! A few pre-translated images and fonts are provided.
//!
//! Using GraWiLi requires an interface to a (presumably LCD) screen, 
//! and for the widget part, a way to capture events. 
//! Interfaces are available for Windows (to quickly test an application, 
//! I use DevCpp), NintendoDS (using ??) 
//! and the FriendlyARM micro2440 board with 2.5" LCD (using ??).
//! 
//! GraWiLi is licensed under a very permissive license. 
//! Basically the only requirements are that a distribution of 
//! the (original or modified) GraWiLi sources must use the same 
//! license, and that you don't blame the author for anything. 
//! No legal requirements are put on the executable code, or on 
//! the code that you link with the library.
//!
//! \par Class overview
//! 
//!    - \ref gwlib::vector    : x,y coordinates 
//!    - \ref gwlib::color     : 3 * 8 bit RGB color
//!    - \ref gwlib::event     : postion + type of a GUI event
//! 
//! Vector and color are basic classes that implement a 2-dimensional 
//! integer coordinates, and a 3-byte (+ transparency) RGB color encoding.
//! An event is something that the user caused at a specific location on the
//! screen, typically a mouse click or a stylus touch.
//!
//!    - \ref gwlib::drawable  : things that can be drawn on a screen
//!    - \ref gwlib::line      : a line
//!    - \ref gwlib::rectangle : a rectangle (box)
//!    - \ref gwlib::circle    : a circle
//!    - \ref gwlib::photo     : a rectangle of pixels
//!    - \ref gwlib::inline_rgb_photo : a color image stored in ROM
//!    - \ref gwlib::inline_bw_photo  : a BW image stored in ROM
//!    - \ref gwlib::font      : pictures for the set of ASCII characters
//!    - \ref gwlib::format    : specified how a text is drawn
//!    - \ref gwlib::text      : an ASCII string and a format
//!
//! Drawables are things that can be drawn. A drawable specifies its shape
//! and size, but not its location (starting point), that is supplied
//! when it is drawn. A photo is a rectangular block of (readonly) pixels.
//! Inline_rgb_photo and inline_bw_photo are photo's that are stored in
//! in-line code (in ROM).
//! 
//! The inline.py tool can be used to create such an in-line representation.
//!
//!     - \ref gwlib::frame     : something you can read and write by pixel
//!     - \ref gwlib::subframe  : part of a frame
//!
//! A frame is the thing on which you can draw a drawable. 
//! Frame inhertits from photo, so it is also a drawable, so you can draw
//! the content of a frame on (a part of) another frame.
//! A subframe is a rectangular part of another frame, which can be
//! rotated, mirrored, or enlarged.
//!    
//!    - \ref gwlib::widget    : can handle events and can (re)draw itself
//!    - \ref gwlib::wframe    : frame widget (has border and background)
//!    - \ref gwlib::wtoplevel : widget that has a frame and an eventloop
//! 
//! \par GraWiLi Software License - Version 1.0 - 2011-03-16
//! 
//! Permission is hereby granted, free of charge, to any person 
//! or organization obtaining a copy of the software and accompanying 
//! documentation covered by this license (the "Software") to use, 
//! reproduce, display, distribute, execute, and transmit the Software, 
//! and to prepare derivative works of the Software, and to permit 
//! third-parties to whom the Software is furnished to do so, all subject 
//! to the following:
//! 
//! The copyright notices in the Software and this entire statement, 
//! including the above license grant, this restriction and the following 
//! disclaimer, must be included in all copies of the Software, in whole 
//! or in part, and all derivative works of the Software, unless such 
//! copies or derivative works are solely in the form of machine-executable 
//! object code generated by a source language processor.
//! 
//! THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
//! EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
//! OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND 
//! NON-INFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR ANYONE 
//! DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, 
//! WHETHER IN CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
//! CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
//! IN THE SOFTWARE.
//

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <iostream>
#include <limits>

namespace bmptk { namespace graphics {

// ==========================================================================
//
// class vector
//
//! an absolute or relative location on a grid
//
//! A vector is a pair of 16-bit integer values that are the x and y 
//! coordinates of an absolute or relative location on an integer grid. 
//! A vector can be constructed from its x and y values. 
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

   //! the x coordinate, can be read and changed 
   short int x;
  
   //! the y coordinate, can be read and changed 
   short int y;

public:      
   //! get the x component
   short int x_get() const { return x; }
   
   //! get the y component
   short int y_get() const { return y; }
   
   //! construct a vector from its x and y coordinates
   vector( short int x = 0, short int y = 0 ): x( x ), y( y ) {}   
     
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
      
   //! reports whether te vector is within the box [0,p>
   //   
   //! The call a.is_within(b) returns true iff a is within 
   //! the rectangle [(0,0),b], where the boundary lines that go through (0,0) 
   //! are included, but the boundaries that go through b 
   //! are excluded.
   bool is_within( const vector p ) const {
      return is_within( x, p.x ) && is_within( y, p.y ); }
      
   //! the vector (0,0)   
   static const vector origin;
};

//! prints a vector
//
//! \relates vector
//! This operator prints a vector in the (\%d,\%d) format
std::ostream & operator<< ( std::ostream &s, const vector p );


// ==========================================================================
//
// class color
//
//! a color in the 3 * 8 bit RGB format
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
   int rgb15( void ) const {
      return (
           (( r   > 31 ? 31 : r   ) << 0 )
       | (( g > 31 ? 31 : g ) << 5 )
       | (( b  > 31 ? 31 : b  ) << 10 )
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

   //! some basic colors
   //
   //! These constants for some common colors.
   //! \defgroup basic_colors basic colors
   //! @{
   // these constants are delibreately NOT Doxygen-documented     
   static const color black;            //!< ...
   static const color white;            //!< ...
   static const color red;              //!< ...
   static const color green;            //!< ...
   static const color blue;             //!< ...
   static const color gray;             //!< ...
   static const color yellow;           //!< ...
   static const color cyan;             //!< ...
   static const color magenta;          //!< ...
   static const color transparent;      //!< ...
   static const color violet;           //!< ...
   static const color sienna;           //!< ...
   static const color purple;           //!< ... 
   static const color pink;             //!< ...
   static const color silver;           //!< ...
   static const color brown;            //!< ...
   static const color salmon;           //!< ...
   //! @}
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
public:

   //! the location where the event ocurred
   //!
   //! For some event types the location is irrelevant.
   const vector location;
   
   //! the type of the event that ocurred
   const event_type e;
   
   //! creates an event from its location and type
   event( const vector p, const event_type e ):
      location( p ), e( e ) {}   
};

//! prints an event
//
//! \relates event
//! Example: 
//!
//! touch:(47,120)
std::ostream & operator<<( std::ostream &s, const event &e );


// ==========================================================================
//
// class drawable
//
//! an object that can be drawn in a frame
//
//! A drawable is an abstract object that can be drawn in a frame.
//! A drawable has has a forgeground and background color, 
//! and a (line) width.
//! What these properties exactly mean depends on the specific drawable.
//! 
//! A drawable object does not contain or specify the frame on which it is
//! to be drawn, nor the location within the screen wehere it is to be drawn.
//! That information is supplied with the draw call.
//!
//! A drawable contains two colors and one integer. 
//! It has no references to other objects.
//

class frame;

class drawable {
protected:

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
   //! The f and position are the parameters supplied to the draw
   //! call. Address is the location of the pixel that is to be drawn 
   //! (draw_pixel will draw it relative to position), c its color.
   static void drawable_draw_pixel( 
      frame &f, const vector position, const vector address, const color c );
      
public:    
   //! constructor, specifies the fg and bg colors, and the (line) width
   //
   //! The default is to draw in black forgeground, with transparent 
   //! (meaning: do not draw) backround, line width (when applicable)
   //! 1 pixel.
   drawable( 
      const color fg = color::black, 
      const color bg = color::transparent,
      unsigned int width = 1
   ): fg( fg ), bg( bg ), width( width ) {}
      
   //! draw the object on f, at the indicated position
   //
   //! A concrete drawable must supply the draw function, that draws the
   //! object on the supplied frame, at the specified location, and to the
   //! specified scale. When scale==0 nothing will be drawn.   
   virtual void draw( 
      frame &f, 
      const vector position = vector::origin
   ) const = 0;    
   
   //! get the forgeround color
   color fg_get() const { return fg; }
      
   //! get the background color
   color bg_get() const { return bg; }
      
  //! get the width
   unsigned int width_get() const { return width; }
      
  //! set the forgeround color
   void fg_set( const color c ){ fg = c; }
      
   //! set the background color
   void bg_set( const color c ){ bg = c; }
      
   //! set the width
   void width_set( unsigned int w ){ width = w; }
      
} ;


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
         
   //! draw the drawable object at the indicated position.
   //
   //! This function is just a convenient way to call the drawable::draw
   //! operation.
   void draw( const vector position, const drawable &object ){
      object.draw( *this, position ); }        
         
   //! draw the drawable object at the origin
   //
   //! This function is just a convenient way to call the drawable::draw
   //! operation.
   void draw( const drawable &object ){
      object.draw( *this, vector::origin ); }     
   
   //! fill the full frame with the indicated color
   virtual void clear( const color c = color::white );
};


// ==========================================================================
//
// class subframe
//
//! frame that represents a rectangular part of another (master) frame
//!
//! A subframe is created by specifying the master frame, and the top-left 
//! and bottom-right vectors of the subframe corners within the master.
//! The top-left and bottom-right vectors can be reversed to get a 
//! subframe that appears rotated and/or mirrored within the master.
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
//! but also its neigbouring pixels in the direction towards the bottopright
//! pixel.
//

class subframe : public frame {
public:

   //! the master of this subframe
   frame & master;              
   
   //! location within the master of this subframes topleft pixel
   const vector top_left;
   
   //! location within the master of this subframes topleft pixel
   const vector bottom_right;
   
   //! the scale at which this subframe appears in its master
   const unsigned int scale;
         
   //! translate a subframe coordinate to a master frame coordinate      
   vector translate( const vector p ) const {
      return vector( 
         top_left.x_get() < bottom_right.x_get()
            ? top_left.x_get() + p.x_get() 
            : top_left.x_get() - p.x_get(),
         top_left.y_get() < bottom_right.y_get()  
            ? top_left.y_get() + p.y_get()
            : top_left.y_get() - p.y_get()
       ); }       
      
   //! create a subframe, endpoints specified as vectors
   subframe( 
      frame &f, 
      vector top_left, 
      vector bottom_right,
      unsigned int scale = 1 
   ):
      frame(( top_left - bottom_right ).abs() ),
      master( f ), 
      top_left( top_left ), 
      bottom_right( bottom_right ),
      scale( scale ) {}
  
protected:

   //! checked_write implementation as requiredby frame
   //
   //! When scale == 0 checke_write will do nothing, hence the
   //! subframe will not appear in the master frame.
   //! 
   void checked_write( const vector p, const color c ){
      if( scale == 0 ) return; 
      if( is_valid( p )){
         for( unsigned int x = 0; x < scale; x++ ){
            for( unsigned int y = 0; y < scale; y++ ){       
               master.write( translate( p * scale + vector( x, y )), c ); 
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
//! \b not its endpoint.
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
   
   //! constructs a line from its \ref size, color, and width
   line( const vector size, const color fg = color::black, int width = 1 ):
      drawable( fg, color::transparent, width ), size( size ){}     
      
   //! constructs a line from its \size x and y, color, and width
   line( int x, int y, const color fg = color::black, int width = 1 ):
      drawable( fg, color::transparent, width ), size( vector( x, y )){}     
      
   //! draw the line on f, at position, and to scale   
   void draw( frame &f, const vector position ) const;
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

   //! bright border color; can be read or written; intitally gray
   //
   //! This is the color used for the bright parts of teh border
   //! when its relief is raised or sunken.
   color bright;
   
   //! dark border color; can be read or written; intially black
   //
   //! This is the color used for the bright parts of teh border
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
   
   //! constructs a rectangle from its far corner, bg, fg and width
   rectangle(
      const vector size, 
      const color fg     = color::black,
      const color bg     = color::transparent,
      int width = 1
   ):
      drawable( fg, bg, width ), 
      bright( color::gray ), 
      dark( color::black ), 
      border( relief_flat ),
      size( size ){}     
      
  //! constructs a rectangle from its far corner x and y, bg, fg and width
   rectangle(
      int x, int y,
      const color fg     = color::black,
      const color bg     = color::transparent,      	
      int width = 1 
   ):
      drawable( fg, bg, width ), 
      bright( color::gray ), 
      dark( color::black ), 
      border( relief_flat ),
      size( vector( x, y )){}     

   //! draw the rectangle on f, at position
   void draw( frame &f, const vector position ) const;
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
public:    

   //! the radius of the circle
   //!
   //! The position of a drawable is specified when it is drawn, 
   //! it is not part of the drawable. You can think of the origin
   //! being the centre.
   unsigned int radius; 
   
   //! create a circle from a radius, fg and bg colcors, and line width
   circle( 
      unsigned int radius,
      const color fg = color::black,
      const color bg = color::transparent,
      unsigned int width = 1
   ):
      drawable( fg, bg, width ), radius( radius ){}        
      
   //! draw the circle on f, at position
   void draw( frame &f, const vector position ) const; 
};    


// ==========================================================================
//
// class photo
//
//! a recangular block of read-only pixels
//
//! The abstract photo class is a read-only store of the colors of 
//! all pixels in a rectangular area. 
//! It is an abstract class, storing only the size of the area. 
//! A concrete child class must implement a \ref checked_read() 
//! function that returns the color of the requested pixel. 
//! The photo class itself provides a read() function that first 
//! checks whether the requested pixel is within the area (otherwise 
//! it returns color::transparent) and if so, calls \ref checked_read() 
//! to do the real work. 
//

class photo : public drawable {
protected:

   //! the size of the photo
   //
   //! A concrete photo might have functions that change the size,
   //! but otherwise it is read-only.
   const vector protected_size; 
    
   //! get the color of the specified pixel    
   //
   //! This function must be provided by a concrete photo class.
   //! It can assume that the requested pixel is valid (within
   //! the photo).
   virtual color checked_read( const vector p ) const = 0;
           
public:
    
   //! the size of the photo
   //!
   //! The size is also the position of the first pixel down-right 
   //! beyond the bottom right pixel.
   const vector &size;
   
   //! construct a photo, specify its size
   photo( const vector size ):
      protected_size( size ), size( protected_size ) {}       
      
   //! report whether p is within the photo   
   bool valid( const vector p ) const {
      return p.is_within( size ); }         
      
   //! report whether (x,y) is within the photo
   bool valid( int x, int y ) const {
      return valid( vector( x, y )); }        
   
   //! read one pixel, address specified by vector
   //
   //! This function will return color::transparent
   //! when the requested pixel is outside the picture.
   //! Otherwise it will call \ref protected_read.
   color read( const vector p ) const {
      if( valid( p )){
         return checked_read( p );
      } else {
         return color::transparent;
      }               
   }          
   
   //! read one pixel, address specified in x,y form
   //
   //! @copydetails photo::read( const vector p )  
   color read( int x, int y ) const { 
      return read( vector( x, y )); }     
   
   //! draw the picture on f, at position     
   void draw( frame &f, const vector position ) const;
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

class inline_rgb_photo : public photo {
private:

   //! the pixel data     
   const unsigned char *data;  
   
   //! read one pixel
   color checked_read( const vector p ) const;
     
public:     
   
   //! create an inline_rgb_photo object   
   inline_rgb_photo( int x, int y, const unsigned char *data ):
      photo( vector( x, y )), data( data ){}        

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

class inline_bw_photo : public photo {
private:

   //! the pixel data     
   const unsigned char *data;  

public:        
    
   //! read one pixel, return bool
   bool bool_read( const vector p ) const; 
   
   //! create an inline_bw_photo object   
   inline_bw_photo( 
   
      //! the x size of the photo
      int x, 
      
      //! the y size of the photo
      int y,    
      
      //! the pixel data
      //
      //! The data contains one bit per pixel, x address varies fastest.
      //! No filling except (when needed) the last byte.
      //! The bits within a byte are stored LSB first.
      const unsigned char *data 
   ):
      photo( vector( x, y )), data( data ){}        
      
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
   
   // create a font, specify its fixedness and 
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
//! a photo of a single charv from a font
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

class char_photo : public photo {   
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
      const color &fg = color::black, 
      const color &bg = color::transparent
   ):
      photo( f.char_size( c )),
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
//! a font stored in ROM
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
      
      //! the x size of the photo
      unsigned int x,
      
      //! the y size of the photo
      unsigned int y,
      
      //! the photo pixels, see \ref inline_bw_photo
      const unsigned char *data
   ):         
      font( fixed, char_size ), 
      inline_bw_photo( x, y, data ),
      start( start )
   {}        
           
   //! report whether the font has a photo for char c        
   bool has( char c ) const { 
   return     
         true
      #if CHAR_MIN < 0
         && ( c >= 0 )
      #endif
      #if CHAR_MAX > 127
         && ( c <= 127 ) 
      #endif
         && ( start[ (int) c ] >= 0); 
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

//
//! how a text is rendered on a screen 
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
      const font &f,
      font_alignment h = align_near, 
      font_alignment v = align_near, 
      bool wrap = 0,
      unsigned int scale = 1,
      vector spacing = vector::origin,
      vector top_left_margin = vector( 2,2 ),
      vector bottom_right_margin = vector( 2,2 ),
      color fg = color::black,
      color bg = color::transparent
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

extern const format format_default;


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
class text : public drawable {
public:    
	
	 //! the ASCII text string; can be changed
   const char *s;
   
   //! the size of the rectangle available for the text; can be changed
   vector size;
   
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
      
      //! teh size of the rectangle available for drawing the text
      const vector size,
      
      //! the format used to draw the text
      //
      //! This format is copied. You can change the 
      //! copy (the \ref f attribute) as you see fit.
      const format f = format_default
   ):
      drawable( f.fg, f.bg, f.scale ), s( s ), size( size ), f( f ){}        
      
   //! draw the text on f, at position
   //
   //! The text is drawn according to the current format 
   //! (the \ref f attribute), and the text that is drawn is the 
   //! current string (whatever the \ref s attribute points to).
   void draw( frame &f, const vector position ) const; 
};    



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
         event ew( w->inner->translate( e.location ), e.e );
         if( ew.location.is_within( w->inner->size_get() )){   
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
      const color &bg = color::red,
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
      if( e.e == event_down ){
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
      const color &bg = color::red,
      const color &fg = color::blue
   ):
      widget( &parent, &subf ),
      subf( *parent.inner, origin, origin + size ),
      size( size ),
      origin( origin ),
      bg( bg ), fg( fg )
   {}
      
   void handle( const event &e ){
      if( e.e == event_touch ){
        inner->write( e.location, fg );      
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

}} // namespace bmptk { namespace graphics {

#endif // #ifdef _GRAPHICS_H_                            
