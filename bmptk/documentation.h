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
//! bmptk is a C++ library and build tool for bare-metal use on 32-bit 
//! embedded systems. 
//! Bare-metal refers to the situation that there is only the application. 
//! The application has full control of the hardware, and is running 
//! without the support of a separate operating system. 
//! Operating system functionality can still be present, but must be in the 
//! form of libraries, which are statically linked into the application. 
//!
//! This version supports the targets 
//!    - win: uses DevCpp to create a windows executable
//!    - nds: uses DevKitPro and DeSmuME
//
// ==========================================================================



// ==========================================================================
//
//! \page PSPad
//!
//! PSPad (www.pspad.com) is an editor that can be configured to work
//! with bmptk. 
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
//! (or CTRL-F9) can be used to run the "make build".
//! When this has been succesfull the "open active file in extrenal program"
//! button (the one with the lightning bolt, no CTRL shortcut) can be used
//! to run "make run" or "make clean". This last route is less usefull for
//! the "make run" because the command output is not captured by PSPad.
//! For these buttos and commands to work it is required that the 
//! active file is one of the source (.h or .cpp) files in the working 
//! directory. If another type of file is open the buttons will be inactive
//! (grayed out). If a source file in another directory is active PSPad
//! will run the make commands in that directory.
//!
//! Watch out when you open a new file in PSPad. The default directory
//! that appears in the file selection menu is the last directory that
//! you used in this way, NOT the directory PSPad was started in.
//! Hence it is very easy to open the wrong source of Makefile and 
//! be flabbergasted that your changes seem to have no effect at all.
//!
//! To open the Makefile you must right-click it and choose 
//! "open as text file". Don't forget to switch to a source file when
//! you subsequently want to build or run the application.
//!
//! On my 64-bit windows the 32-bit PSPad does not show the names of the
//! files in the edit tabs. IIRC this worked correctly on my previous
//! (win32) machine.
//
// ==========================================================================

// ==========================================================================
//
//! \page target win
//!
//! The target win is used to create a windows executable.
//! This target provides a quick way to run a bmptk application.
//! it is not meant to be used to create full-blown windows applications.
//!
//! To support this target the DevCpp environment must be installed.
//! When it is not installed in the default installation directory
//! (C:\Dev-Cpp) the makefile.local must be edited to reflect the directory
//! DevCpp is installed in.
//! 
//
// ==========================================================================

// ==========================================================================
//
//! \page target nds
//!
//! PSPad (www.pspad.com) is an editor that can be configured to work
//! with bmptk. 
//
// ==========================================================================

