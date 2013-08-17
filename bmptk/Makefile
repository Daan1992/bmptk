#############################################################################
# 
# Makefile for bmptk documentation, zip file, web pages, etc.
#
# (c) Wouter van Ooijen (www.voti.nl) 2012, 2013
#
# targets:
#    clean                     : total cleanup 
#    doc, docs, documentation  : create documentation (docs subdir)
#    build, examples           : build all examples
#    push                      : clean and git push (does not work??)
#    zip                       : create bmptk.zip, includes documentation
#    www                       : create webpages (www subdir, includes zip)
#    ftp                       : create webpages and ftp to website
#                                (won't work from your PC)
#    th04                      : create .zip for TH04 lessons
# 
#############################################################################

.PHONY: clean doc docs documentation build all examples zip push

docs : doc
documentation: doc
all: doc examples
build: examples

REDIRECT := <META HTTP-EQUIV='refresh' CONTENT='0;URL=./docs/index.html'>

doc: 
		#cd docsrc/generate && make pictures
		Doxygen docsrc/Doxyfile
		echo "$(REDIRECT)" >index.html
		
examples:
		cd examples && make build

zip: clean doc		
		rm -rf index.html 
		cd docsrc/generate && make clean
		pkzip25 -add -path=relative -recurse bmptk.zip 
		
clean:
		rm -rf index.html docs bmptk.zip th04.zip transfer
		cd docsrc/generate && make clean
		cd examples && make clean	
		
push:
		make clean
		git push
		
ftp: zip
		mkdir transfer
		-cp ../_ftp_crc_s transfer
		cp -R docsrc/www/* transfer
		cp bmptk.zip transfer
		cp -R docs transfer/docs
		python \ftpbmptk.py
		cp transfer/_ftp_crc_s ..
		
TH04LIST += bmptk.h license.txt Makefile Makefile.inc Makefile.local
TH04LIST += targets/bmptk_lpc1114* targets/libaeabi-cortexm0/*
TH04LIST += targets/cmsis/11xx/*/* 
TH04LIST += tools/lpc21isp.exe
TH04LIST += examples/TH04/*
TH04LIST += examples/TH04/blink/* 
TH04LIST += examples/TH04/switch/* 
TH04LIST += examples/TH04/beep/*
TH04LIST += examples/TH04/440Hz/*
TH04LIST += examples/TH04/timer/*
TH04LIST += examples/TH04/uart/*
TH04LIST += examples/TH04/servo/*
TH04LIST += examples/TH04/ds1990/*
TH04LIST += examples/TH04/i2c/*
TH04LIST += examples/TH04/lcd5510/*
TH04LIST += examples/TH04/rfm73/*

th04: clean
		pkzip25 -add -path=current th04.zip $(TH04LIST)
		
		