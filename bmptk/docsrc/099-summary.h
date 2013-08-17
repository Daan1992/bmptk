/*!

\page summary Summary

Bmptk is a free (MIT-license) library and make-based build tool for 
making bare-metal C++ applications. 
The primary targets are 32-bit microcontrollers
like the NXP LPC series, but some other targets are supported like
Windows, Raspberry Pi (bare metal) and NintendoDS.
Bmptk does not contain a toolchain (compiler and linker), but
relies on the free GCC compiler suites for the various targets.

Bmptk has abstractions for pins and ports, and for
protocols like SPI, I2C and Dallas OneWire. 
These abstractions are used in the interfaces for a range of chips.
The PCF8574 for instance, is controlled by an I2C bus that
uses 2 open-collector pins, but in turn it provides 8 open-collector
pins, that can used the same way as any other pin, including 
for interfacing to a secondary I2C bus.


range of peripheral chips using which makes it
possibe 

Bmptk uses static class templates to make 

*/
