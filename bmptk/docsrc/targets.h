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
//! classes to draw pixels and get mouse events. It is not
//! very fast but it does the job.
//!
//! Some tips:
//! - linking while the application is still running will fail
//!   because the .exe file is locked
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
//! All publicly available Nintendo DS documentation is essentially
//! reverse-engineered by hobbyists. There are lots of resources on
//! the web, but no single place where you can find it all. 
//! Some places to check:
//!
//! - http://libnds.devkitpro.org/
//! - http://osdl.sourceforge.net/main/documentation/misc/nintendo-DS/homebrew-guide/HomebrewForDS.html
//!
//! DesMuMe seems to be the most used emulator, but there are others 
//! that you could try, check this
//! <A HREF="http://www.emulator-zone.com/doc.php/nds/">
//! list of emulators</A>
//!
//
// ==========================================================================

// ==========================================================================
//
//! \page huarmbord target HU ARM Board
//!
//! \image html huarm-board.jpg
//!
//! The HU ARM Board V4.2 is PCB with 
//! - an LPC2148 ARM microcontroller;
//! - a 1x16 or 2x20 characer LCD;
//! - a 4 x 4 keypad (interfaced via a preprogrammed Microchip 16F630);
//! - an FT2232-based ISP and ISD interface;
//! - 8 LEDs (interfaced by a HC595 shiftregister);
//! - a few other interfaces;
//! - two extension connectors.
//!
//! To support this target the ARMDevEnv environment or the 
//! CodeSourcery toolchain must be installed.
//! When the toolchain you use is not installed in its default location
//! (C:\\Program Files\\ArmDevEnv rsp. C:\\CodeSourcery\\Sourcery G++ Lite)
//! the Makefile.local must be copied to Makefile.custom and 
//! edited to reflect the directory you used. 
//!
//! The board supports hands-off downloading via the simulated serial 
//! port and lpc21isp. 
//! The Bmptk "make run" command assumes this port is mapped to COM4, 
//! if it is not you must define a SERIAL_PORT in Makefile.custom or 
//! in your project's makefile.
//! By default std::cout is directed to the (same) serial port, and 
//! the "make run" command will use lpc21isp's built-in terminal
//! to let you interact with your application.
//! 
//! Note that at the default baudrate (38k4) each character will 
//! take ~ 250 us.
//! Default there is no buffering, so using 
//! cout (or trace) can change the timing of an application considerably.
//!
//
// ==========================================================================
