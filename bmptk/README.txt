file: bmptk/README

Copyright (c) 2012 .. 2105 Wouter van Ooijen (wouter@voti.nl)

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at 
http://www.boost.org/LICENSE_1_0.txt)

This is the Bare Metal Programming Tool Kit main directory.
Bmptk is a light-weight make-based development environment for 
small micro-controllers using GCC C, C++ or assembler on windows.

Bmptk contains hwcpp, a C++ library for programming small micro-controllers.
Bmptk can be used without hwcpp, and with some effort hwcpp could be
used without bmptk.

more information:
   - http://www.voti.nl/bmptk
   - ./index.html (might require running 'make doc', which requires Doxygen)

This directory contains :

   files
      - bmptk.h            : top-level header file for bmptk
      - license.txt        : license (== boost license)
      - license_1_0.txt    : boost license
	   - Makefile           : makes documentation, examples, etc.
      - Makefile.local     : locations of the external tools (edit as needed)
      - Makefile.template  : template for a bmptk project(s) makefile
      - Makefile.inc       : included by the project makefile
      - README             : this file

   subdirectories
      - attic              : resting place for old stuff
      - docsrc             : input for Doxygen documentation generation
      - examples           : example directories
      - targets            : basic chip and board specific stuff
      - tools              : tools (executables, scripts)
      
   generated      
      - doc                : documentation (do a 'make doc', requires Doxygen)
      - index.html         : forwards to the html documentation in doc
      