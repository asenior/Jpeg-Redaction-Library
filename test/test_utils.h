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


#ifndef INCLUDE_JPEG_REDACTION_TEST_TEST_UTILS
#define INCLUDE_JPEG_REDACTION_TEST_TEST_UTILS

#include <string>
#include <vector>
namespace jpeg_redaction {
  //  class Redaction::Rect;
  namespace tests {
    bool compare_to_golden(const char * const filename,
			   const char * const golden_name);
    bool compare_bytes(const std::vector<unsigned char> &orig,
		       const std::vector<unsigned char> &other);
    void SaveBytes(const std::vector<unsigned char> &bytes,
		   const char *const fn);
    int test_loadallfalse(const char * const filename);
    int test_readwrite(const char * const filename);
    int test_redaction(const char * const filename);
    int test_reversingredaction(const char * const filename,
				const Redaction::Region &rect);
    int test_reversingredactions_multi(const std::string &filename);
  } // namespace tests
} // namespace jpeg_redaction
#endif  // INCLUDE_JPEG_REDACTION_TEST_TEST_UTILS
