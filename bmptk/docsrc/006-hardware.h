/*!

\page hardware Pins and ports

Bmptk abstracts pins, ports and most chips as static classes.
There are no objects of these classes, 
innstead the name of the class representing the abstraction is 
passed as a template parameter to other abstractions.

For example, to interface to a PCF8574A I2C I/O extender, the pins
that are used as I2C clock and I2C data 
(which are themselves static classes in the bmptk::target class)
are used to instatiate an I2C bus class. 
This I2C bus class is in turn uses to instantiate a PCF8574A class.
Finally, to blink a single LED connected to an output pin of
the PCF8574A, a blink function is instantiated and called.
(examples/db103/pcf8574a-blink/main.cpp):
@include examples/db103/pcf8574a-blink/main.cpp

The main reason to use static classes is that the compiler can 
generate compact and fast code.
A consequence is that the abstractions are not objects,
hence they can not be passed around as parameters.
Hence the 'structure' describing the hardware is static: 
it can not be changed at run time.
This means that there can be no problems
like dangling references or stale pointers.

No objects of the static classes are ever made, hence the 
initialization can not be done in the constructor. 
Instead each static class has an init() method, that must be called before 
any other method of the class is used. 
Calling the init() method is the responsibility of the code
that acually uses the pin (not of the main).

This page describes the elements of the bmptk hardware abstraction:
- \ref hardware-pins "Pins"
- \ref hardware-ports "Ports"
- \ref hardware-conversions "Conversions"
- \ref hardware-decorators "Decorators"
.


-----------------------------------------------------------------------------

\anchor hardware-pins
## Pins

A microcontroller interfaces with external electronics via its I/O pins.
These pins come in different flavours: input-only, output-only, 
input-output and open-collector. Each flavour is characterised by an
(interface-) class that defines the methods it supports.

An input-only pin class has a get() method that returns the current state of
the pin, as determined by the external hardware.

And output-only pin class has a set() method that sets the external level
of the pin according to the argument value.

And input-output pin has the get() and set() methods, and in addiion it
has the direction_set_input(), direction_set_output(), direction-set()
methods. The direction must be set to output before set() can be used,
and must be set to input before get() can be used. 

An open-collector (or more accurate: open-drain) pin can only sink
current (short the pin to ground), not source. \
An open-collector pin is generally connected to open-collector pins
of other chips, with a single pull-up resistor. 
This arrangement is called a wired-AND: any of the pins can pull the
line low, only when all pins don't will it be pulled high by the resistor.

An open-collector pin has the get() and set() methods. 
It does not need the direction_set methods, because setting the pin high
effectively makes the pin an input.

-----------------------------------------------------------------------------

\anchor hardware-ports
## Ports

A port is a collection of (up to 32) pins. 
Ports exist in the same flavous as pins: input-only, output-only, 
input/output, and open-collector. 
A port is used when it is more convenient to read 
(get() method) or write (set() method) an N-bit value
than to read or write the individual pins.
For an input/output the direction can be set, exactly as for a pin_in_out,
but only for all pins of tbe port at once.
A port has an n_pins() method that returns the number of pins in the port.

A port can be constructed from a set of pins. 
The pins must be 'convertible' (see \ref conversions "Conversions") to the
flavour required by the port. 
Up to 32 pins can be combined in this way.

The following class macros are available to create 
a port from a list of pins:
- \ref bmptk::hardware::port_in_from< pin, ... >
- \ref bmptk::hardware::port_out_from< pin, ... >
- \ref bmptk::hardware::port_in_out_from< pin, ... >
- \ref bmptk::hardware::port_oc_from< pin, ... >
.

A simple piece of code that uses a port is the kitt template:
@snippet hardware/demos.h kitt
The function takes a port as template argument, and optionally a time
as function argument (a time value cannot be a template argument because it
is neither a type nor a value of an built-in type).

This function template is used in the kitt demo
(examples/db103/kitt/main.cpp):
@include examples/db103/kitt/main.cpp
This demo creates a kitt-style display on the 12 pins of a 
\ref target-db103 "DB103" board.

-----------------------------------------------------------------------------

\anchor hardware-conversions
## Conversions

When this makes sense, one flavour of pin 
or port can be converted to another. 
These conversions are used, for instance by function templates,
to convert an argument pin to the pin flavour that they can use.

The class templates that create a pin from another pin are:
- \ref bmptk::hardware::pin_in_from< pin >
- \ref bmptk::hardware::pin_out_from< pin >
- \ref bmptk::hardware::pin_in_out_from< pin >
- \ref bmptk::hardware::pin_oc_from< pin >
.

The template \ref bmptk::hardware::pin_in_from< pin > creates an input pin from 
- an input pin,  
- an open-collector pin, or
- an input/output pin. 
.
It can't create an input pin from an output-only pin, of course, because
an output-only pin offers no way to impement the get() method.

Likewise, \ref bmptk::hardware::pin_out_from< pin > creates an output pin from
- an output pin,
- an open-collector pin, or
- an input/output pin.
.
It can't create an output pin from an input pin, because an input-only pin
offers no way to implement the set() method.

\ref bmptk::hardware::pin_in_out_from< pin > creates an input/output pin from
- an open-collector pin, or
- an input/output pin.
.
It can't create an input/output pin from an 
input-only pin (no way to implement set()) or an output-only
pin (no way to implement get().
pin.

\ref bmptk::hardware::pin_oc_from< pin > creates an open-collector pin from
- an open-collector pin, or
- an input/output pin.
.
It can't create an open-collector pin from an 
input-only pin (no way to implement set()) or an output-only
pin (no way to implement get()).

Likewise, the following class templates create a port from another port:
- \ref bmptk::hardware::port_in_from< port >
- \ref bmptk::hardware::port_out_from< port >
- \ref bmptk::hardware::port_in_out_from< port >
- \ref bmptk::hardware::port_oc_from< port >
.

-----------------------------------------------------------------------------

\anchor hardware-decorators
## Decorators

A decorator creates somthing with the same syntactic interface as the 
original, but with a twist.

The bmptk::hardware::invert static class macro creates a class
of the same flavour from a pin or port, but with all the bits inverted.
This inversion happens both on reading (get) and on writing (set).

The bmptk::hardware:tee static class macro combines a list of 
pins into a single pin_out. Writing to that single pin causes writes
to all the underlying pins. Those pins must be 'convertible'
to a pin_out. 







*/
