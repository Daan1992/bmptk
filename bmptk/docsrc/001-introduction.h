//***************************************************************************
//
// This file contains Doxygen-format bmptk documentation that is not
// related to specific source code. 
//
//***************************************************************************

/*!

\anchor introduction
\mainpage

# Introduction

Bmptk is a C++ library and build tool for bare-metal use on 32-bit 
embedded systems. 
Bare-metal refers to the situation that there is only the application. 
This application has full control of the hardware, and is running 
without the support of a separate operating system. 
Operating system functionality can still be present, but must be in the 
form of libraries, which are statically linked into the application.

Bmptk tries to make your application portable across various targets,
rather than trying to make optimal use of a specific target. 
Hence the features of the target are used trough abstractions. 
An LCD screen for instance is a \ref bmptk::graphics::frame instance, 
which has (except for its size and color depth) the same properties on all 
targets that have a screen (or more than one).
Similarly, static classes are used to represet I/O pins and ports
in a uniform but fast way.

Bmptk uses a subset of C++, which excludes features that don't 
match small targets:
- no RTTI (Run Time Type Information)
- no std streams (but a std::cout work-alike is provided)
- no exceptions
- no heap memory deallocation (but allocation and self-managed pools are OK)
- no global objects that require initialization
- no libraries that use these features 
- no interrupts (the \ref rtos "RTOS" provides non-preemptive multitasking)
.

Bmptk does not include a toolchain (compiler, linker etc.). 
Instead it relies on an external GCC toolchain that is invoked
by the bmptk makefile. 

The bmptk makefile can be used from the \ref command-line "command line", 
or it can be called from an \ref editors "editor or IDE". 

The bmptk code license is the MIT 'expat' license, which basically means that
you can do everything you want with this software, except that
when you re-distribute the source, it must be under that same license.
What you do with your application that uses bmptk is completely 
up to you, without any strings attached.

Bmptk is very much 'work in progress'. 
You can download the bleeding edge code from 
<A HREF="http://code.google.com/p/bmptk" target="_blank">
google code</A>, 
or get it as a git repository:

\code
   git clone https://code.google.com/p/bmptk
\endcode

or you can download the latest release as 
<A HREF="http://www.voti.nl/bmptk/bmptk.zip">zip file</A>.
The documentation you are reading now is in the zip file, 
or you can read it 
<A HREF="http://www.voti.nl/bmptk/docs/index.html" 
target="_blank">online</A>.
(The documentation is not in the repository, only the
Doxygen sources to it.)

Bmptk is used in a number of courses I give 
at the <A target=_blank href="http://www.hu.nl">Hogeschool Utrecht</A>.

If you somehow found and used bmptk I am interested to
hear your experiences and comments. You can reach me at
<A HREF="mailto:wouter@voti.nl">wouter@voti.nl</A>.

---

# Table of contents
   - \ref introduction "Introduction"
   - \ref getting-started
   - \ref editors
   - \ref targets
   - \ref basics
   - \ref hardware
   - \ref graphics
   - \ref rtos
   - \ref background
   - \ref architecture
   - \ref target-interface
   - \ref cpp-limitations
   - \ref background
   - \ref architecture
   - \ref summary
   
*/
