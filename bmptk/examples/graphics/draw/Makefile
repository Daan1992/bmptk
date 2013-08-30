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
TARGET     := nds

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

# how to create the wouter.h and wouter.cpp inline image files
wouter     := wouter32
CLEAN      += wouter.cpp wouter.h
LIBS       += wouter
wouter.cpp wouter.h: $(wouter).jpg $(IMGTOCPP-TOOL)
		$(IMGTOCPP) $(wouter).jpg wouter
      
# how to create the next.h and next.cpp inline image files
# the gif was taken from http://www.fg-a.com/arrows.htm
next       += next
CLEAN      += next.cpp next.h
LIBS       += next
next.cpp next.h: $(next).gif $(IMGTOCPP-TOOL)
		$(IMGTOCPP) $(next).gif next
      
# how to create the bigfont.h and bigfont.cpp inline fonst files
# the Caty font is by Arturo Cedillo, free for all use according to 
# http://www.dafont.com/arturo-cedillo.d4028?fpp=50
bigfont     := caty
CLEAN       += bigfont.cpp bigfont.h
LIBS        += bigfont
bigfont.cpp bigfont.h: $(bigfont).ttf $(FFTTOCPP-TOOL)
		$(FFTTOCPP) $(bigfont).ttf bigfont bigfont 30 ""

# The Makefile.inc does all the work
include $(BMPTK)/Makefile.inc
