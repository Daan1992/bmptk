#############################################################################
# 
# Makefile for bmptk v1th04 examples 
#
# (c) Wouter van Ooijen (www.voti.nl) 2012, 2013
# 
#############################################################################

SUBDIRS += blink switch beep
SUBDIRS += 440hz timer sw-uart ds1990
SUBDIRS += lcd5510

BUILDDIRS = $(SUBDIRS:%=build-%)
CLEANDIRS = $(SUBDIRS:%=clean-%)

.phony: clean build $(BUILDDIRS) $(CLEANDIRS)

build: $(BUILDDIRS)
$(BUILDDIRS):
		bmptk-make -C $(@:build-%=%) build

clean: $(CLEANDIRS)
$(CLEANDIRS):
		bmptk-make -C $(@:clean-%=%) clean

