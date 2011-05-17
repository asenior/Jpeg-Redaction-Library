// Simple binary to apply redaction to a JPEG image
// Or reverse that redaction.

#include <stdlib.h>
#include <stdio.h>

#include <string>
#include "jpeg.h"
#include "redaction.h"

namespace jpeg_redaction {
    int reverse_redaction(const char * const filename) {
      try {
	Jpeg j2(filename, true);
	Redaction::Rect rect(50, 300, 50, 200);  // l,r, t, b
	Redaction redaction;

	redaction.AddRegion(rect);
	j2.DecodeImage(&redaction, NULL);
	if (!redaction.ValidateStrips())
	  throw("Strips not valid");
	j2.ReverseRedaction(redaction);
	std::string output_filename = "testout/testunredacted.jpg";
	if (j2.Save(output_filename.c_str()) != 0) {
	  fprintf(stderr, "Couldn't save %s\n", output_filename.c_str());
	  return 1;
	}
      } catch (const char *error) {
	fprintf(stderr, "Error: <%s> at outer level\n", error);
	return 1;
      }
      return 0;
    }
    int redact(const char * const filename) {
      try {
	Jpeg j2(filename, true);
	Redaction::Rect rect(50, 300, 50, 200);  // l,r, t, b
	Redaction redaction;

	redaction.AddRegion(rect);
	j2.DecodeImage(&redaction, NULL);
	if (!redaction.ValidateStrips())
	  throw("Strips not valid");
	j2.ReverseRedaction(redaction);
	std::string output_filename = "testout/testunredacted.jpg";
	if (j2.Save(output_filename.c_str()) != 0) {
	  fprintf(stderr, "Couldn't save %s\n", output_filename.c_str());
	  return 1;
	}
      } catch (const char *error) {
	fprintf(stderr, "Error: <%s> at outer level\n", error);
	return 1;
      }
      return 0;
    }
} // namespace jpeg_redaction

int main(int argc, char **argv) {
  std::string filename("testdata/windows.jpg");
  if (argc < 3) {
    fprintf(stderr, "%s <infile> <outfile>\n", argv[0]);
    exit(1);
  }
  filename = argv[1];
  outfile = argv[2];
  jpeg_redaction::tests::test_redaction(filename.c_str());
}
