#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2012
# 
#############################################################################

# Specify the project name, this is also the main .cpp file to compile
PROJECT    := draw

# Specify target chip or board or system
TARGET     := nds

# Specify the location of the bmptk library
BMPTK      := ../..

# Specify project files (other than $(PROJECT).cpp) (if any)
HEADERS    := 
LIBS       := graphics wouter

# specify (extra) results you want to get (if any)
RESULTS    := 

# how to create the wouter.h and wouter.cpp inline image files
wouter     := wouter32
CLEAN      += wouter.cpp wouter.h
wouter.cpp wouter.h: $(wouter).jpg
		$(IMGTOCPP) $(wouter).jpg wouter

# The Makefile.inc does all the work
include $(BMPTK)/Makefile.inc
