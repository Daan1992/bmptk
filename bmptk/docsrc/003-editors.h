//***************************************************************************
//
// This file contains Doxygen-format bmptk documentation that is not
// related to specific source code. 
//
//***************************************************************************

/*!

\page editors Editors

Bmptk is used via its makefile. 
So if you want, you can use bmptk from the \ref command-line "command line",
but most people will prefer using an editor to get an 'IDE' feeling.
The editor must be able to start the 'make build', 'make clean' and 
'make run' commands, to capture the output of 'make build', and preferrably
jump to the correct source line when you click on a build error.
This page describes how you can use the following editors to do this:

- \ref pspad "PSPad"
- \ref geany "Geany"
- \ref notepad-pp "Notepad++"
- \ref programmers-notepad "Programmers Notepad"

The bmptk examples have a _project.ppr and build.bat in their
directory so they can be used 'out of the box' with \ref pspad "PSPad".

-----------------------------------------------------------------------------

\anchor pspad
## PSPad

\image html pspad-logo.gif

PSPad (<A HREF="http://www.pspad.com">www.pspad.com</A>) is a free 
code editor that can be configured to work with bmptk. 
This is done by including a few lines in the .ppr file to attach
external commands to two buttons, and by including a build.bat file
in the work dircetory
that contains just the one line "make build". 
This .bat file is required because it
seems impossible to run make directly from the .ppr file.
The examples directories contain the appropriate .ppr
and build.bat files.

With this setup in place the "run external compiler" button
(or CTRL-F9) can be used to run "make build".
The result of this command is captured in the result window,
and you can click on an error message to jump to the
corresponding scource location.
When this has been succesfull the "open active file in external program"
button (the one with the lightning bolt, no CTRL shortcut) can be used
to run "make run" or "make clean". 
This button is less usefull for building the application 
because the command output is not captured by PSPad.
For these buttons and commands to work it is required that the 
active file is one of the source (.h or .cpp) files in the working 
directory. If another type of file is open the buttons will be inactive
(grayed out). If a source file in another directory is active PSPad
will run the make commands in that directory.

Watch out when you open a new file in PSPad. The default directory
that appears in the file selection menu is the last directory that
you used in this way, NOT the directory PSPad was started in.
Hence when you switch projects it is very easy to open 
the wrong source of Makefile and 
be flabbergasted that your changes seem to have no effect at all.

To open the Makefile you must right-click it and choose 
"open as text file". Don't forget to switch to a source file when
you subsequently want to build or run the application.
If things seem to go very wrong it might be an idea to 
open the .ppr file as text file to check which source files
you realy are editing.

On my 64-bit Windows 7 PC PSPad does not show the names of the
files in the edit tabs. This worked correctly on my laptop
(windows XP).

PSPad will save all changed files before it runs an external command.

By default PSPad will use tab characters to create indendation, which
IMHO is very bad because no two programs agree on how much indendation 
each tab creates. Use Settings => Program Settings => Editor Settings 
to correct this. I prefer 3 spaces per indentadion level.

\image html pspad-editor-settings.gif

Some tips:
- view => Line Numbers : toggle showing line numbers 
- view => Special Chars : toggle showing tabs, newlines, etc
- Tools => EyeDropper : get the 16-bit color value of a 
  pixel on our screen
- Setings => Program Settings => Editor - behaviour : uncheck
  "completion of chars ({[<"'"
- On some versions of windows, opening a second copy of PSPad by 
  clicking on a .ppr file will give you a number of very
  weird error messages. Eventually the new project will be opened.
- Running bmptk's make completely ignores the project file list as
  maintained by PSPad.
- By default PSPad replaces TABs with spaces. This is OK, except when
  editng makefiles. You can insert a TAB in a makefile by cut-n-pasting
  an existing TAB.
- You can use the run command (ALT-F9) to build and run your 
  application, but remember that unlike the build command (CTRL-F9)
  it won't save your changes first! This is PSPad behaviour, I don't 
  know how to change it.  
- When you run the application a (great) number of times from
  PSPad some resource seem to get depleted and further runs
  (other make calls: build and clean) will fail. Restarting PSPad
  will solve this, but it is rather annoying when you don't know
  why everything fails.

When freshly started with a project directory copied from another
PC PSPad will sometimes produce this error:
\image html pspad-error1.gif
I am not sure what causes this problem but it can be fixed by
quitting PSPad and restarting it. Apparently it rewrites the .ppr
file and in the process removes some problem, maybe with line endings.


-----------------------------------------------------------------------------
  
\anchor geany  
## Geany

\image html geany-logo.gif

Geany (<A HREF="http://www.geany.org">www.geany.org</A>) is a free code 
editor that can call an external makefile. 
This works 'out-of-the-box' with bmptk.
You can use Build => Make (Shift-F9) to build the project
in the directory of the currently active file.
To run or clean you can use Build => Make Custom Target 
(Shift-Control-F9) and type the target you want to 'build'.

While the make is running the editor is totally blocked, and no 
progress is shown untill the make has finished. (When you called for
a 'make run' this means: untill you stop the executable or emulator!)

You can use Build => Set Build Commands to add new commands, for instance
'run' to call 'make run' directly. Such options appear in the Build menu 
item, but not in the Build dropdown list.
These customizations seem to be stored globally, so you 
have to customize geany only once.

By default Geany does NOT save the edited files before starting an
external command, which is a bit counter-intuitive. I suppose
this behaviour can be changed, but I have not yet found how.


-----------------------------------------------------------------------------
  
\anchor notepad-pp
## Notepad++

\image html npp-logo.gif

Notepad++ 
(<A HREF="http://notepad-plus-plus.org/">notepad-plus-plus-org.com</A>) 
is a free lightweight text editor.
With the NppExec plugin Notepad++ can call do the basic chores of an IDE:
call an external tool, capture the output, and jump to the source line
that corresponds to an error message.

To install the NppExec choose Plugins => Plugin Manager 
=> Show Plugin Manager. 
\image html npp-plugin-manager.gif

Check the box for NppExec and click Install, and let 
Notepad++ restart itself. 
\image html npp-install-nppexec.gif

Now F6 will open a window
in which you can type the command you want to be executed:
\code
   npp_saveall
   CD $(CURRENT_DIRECTORY)
   make build
\endcode
\image html npp-f6-window.gif

You can save it, I suggest the name 'make build'. 
This script will save all files, CD to the directory of the
active file, and run 'make build'. The output will be captured
in an output window at the bottom of the screen.
Likewise you can create 'make run' and 'make clean'.

The next step is to get the NppExec to recognise the source 
references in the output.
Open Plugins => NppExec => Console Output Filters (.
\image html npp-open-filters.gif

In the filters window you must specify the pattern for
errors, and I suggest the pattern for warnings too.
Select B for the errors to make them stand out.
\code
   %FILE%:%LINE%: error:*
   %FILE%:%LINE%: warning:*
\endcode
\image html npp-filters.gif

Now you can use F6 to open the command window, change the command,
and run it, or CTRL-F6 to run the previous command without
getting the command window. The result will appear in the
command result window at the bootom, with the errors in bold.
Click on an error to jump to the coresponding source line.
\image html npp-build.gif

When you have a succesfull build you can use run (F5) to 
run 'make run'.

More information about the command language can be found at
http://sourceforge.net/apps/mediawiki/notepad-plus/index.php?title=Compiling_Source_Code

-----------------------------------------------------------------------------

\anchor programmers-notepad
## Programmer's Notepad

\image html pn-logo.gif

Programmer's Notepad (<A HREF="http://www.pnotepad.org/">www.pnotepad.org</A>) 
is a free lightweight text editor, designed for editing source code.
With a little customization PN can call do the basic chores of an IDE:
call an external tool, capture the output, and jump to the source line
that corresponds to an error message.

OPen a C++ file, and Select Tools => Options => Tools. 
\image html pn-tools-empty.gif

Click Add. Create an entry for 'make build':
\image html pn-make-build.gif

You can leave the Console I/O tab unchanged.
The shortcut key can be configured by selected the field and
pressing the key (or combination) you prefer. 
I choose F6 for building, F8 for running, no shortcut  for cleaning.
Likewise make the entry for cleaning:
\image html pn-make-clean.gif
And for runnung:
\image html pn-make-run.gif
You can activate the make commands either by their shortcut or
from the Tools menu. 
\image html pn-tools-configured.gif

As shown the make commands will first save all modified files.

Tips:
- enable View => Line Numbers  
- the make commands will be executed in the directory of the
  selected file, hence you must select a file in your 
  project directory!

*/