#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2015
#
# This file is in the public domain.
# 
#############################################################################

# Specify target chip or board or system
TARGET     := lpc1114fn28

# Specify the location of the bmptk library
BMPTK      := ../../../..

# Specify project files (other than $(PROJECT).cpp) (if any) 
SOURCES    := sw-uart.c timer.c
HEADERS    := sw-uart.h timer.h

# Specify directories, other than the current, that contain sources
SEARCH     := ../sw-uart ../timer

# The Makefile.inc does all the work
include $(BMPTK)/Makefile.inc
