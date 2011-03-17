#include <stdlib.h>
#include <stdio.h>

#include <string>
#include "jpeg.h"

int main(int argc, char **argv) {
  std::string filename("testdata/windows.jpg");
  if (argc > 1)
    filename = argv[1];
  // printf("Testing with loadall=false\n");
  // Jpeg j(filename.c_str(), false);
  printf("Testing with loadall=true\n");
  try {
    jpeg_redaction::Jpeg j2(filename.c_str(), true);
    j2.Save("testout/testoutput.jpg");
  } catch (const char *error) {
    fprintf(stderr, "Error: <%s> at outer level\n", error);
    exit(1);
  }
  // j2.Corrupt(0,0,100);
  // j2.Save("testcorrupted.jpg");
}
