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


#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <vector>
#include "../lib/debug_flag.h"
#include "jpeg.h"
#include "redaction.h"
#include "test_utils.h"

int main(int argc, char **argv) {
  // An 1152 x 693 JPEG without EXIF.
  std::string filename("testdata/devices/samsung1.jpg");
  if (argc > 1)
    filename = argv[1];
  // We'll compare goldens of stdout for each file, so dump 
  // interesting info to stdout.
  jpeg_redaction::debug = 0;
  // l,r,t,b[:method];...
  string regions = ";50,300,50,200;";
  rv = jpeg_redaction::tests::test_redaction(filename.c_str(), regions.c_str());
  if (rv)  {
    fprintf(stderr, "Failed on test_redaction %s\n", regions.c_str());
    exit(1);
  }
  regions = "-50,-10,50,200;";
  rv = jpeg_redaction::tests::test_redaction(filename.c_str(), regions.c_str());
  if (rv)  {
    fprintf(stderr, "Failed on test_redaction %s\n", regions.c_str());
    exit(1);
  }
}
