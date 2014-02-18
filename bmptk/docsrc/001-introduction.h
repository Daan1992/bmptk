//***************************************************************************
//
// This file contains Doxygen-format bmptk documentation that is not
// related to specific source code. 
//
//***************************************************************************

/*!

\anchor introduction
\mainpage

# Table of contents
   - \ref introduction "Introduction"
   - \ref getting-started
   - \ref editors
   - \ref targets
   - \ref summary
   
-----------------------------------------------------------------------------

# Introduction

Bmptk is a build tool for bare-metal use on 32-bit embedded systems
using assembler, C or C++.
Bare-metal refers to the situation that there is only the application
running.
This application has full control of the hardware, and is running 
without the support of a separate operating system. 
Operating system functionality can still be present, but must be in the 
form of libraries, statically linked into the application.

Bmptk does not include a toolchain (compiler, linker etc.). 
Instead it relies on an external GCC toolchain that is invoked
by the bmptk makefile. 
Different targets require different versions of the GCC toolchain.

The bmptk makefile can be used from the \ref command-line "command line", 
or it can be called from an \ref editors "editor or IDE".

The bmptk files are provided under the Boots license, 
which basically means that you can do everything you want with this software,
except that when you re-distribute the source, 
it must be under that same license.
What you do with your application that uses bmptk is completely 
up to you, without any strings attached.

Bmptk makes a number of assumptions that are appropriate for
the kind of targets it is intended for. In particular:
- By default, no standard libraries are linked with the
application.
The -Wall -Werror flags are used, so (with a few exceptions)
all warnings are enabled and all warnings are treated as errors.
- Memory allocation is supported, but memory
deallocation is not.
- For C++: RTTI, exceptions, and global objects that 
require initialization are disabled.

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
Doxygen sources to it are.)

Bmptk is used in a number of courses I give 
at the <A target=_blank href="http://www.hu.nl">Hogeschool Utrecht</A>.

If you somehow found and used bmptk I am interested to
hear your experiences and comments. You can reach me at
<A HREF="mailto:wouter@voti.nl">wouter@voti.nl</A>.

---


   
*/
