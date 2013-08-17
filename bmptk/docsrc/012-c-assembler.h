/*!

\page c-and-assembler C and assembler

The interface between the target-independent part of the bmptk library 
and the target-specific parts is small. This chapter explains this interface.

The target code must implement the bmptk::curent_time() function, 
which must return the current time in a unit that is between 1ns and 1us.
The makefile.inc must make the value of 1us available via the macro
BMPTK_TICKS_PER_US, which is used by the bmptk::time::us(),
which is in turn used by bmptk::us, bmptk::ms and bmptk::s.
The first call to bmptk::curent_time() must take care of any 
initialization that is required. 

*/
