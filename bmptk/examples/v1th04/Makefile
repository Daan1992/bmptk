#############################################################################
# 
# Makefile for bmptk examples (sub) directory
#
# (c) Wouter van Ooijen (www.voti.nl) 2012 ... 2013
# 
#############################################################################

SUBDIRS += 440hz beep blink ds1990 heap
SUBDIRS += lcd5510 servo switch sw-uart timer

# i2c is a library directory, not a project

BUILDDIRS = $(SUBDIRS:%=build-%)
CLEANDIRS = $(SUBDIRS:%=clean-%)

.phony: clean build $(BUILDDIRS) $(CLEANDIRS)

build: $(BUILDDIRS)
$(BUILDDIRS):
		bmptk-make -C $(@:build-%=%) build

clean: $(CLEANDIRS)
$(CLEANDIRS):
		bmptk-make -C $(@:clean-%=%) clean

