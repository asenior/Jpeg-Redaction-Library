#include <stdlib.h>
#include <stdio.h>

#include <string>
#include "jpeg.h"
#include "redaction.h"

namespace jpeg_redaction {
int test_redaction(const char * const filename) {
  printf("Testing with loadall=true\n");
  try {
    Jpeg j2(filename, true);
    Redaction::Rect rect(50, 300, 50, 200);
    Redaction redaction;

    redaction.AddRegion(rect);
    j2.ParseImage(redaction, "testout/rawgrey.pgm");
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
  jpeg_redaction::test_redaction(filename.c_str());
}
