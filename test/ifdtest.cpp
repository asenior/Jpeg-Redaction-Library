// Copyright (C) 2011 Andrew W. Senior andrew.senior[AT]gmail.com
// Part of the Jpeg-Redaction-Library to read, parse, edit redact and
// write JPEG/EXIF/JFIF images.
// See https://github.com/asenior/Jpeg-Redaction-Library

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


// Simple test of reading an IFD at a given location.
#include <stdlib.h>
#include <stdio.h>

#include <string>
#include "tiff_ifd.h"

int main(int argc, char **argv) {
  std::string filename("testdata/windows.jpg");
  unsigned int ifd_loc = 20;
  unsigned int tiff_offset = 12;
  bool byte_swapping = false;
  if (argc > 1)
    filename = argv[1];
  if (argc > 2)
    ifd_loc = atoi(argv[2]);
  printf("Dumping IFD in %s at position %u byteswapping %s\n",
         filename.c_str(), ifd_loc, (byte_swapping ? "on" : "off"));
  try {
    FILE *pFile = fopen(filename.c_str(), "rb");
    if (pFile == NULL) {
      fprintf(stderr, "Can't open file %s\n", filename.c_str());
      exit(1);
    }
    bool load_all = true;
    jpeg_redaction::TiffIfd ifd(pFile, ifd_loc, load_all,
                                tiff_offset, byte_swapping);
    printf("%d tags in IFD\n",  ifd.GetNTags());
    if (ifd.GetNTags() != 14) throw("Wrong number of tags");
    const int tag_types[] = {0x10f, 0x110, 0x112, 0x11a, 0x11b, 0x128,
			     0x131, 0x132, 0x213, 0x8769, 0x8825, 0xc4a5,
			     0xc6d2, 0xc6d3};
    const int tag_counts[] = {10,8, 1, 1, 1, 1, 10, 20, 1, 1, 1, 208, 64, 128};
    for (int i=0; i < 14; ++i) {
      if (ifd.GetTag(i)->GetCount() != tag_counts[i])
	throw("Wrong length at tag N");
      if (ifd.GetTag(i)->GetTag() != tag_types[i])
	throw("Wrong type at tag N");
    }
  } catch (const char *error) {
    fprintf(stderr, "Error: <%s> at outer level\n", error);
    exit(1);
  }
}
