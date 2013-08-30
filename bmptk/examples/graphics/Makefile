#############################################################################
# 
# Makefile for bmptk examples/db103 
#
# (c) Wouter van Ooijen (www.voti.nl) 2012, 2013
# 
#############################################################################

SUBDIRS += lines clocks draw

BUILDDIRS = $(SUBDIRS:%=build-%)
CLEANDIRS = $(SUBDIRS:%=clean-%)

.phony: clean build $(BUILDDIRS) $(CLEANDIRS)

build: $(BUILDDIRS)
$(BUILDDIRS):
		make -C $(@:build-%=%) build

clean: $(CLEANDIRS)
$(CLEANDIRS):
		make -C $(@:clean-%=%) clean
