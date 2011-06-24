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

#ifndef INCLUDE_JPEG_MARKER
#define INCLUDE_JPEG_MARKER

#include <vector>

namespace jpeg_redaction {
class JpegMarker {
 public:
  // Length is payload size- includes storage for length itself.
  JpegMarker(unsigned short marker, unsigned int location,
	     int length) {
    marker_ = marker;
    location_ = location_;
    length_ = length;
  }
  void LoadFromLocation(FILE *pFile) {
    fseek(pFile, location_ + 4, SEEK_SET);
    LoadHere(pFile);
  }
  void LoadHere(FILE *pFile) {
    data_.resize(length_-2);
    int rv = fread(&data_[0], sizeof(char), length_-2, pFile);
    if (rv  != length_-2) {
      printf("Failed to read marker %x at %d\n", marker_, length_);
      throw("Failed to read marker");
    }
  }
  void WriteWithStuffBytes(FILE *pFile);
  void RemoveStuffBytes();
  int Save(FILE *pFile);
  unsigned short slice_;
  // Length is payload size- includes storage for length itself.
  int length_;
  unsigned int location_;
  unsigned short marker_;
  std::vector<unsigned char> data_;
};  // JpegMarker
}  // namespace redaction

#endif // INCLUDE_JPEG_MARKER
