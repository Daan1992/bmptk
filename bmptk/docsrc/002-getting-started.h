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
These tools are Python scripts, and use PIL (Python Imaging Library).

Hence to use these tools you must install both Python and (a matching) PIL.
Note that not for all Python versions are ready-made PIL 
version can be found.
The PIL for Python 3.2 seems to have a bug.
I use Python 2.7 with the matching PIL. 
Note that PIL must be installed!

Multiple Python versions can coexits on your machine.
When your Python is not 2.7 or is not installed in it default directory 
(C:\\Python27) the Makefile.local must be copied to Makefile.custom
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
Take the MinGW32 vesrion, I used 4.7.2
(<A HREF="http://sourceforge.net/projects/orwelldevcpp/files/Setup%20Releases/Dev-Cpp%205.4.2%20MinGW%204.7.2%20Setup.exe/download">direct link</A>).

Bmptk uses only the toolchain (compiler, linker, libraries) of this
IDE, not the editor.

This target is not meant to be used to develop full-featured 
windows applications, but to test-run your code on a Windows platform.

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


-----------------------------------------------------------------------------
\anchor install-test
## A test 

For a first test you can go to one of the examples, for instance
   - bmptk\\examples\\db103\\blink (LPC1114 target)
   - bmptk\\examples\\graphics\\lines (win or nds target)
   
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
For a mcicrontroller target this will start the download tool
to download the application to the microcontroller and run it.
   
 -----------------------------------------------------------------------------
\anchor install-start-a-project
## Start your own project
   
   
To start a new bmptk project you can either copy an example,
or copy the Makefile.template to your_directory/Makefile.
In both cases you will have to edit your new Makefile
to reflect your project. 
  
  
-----------------------------------------------------------------------------
\anchor install-command-line 
## Command line use  

Bmptk is 'driven' by its makefile(s).
Each project has its own makefile, that specifies
- the project name. Bmptk assumes that the project name, 
  with a .cpp or .c extension, is a source file of the project.
- the target. This must be one of the supported \ref targets
- the location of the bmptk files. The examples are part of the
  bmptk files, so they know the bmptk root is at ../..//.. 
  Your own projects will probably be outside the bmptk tree, and
  you might want to move hem around, so I suggest you specify the 
  bmptk root as an absolute location like C:\\bmptk (assuming
  you installed it there).
  





*/
