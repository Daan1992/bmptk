/*!

\page basics Basics



This page explains the following features of the bmptk library:
- \ref error-handling "Error handling"
- \ref memory-management "Memory management"
- \ref timing "Timing"
.

-----------------------------------------------------------------------------  
\anchor error-handling
## Error Handling

-----------------------------------------------------------------------------  
\anchor memory-management
## memory management

-----------------------------------------------------------------------------  
\anchor timing
## Timing

Time is represented by the ADT (Abstract Data Type) 
\ref bmptk::time, which holds a 64-bit signed integer value.
This value represents the amount of time elapsed since 
some arbitrary epoch (in most cases the  
moment the function bmptk::current_time() was first called). 

The unit in which this time is expressed is some value in the range
1 nanosecond .. 1 microsecond.
The user does not need to know the actual unit (which depends on the
target and possibly on its clock frequency) because
time can (and must!) be expressed by multiplying an integer with one 
of the constants bmptk::us, bmptk::ms or bmptk::s. 

The time class provides the usual arithmetic operations, as far as they make
sense: times can be added and substracted, but not multiplied.
But a time can be multiplied by an integer, and
a time can be divided by an integer (yielding a time) or by another time
(yielding an integer).
Times can be compared with the usual comparison operators.

The function bmptk::current_time() returns, as its name suggests, 
the current time. 
On most targets this is done by setting up a freerunning hardware timer 
to tick at an appropriate rate.
When only a 32-bit timer is available a 32-bit extension is maintaned
to produce a 64 bits time value.
This extension is incremented whenever a bmptk::current_time() call detects
that the timer has overflowed (wrapped around to 0). 
To make sure that no overflows are missed, the interval between 
bmptk::current_time() calls must be less than half the timer's timeout time.
All wait functions call bmptk::current_time(), so in practice this
is not much of a problem.

The following wait functions are available:

- \ref bmptk::wait( const time t )
- \ref bmptk::wait_until( const time t )
- \ref bmptk::wait_busy( const time t )
- \ref bmptk::wait_busy_until( const time t )

The bmptk::wait and bmptk::wait_busy functions will wait for the requested
*amount of time*.
The bmptk::wait_until and bmptk::wait_busy_until functions will wait 
untill the requested *moment in time*.

The wait() and wait_busy() functions simply get the current_time, 
add the wait time, and then call the wait_until or wait_busy_until.

The busy_wait_until function loops until the current time is equal to 
or larger than the current time.

The default wait_until calls wait_busy_until, so there is no difference
between the plain and the busy wait functions. 
When the RTOS is activated however, the wait_until invokes the RTOS
scheduler to suspend the current task until the requested time.
Due to other tasks it can take longer for the current taks to be reactivated
than it requested.
Hence the busy versions must be used for things like generating an accurate
pulse (for instance for a servo) or measuring the length of 
a pulse or pause (to receive an IR signal). 
For all other purposes the plain (not-busy) wait functions should be used,
in order to be useable with and without the RTOS.

-----------------------------------------------------------------------------  
\anchor pins-and-ports






*/
