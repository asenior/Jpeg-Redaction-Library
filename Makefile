# makefile for building JPEG/EXIF tests under cygwin
include Makefile.common

.PHONY: clean test depend

test: 
	cd test; $(MAKE) test

lib: 
	cd lib; $(MAKE) lib

clean:
	cd lib; $(MAKE) clean
	cd test; $(MAKE) clean

depend: 
	cd lib; $(MAKE) dependlocal
	cd test; $(MAKE) dependlocal
