#include <stdlib.h>
#include <stdio.h>

#include <string>
#include "jpeg.h"
#include "redaction.h"

namespace jpeg_redaction {
int test_exif_removal(const char * const filename) {
  try {
    Jpeg j2;
    bool success = j2.LoadFromFile(filename, true);
    if (!success) exit(1);
    j2.RemoveTag(TiffTag::tag_exif);
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
    int rem = j2.RemoveTag(TiffTag::tag_exif);
    if (rem != 1) throw("Didn't remove one exif tag");
    rem = j2.RemoveTag(TiffTag::tag_gps);
    if (rem != 1) throw("Didn't remove one gps tag");
    j2.Save("testout/test_noexifgps.jpg");
  } catch (const char *error) {
    fprintf(stderr, "Error: <%s> at outer level\n", error);
    exit(1);
  }
  return 0;
}
} // namespace jpeg_redaction

int main(int argc, char **argv) {
  std::string filename("testdata/windows.jpg");
  if (argc > 1)
    filename = argv[1];
  jpeg_redaction::test_exif_removal(filename.c_str());
  jpeg_redaction::test_gps_removal(filename.c_str());
  jpeg_redaction::test_sensitive_removal(filename.c_str());
}
