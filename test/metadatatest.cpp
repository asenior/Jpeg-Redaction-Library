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


// Simple test of inserting a metadata blob into a JPEG file and
// retrieving it.

#include <stdlib.h>
#include <stdio.h>

#include <string>
#include "jpeg.h"

namespace jpeg_redaction {
namespace tests {
  int test_insert_and_save(const char * const filename) {
    try {
      Jpeg original;
      bool success = original.LoadFromFile(filename, true);
      if (!success) exit(1);
      std::vector<unsigned char> blob;
      blob.resize(2000);
      for (int i = 0; i < blob.size(); ++i) {
	blob[i] = (unsigned char)(rand() * 255);
      }
      original.SetObscuraMetaData(blob.size(), &blob.front());

      std::string output_filename = "testout/test_with_metadata.jpg";
      original.Save(output_filename.c_str());

      Jpeg reread;
      success = reread.LoadFromFile(output_filename.c_str(), true);
      if (!success) exit(1);
      unsigned int length = 0;
      const unsigned char *retrieved_blob = reread.GetObscuraMetaData(&length);
      if (length != blob.size()) throw("Retrieved metadata wrong length");
      if (memcmp(retrieved_blob, &blob.front(), length) != 0)
	throw("Retrieved metadata not identical.");
    } catch (const char *error) {
      fprintf(stderr, "Error: <%s> at outer level\n", error);
      exit(1);
    }
    return 0;
  }
}  // namespace tests
}  // namespace jpeg_redaction

int main(int argc, char **argv) {
  std::string filename("testdata/windows.jpg");
  if (argc > 1)
    filename = argv[1];
  jpeg_redaction::tests::test_insert_and_save(filename.c_str());
}
