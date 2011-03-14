# makefile for building JPEG/EXIF tests under cygwin
include ../Makefile.common

BINARY = jpegtest

LIB = ../lib/libredact.a

test: $(BINARY) test_simple test_windows

$(LIB): 
	cd ../lib; $(MAKE) lib

$(BINARY): $(LIB) jpegtest.cpp
	$(CC) $(CXXFLAGS) -I../lib jpegtest.cpp $(LIBPATH) $(LIB)  -o $@

.PHONY: clean test clean_rawgrey clean_test test_simple test_windows

clean_test: clean_rawgrey
	rm -f testdata/windows_output.log
	rm -f testdata/simple_output.log

clean_rawgrey:
	rm -f rawgrey.pgm

clean:
	rm -f $(BINARY)

depend: 
	$(MAKE) dependlocal

test_simple: $(BINARY) clean_rawgrey
	$(BINARY) testdata/simple.jpg > testdata/simple_output.log
	@cmp  testdata/simple_output.log testdata/simple_output_golden.log
	@cmp  rawgrey.pgm testdata/simple_rawgrey_golden.pgm
	@echo ""

test_windows: $(BINARY) clean_rawgrey
	$(BINARY) testdata/windows.jpg > testdata/windows_output.log
	@cmp  testdata/windows_output.log testdata/windows_output_golden.log
	@cmp  rawgrey.pgm testdata/windows_rawgrey_golden.pgm
	@echo ""