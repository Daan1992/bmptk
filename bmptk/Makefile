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
#    zip                       : create bmptk.zip, includes documentation
#    www                       : create webpages (www subdir, includes zip)
#    ftp                       : create webpages and ftp to website
# 
#############################################################################

.PHONY: clean doc docs documentation build all examples zip

docs : doc
documentation: doc
all: doc examples
build: examples

REDIRECT := <META HTTP-EQUIV='refresh' CONTENT='0;URL=./docs/index.html'>

doc: 
		Doxygen docsrc/Doxyfile
		echo "$(REDIRECT)" >index.html
		
examples:
		cd examples && make build

zip: clean doc		
		rm -rf index.html 
		pkzip25 -add -path=relative -recurse bmptk.zip 
		
clean:
		rm -rf index.html docs bmptk.zip
		cd examples && make clean	
		
