#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2013
# 
#############################################################################

# Specify the location of the bmptk library
BMPTK      := ../../..

# Specify target chip or board or system, default is nds
TARGET     := win

# Specify the project name, the default is main (.cpp, .c, or .asm)
# this is also the main .cpp file to compile
# the default is main.cpp (or main.c, or main.asm)
PROJECT    := 

# Specify project-specific files (other than the main, if any)
OBJECTS    := 
SOURCES    := 
HEADERS    := 
LIBS       := 

# specify (extra) results you want to get (if any)
RESULTS    := 

CLEAN      += mariner.cpp mariner.h
LIBS       += mariner
mariner.cpp mariner.h: mariner.txt $(TXTTOCPP-TOOL)
		$(TXTTOCPP) mariner.txt mariner

# The Makefile.inc does all the work
include $(BMPTK)/Makefile.inc
