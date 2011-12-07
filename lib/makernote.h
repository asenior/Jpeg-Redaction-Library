// Copyright (C) 2011 Andrew W. Senior andrew.senior[AT]gmail.com

// makernote.h
// classes for reading, writing & accessing makernote sections of
// EXIF files. Currently only some of these are partially implemented.

// Makernotes for various manufacturers are described in 
// http://www.ozhiker.com/electronics/pjmt/jpeg_info/makernotes.html

#ifndef JPEG_REDACTION_LIB_MAKERNOTE
#define  JPEG_REDACTION_LIB_MAKERNOTE

#include <stdio.h>
#include <string>
#include "tiff_ifd.h"

namespace jpeg_redaction {

class MakerNote {
public:
  MakerNote() {}
  virtual int Read(FILE *pFile, int subfileoffset, int length) = 0;
  virtual int Write(FILE *pFile, int subfileoffset) const = 0;
};

  // A Generic Makernote where we just read a block of data.
  // without parsing, hoping that it is relocatable.
  class GenericMakerNote : public MakerNote {
  public:
    GenericMakerNote() {}
    virtual int Read(FILE *pFile, int subfileoffset, int length) {};
    virtual int Write(FILE *pFile, int subfileoffset) const {};
  };

  // A makernote stored in a standard TiffIfd, e.g. Canon.
  class IfdMakerNote : public MakerNote {
  public:
    IfdMakerNote() {}
    virtual int Read(FILE *pFile, int subfileoffset, int length) {};
    virtual int Write(FILE *pFile, int subfileoffset) const {};
  protected:
    TiffIfd *ifd_;
  };

  class Panasonic: public MakerNote {
  public:
    Panasonic() {}
    virtual int Read(FILE *pFile, int subfileoffset, int length) {
      int start = ftell(pFile);
      char header[12];
      int iRV = fread(header, sizeof(char), 12, pFile);
      if (strcmp(header, "Panasonic") != 0) {
	fseek(pFile, start, SEEK_SET);
	return 0;
      }
      printf("Loading Panasonic\n");
      ifd_ = new TiffIfd(pFile, start+12, true, subfileoffset);
      TiffTag *tag;
      tag = ifd_->FindTag(0x69);
      if (tag) printf("Panasonic69: %s\n", (const char *)tag->GetData());
      tag = ifd_->FindTag(0x6b);
      if (tag) printf("Panasonic6b: %s\n", (const char *)tag->GetData());
      tag = ifd_->FindTag(0x6d);
      if (tag) printf("Panasonic6d: %s\n", (const char *)tag->GetData());
      tag = ifd_->FindTag(0x6f);
      if (tag) printf("Panasonic6f: %s\n", (const char *)tag->GetData());
      delete ifd_;

      fseek(pFile, start, SEEK_SET);
      return 1;
    }
    virtual int Write(FILE *pFile, int subfileoffset) const {
      return 1;
    }
  protected:
    const char *header;
    TiffIfd *ifd_;
  };

  class MakerNoteFactory {
  public:
    MakerNoteFactory() {}
    void SetManufacturer(const char *const manuf) { manufacturer_ = manuf;}
    MakerNote *Read(FILE *pFile, int subfileoffset, int length) {
      if (manufacturer_.compare("Canon") == 0) {
	IfdMakerNote *canon = new IfdMakerNote;
	canon->Read(pFile, subfileoffset, length);
	return canon;
      } else if (manufacturer_.compare("Panasonic") == 0) {
	Panasonic *panasonic = new Panasonic;
	panasonic->Read(pFile, subfileoffset, length);
	return panasonic;
      } else {
	GenericMakerNote *generic = new GenericMakerNote;
	generic->Read(pFile, subfileoffset, length);
	return generic;
      }      
    }
  protected:
    std::string manufacturer_;
  };

}  // namespace jpeg_redaction
#endif // JPEG_REDACTION_LIB_MAKERNOTE
