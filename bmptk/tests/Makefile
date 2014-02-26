#############################################################################
# 
# Makefile for bmptk tests directory 
#
# (c) Wouter van Ooijen (www.voti.nl) 2012, 2013
# 
#############################################################################

SUBDIRS += hwcpp

BUILDDIRS = $(SUBDIRS:%=build-%)
CLEANDIRS = $(SUBDIRS:%=clean-%)

.phony: clean build $(BUILDDIRS) $(CLEANDIRS)

build: $(BUILDDIRS)
$(BUILDDIRS):
		make -C $(@:build-%=%) build

clean: $(CLEANDIRS)
$(CLEANDIRS):
		make -C $(@:clean-%=%) clean

