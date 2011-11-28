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
#include "jpeg.h"
#include "redaction.h"

namespace jpeg_redaction {
  namespace tests {
    int test_exif_removal(const char * const filename) {
      try {
	Jpeg j2;
	bool success = j2.LoadFromFile(filename, true);
	if (!success) exit(1);
	j2.RemoveTag(TiffTag::tag_ExifIFDPointer);
	j2.Save("testout/test_noexif.jpg");
      } catch (const char *error) {
	fprintf(stderr, "Error: <%s> at outer level\n", error);
	exit(1);
      }
      return 0;
    }

    int test_sensitive_removal(const char * const filename) {
      try {
	Jpeg j2;
	bool success = j2.LoadFromFile(filename, true);
	if (!success) exit(1);
	j2.RemoveAllSensitive();
	j2.Save("testout/test_nosensitive.jpg");
      } catch (const char *error) {
	fprintf(stderr, "Error: <%s> at outer level\n", error);
	exit(1);
      }
      return 0;
    }

    int test_gps_removal(const char * const filename) {
      try {
	Jpeg j2;
	bool success = j2.LoadFromFile(filename, true);
	if (!success) exit(1);
	int rem = j2.RemoveTag(TiffTag::tag_ExifIFDPointer);
	if (rem != 1) throw("Didn't remove one exif tag");
	rem = j2.RemoveTag(TiffTag::tag_GpsInfoIFDPointer);
	if (rem != 1) throw("Didn't remove one gps tag");
	j2.Save("testout/test_noexifgps.jpg");
      } catch (const char *error) {
	fprintf(stderr, "Error: <%s> at outer level\n", error);
	exit(1);
      }
      return 0;
    }

    int test_overwrite_old_string(const char * const filename) {
      try {
	Jpeg j2;
	const char *new_maker = "Replace the maker with this string";
	bool success = j2.LoadFromFile(filename, true);
	if (!success) exit(1);
	TiffTag *make = j2.FindTag(TiffTag::tag_Make);
	if (make == NULL) throw("Couldn't find Make tag");
	if (strcmp(make->GetStringValue(), "Panasonic") != 0)
	  throw("Maker not \"Panasonic\"");
	make->SetStringValue(new_maker);
	if (strcmp(make->GetStringValue(), new_maker) != 0)
	  throw("Maker not reset");
	j2.Save("testout/test_newmaker.jpg");
      } catch (const char *error) {
	fprintf(stderr, "Error: <%s> at outer level\n", error);
	exit(1);
      }
      return 0;
    }
  } // namespace tests
} // namespace jpeg_redaction

int main(int argc, char **argv) {
  std::string filename("testdata/windows.jpg");
  if (argc > 1)
    filename = argv[1];
  jpeg_redaction::tests::test_exif_removal(filename.c_str());
  jpeg_redaction::tests::test_gps_removal(filename.c_str());
  jpeg_redaction::tests::test_sensitive_removal(filename.c_str());
  jpeg_redaction::tests::test_overwrite_old_string(filename.c_str());
}
