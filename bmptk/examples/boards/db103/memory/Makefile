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
TARGET      := db103

# Specify the location of the bmptk library
BMPTK       := ../../../..

RESULTS := main.lst

# Specify project-specific files (other than the main, if any)
HEADERS     += timer.h sw-uart.h
SOURCES     += timer.c sw-uart.c


# The Makefile.inc does all the work
include $(BMPTK)/Makefile.inc
