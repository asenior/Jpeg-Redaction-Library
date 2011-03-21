#include <stdlib.h>
#include <stdio.h>

#include <string>
#include "jpeg.h"
#include "redaction.h"

namespace jpeg_redaction {
int test_exif_removal(const char * const filename) {
  printf("Testing with loadall=true\n");
  try {
    Jpeg j2(filename, true);
    j2.RemoveExif();
    j2.Save("testout/testoutput.jpg");
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
  // printf("Testing with loadall=false\n");
  // Jpeg j(filename.c_str(), false);
  jpeg_redaction::test_exif_removal(filename.c_str());
}
