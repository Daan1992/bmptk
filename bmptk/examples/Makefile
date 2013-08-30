#############################################################################
# 
# Makefile for bmptk examples 
#
# (c) Wouter van Ooijen (www.voti.nl) 2012, 2013
# 
#############################################################################

SUBDIRS += db103

BUILDDIRS = $(SUBDIRS:%=build-%)
CLEANDIRS = $(SUBDIRS:%=clean-%)

.phony: clean build $(BUILDDIRS) $(CLEANDIRS)

build: $(BUILDDIRS)
$(BUILDDIRS):
		make -C $(@:build-%=%) build

clean: $(CLEANDIRS)
$(CLEANDIRS):
		make -C $(@:clean-%=%) clean

