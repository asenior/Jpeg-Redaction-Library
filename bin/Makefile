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


# Makefile for building JPEG/EXIF reading, writing, redaction
# binaries.

include ../Makefile.common

BINARY = redact

LIB = ../lib/libredact.a

default: $(BINARY)

test:
	cd ../test; $(MAKE) test

$(LIB): 
	cd ../lib; $(MAKE) lib

$(BINARY): $(LIB) redaction_main.cpp
	$(CC) $(CXXFLAGS) -I../lib redaction_main.cpp $(LIBPATH) $(LIB)  -o $@

.PHONY: clean cleanall clean_rawgrey clean_test \
	$(LIB)

clean:
	rm -f $(BINARY)

veryclean: cleanall

cleanall: clean
	cd ../lib; $(MAKE) clean
	cd ../test; $(MAKE) clean

depend: 
	$(MAKE) dependlocal

# DO NOT DELETE
