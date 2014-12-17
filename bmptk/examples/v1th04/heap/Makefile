#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2013
# 
#############################################################################

# Specify target chip or board or system
TARGET      := lpc1114fn28

# Specify the location of the bmptk library
BMPTK       := ../../..

# Specify project files (other than $(PROJECT).cpp) (if any)
SOURCES     := sw-uart.c timer.c
HEADERS     := sw-uart.h timer.h
SEARCH      := ../sw-uart ../timer

# specify the stack size (default depends on the target)
STACK_SIZE  := 3k

# The Makefile.inc does all the work
include $(BMPTK)/Makefile.inc
