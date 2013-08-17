#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2012
# 
#############################################################################

# Specify the project name, this is also the main .cpp file to compile
PROJECT    := generate

# Specify target chip or board or system
TARGET     := windows

# Specify the location of the bmptk library
BMPTK      := ../..

# Specify project files (other than $(PROJECT).cpp) (if any)
HEADERS    := 
LIBS       := 

# specify (extra) results you want to get (if any)
PICTURES   += lcd_pixel_coordinates
GIFS       := $(patsubst %,%.gif,$(PICTURES))
BMPS       := $(patsubst %,%.bmp,$(PICTURES))
CLEAN      += $(GIFS) $(BMPS)
RESULTS    := $(GIFS)
$(BMPS): run
.DEFAULT_GOAL := pictures
.PHONY: pictures
pictures: $(GIFS)

# how to make a gif from a bmp
%.gif: %.bmp; rm -rf $@ && convert $< $@

# The Makefile.inc does all the work
include $(BMPTK)/Makefile.inc
