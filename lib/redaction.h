#ifndef INCLUDE_REDACTION
#define INCLUDE_REDACTION
#include "jpeg_decoder.h"

namespace jpeg_redaction {
// Class to store information redacted from a horizontal strip of image.
class JpegStrip {
public:
  JpegStrip(int x, int y) : x_(x), y_(y) {
    blocks_ = 0;
    bits_ = 0;
  }
  int AppendBlock(const unsigned char* data, int bits) {
    data_.resize((bits_ + bits + 7)/8);
    // copy over bits.

    bits_ += bits;
    return ++blocks_;
  }
  std::string data_; // Raw binary encoded data.
  int bits_;
  int x_; // Coordinate of start (from left)
  int y_; // Coordinate of start (from top)
  int blocks_; // Number of blocks stored.
};

// Class to define the areas to be redacted, and return the redacted
// information.
class Redaction {
public:
  Redaction() {};
  void AddRegion(JpegDecoder::Rect r) {
    regions_.push_back(r);
  }
  JpegDecoder::Rect GetRegion(int i) {
    return regions_[i];
  }
  // Information redacted.
  std::vector<JpegStrip> strips_;
  std::vector<JpegDecoder::Rect> regions_;
};
} // namespace jpeg_redaction
#endif // INCLUDE_REDACTION
