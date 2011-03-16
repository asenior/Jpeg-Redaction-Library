# makefile for building JPEG/EXIF tests under cygwin
include Makefile.common

.PHONY: clean test depend test testbin

testbin:
	cd test; $(MAKE) jpegtest

run:
	test/jpegtest test/testdata/windows.jpg

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
