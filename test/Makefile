#  Copyright (C) 2011 Andrew W. Senior andrew.senior[AT]gmail.com
#  Part of the Jpeg-Redaction-Library to read, parse, edit redact and
#  write JPEG/EXIF/JFIF images.
#  See https://github.com/asenior/Jpeg-Redaction-Library

#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.

#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.

#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.


# Makefile for building & running  JPEG/EXIF reading, writing, redaction
# unit tests.

include ../Makefile.common

BINARY = jpegtest
READWRITE = testreadwrite
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

$(BITSHIFTS): bit_shifts_test.cpp ../lib/bit_shifts.h testout_dir ../lib/debug_flag.cpp
	$(CC) $(CXXFLAGS) -I../lib bit_shifts_test.cpp ../lib/debug_flag.cpp $(LIBPATH)  -o $@

$(BINARY): $(LIB) jpegtest.cpp test_utils.cpp testout_dir
	$(CC) $(CXXFLAGS) -I../lib jpegtest.cpp  test_utils.cpp $(LIBPATH) $(LIB)  -o $@

$(READWRITE): $(LIB) testreadwrite.cpp testout_dir test_utils.cpp testout_dir
	$(CC) $(CXXFLAGS) -I../lib testreadwrite.cpp  test_utils.cpp $(LIBPATH) $(LIB)  -o $@

$(EXIF_REMOVE): $(LIB) exiftest.cpp testout_dir
	$(CC) $(CXXFLAGS) -I../lib exiftest.cpp $(LIBPATH) $(LIB)  -o $@

$(IFDTESTBINARY): $(LIB) ifdtest.cpp testout_dir
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
	./$(IFDTESTBINARY) > testout/ifdtest.log
	diff testdata/ifdtest_golden.log testout/ifdtest.log
	@echo "== " $@ " passed"

test_simple: $(BINARY) testout_dir clean_rawgrey
	./$(BINARY) testdata/simple.jpg > testout/simple_output.log
	diff  testdata/simple_output_golden.log testout/simple_output.log > /dev/null
	@cmp  testdata/simple_rawgrey_golden.pgm testout/rawgrey.pgm 
	@echo "== " $@ " passed"


test_windows: $(BINARY) testout_dir clean_rawgrey
	./$(BINARY) testdata/windows.jpg > testout/windows_output.log
	diff  testdata/windows_output_golden.log testout/windows_output.log > /dev/null
	@cmp  testdata/windows_out_redacted.jpg testout/testoutput.jpg
	@cmp  testdata/windows_rawgrey_golden.pgm testout/rawgrey.pgm
	@echo "== " $@ " passed"


test_exif:  $(BINARY) testout_dir clean_rawgrey
	./$(BINARY) testdata/testexif.jpg > testout/testexif_output.log
	$(EXIFTOOL) testoutput.jpg | grep -v "^File"  > testout/testexif.exiftxt
	diff testdata/testexif_golden.exiftxt testout/testexif.exiftxt > /dev/null
	@echo "== " $@ " passed"

test_exif_removal:  $(EXIF_REMOVE) testout_dir clean_rawgrey
	./$(EXIF_REMOVE) testdata/windows.jpg > testout/testexif_removal_output.log
	@cmp testout/test_noexif.jpg testdata/test_noexif_golden.jpg
	@cmp testout/test_noexifgps.jpg testdata/test_noexifgps_golden.jpg
	@cmp testout/test_nosensitive.jpg testdata/test_nosensitive_golden.jpg
	@echo "== " $@ " passed"

test_bit_shifts: $(BITSHIFTS)
	./$(BITSHIFTS)  > testout/test_bit_shifts_output.log
	@echo "== " $@ " passed"

test_devices:  $(READWRITE) testout_dir
	./testreadwrite.sh

# DO NOT DELETE
