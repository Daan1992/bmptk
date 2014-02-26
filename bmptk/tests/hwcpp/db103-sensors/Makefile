#############################################################################
# 
# Makefile for bmptk/hwcpp/tests/leds
#
# Tests for a DB103 board (LPC1114) with 12 LEDs
#
# (c) Wouter van Ooijen (www.voti.nl) 2014
# 
#############################################################################

# read a DHT11 temperature & humidity sensor
SUBDIRS += dht11

BUILDDIRS = $(SUBDIRS:%=build-%)
CLEANDIRS = $(SUBDIRS:%=clean-%)

.phony: clean build $(BUILDDIRS) $(CLEANDIRS)

build: $(BUILDDIRS)
$(BUILDDIRS):
		make -C $(@:build-%=%) build

clean: $(CLEANDIRS)
$(CLEANDIRS):
		make -C $(@:clean-%=%) clean

