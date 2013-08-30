#############################################################################
# 
# Makefile for bmptk examples/db103 
#
# (c) Wouter van Ooijen (www.voti.nl) 2012, 2013
# 
#############################################################################

SUBDIRS += blink blink-p0-0  wiggle
SUBDIRS += kitt kitt-pullups
SUBDIRS += hc595-kitt
SUBDIRS += pcf8574a-blink pcf8574a-kitt pcf8574a-pcf8574a
SUBDIRS += hello hello-buffered
SUBDIRS += ad-conversion
SUBDIRS += lcd5510-lines

BUILDDIRS = $(SUBDIRS:%=build-%)
CLEANDIRS = $(SUBDIRS:%=clean-%)

.phony: clean build $(BUILDDIRS) $(CLEANDIRS)

build: $(BUILDDIRS)
$(BUILDDIRS):
		make -C $(@:build-%=%) build

clean: $(CLEANDIRS)
$(CLEANDIRS):
		make -C $(@:clean-%=%) clean
