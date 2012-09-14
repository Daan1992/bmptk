//***************************************************************************
//
// file : bmptk/start.h
//
// This file contains Doxygen-format bmptk documentation that is not
// related to specific source code. It could be in bmptk.h, but that might
// slow down compilation.
//
//***************************************************************************

// ==========================================================================
//
//! \mainpage
//!
//! Bmptk is a C++ library and build tool for bare-metal use on 32-bit 
//! embedded systems. 
//! Bare-metal refers to the situation that there is only the application. 
//! The application has full control of the hardware, and is running 
//! without the support of a separate operating system. 
//! Operating system functionality can still be present, but must be in the 
//! form of libraries, which are statically linked into the application.
//! Bmptk does not include a toolchain (compiler, linker etc.). 
//! Instead it relies on an external toolchain that is invoked
//! by the bmptk makefile.
//!
//! This version supports the targets 
//!    - win: uses DevCpp to create windows executables
//!    - nds: uses DevKitPro and DeSmuME to create (and run) NDS applications
//!
//! The bmptk makefile can be used from the command line, or it
//! can be called from an editor or IDE. 
//
// ==========================================================================

// ==========================================================================
//
//! \page start Getting Started
//!
//! To start using bmptk you must first get bmptk itself and
//! place it somwehere on your PC. It does not care where, but I'll assume
//! you put it in C:\\bmptk.
//! 
//! Assuming you will use the bmptk graphics library you must
//! install Python and a matching PIL. Check \ref tools for details.
//!
//! Next you must install the toolchain for your taget:
//! - for NinendoDS install DevKitPro and DeSmuME (check \ref nds)
//! - for Windows install DevCpp (check \ref win)
//!
//! You can use bmptk from the command line, but it is probably
//! easier to use an editor, if only to jump to the source line where
//! the compiler found an error. Install the editor of your
//! choice:
//! - \ref pspad
//! - \ref geany
//!
//! The file C:\\bmptk\\Makefile.local contains the paths where
//! bmptk will look for your tools. If you installed any of the tools
//! in a non-standard location (or when you use 64-bit windows, which 
//! installs 32-bit applications in a different directory, sigh...)
//! this Makefile.local must be copied to Makefile.custom
//! and edited to reflect to reflect your situation.
//!
//! For a first test you can take one of the examples, for instance
//! C:\\bmptk\\examples\\lines. 
//!
//! To start a new bmptk project you can either copy an example,
//! or copy the Makefile.template to your_directory/Makefile.
//! In both cases you will have to edit your new Makefile
//! to reflect your project.
// 
// ==========================================================================

// ==========================================================================
//
//! \page cmd Command Line
//!
//! Bmptk is 'driven' by its makefile(s).
//! Each project has its own makefile, that specifies
//! - the project name. Bmptk assumes that the project name, 
//!   with a .cpp extension, is a source file of the project.
//! - the target. This must be one of the supported targets, currently
//!   'win' and 'nds' (without the quotes).
//! - the location of the bmptk files. The examples are part of the
//!   bmptk files, so they know the bmptk root is at ../.. 
//!   Your own projects will probably be outside the bmptk tree, and
//!   you might want to move hem around, so I suggest you specify the 
//!   bmptk root as an absolute location like C:\\bmptk (assuming
//!   you installed it there).
//! 
//
// ==========================================================================


// ==========================================================================
//
//! \page tools other external tools
//!
//! The font and image conversion tools used by bmptk are Python scripts,
//! and use PIL (Python Imaging Library).
//! Hence you must install both Python and (a matching) PIL.
//!  Note that not for all Python 
//! versions are ready-made PIL version can be found.
//! The PIL for Python 3.2 seems to have a bug.
//! I use Python 2.7 with the matching PIL. 
//!
//! Multiple Python versions can coexits on your machine.
//! When your Python is not installed in it default directory (e.g.
//! C:\\Python27) the Makefile.local must be copied to Makefile.custom
//! and edited to reflect the directory python is installed in.
//!
//
// ==========================================================================

// ==========================================================================
//
//! /page graphics
//
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
//
// ==========================================================================


