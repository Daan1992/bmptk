#############################################################################
# 
# Makefile for bmptk documentation, zip file, web pages, etc.
#
# (c) Wouter van Ooijen (www.voti.nl) 2012
#
# targets:
#    clean                     : total cleanup 
#    doc, docs, documentation  : create documentation (docs subdir)
#    build, examples           : build all examples
#    push                      : clean and git push
#    zip                       : create bmptk.zip, includes documentation
#    www                       : create webpages (www subdir, includes zip)
#    ftp                       : create webpages and ftp to website
#                                (won't work from your PC)
# 
#############################################################################

.PHONY: clean doc docs documentation build all examples zip push

docs : doc
documentation: doc
all: doc examples
build: examples

REDIRECT := <META HTTP-EQUIV='refresh' CONTENT='0;URL=./docs/index.html'>

doc: 
		cd docsrc/generate && make picturesma
		Doxygen docsrc/Doxyfile
		echo "$(REDIRECT)" >index.html
		
examples:
		cd examples && make build

zip: clean doc		
		rm -rf index.html 
		cd docsrc/generate && make clean
		pkzip25 -add -path=relative -recurse bmptk.zip 
		
clean:
		rm -rf index.html docs bmptk.zip transfer
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