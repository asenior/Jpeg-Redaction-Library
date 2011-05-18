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
				const Redaction::Rect &rect);
    int test_reversingredactions_multi(const std::string &filename);
  } // namespace tests
} // namespace jpeg_redaction
#endif  // INCLUDE_JPEG_REDACTION_TEST_TEST_UTILS
