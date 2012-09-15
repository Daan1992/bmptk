//***************************************************************************
//
// file : bmptk/docsrc/editors.h
//
// This file contains Doxygen-format bmptk documentation that is not
// related to specific source code. 
//
//***************************************************************************


// ==========================================================================
//
//! \page pspad PSPad
//!
//! \image html pspad-logo.gif
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
//! - Opening a second copy of PSPad by clicking on a .ppr file
//!   will give you a number of very
//!   weird error messages. Eventually the new project will be opened.
//! - Running bmptk's make completely ignores the project file list as
//!   maintained by PSPad.
//! - By default PSPad replaces TABs with spaces. This is OK, except when
//!   editng makefiles. You can insert a TAB in a makefile by cut-n-pasting
//!   an existing TAB.
//
// ==========================================================================

// ==========================================================================
//
//! \page geany Geany
//!
//! \image html geany-logo.gif
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
//! \page notepadpp Notepad++
//!
//! \image html npp-logo.gif
//!
//! Notepad++ (http://notepad-plus-plus.org/) is a free 
//! lightweight text editor.
//! With the NppExec plugin Notepad++ can call do the basic chores of an IDE:
//! call an external tool, capture the output, and jump to the source line
//! that corresponds to an error message.
//!
//! To install the NppExec choose Plugins => Plugin Manager 
//! => Show Plugin Manager. 
//! \image html npp-plugin-manager.gif
//!
//! Check the box for NppExec and click Install, and let 
//! Notepad++ restart itself. 
//! \image html npp-install-nppexec.gif
//!
//! Now F6 will open a window
//! in which you can type the command you want to be executed:
//! \code
//!    npp_saveall
//!    CD $(CURRENT_DIRECTORY)
//!    make build
//! \endcode
//! \image html npp-f6-window.gif
//!
//! You can save it, I suggest the name 'make build'. 
//! This script will save all files, CD to the directory of the
//! active file, and run 'make build'. The output will be captured
//! in an output window at the bottom of the screen.
//! Likewise you can create 'make run' and 'make clean'.
//!
//! The next step is to get the NppExec to recognise the source 
//! references in the output.
//! Open Plugins => NppExec => Console Output Filters (.
//! \image html npp-open-filters.gif
//!
//! In the filters window you must specify the pattern for
//! errors, and I suggest the pattern for warnings too.
//! Select B for the errors to make them stand out.
//! \code
//!    %FILE%:%LINE%: error:*
//!    %FILE%:%LINE%: warning:*
//! \endcode
//! \image html npp-filters.gif
//!
//! Now you can use F6 to open the command window, change the command,
//! and run it, or CTRL-F6 to run the previous command without
//! getting the command window. The result will appear in the
//! command result window at the bootom, with the errors in bold.
//! Click on an error to jump to the coresponding source line.
//! \image html npp-build.gif
//!
//! When you have a succesfull build you can use run (F5) to 
//! run 'make run'.
//! 
//! More information about the command language can be found at
//! http://sourceforge.net/apps/mediawiki/notepad-plus/index.php?title=Compiling_Source_Code
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


