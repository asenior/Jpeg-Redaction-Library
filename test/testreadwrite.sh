#!/bin/bash
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
