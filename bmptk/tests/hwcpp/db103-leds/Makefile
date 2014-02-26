#############################################################################
# 
# Makefile for bmptk/hwcpp/tests/leds
#
# Tests for a DB103 board (LPC1114) with 12 LEDs
#
# (c) Wouter van Ooijen (www.voti.nl) 2014
# 
#############################################################################

# empty main, to check the overhead
SUBDIRS += empty-12MHz

# blink LEDs with 12 MHz timing sources, tests all and invert
SUBDIRS += blink-12MHz-waiting blink-12MHz-timing blink-12MHz-callback

# blink LEDs with 48 MHz timing source
SUBDIRS += blink-48MHz-callback

# basic kitt, tests port_from_pins
SUBDIRS += kitt12-timing 

# tests all (from pins)
SUBDIRS += kitt4-waiting

# cout tests 
SUBDIRS += hello-12MHz hello-48MHz hello-48MHz-sw

# two source files (checks weak declaration of std::cout)
SUBDIRS += two-sources-blink

BUILDDIRS = $(SUBDIRS:%=build-%)
CLEANDIRS = $(SUBDIRS:%=clean-%)

.phony: clean build $(BUILDDIRS) $(CLEANDIRS)

build: $(BUILDDIRS)
$(BUILDDIRS):
		make -C $(@:build-%=%) build

clean: $(CLEANDIRS)
$(CLEANDIRS):
		make -C $(@:clean-%=%) clean

