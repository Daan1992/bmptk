//***************************************************************************
//
// file : bmptk/docsrc/start.h
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
//!
//! Bmptk does not include a toolchain (compiler, linker etc.). 
//! Instead it relies on an external toolchain that is invoked
//! by the bmptk makefile.
//!
//! This version supports the targets 
//!    - \ref win : uses DevCpp to create windows executables
//!    - \ref nds : uses DevKitPro and DeSmuME to create 
//!      (and run) NDS applications
//!
//! The bmptk makefile can be used from the \ref cmd, or it
//! can be called from an editor or IDE. 
//!
//! The bmptk license is the MIT license, which basically means that
//! you can do everything you want with this software, except that
//! when you re-distribute the source, it must be under that same license.
//! What you do with your application that uses bmptk is completely 
//! up to you, without any strings attached.
//!
//! Bmptk is very much 'work in progress'. 
//! You can download the bleeding edge code from 
//! <A HREF="http://code.google.com/p/bmptk" target="_blank">
//! google code</A>, 
//! or get it as a git repository:
//! \code
//!    git clone https://code.google.com/p/bmptk
//! \endcode
//! or you can download the latest release as 
//! <A HREF="http://www.voti.nl/bmptk/bmptk.zip">zip file</A>.
//! The documentation you are reading now is in the zip file, 
//! or you can read it 
//! <A HREF="http://www.voti.nl/bmptk/docs/index.html" 
//! target="_blank">online</A>.
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
//! You can use bmptk from the \ref cmd, but it is probably
//! easier to use an editor, if only to jump to the source line where
//! the compiler found an error. Install the editor of your
//! choice:
//! - \ref pspad
//! - \ref geany
//! - \ref notepadpp
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
//! \page background Background
//!
//! Around 1995 I got interested in microcontrollers, more specifically in
//! the Microchip 16C84, then the only easily-available 
//! microcontroller that could be re-programmed electronically. 
//! The 16C84 is primitive and expensive at todays standards, but for me 
//! it opened a whole new world. There were some hurdles.
//! You could either buy an expensive programmer, or build your own
//! (unreliable) serial-port controlled programmer.
//! Toolchains were expensive, except the free but assembler-only MPLAB. 
//!
//! ToBeContinued
//!
//
// ==========================================================================
