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
SOURCES    := timer.c sw-uart.c
HEADERS    := timer.h sw-uart.h

# Specify directories, other than the current, that contain sources
SEARCH     := ../timer

# The Makefile.inc does all the work
include $(BMPTK)/Makefile.inc
