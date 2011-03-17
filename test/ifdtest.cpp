// Simple test of reading an IFD at a given location.
#include <stdlib.h>
#include <stdio.h>

#include <string>
#include "jpeg.h"

int main(int argc, char **argv) {
  std::string filename("testdata/windows.jpg");
  unsigned int ifd_loc = 20;
  unsigned int tiff_offset = 12;
  bool byte_swapping = false;
  if (argc > 1)
    filename = argv[1];
  if (argc > 2)
    ifd_loc = atoi(argv[2]);
  //  ifd_loc -= tiff_offset;
  printf("Dumping IFD in %s at position %u byteswapping %s\n",
         filename.c_str(), ifd_loc, (byte_swapping ? "on" : "off"));
  try {
    FILE *pFile = fopen(filename.c_str(), "rb");
    if (pFile == NULL) {
      fprintf(stderr, "Can't open file %s\n", filename.c_str());
      exit(1);
    }
    bool load_all = true;
    jpeg_redaction::TiffIfd ifd(pFile, ifd_loc, load_all,
                                tiff_offset, byte_swapping);
  } catch (const char *error) {
    fprintf(stderr, "Error: <%s> at outer level\n", error);
    exit(1);
  }
}
