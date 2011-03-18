// tiff_ifd.h: interface for the TiffIfd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIFFIIFD_H__669C3A6A_A035_418B_A615_B78E8E1467E3__INCLUDED_)
#define AFX_TIFFIIFD_H__669C3A6A_A035_418B_A615_B78E8E1467E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>

#include "tiff_tag.h"

namespace jpeg_redaction {
class TiffTag;
class Jpeg;
class ExifIfd;

class TiffIfd
{
public:
  TiffIfd(FILE *pFile, unsigned int ifdoffset, bool loadall = false,
	    unsigned int subfileoffset=0, bool byte_swapping = false);
  TiffIfd() {  }
  virtual ~TiffIfd() {
    Reset();
  }

  int AddTag(TiffTag *tag, bool allowmultiple);
  int LoadImageData(FILE *pFile, bool loadall);
  unsigned int Write(FILE *pFile,
		     unsigned int nextifdoffset,
		     int subfileoffset) const;
  ExifIfd *GetExif() { return (ExifIfd*)FindTag(TiffTag::tag_exif); }

    // Find a tag with a particular number. Return -1 if not in this IFD.
  TiffTag * FindTag(const int tagno) const {
    for(int tagindex=0 ; tagindex < tags_.size(); ++tagindex) {
      if (tags_[tagindex]->GetTag() == tagno)
        return(tags_[tagindex]);
    }
    return NULL;
  }

  // For anything referenced by a pointer, then seek to it and load it into data_blocks_
  int LoadAll(FILE *pFile);
  int GetNextIfdOffset() const {return nextifdoffset_;}

  TiffTag *  GetTag(int tagindex) const {
    if (tagindex < 0 || tagindex >= tags_.size())
      return NULL;
    return(tags_[tagindex]);
  }
  int GetNTags() const { return tags_.size(); }
  void  ListTags() const;
protected:

  void Reset();

  bool byte_swapping_;
  unsigned int nextifdoffset_;
  std::vector<TiffTag *> tags_;
  unsigned char *data_;
  unsigned int subfileoffset_;
  Jpeg *jpeg_;
};

// Subclass if it is an exif block
class ExifIfd : public TiffIfd {
 public:
 ExifIfd(FILE *pFile, unsigned int ifdoffset, bool loadall = false, unsigned int subfileoffset=0):
  TiffIfd(pFile, ifdoffset, loadall, subfileoffset) {
  }
};
}// namespace jpeg_redaction
#endif // !defined(AFX_TIFFIIFD_H__669C3A6A_A035_418B_A615_B78E8E1467E3__INCLUDED_)
