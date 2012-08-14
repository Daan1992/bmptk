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
LIBS       := graphics

# specify (extra) results you want to get (if any)
RESULTS    := 

# how to create the wouter.h and wouter.cpp inline image files
wouter     := wouter32
CLEAN      += wouter.cpp wouter.h
LIBS       += wouter
wouter.cpp wouter.h: $(wouter).jpg
		$(IMGTOCPP) $(wouter).jpg wouter
      
# how to create the bigfont.h and bigfont.cpp inline fonst files
# the Caty font is by Arturo Cedillo, free for all use according to 
# http://www.dafont.com/arturo-cedillo.d4028?fpp=50
bigfont     := caty
CLEAN       += bigfont.cpp bigfont.h
LIBS        += bigfont
bigfont.cpp bigfont.h: $(bigfont).ttf
		$(FFTTOCPP) $(bigfont).ttf bigfont 30

# The Makefile.inc does all the work
include $(BMPTK)/Makefile.inc
