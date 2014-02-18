//***************************************************************************
//
// This file contains Doxygen-format bmptk documentation that is not
// related to specific source code. 
//
//***************************************************************************

/*!

\page getting-started Getting Started

\anchor install-bmptk
## bmptk

To start using bmptk you must first get bmptk itself and
place it somwehere on your PC. It does not care where, but I'll assume
you put it in C:\\bmptk.

You can download the bleeding edge code from 
<A HREF="http://code.google.com/p/bmptk" target="_blank">
google code</A>, 
or get it as a git repository:
\code
   git clone https://code.google.com/p/bmptk
\endcode


-----------------------------------------------------------------------------
\anchor install-python
## Python

The bmptk font and image conversion tools can be used 
to convert images, fonts, or text, to code.
These tools are Python scripts, and use the Pillow library
(a Python Imaging Library fork).

Hence to use these tools you must install both Python and (a matching) Pillow,
Note that not for all Python versions are ready-made Pillow 
version can be found.
I use Python 3.3.2
(<A HREF="http://www.python.org/ftp/python/3.3.2/python-3.3.2.msi">direct link</A>)
with the matching Pillow from <A HREF=""></A>
(<A HREF="http://www.lfd.uci.edu/~gohlke/pythonlibs/w9rir8do/Pillow-2.1.0.win32-py3.3.exe">direct link</A>).

Multiple Python versions can coexits on your machine.
When your Python is not 3.3 or is not installed in it default directory 
(C:\\Python33) the Makefile.local must be copied to Makefile.custom
and edited to reflect the directory python is installed in.


-----------------------------------------------------------------------------
\anchor install-toolchain
## Toolchains

Bmptk can generate executables for a number of targets. 
You need to install only the toolchains for the targets that you
are interested in. The supported targets are:

   - \ref target-windows "Windows": uses 
      \ref install-windows "Orwell DevCpp" to create windows executables
   - \ref target-nds "Nintendo DS": uses 
      \ref install-nds "DevKitPro" to create and DeSmuME to run 
      NDS applications
   - \ref target-lpc1114 "LPC1114": uses the 
      \ref install-arm "MentorGraphics/CodeSourcery GCC" toolchain for ARM
   
\anchor install-windows
### for Windows

To create and run applications for Windows you must install 
the <A HREF="http://orwelldevcpp.blogspot.nl/">Orwell DevCpp</A> IDE.
Take the MinGW32 version, I used 4.7.2
(<A HREF="http://sourceforge.net/projects/orwelldevcpp/files/Setup%20Releases/Dev-Cpp%205.4.2%20MinGW%204.7.2%20Setup.exe/download">direct link</A>).

Bmptk uses only the toolchain (compiler, linker, libraries) of this
IDE, not the editor.

This target is not meant to be used to develop full-featured 
windows applications, but to test-run your code on a Windows platform.

The win target support a graphic screen (bmptk::graphics::target_screen) 
and a character output (std::cout). Don't expect real-time
performance from the win target: your PC will do other things beside
running the application.

\anchor install-nds
### for Nintendo DS

To create and run applications for the Nintendo DS game console 
you must install the DevKitARM version of the 
<A HREF="http://devkitpro.org/">DevKitPro</A> development environment,
and (if you want to test-run on your PC) the DeSmuME emulator.
I used the devkitProUpdater 1.5.3 
(<A HREF="http://sourceforge.net/projects/devkitpro/files/Automated%20Installer/devkitProUpdater-1.5.3.exe/download">direct link</A>).
<A HREF="http://www.desmume.com/">DeSmuME</A> is a zip file,
which you must unpack to the emulators\\desmume subdirectory 
of your devkitPro installation.
I used version 0.9.8 (<A HREF="http://www.desmume.com.nyud.net/download/DeSmuME.0.9.8-win32.zip">direct link</A>)

\anchor install-arm
### for ARM chips

To create and run applications for ARM chips (including the newer Cortex versions) you must install the
MentorGraphics 
<A HREF="http://www.mentor.com/embedded-software/sourcery-tools/sourcery-codebench/editions/lite-edition/">Sourcery CodeBench Lite Edition</A>,
EABI release, for ARM. 
To get this toolchain, you must create an account and log in.


-----------------------------------------------------------------------------
\anchor install-configuration
## bmptk configuration

Bmptk must be able to find the various tools (Python, compiler tool chain(s))
that it needs. 
By default bmptk assumes that thall tools are installed in their default
locations on drive C. 
If this is the case for you, you can ignore this section.

When you have installed one or more tools in a different location
(or for Python, you used a different version) you must do the following:

   - in the btmptk root directory, make a copy of the Mafefile.local
      file and call it Makefile.custom
   - in that Makefile.custom, edit the appropriate lines to reflect where you
      put the tools.   
      
The purpose of this is that when you install a new version of bmptk over the 
old one, your Makefile.custom (which is not present in bmptk) will be 
preserved.

Depending on the Windows variety you have tools that have a Windows installer 
can install either in C:\\Program Files or in C:\\Program Files (x86).
The Makefile.local file will take care of this.


-----------------------------------------------------------------------------
\anchor install-editors
## Editors

You can use bmptk from the \ref cmd "command line", but it is probably
easier to use an editor, if only to jump to the source line where
the compiler found an error. A \ref editors "number of editors" are supported,
choose your favourite and install it. 
I prefer \ref pspad "PSPad", not because it is the world's best editor, but
because the integration with bmptk is simple and reliable.

-----------------------------------------------------------------------------
\anchor install-test
## A first test 

For a first test you can go to one of the examples, for instance
   - bmptk\\examples\\db103\\blink (LPC1114 target)
   - bmptk\\examples\\graphics\\draw (nds target)
   - bmptk\\examples\\graphics\\lines (win target)
   
You can start the PSPad editor (assuming you have installed it) 
by double-clicking on the _project.ppr file.   
If that does not work immediately, associate the .ppr extension
with the editor's executable.

In the editor, press CTRL-F9 or click the file-to-10101 icon
to build the application. 
Next you can press SHIFT-F9 or click the file-with-lightningbolt icon 
and select run to run the application. 
For the win target this will run the application. 
For the nds target it will start the DesMuME emulator to run the
application.
For a microntroller target this will start the download tool
to download the application to the microcontroller and run it.

If things don't work as expected, the window in which the action takes
place often closes before you can read what has happened.
In this case, try the \ref install-command-line command line.

   
 -----------------------------------------------------------------------------
\anchor install-start-a-project
## Start your own project: adapt the Makefile
   
To start a new bmptk project you can either copy an example,
or copy the bmptk/Makefile.template to your_directory.

The Makefile contains the instructions for the make tool to build
your project. The Makefile in your project directory contains
only the project-specific aspects. As its last line it includes
the bmptk/Makfile.inc, which does the real work.

The makefile must know where to find the bmptk files.
The example directories are three directories deep in the bmptk tree,
so the Mafiles in the examples have the line

\code
# specify where the bmptk files can be found
BMPTK      := ../../..
\endcode

If you place your new project somewhere within the bmptk tree you 
might have to change the number of ../'s.
If you place a bmptk project outside the bmptk tree you should
probably specify an absolute path, like

\code
BMPTK      := C:/bmptk
\endcode

By default the Makefile assumes that main.cpp (or main.c, or main.asm) 
is the one and only source file to be compiled,
and that this is to be the (base) name of the project files
(executable, map file, etc) that are to be generated.
If your project has a different name, you must mention it in the Makefile,
(without extension):

\code
# specify the name of the project
PROJECT    := alarmclock
\endcode

If your project has more source files than just the main file, 
you must add them in the makefile. You can specify
the files explicity:

\code
# Specify project-specific files (other than the main, if any)
SOURCES    += display.c timer.cpp
HEADERS    += display.h timer.hpp
\endcode

The sources are the files that will be compiled.
When any of the headers are changed, the makefile will re-compile
all your source files. This is a bit conservative, but at least
it is on the safe side.

Alternatively, you can specify the header files, and
the system will assume that source files (.asm, .c, or .cpp) 
with the same base name exist and must be compiled.

\code
# Specify header files that have a corresponding source file
LIBRARIES  += display.h timer.hpp
\endcode

If you have object files that must be included in your
application, you can specify those too. If any of these
files do not exits, the system will attempt to make them,
from an existing .asm, .c or .cpp file.

\code
# Specify object files that must be linked with the application
OBJECTS  += blob.o
\endcode
 
If some of the files are not in the current directory
you must specify in which directories the system must look
for missing files.

\code
# Specify locations weher the system must look for files
SEARCH  += ../uart ../timer
\endcode  

When a serial port is needed to download the application to the
target (and to communicate with the target) the system by default
uses COM4 at 38400 baud. When this does not suit you
you can specify the port and/or the baudrate.

\code
# Specify port and baudrate for serial downloading and communication
SERIAL_PORT      := COM1
SERIAL_BAUDRATE  := 115200
\endcode  

If you want to change settings globally (for all your projects) you
can do so in the bmptk/Makefile.custom file. 
Create it by copying bmptk/Makefile.local and add the declarations you
want to have effect for all your projects.
Note that this file is included after your local Makefile, 
so assignments should be made with ?= so they do not overrule 
assignments made in the project's Makefile.

\code
# in bmptk/Makefile.custom

# The serial port I use on this PC
SERIAL_PORT      ?= COM19
\endcode 
  
-----------------------------------------------------------------------------
\anchor install-command-line 
## Command line use  

Bmptk is 'driven' by its makefile(s).
In most cases you will invoke the Makefile through your GUI, 
but sometimes it is handy to invoke it directly.
The make commands that are recognized are shown in the next table.

command      | effect                                           |
-------------|--------------------------------------------------|
make build   | Build the application (compile, link, etc.)      |
-------------|--------------------------------------------------|
make run     | Download the application (if needed) and run it. |
             | (If necessary, build it first)                   |
-------------|--------------------------------------------------|
make clean   | Delete all files generated                       |
             | by building the application                      |
  





*/
