#ifndef INCLUDE_REDACTION
#define INCLUDE_REDACTION
// Class to store information redacted from a horizontal strip of image.
class JpegStrip {
public:
  JpegStrip(int x, int y) : x_(x), y_(y) {
    blocks_ = 0;
    bits_ = 0;
  }
  AppendBlock(const unsigned char* data, int bits) {
    data_.resize((bits_ + bits + 7)/8);
    // copy over bits.

    bits_ += bits;
    ++blocks_;
  }
  string data_; // Raw binary encoded data.
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
  void AddRegion(Rect r) {
    regions_.push_back(r);
  }
  Rect GetRegion(int i) {
    return regions_[i];
  }
  // Information redacted.
  std::vector<JpegStrip> strips_;
  std::vector<Rect> regions_;
};
#endif INCLUDE_REDACTION
