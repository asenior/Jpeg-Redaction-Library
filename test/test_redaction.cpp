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

int TestRedaction(const std::string &filename, const char *const regions) {
  int rv = jpeg_redaction::tests::test_redaction(filename.c_str(), regions);
  if (rv)  {
    fprintf(stderr, "Failed on test_redaction %s\n", regions);
    return 1;
  }
  return 0;
}

int TestRedactionPack(const std::string &filename, const char *const regions) {
  int rv = jpeg_redaction::tests::test_redaction_pack_unpack(filename.c_str(),
							     regions);
  if (rv)  {
    fprintf(stderr, "Failed on test_redaction_pack_unpack %s\n", regions);
    return 1;
  }
  return 0;
}

int main(int argc, char **argv) {
  // An 1152 x 693 JPEG without EXIF.
  // redaction strings are l,r,t,b[:method];...
  // [c]opystrip, [S]olid, [p]ixellate,[i]nverse pixellate
  std::string filename("testdata/devices/samsung1.jpg");
  if (argc > 1)
    filename = argv[1];
  jpeg_redaction::debug = 0;

  if (TestRedactionPack(filename, ";50,300,50,200:p;")) return 1;

  // Different redaction types.
  if (TestRedaction(filename, ";50,300,50,200:p;")) return 1;
  if (TestRedaction(filename, ";50,300,50,200:s;")) return 1;
  if (TestRedaction(filename, ";50,300,50,200:c;")) return 1;
  if (TestRedaction(filename, ";50,300,50,200:i;")) return 1;

  // Corner cases.
  // Completely off left.
  if (TestRedaction(filename, "-50,-10,50,200:p;")) return 1;
  // Overlapping top left.
  if (TestRedaction(filename, "-50,800,-50,200:p;")) return 1;
  if (TestRedaction(filename, "50,199,-500,-200:p;")) return 1;
  if (TestRedaction(filename, "2000,2200,50,200:p;")) return 1;
  if (TestRedaction(filename, "-10,1200,50,200:p;")) return 1;

  // Multiple regions.
  if (TestRedaction(filename, ";50,300,50,200:p;200,500,120,500:p")) return 1;
  // ... with different types.
  if (TestRedaction(filename, ";50,300,50,200:s;200,500,120,500:p")) return 1;
  if (TestRedaction(filename, ";50,300,50,200:p;200,500,120,500:s")) return 1;
  if (TestRedaction(filename, ";50,300,50,200:s;200,500,120,500:s")) return 1;
}
