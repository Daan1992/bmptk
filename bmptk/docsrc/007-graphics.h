//***************************************************************************
//
// This file contains Doxygen-format bmptk documentation that is not
// related to specific source code.
//
//***************************************************************************

/*!

/page graphics

The graphics part of the library provides the interface to a 
few LCD screens, and basic graphics functions of drawing lines, 
circles, boxes, characters, and pictures on a graphic screen. 

The widget part builds on the graphics part to provide a widget 
toolkit with event processing and objects like button, 
slider, and tab. 

GraWiLi provides a command-line tool (the Python script inline.py) 
that can be used to translate an image (.bmp or .jpg) or font (.ttf) 
files to C++ code that can be included in an application. 
A few pre-translated images and fonts are provided.

Using GraWiLi requires an interface to a (presumably LCD) screen, 
and for the widget part, a way to capture events. 
Interfaces are available for Windows (to quickly test an application, 
I use DevCpp), NintendoDS (using ??) 
and the FriendlyARM micro2440 board with 2.5" LCD (using ??).

\par Class overview

   - \ref gwlib::vector    : x,y coordinates 
   - \ref gwlib::color     : 3 * 8 bit RGB color
   - \ref gwlib::event     : postion + type of a GUI event

Vector and color are basic classes that implement a 2-dimensional 
integer coordinates, and a 3-byte (+ transparency) RGB color encoding.
An event is something that the user caused at a specific location on the
screen, typically a mouse click or a stylus touch.

   - \ref gwlib::drawable  : things that can be drawn on a screen
   - \ref gwlib::line      : a line
   - \ref gwlib::rectangle : a rectangle (box)
   - \ref gwlib::circle    : a circle
   - \ref gwlib::photo     : a rectangle of pixels
   - \ref gwlib::inline_rgb_photo : a color image stored in ROM
   - \ref gwlib::inline_bw_photo  : a BW image stored in ROM
   - \ref gwlib::font      : pictures for the set of ASCII characters
   - \ref gwlib::format    : specified how a text is drawn
   - \ref gwlib::text      : an ASCII string and a format

Drawables are things that can be drawn. A drawable specifies its shape
and size, but not its location (starting point), that is supplied
when it is drawn. A photo is a rectangular block of (readonly) pixels.
Inline_rgb_photo and inline_bw_photo are photo's that are stored in
in-line code (in ROM).

The inline.py tool can be used to create such an in-line representation.

    - \ref gwlib::frame     : something you can read and write by pixel
    - \ref gwlib::subframe  : part of a frame

A frame is the thing on which you can draw a drawable. 
Frame inhertits from photo, so it is also a drawable, so you can draw
the content of a frame on (a part of) another frame.
A subframe is a rectangular part of another frame, which can be
rotated, mirrored, or enlarged.
   
   - \ref gwlib::widget    : can handle events and can (re)draw itself
   - \ref gwlib::wframe    : frame widget (has border and background)
   - \ref gwlib::wtoplevel : widget that has a frame and an eventloop

*/



