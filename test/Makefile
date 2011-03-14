# Makefile for building & running  JPEG/EXIF reading, writing, redaction
# unit tests.

include ../Makefile.common

BINARY = jpegtest
EXIFTOOL = exiftool

LIB = ../lib/libredact.a

test: $(BINARY) test_exif test_simple test_windows

$(LIB): 
	cd ../lib; $(MAKE) lib

$(BINARY): $(LIB) jpegtest.cpp
	$(CC) $(CXXFLAGS) -I../lib jpegtest.cpp $(LIBPATH) $(LIB)  -o $@

.PHONY: clean test clean_rawgrey clean_test \
	test_exif test_simple test_windows

clean_test: clean_rawgrey
	rm -f testout/windows_output.log
	rm -f testout/simple_output.log
	rm -f testout/testexif_output.log testout/testexif.exiftxt

clean_rawgrey:
	rm -f rawgrey.pgm

clean:
	rm -f $(BINARY)
	rm -rf testout

depend: 
	$(MAKE) dependlocal

testout: 
	@mkdir testout

test_simple: $(BINARY) testout clean_rawgrey
	$(BINARY) testdata/simple.jpg > testout/simple_output.log
	@cmp  testdata/simple_output_golden.log testout/simple_output.log 
	@cmp  testdata/simple_rawgrey_golden.pgm rawgrey.pgm 
	@echo ""

test_windows: $(BINARY) testout clean_rawgrey
	$(BINARY) testdata/windows.jpg > testout/windows_output.log
	@cmp  testdata/windows_output_golden.log testout/windows_output.log
	@cmp  testdata/windows_rawgrey_golden.pgm rawgrey.pgm
	@echo ""


test_exif:  $(BINARY) testout clean_rawgrey
	$(BINARY) testdata/testexif.jpg > testout/testexif_output.log
	$(EXIFTOOL) testoutput.jpg | grep -v "^File"  > testout/testexif.exiftxt
	diff testdata/testexif_golden.exiftxt testout/testexif.exiftxt

