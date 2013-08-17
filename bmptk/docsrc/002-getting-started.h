//***************************************************************************
//
// This file contains Doxygen-format bmptk documentation that is not
// related to specific source code. 
//
//***************************************************************************

/*!

\page getting-started Getting Started

To start using bmptk you must first get bmptk itself and
place it somwehere on your PC. It does not care where, but I'll assume
you put it in C:\\bmptk.

Assuming you will use the bmptk graphics library you must
install Python and a matching PIL. Check \ref tools for details.

The current version supports the targets 
   - \ref target-windows : uses Orwell DevCpp to create windows executables
   - \ref target-nds : uses DevKitPro to create 
   and DeSmuME to run NDS applications
   - \ref target-lpc1114 : uses the MentorGraphics/CodeSourcery GCC
   for ARM
   
\anchor command-line   

Next you must install the toolchain for your taget:
- for NinendoDS install DevKitPro and DeSmuME (check \ref target-nds)
- for Windows install DevCpp (check \ref target-windows)

You can use bmptk from the \ref cmd, but it is probably
easier to use an editor, if only to jump to the source line where
the compiler found an error. Install the editor of your
choice:
- \ref pspad
- \ref geany
- \ref notepad-pp

The file C:\\bmptk\\Makefile.local contains the paths where
bmptk will look for your tools. If you installed any of the tools
in a non-standard location 
this Makefile.local must be copied to Makefile.custom
and edited to reflect to reflect your situation.

For a first test you can take one of the examples, for instance
C:\\bmptk\\examples\\lines. 

To start a new bmptk project you can either copy an example,
or copy the Makefile.template to your_directory/Makefile.
In both cases you will have to edit your new Makefile
to reflect your project.


\page cmd Command Line

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


\page tools other external tools

The font and image conversion tools used by bmptk are Python scripts,
and use PIL (Python Imaging Library).
Hence you must install both Python and (a matching) PIL.
Note that not for all Python 
versions are ready-made PIL version can be found.
The PIL for Python 3.2 seems to have a bug.
I use Python 2.7 with the matching PIL. 
Note that PIL must be installed!

Multiple Python versions can coexits on your machine.
When your Python is not installed in it default directory (e.g.
C:\\Python27) the Makefile.local must be copied to Makefile.custom
and edited to reflect the directory python is installed in.

*/
