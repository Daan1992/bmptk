#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2012
# 
#############################################################################

# Specify the project name, this is also the main .cpp file to compile
PROJECT    := text

# Specify target chip or board or system
TARGET     := win

# Specify the location of the bmptk library
BMPTK      := ../..

# Specify project files (other than $(PROJECT).cpp) (if any)
HEADERS    := 
LIBS       := bmptk_graphics

# specify (extra) results you want to get (if any)
RESULTS    := 

CLEAN      += mariner.cpp mariner.h
LIBS       += mariner
next.cpp next.h: mariner.txt $(TXTTOCPP-TOOL)
		$(TXTTOCPP) mariner.txt mariner

# The Makefile.inc does all the work
include $(BMPTK)/Makefile.inc
