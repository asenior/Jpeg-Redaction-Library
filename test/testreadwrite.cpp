#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <vector>
#include "jpeg.h"
#include "redaction.h"
#include "test_utils.h"

int main(int argc, char **argv) {
  std::string filename("testdata/windows.jpg");
  if (argc > 1)
    filename = argv[1];
  jpeg_redaction::tests::test_readwrite(filename.c_str());
}
