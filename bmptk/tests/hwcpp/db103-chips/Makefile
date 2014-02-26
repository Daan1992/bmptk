#############################################################################
# 
# Makefile for bmptk/hwcpp/tests/leds
#
# Tests for DB103 boards with various extra (peripheral) chips
#
# (c) Wouter van Ooijen (www.voti.nl) 2014
# 
#############################################################################

SUBDIRS += pcf8574a
SUBDIRS += pcf8574a-pcf8574a
SUBDIRS += hc595-kitt8
SUBDIRS += pcf8591-ad pcf8591-da
SUBDIRS += mcp23008-kitt4 mcp23008-sw
SUBDIRS += mcp23017-kitt3

BUILDDIRS = $(SUBDIRS:%=build-%)
CLEANDIRS = $(SUBDIRS:%=clean-%)

.phony: clean build $(BUILDDIRS) $(CLEANDIRS)

build: $(BUILDDIRS)
$(BUILDDIRS):
		make -C $(@:build-%=%) build

clean: $(CLEANDIRS)
$(CLEANDIRS):
		make -C $(@:clean-%=%) clean

