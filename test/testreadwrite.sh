#!/bin/bash
binary=./testreadwrite

if [[ ! -x ${binary} ]]; then 
    echo "binary $binary does not exist"
    exit 1;
fi

for i in testdata/devices/*; do
    echo "testing $i"
    ${binary} $i > testout.log
    exiftool $i > src.exiflog
    exiftool testout/testoutput.jpg > out.exiflog
    # if diff src.exiflog out.exiflog; then
    # 	echo "src.exiflog out.exiflog differ"
    # 	exit 1;
    # fi
done
exit 0
