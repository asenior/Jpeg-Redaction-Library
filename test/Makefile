# Makefile for building & running  JPEG/EXIF reading, writing, redaction
# unit tests.

include ../Makefile.common

BINARY = jpegtest
IFDTESTBINARY = ifdtest
EXIF_REMOVE = exiftest
BITSHIFTS = bit_shifts_test
EXIFTOOL = exiftool

LIB = ../lib/libredact.a

default: all

all: $(BINARY) $(BITSHIFTS) $(EXIF_REMOVE) $(IFDTESTBINARY)

test: $(BINARY) test_ifd test_simple test_windows test_exif_removal \
	test_bit_shifts

$(LIB): 
	cd ../lib; $(MAKE) lib

$(BITSHIFTS): bit_shifts_test.cpp ../lib/bit_shifts.h
	$(CC) $(CXXFLAGS) -I../lib bit_shifts_test.cpp $(LIBPATH)  -o $@

$(BINARY): $(LIB) jpegtest.cpp
	$(CC) $(CXXFLAGS) -I../lib jpegtest.cpp $(LIBPATH) $(LIB)  -o $@

$(EXIF_REMOVE): $(LIB) exiftest.cpp
	$(CC) $(CXXFLAGS) -I../lib exiftest.cpp $(LIBPATH) $(LIB)  -o $@

$(IFDTESTBINARY): $(LIB) ifdtest.cpp
	$(CC) $(CXXFLAGS) -I../lib ifdtest.cpp $(LIBPATH) $(LIB)  -o $@

.PHONY: clean cleanall clean_rawgrey clean_test \
	test testout_dir test_exif test_simple test_windows test_ifd \
	test_exif_removal \
	$(LIB)

clean_test: clean_rawgrey
	rm -f testout/windows_output.log
	rm -f testout/simple_output.log
	rm -f testout/testexif_output.log testout/testexif.exiftxt

clean_rawgrey:
	@rm -f testout/rawgrey.pgm

clean:
	rm -f $(BINARY) $(IFDTESTBINARY)
	rm -rf testout

veryclean: cleanall

cleanall: clean
	cd ../lib; $(MAKE) clean

depend: 
	$(MAKE) dependlocal

testout_dir: 
	@mkdir -p testout

test_ifd: $(IFDTESTBINARY) testout_dir
	./$(IFDTESTBINARY) | sed -e 's/nan/-nan/g' > testout/ifdtest.log
	diff testdata/ifdtest_golden.log testout/ifdtest.log
	@echo "== " $@ " passed"

test_simple: $(BINARY) testout_dir clean_rawgrey
	./$(BINARY) testdata/simple.jpg > testout/simple_output.log
	diff  testdata/simple_output_golden.log testout/simple_output.log 
	@cmp  testdata/simple_rawgrey_golden.pgm testout/rawgrey.pgm 
	@echo "== " $@ " passed"


test_windows: $(BINARY) testout_dir clean_rawgrey
	./$(BINARY) testdata/windows.jpg  | sed -e 's/nan/-nan/g' > testout/windows_output.log
	diff  testdata/windows_output_golden.log testout/windows_output.log
	@cmp  testdata/windows_out_redacted.jpg testout/testoutput.jpg
	@cmp  testdata/windows_rawgrey_golden.pgm testout/rawgrey.pgm
	@echo "== " $@ " passed"


test_exif:  $(BINARY) testout_dir clean_rawgrey
	./$(BINARY) testdata/testexif.jpg > testout/testexif_output.log
	$(EXIFTOOL) testoutput.jpg | grep -v "^File"  > testout/testexif.exiftxt
	diff testdata/testexif_golden.exiftxt testout/testexif.exiftxt
	@echo "== " $@ " passed"

test_exif_removal:  $(EXIF_REMOVE) testout_dir clean_rawgrey
	./$(EXIF_REMOVE) testdata/windows.jpg > testout/testexif_removal_output.log
	@cmp testout/test_noexif.jpg testdata/test_noexif_golden.jpg
	@cmp testout/test_noexifgps.jpg testdata/test_noexifgps_golden.jpg
	@cmp testout/test_nosensitive.jpg testdata/test_nosensitive_golden.jpg
	@echo "== " $@ " passed"

test_bit_shifts: $(BITSHIFTS)
	./$(BITSHIFTS)

# DO NOT DELETE
