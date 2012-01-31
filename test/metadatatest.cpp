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
#include "debug_flag.h"

namespace jpeg_redaction {
namespace tests {
  int test_insert_and_save(const char * const filename) {
    try {
      Jpeg original;
      printf("*********** Loading original image*********** ***************\n");
      bool success = original.LoadFromFile(filename, true);
      if (!success) exit(1);
      std::vector<unsigned char> blob;
      blob.resize(2000);
      for (int i = 0; i < blob.size(); ++i) {
	blob[i] = (unsigned char)(rand() * 255);
      }
      original.SetObscuraMetaData(blob.size(), &blob.front());

      printf("*********** Writing image with added metadata ***************\n");
      std::string output_filename = "testout/test_with_metadata.jpg";
      debug = 1;
      original.Save(output_filename.c_str());

      printf("*********** Reloading image with added metadata *************\n");
      Jpeg reread;
      success = reread.LoadFromFile(output_filename.c_str(), true);
      if (!success) {
	fprintf(stderr, "Failed read");
	exit(1);
      }
      unsigned int length = 0;
      const unsigned char *retrieved_blob = reread.GetObscuraMetaData(&length);
      if (length != blob.size()) {
	fprintf(stderr, "Original length %zu, new length %u\n",
		blob.size(), length);
	throw("Retrieved metadata wrong length");
      }
      if (memcmp(retrieved_blob, &blob.front(), length) != 0)
	throw("Retrieved metadata not identical.");
    } catch (const char *error) {
      fprintf(stderr, "Error: <%s> at outer level\n", error);
      exit(1);
    }
    return 0;
  }

  // Take an image, redact it. 
  // Write redaction regions in the redacted JPEG file.
  // Reload and reverse the redaction.
  int test_save_redaction_regions(const char * const filename) {
    try {
      Jpeg original;
      printf("*********** Loading original image*********** ***************\n");
      bool success = original.LoadFromFile(filename, true);
      if (!success) exit(1);
      Redaction redaction;
      Redaction::Region rect(50, 300, 64, 79);  // l,r, t, b
      redaction.AddRegion(rect);
      original.DecodeImage(&redaction, NULL);
      if (!redaction.ValidateStrips())
	throw("Strips not valid");
      // sos_block = j2.GetMarker(Jpeg::jpeg_sos);
      //	SaveBytes(sos_block->data_, "testout/redactedsos");
      std::vector<unsigned char> redaction_pack;
      redaction.Pack(&redaction_pack);
      printf("Packed redaction object is %d bytes\n", redaction_pack.size());
      original.SetObscuraMetaData(redaction_pack.size(),
				  &redaction_pack.front());
      
      printf("*********** Writing image with added metadata ***************\n");
      std::string output_filename = "testout/test_with_metadata.jpg";
      debug = 1;
      original.Save(output_filename.c_str());

      printf("*********** Reloading image with added metadata *************\n");
      Jpeg reread;
      success = reread.LoadFromFile(output_filename.c_str(), true);
      if (!success) {
	fprintf(stderr, "Failed read");
	exit(1);
      }
      unsigned int length = 0;
      const unsigned char *retrieved_blob = reread.GetObscuraMetaData(&length);
      if (length != redaction_pack.size()) {
	fprintf(stderr, "Original length %zu, new length %u\n",
		blob.size(), length);
	throw("Retrieved metadata wrong length");
      }
      // Check that the two packs are the same.
      if (memcmp(retrieved_blob, &redaction_pack.front(), length) != 0)
	throw("Retrieved metadata not identical.");
      
      // Now use the pack to reverse the redaction.
      std::vector<unsigned char> loaded_pack(retrieved_blob, length);
      Redaction unpacked_redaction;
      unpacked_redaction.Unpack(loaded_pack);
      reread.ReverseRedaction(unpacked_redaction);
      std::string output_filename = "testout/testunredacted_pack.jpg";
      if (reread.Save(output_filename.c_str()) != 0) {
	fprintf(stderr, "Couldn't save %s\n", output_filename.c_str());
	return 1;
      }
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
  jpeg_redaction::tests::test_save_redaction_regions(filename.c_str());
}
