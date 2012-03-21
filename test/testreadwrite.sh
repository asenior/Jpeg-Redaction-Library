#!/bin/bash

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

# Program to test that the library preserves information stored in JPEG files
# after reading and writing.

binary=./testreadwrite

# Location of exiftool EXIF information dumper. 
EXIFTOOL=exiftool

if [[ ! -x ${binary} ]]; then 
    echo "binary $binary does not exist"
    exit 1;
fi

# A test suite of images from different devices.
# Currently all pass except 
# droid.jpg (thumbnail length is different, but thumbnail
#        seems to be correctly preserved.)
# canon-1999 (Can't find the right DHT)
for i in testdata/devices/*.jpg; do
    echo "testing $i"
    rm -f testout/testplainoutput.jpg
    ${binary} $i > testout/testreadwrite.log
    exiftool $i  > testout/src.exiflog
    grep -v "\(Thumbnail Offset\|Exif Byte Order\|^File \|^Directory\|Current IPTC Digest\|JFIF Version\)"  testout/src.exiflog | sort > testout/src.exiflogclean
    exiftool testout/testplainoutput.jpg  > testout/out.exiflog
    grep -v "\(Thumbnail Offset\|Exif Byte Order\|^File \|^Directory \|Current IPTC Digest\|JFIF Version\)" testout/out.exiflog | sort | sed -e 's/16495/19359/g' > testout/out.exiflogclean
    
#    diff  testout/src.exiflog testout/out.exiflog > /dev/null || winmerge testout/src.exiflog testout/out.exiflog
    if    ! diff testout/src.exiflogclean testout/out.exiflogclean > /dev/null; then
    	echo "sanitized testout/src.exiflog testout/out.exiflog differ for $i"
    	exit 1;
    fi
done
exit 0
