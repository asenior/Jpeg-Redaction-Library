// Simple binary to apply redaction to a JPEG image
// Or reverse that redaction.

#include <stdlib.h>
#include <stdio.h>

#include <string>
#include "jpeg.h"
#include "redaction.h"
using std::string;
namespace jpeg_redaction {
    int reverse_redaction(const char * const filename) {
      try {
	Jpeg j2;
	bool success = j2.LoadFromFile(filename, true);
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
  int redact(const string &filename,
	     const string &output,
	     const string &region) {
      try {
	Jpeg j2;
	bool success = j2.LoadFromFile(filename.c_str(), true);
	Redaction redaction;

	redaction.AddRegion(region);
	j2.DecodeImage(&redaction, NULL);
	if (!redaction.ValidateStrips())
	  throw("Strips not valid");
	if (j2.Save(output.c_str()) != 0) {
	  fprintf(stderr, "Couldn't save %s\n", output.c_str());
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
  std::string filename;
  std::string outfile;
  std::string region;
  if (argc < 3) {
    fprintf(stderr, "%s <infile> <outfile> <l,r,t,b>\n", argv[0]);
    exit(1);
  }
  filename = argv[1];
  outfile = argv[2];
  region = argv[3];
  jpeg_redaction::redact(filename, outfile, region);
}
