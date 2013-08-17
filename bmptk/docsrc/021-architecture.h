//***************************************************************************
//
// file : bmptk/docsrc/architecture.h
//
// This file contains Doxygen-format bmptk documentation that is not
// related to specific source code. 
//
//***************************************************************************

/*!

\page architecture Principles and architecture

timing

A basic feature of bmptk is timing.
Time is represented in an ADT which holds a 64-bit signed integer, 
which represents the amount of time elapsed since 
some arbitrary epoch (in most cases the  
moment the function bmptk::current_time() was first called). 
The unit in which this time is expressed is some value in the range
1 nanosecond .. 1 microsecond.
The user does not need to know the actual unit (which depends on the
target and possibly on the targets clock frequency) because
time can (and must!) be expressed by multiplying an integer with one 
of the constants bmptk::us, bmptk::ms or bmptk::s. 
The time class provides the usual integer operations, as far as they make
sense: times can be added and substracted, but not multiplied.
But a time can be multiplied by an integer.
A time can be divided by an integer (yielding a time) or by another time
(yielding an integer).

- rollover
- call frequently

memory use

On embedded targets bmptk uses ROM (Flash) to hold the code, 
constant data, and the initial values of writeable data (all in CSEG). 
RAM is used for writeable data (DSEG), 0-initialized data (BSS)
and the main stack (SSEG).
Those are the linker-allocated segments, which occupy RAM from the
start of RAM up to what they need. 
The rest of RAM is available for dynamic allocation (using malloc or new). 
Freeing dynamic memory is not supported, when you code tries to do so
you will get a linker error.

hardware

The lower levels of bmptk use static classes to represent things like pins,
ports, chips, etc. 
The big advantage of this is that only things that are used in the 
application take up code space. 
A disadvantage is that templates must be used to parametrize these classes, 
which can result in some pretty difficult syntax 
(fortunately this is mostly in the library itself, not in application code) 
and even weirder compiler error messages when something goes wrong.

The type system is provided by the interface classes like pin_in, pin_out,
pin_in_out, pin_oc, and the compareable classes for ports. 
These classes serve to define the interface for a particular type of object, 
and can provide some shared implementation and Doxygen comments. 
All classes that provide a concrete object of one of these types inherit 
from the appropriate interface classes.
This makes it possible for class templates to check their arguments at 
compile time.

An object (pin, port, peripheral chip, etc.) is represented by a static class.
There is no need to create objects of such classes, which is fine, because
no objects means no onwership problems and no chance to use 
no-longer-existing objects. A disadvantage is that initialization can not
be done by constructors (because no objects are ever created). 
Hence these static classes all have an init() method that performs the
necessary initialization steps. 

Classes that implement (inherit) the pin_in, pin_out, pin_in_out etc. 
interfaces represent a way to use a pin. 
A single physical GPIO pin will in most cases be accessible via more than
one class.
In such cases the init() method of each class will configure the
pin for the use conform the class: the init of a pin_out class 
for instance will set the pin to output. 
Each user of such a static class 'object' is responsible for calling
the init() method: do not assume that the init() method of any 'object'
for which you are instantiated has already been called.
When such a user is itself a static class he will call the init() methods
of its constituents in its own init() method.
When such a user 


Decorator class templates take one (or more) classes and provide a class
of the same type but with different semantics.
Adapter class templates take one (or more) classes and provide a class
of the a different type but often with the same semantics.

The names of adapter classes formed from the type they produce,
with the suffix _from.
An example of an adapter is pin_oc_from<>, which takes a pin_in_out an
turns it into a pin_oc. pin_in_out_from<> does the opposite. 

An example of a decorator is invert_from, which can be used on all
types of pins, and inverts the levels: 1 now means a low level,
and 0 means a high level. This works for both set() (writing) and
get() (reading).

type and feature identification

The bmptk hardware library relies heavily on templates and template 
specialization.
To help specialization each static class has a type value and a 
features value (both are enum values).
The type value identifies the class as one of a number of specific types,
like input-output pin, or open-collector port. 
Templates that must be implemented differently depending on the 
argument they get are specialized using the type value.

The pin_oc_from template is an example of how the type  value is used.
It takes one explicit argument, and (invisble for the user) a second
implicit argument, which is the type value found in the first argument.
The default implementation (used only when no specific implementation
is selected) gives a compile-time error:

\snippet pins.h pin_oc_from default

The implementation for a pin that is already an open-collector pin
is of course trivial:

\snippet pins.h pin_oc_from pin_oc

The implementation for an input/output pin must do some tricking in the 
set method: 
- when the pin is set high, the underlying pin is made an input,
- when then pin is set low the underlying pin is made
an output and then set low.
.
\snippet pins.h pin_oc_from pin_in_out



*/