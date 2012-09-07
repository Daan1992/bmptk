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

