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
  } catch (const char *error) {
    fprintf(stderr, "Error: <%s> at outer level\n", error);
    exit(1);
  }
}
