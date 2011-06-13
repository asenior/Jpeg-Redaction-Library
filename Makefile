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
	cd bin; $(MAKE) clean

dependall: 
	cd lib; $(MAKE) dependlocal
	cd test; $(MAKE) dependlocal
	cd bin; $(MAKE) dependlocal

cleandependall: 
	cd lib; $(MAKE) cleandependlocal
	cd test; $(MAKE) cleandependlocal
	cd bin; $(MAKE) cleandependlocal
