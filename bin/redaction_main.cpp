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


// Simple binary to apply redaction to a JPEG image
// Or reverse that redaction.

#include <stdlib.h>
#include <stdio.h>

#include <string>
#include "jpeg.h"
#include "redaction.h"
using std::string;
namespace jpeg_redaction {
    int reverse_redaction(const char * const filename) {
      try {
	Jpeg j2;
	bool success = j2.LoadFromFile(filename, true);
	Redaction::Region rect(50, 300, 50, 200);  // l,r, t, b
	Redaction redaction;

	redaction.AddRegion(rect);
	j2.DecodeImage(&redaction, NULL);
	if (!redaction.ValidateStrips())
	  throw("Strips not valid");
	j2.ReverseRedaction(redaction);
	std::string output_filename = "testout/testunredacted.jpg";
	if (j2.Save(output_filename.c_str()) != 0) {
	  fprintf(stderr, "Couldn't save %s\n", output_filename.c_str());
	  return 1;
	}
      } catch (const char *error) {
	fprintf(stderr, "Error: <%s> at outer level\n", error);
	return 1;
      }
      return 0;
    }
  int redact(const string &filename,
	     const string &output,
	     const string &regions) {
      try {
	Jpeg j2;
	bool success = j2.LoadFromFile(filename.c_str(), true);
	Redaction redaction;

	redaction.AddRegions(regions);
	j2.DecodeImage(&redaction, NULL);
	if (!redaction.ValidateStrips())
	  throw("Strips not valid");
	if (j2.Save(output.c_str()) != 0) {
	  fprintf(stderr, "Couldn't save %s\n", output.c_str());
	  return 1;
	}
      } catch (const char *error) {
	fprintf(stderr, "Error: <%s> at outer level\n", error);
	return 1;
      }
      return 0;
    }
} // namespace jpeg_redaction

int main(int argc, char **argv) {
  std::string filename;
  std::string outfile;
  std::string regions;
  int start_arg = 1;
  if (argc - start_arg <= 2) {
    fprintf(stderr, "%s <infile> <outfile> <l,r,t,b[:method];...>\n"
	    "method is one of [c]opystrip, [S]olid, [p]ixellate,"
	    "[i]nverse pixellate\n", argv[0]);
    exit(1);
  }
  filename = argv[start_arg];
  outfile = argv[start_arg+1];
  regions = argv[start_arg+2];
  return jpeg_redaction::redact(filename, outfile, regions);
}
