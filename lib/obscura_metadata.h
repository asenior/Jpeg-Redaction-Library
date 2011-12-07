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

#ifndef INCLUDE_OBSCURA_METADATA
#define INCLUDE_OBSCURA_METADATA

#include <vector>

// A class to store metadata specific to ObscuraCam: 
// A general metadata block to be parsed by the application.
// and a redaction-reversal block to be parsed by this library.
namespace jpeg_redaction {
class JpegMarker;

class ObscuraMetadata {
public:
  ObscuraMetadata() {}
  void SetDescriptor(unsigned int length,
		     const unsigned char *data) {
    if (length > 64 * 1024 - 2)
      throw("Metadata is too long");
    descriptor_.resize(length);
    if (length > 0)
      memcpy(&descriptor_[0], data, length);
  }
  const unsigned char *GetDescriptor(unsigned int *length) const {
    if (descriptor_.size() > 0) {
      *length = descriptor_.size();
      return &descriptor_[0];
    }
    *length = 0;
    return NULL;
  }
  
  // Return true if the marker was understood and stored in this structure.
  bool ImportMarker(JpegMarker * marker) {
    if (marker == NULL || marker->marker_ != kObscuraMarker)
      return false;
    if (strcmp(marker->data_, descriptor_type_) == 0) {
      AddDescriptor(marker->length_ - strlen(descriptor_type_) - 1,
		    marker->data_ + strlen(descriptor_type_) + 1);
      return true;
    } else if (strcmp(marker->data_, redaction_data_type_) == 0) {
      // TODO: import redaction data.
      return true;
    }
    fprintf(stderr, "Unknown AppN Marker");
    return false;
  }
  // Store all the metadata in a file as APPN JPEG Markers.
  int Write(FILE *pFile) {
    // Convert the data to markers and write.
    JpegMarker *descriptor = MakeDescriptorMarker();
    if (descriptor != NULL) {
      descriptor->Write(pFile);
      delete descriptor;
    }
    std::vector<JpegMarker *> *redaction_markers = MakeRedactionMarkers();
    if (redaction_markers != NULL) {
      for (int i = 0; i < redaction_markers->size(); ++i) {
	(*redaction_markers)[i]->Write(pFile);
	delete (*redaction_markers)[i];
      }
      delete redaction_markers;
    }
    return 0;
  }

protected:
  // Make a JPEG marker containing the descriptor information.
  JpegMarker *MakeDescriptorMarker() {
    if (descriptor_.size() == 0)
      return NULL;
    int string_len = strlen(descriptor_type_) + 1;
    int len = string_len + length;
    vector<unsigned char> long_data(len);
    memcpy(&long_data[0], descriptor_type_, string_len);
    memcpy(&long_data[string_len], descriptor_type_, length);
    JpegMarker *marker = new JpegMarker(kObscuraMarker, data,
					string_len + length);
    return marker;
  }
  // Return a vector of markers containing the redaction data.
  std::vector<JpegMarker *> *MakeRedactionMarkers() {
    return NULL;
  }
  std::vector<unsigned char> descriptor_;
  const int kObscuraMarker = appn + 7;
  const char *descriptor_type_ = "ObscuraMetaData";
  const char *redaction_data_type_ = "ObscuraRedaction";
};
}  // namespace redaction

#endif
