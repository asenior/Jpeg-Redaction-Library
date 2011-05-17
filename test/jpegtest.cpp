#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <vector>
#include "jpeg.h"
#include "redaction.h"

namespace jpeg_redaction {
  namespace tests {
    // return true if files are byte identical.
    bool compare_to_golden(const char * const filename,
			   const char * const golden_name) {
      FILE *output = fopen(filename, "rb");
      if (output == NULL) throw("Can't open output file");
      FILE *golden = fopen(golden_name, "rb");
      if (golden == NULL) throw("Can't open golden file");
      int count = 0;
      bool success = false;

      while (1) {
	unsigned char o_byte, g_byte;
	fread(&o_byte, sizeof(char), 1, output);
	fread(&g_byte, sizeof(char), 1, golden);
	if (feof(output)) {
	  if (feof(golden))
	    success = true;
	  else
	    fprintf(stderr, "output %s shorter (%d bytes) than golden %s\n",
		    filename, count, golden_name);
	  break;
	} else {
	  if (feof(golden)) {
	    fprintf(stderr, "golden %s shorter (%d bytes) than output %s\n",
		    golden_name, count, filename);
	    break;
	  }
	}
	if (o_byte != g_byte) {
	  fprintf(stderr, "golden %s differs from output %s at byte %d\n",
		    golden_name, filename, count);
	    break;
	}
	++count;
      }
      return success;
    }
    bool compare_bytes(const std::vector<unsigned char> &orig,
		       const std::vector<unsigned char> &other) {
      if (orig.size() > other.size() ||
	  orig.size() < other.size() - 1) {
	fprintf(stderr, "Size mismatch: %d vs %d\n", orig.size(), other.size());
	return false;
      }
      for(int i = 0; i < orig.size(); ++i) {
	if (orig[i] != other[i]) {
	  fprintf(stderr, "Byte mismatch at %d of %d\n", i, orig.size());
	  return false;
	}
      }
      return true;
    }
    void SaveBytes(const std::vector<unsigned char> &bytes,
		   const char *const fn) {
      FILE *file = fopen(fn, "wb");
      if (file == NULL) {
	fprintf(stderr, "Can't write to file %s\n", fn);
	return;
      }
      int rv = fwrite(&(bytes[0]), sizeof(unsigned char), bytes.size(), file);
      if (rv != bytes.size()) throw("can't save bytes");
      fclose(file);
    }

    int test_loadallfalse(const char * const filename) {
      printf("Testing with loadall=false\n");
      try {
	Jpeg j2(filename, false);
      } catch (const char *error) {
	fprintf(stderr, "Error: <%s> at outer level\n", error);
	return 1;
      }
      return 0;
    }

    int test_readwrite(const char * const filename) {
      printf("Testing with loadall=true\n");
      try {
	Jpeg j2(filename, true);

	j2.DecodeImage(NULL, "testout/rawgrey.pgm");
	std::string output_filename = "testout/testplainoutput.jpg";
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

    int test_redaction(const char * const filename) {
      printf("Testing redaction\n");
      try {
	Jpeg j2(filename, true);
	Redaction::Rect rect(50, 300, 50, 200);
	Redaction redaction;

	redaction.AddRegion(rect);
	j2.DecodeImage(&redaction, NULL);
	if (!redaction.ValidateStrips())
	  throw("Strips not valid");
	std::string output_filename = "testout/testoutput.jpg";
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

    int test_reversingredaction(const char * const filename,
				const Redaction::Rect &rect) {
      try {
	Jpeg j2(filename, true);
	JpegMarker *sos_block = j2.GetMarker(Jpeg::jpeg_sos);
	std::vector<unsigned char> original(sos_block->data_);
	SaveBytes(sos_block->data_, "testout/originalsos");
	//	Redaction::Rect rect(50, 300, 50, 200);  // l,r, t, b
	// One strip
	Redaction redaction;
	redaction.AddRegion(rect);
	j2.DecodeImage(&redaction, NULL);
	if (!redaction.ValidateStrips())
	  throw("Strips not valid");
	// sos_block = j2.GetMarker(Jpeg::jpeg_sos);
	//	SaveBytes(sos_block->data_, "testout/redactedsos");
	const JpegStrip *strip = redaction.GetStrip(0);
	j2.ReverseRedaction(redaction);
	sos_block = j2.GetMarker(Jpeg::jpeg_sos);
	if (! compare_bytes(original, sos_block->data_))
	  throw("Compare test fail");
	SaveBytes(sos_block->data_, "testout/unredactedsos");
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
    int test_reversingredactions_multi(const std::string &filename) {
      Redaction::Rect rectonestrip(50, 300, 64, 79);  // l,r, t, b
      int rv = test_reversingredaction(filename.c_str(), rectonestrip);
      if (rv) return 1;
      Redaction::Rect rect(50, 500, 89, 245);  // l,r, t, b
      rv = test_reversingredaction(filename.c_str(), rect);
      if (rv) return 1;
      return 0;
    }

  } // namespace tests
} // namespace jpeg_redaction

int main(int argc, char **argv) {
  std::string filename("testdata/windows.jpg");
  if (argc > 1)
    filename = argv[1];
  jpeg_redaction::tests::test_loadallfalse(filename.c_str());
  jpeg_redaction::tests::test_readwrite(filename.c_str());
  jpeg_redaction::tests::test_redaction(filename.c_str());
  jpeg_redaction::tests::test_reversingredactions_multi(filename);
}
