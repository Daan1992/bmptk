//***************************************************************************
//
// file : bmptk/docsrc/targets.h
//
// This file contains Doxygen-format bmptk documentation that is not
// related to specific source code. 
//
//***************************************************************************

// ==========================================================================
//
//! \page win target win
//!
//! \image html win-lines.gif
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
//! \image html nds-red.jpg
//!
//! The nds target is used to create a .nds file that can be run on 
//! a real NintendoDS or DSLite (using an R4 card or similar), or 
//! on a PC using the DeSmuME emulator.
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
