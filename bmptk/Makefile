#############################################################################
# 
# Makefile for bmptk documentation, zip file, web pages, etc.
#
# Copyright (c) 2012 .. 2015 Wouter van Ooijen (www.voti.nl)
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)
#
# This file makes some meta-stuff that is probably 
# interesting only for me as bmptk maintainer.
#
# targets:
#    clean                     : total cleanup 
#    doc, docs, documentation  : create documentation (docs subdir)
#    build, examples           : build all examples
#    push                      : clean, git commit -a, git push 
#    zip                       : create bmptk.zip, includes documentation
#    www                       : create webpages (www subdir, includes zip)
#    ftp                       : create webpages and ftp to website
#                                (won't work from your PC ;) )
#    th04, v2cpse1, v2cpse2    : creates .zip for a specific course
# 
#############################################################################

.PHONY: clean doc docs documentation build all examples zip push tests
.PHONY: th04 

docs : doc
documentation: doc
all: doc examples tests
build: examples tests

REDIRECT := <META HTTP-EQUIV='refresh' CONTENT='0;URL=./docs/index.html'>

doc: 
	Doxygen docsrc/Doxyfile
	echo "$(REDIRECT)" >index.html
	@echo "**** documentation build completed succesfully"
		
examples:
	cd examples && make build
	@echo "**** examples build completed succesfully"

tests:
	cd tests && make build
	@echo "**** tests build completed succesfully"

zip: clean doc		
	rm -rf index.html 
	pkzip25 -add -path=relative -recurse bmptk.zip 
	@echo "**** zip build completed succesfully"
		
clean:
	rm -rf index.html docs bmptk.zip bmptk-th04.zip transfer
	cd examples && make clean	
	cd tests && make clean	
	@echo "**** cleanup completed succesfully"
		
push:
	make clean
	git commit -a -m 'work'
	git push
		
ftp: zip
	mkdir transfer
	-cp ../_ftp_crc_s transfer
	cp -R docsrc/www/* transfer
	cp bmptk.zip transfer
	cp -R docs transfer/docs
	python \ftpbmptk.py
	cp transfer/_ftp_crc_s ..
	@echo "**** ftp transfer completed succesfully"
      
TH04LIST += bmptk.h README
TH04LIST += license.txt license_1_0.txt
TH04LIST += Makefile Makefile.inc Makefile.local
TH04LIST += targets/cortex/* targets/cortex/cmsis/*/inc/* 
TH04LIST += tools/* tools/image_sizes/*
TH04LIST += hwcpp/* hwcpp/*/*
TH04LIST += index.html docs/* docs/search/*
TH04LIST += examples/v1th04/*
TH04LIST += examples/v1th04/*/*

th04: clean doc
	pkzip25 -add -path=current bmptk-th04.zip $(TH04LIST)
		

		