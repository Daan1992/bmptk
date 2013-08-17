//***************************************************************************
//
// This file contains Doxygen-format bmptk documentation that is not
// related to specific source code. 
//
//***************************************************************************

/*!

\page background Background

Around 1995 I got interested in microcontrollers, more specifically in
the Microchip 16C84, then the only easily-available 
microcontroller that could be re-programmed electronically. 
The 16C84 is primitive and expensive at todays standards, but for me 
it opened a whole new world. There were some hurdles.
You could either buy an expensive programmer, or build your own
(unreliable) serial-port controlled programmer.
And toolchains were expensive, except the free but assembler-only MPLAB. 

No problem, I love tinkering! I built my own intelligent serial-port
PIC programmer, based on (of course) a 16C84, programmed in assembler.
I still sell the grandchild of that design in my webshop. 
In the process I also tried to make a library of reuseable PIC assembler
code, but that got nowehere. 
I tried to make macro's for high-level language constructs, including
assembly-time memory management and unused code removal.
My macro efforts soon became too much for the assembler, 
it ran out of macro space). 
So in order to write re-useable code (and for a few other reasons) I 
switched to writing my own compiler. This resulted in the Jal
language and compiler. After a few years I lacked the time to
develop jal further. I released the sources, and Kyle York 
picked it up and rewrote it to the current Jal. It has a small
but very dedicated group of followers, including a group
that has created an extensive library.

In 2004 I complemented my webshop activities with a part-time assignment
as lecturer at the Hogeschool Utrecht. This got me into teaching
ARM microcontroller programming, first in assembler and C, 
later also in C++.
Compared to the cramped and bizarre PIC architecture this was almost
heaven. Alas, the tools we used (especially the debugger interface)
were not heavenly to used, and this took a lot of attention away 
from (from me and from teh students) away from the
primary subject, being the programming itself.
To solve this problem I wrote a hughe Python script (mkt.py) that
hid all the complexity. It provided taylored chip header files 
and libraries, did the compiling and linking, and started the 
simulator, debugger or download tool to run the application.
The tool (mkt.py) did its job quite well, but no-one besides me
understood what it did and why.
A college critized me for this, and when I got the chance
to teach some extra lecturers I decided to include the makefile
technique into the course, and to rewrite my tool as a makefile and
some small scripts.
This enabled the students could understand what is going in the
environment they have to use, and I had something to 
talk about in the lessons.

The first course I used it for (C++ and Software Engineering) 
is a companion to a NintendoDS project.
Hence the first target is the NintendoDS (and windows, beacuse
that was easy to add), and the first library functionality is
drawing grahics.

*/
