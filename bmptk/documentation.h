//***************************************************************************
//
// file : bmptk/documentation.h
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
//! \page pspad PSPad
//!
//! PSPad (www.pspad.com) is a free code editor that can be configured 
//! to work with bmptk. 
//! This is done by including a few lines in the .ppr file to attach
//! external commands to two buttons, and by including a build.bat file
//! in the work dircetory
//! that contains just the one line "make build". 
//! This .bat file is required because it
//! seems impossible to run make directly from the .ppr file.
//! The examples directories contain the appropriate .ppr
//! and build.bat files.
//!
//! With this setup in place the "run external compiler" button
//! (or CTRL-F9) can be used to run "make build".
//! The result of this command is captured in the result window,
//! and you can click on an error message to jump to the
//! corresponding scource location.
//! When this has been succesfull the "open active file in external program"
//! button (the one with the lightning bolt, no CTRL shortcut) can be used
//! to run "make run" or "make clean". 
//! This button is less usefull for building the application 
//! because the command output is not captured by PSPad.
//! For these buttons and commands to work it is required that the 
//! active file is one of the source (.h or .cpp) files in the working 
//! directory. If another type of file is open the buttons will be inactive
//! (grayed out). If a source file in another directory is active PSPad
//! will run the make commands in that directory.
//!
//! Watch out when you open a new file in PSPad. The default directory
//! that appears in the file selection menu is the last directory that
//! you used in this way, NOT the directory PSPad was started in.
//! Hence when you switch projects it is very easy to open 
//! the wrong source of Makefile and 
//! be flabbergasted that your changes seem to have no effect at all.
//!
//! To open the Makefile you must right-click it and choose 
//! "open as text file". Don't forget to switch to a source file when
//! you subsequently want to build or run the application.
//! If things seem to go very wrong it might be an idea to 
//! open the .ppr file as text file to check which source files
//! you realy are editing.
//!
//! On my 64-bit Windows 7 PC PSPad does not show the names of the
//! files in the edit tabs. This worked correctly on my laptop
//! (windows XP).
//!
//! PSPad will save all changed files before it runs an external command.
//! 
//! Some tips:
//! - view => Line Numbers : toggle showing line numbers 
//! - view => Special Chars : toggle showing tabs, newlines, etc
//! - Tools => EyeDropper : get the 16-bit color value of a 
//!    pixel on our screen
//! - Opening a second copy of PSPad will give you a number of very
//!   weird error messages.
//
// ==========================================================================

// ==========================================================================
//
//! \page geany Geany
//!
//! Geany (www.geany.org) is a free code editor that can call
//! an external makefile. This works 'out-of-the-box' with bmptk.
//! You can use Build => Make (Shift-F9) to build the project
//! in the directory of the currently active file.
//! To run or clean you can use Build => Make Custom Target 
//! (Shift-Control-F9) and type the target you want to 'build'.
//! 
//! While the make is running the editor is totally blocked, and no 
//! progress is shown untill the make has finished. (When you called for
//! a 'make run' this means: untill you stop the executable or emulator!)
//! 
//! You can use Build => Set Build Commands to add new commands, for instance
//! 'run' to call 'make run' directly. Such options appear in the Build menu 
//! item, but not in the Build dropdown list.
//! These customizations seem to be stored globally, so you 
//! have to customize geany only once.
//!
//! By default Geany does NOT save the edited files before starting an
//! external command, which is a bit counter-intuitive. I suppose
//! this behaviour can be changed, but I have not yet found how.
//
// ==========================================================================

// ==========================================================================
//
//! \page notepad++ Notepad++
//!
//! Notepad++ (http://notepad-plus-plus.org/) is a free 
//! lightweight text editor.
//! With the NppExec plugin Notepad++ can call an external 
//!
//! To install the NppExec choose Plugins => Plugin Manager 
//! => Show Plugin Manager. Check the box for NppExec and click Install,
//! and let Nopetapd++ restart itself. Now F6 will open a window
//! in which you can type the command you want to be executed:
//!
//!    npp_save
//!    CD $(CURRENT_DIRECTORY)
//!    make build
//!
//! You can save it, I suggest the name 'make build'. 
//! This script will save all files, CD to the directory of the
//! active file, and run 'make build'. The output will be captured
//! in an output window at the bottom of the screen.
//! Likewise you can create 'make run' and 'make clean'.
//!
//! 

//!
//! http://sourceforge.net/apps/mediawiki/notepad-plus/index.php?title=Compiling_Source_Code
//! 
//! 
//
// ==========================================================================

// ==========================================================================
//
//! \page win target win
//!
//! The win target is used to create a windows executable.
//! This target provides a quick way to run a bmptk application.
//! it is not meant to be used to create full-blown windows applications.
//!
//! To support this target the DevCpp environment must be installed.
//! When it is not installed in the default installation directory
//! (C:\\Dev-Cpp) the Makefile.local must be copied to Makefile.custom
//! and edited to reflect the directory DevCpp is installed in. 
//! The libbga library (included in bmptk) is used by the graphics
//! classes to draw pixels and get mouse events.
//!
//! Some tips:
//! - linking while the application is still running will fail
//!      because the .exe file is locked
//! 
//
// ==========================================================================

// ==========================================================================
//
//! \page nds target nds
//!
//! The nds target is used to create a .nds file that can be run on 
//! a real NintendoDS or DSLite (using an R4 card or similar), or 
//! be run on a PC using the DeSmuME emulator.
//!
//! To support this target DevKitPro must be installed.
//! When it is not installed in its default location (C:\\devkitPro)
//! the Makefile.local must be copied to Makefile.custom and 
//! edited to reflect the directory
//! it devkitPro installed in. The DeSmuME emulator must be placed in
//! devkitPro/emulators/desmume.
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


