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


binary=./testreadwrite

if [[ ! -x ${binary} ]]; then 
    echo "binary $binary does not exist"
    exit 1;
fi

for i in testdata/devices/*; do
    echo "testing $i"
    ${binary} $i > testout/testreadwrite.log
    exiftool $i | grep -v "\(Thumbnail Offset\|Exif Byte Order\|^File \|^Directory\)" > testout/src.exiflog
    exiftool testout/testplainoutput.jpg  | grep -v "\(Thumbnail Offset\|Exif Byte Order\|^File \|^Directory \)"> testout/out.exiflog
    winmerge testout/src.exiflog testout/out.exiflog
    # if     diff testout/src.exiflog testout/out.exiflog; then
    # diff testout/src.exiflog testout/out.exiflog
    # 	echo "src.exiflog out.exiflog differ"
    # 	exit 1;
    # fi
done
exit 0
