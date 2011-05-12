// Test for bit shift class.
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "../lib/bit_shifts.h"

using std::vector;
typedef vector<unsigned char> bitstream;


// Return the nth bit from a stream, assuming the 0th bit is the
// highest order bit of the first byte.
int BitFromStream(const bitstream &stream, int bit) {
  if (bit > stream.size() * 8)
    throw("Index outside stream");
  return (stream[bit / 8] >> (7 - (bit % 8))) & 1;
}

// Check that a range of bits of a given length in old_bits (length old_length)
// matches a range of bits in new_bits.
bool VerifyRange(const bitstream &old_bits, int old_start, int old_length,
		 const bitstream &new_bits, int new_start, int new_length,
		 int length) {
  if (old_start + length > old_length) throw("Old length error");
  if (new_start + length > new_length) throw("New length error");
  for (int i = 0; i < length; ++i) {
    if (BitFromStream(old_bits, old_start + i) != 
	BitFromStream(new_bits, new_start + i)) {
      fprintf(stderr, "Can't verify range %d (%d): %d (%d) at "
	      "bit %d, (byte %d) of %d.\n",
	      old_start, old_length,
	      new_start, new_length,
	      i, i/8, length);
      throw("Failed to verify range");
    }
  }
  return true;
}
void FillRand(bitstream *stream) {
  for (int i= 0; i < stream->size(); ++i) {
    (*stream)[i] = rand() % 256;
  }
}

void FillStream(bitstream *stream, unsigned char b) {
  for (int i= 0; i < stream->size(); ++i) {
    (*stream)[i] = b;
  }
}

bool TestShift(int length, int start, int shift) {
  printf("Testing Shift %d %d %d\n", length, start, shift);
  int bytes = (length + 7)/8;
  bitstream original(bytes);
  FillRand(&original);
  bitstream test(bytes);
  test.assign(original.begin(), original.end());
  int new_length = length;
  bool rv = BitShifts::ShiftTail(&test, &new_length, start, shift);
  if (new_length != length + shift) {
    fprintf(stderr, "new_length %d length %d shift %d\n", 
	    new_length, length, shift);
    throw("TestShift length mismatch");
  }
  // Check the start still matches.
  VerifyRange(original, 0, length,
	      test, 0, new_length,
	      start);
  // Check the end matches.
  VerifyRange(original, start, length,
	      test, start + shift, new_length,
	      length-start);
  return true;
}


// Verify that the Verification works.
bool TestNoShift(int length) {
  printf("Testing NoShift\n");
  int bytes = (length + 7)/8;
  bitstream original(bytes);
  FillRand(&original);
  bitstream test(bytes);
  test.assign(original.begin(), original.end());

  // Check the start still matches.
  VerifyRange(test, 0, length,
	      original, 0, length,
	      0);
  VerifyRange(test, 0, length,
	      original, 0, length,
	      length/2);
  VerifyRange(test, length/2, length,
	      original, length/2, length,
	      length/2);
  return true;
}

bool TestInsert(int length, int start, int shift) {
  printf("Testing insert %d %d %d\n", length, start, shift);
  int bytes = (length + 7)/8;
  int insert_bytes = (shift + 7)/8;
  bitstream original(bytes);
  bitstream insertion(insert_bytes);
  FillRand(&original);
  FillRand(&insertion);
  bitstream test(bytes);
  test.assign(original.begin(), original.end());
  int new_length = length;
  bool rv = BitShifts::Insert(&test, &new_length, start, insertion, shift);
  if (new_length != length + shift) {
    fprintf(stderr, "new_length %d length %d shift %d\n", 
	    new_length, length, shift);
    throw("TestSplice length mismatch");
  }
  // Check the start still matches.
  printf("Check start\n");
  VerifyRange(original, 0, length,
	      test, 0, new_length,
	      start);
  // Check the end matches.
  printf("Check end\n");
  VerifyRange(original, start, length,
	      test, start + shift, new_length,
	      length - start);
  // Check the insertion matches.
  printf("Check insertion\n");
  VerifyRange(insertion, 0, shift,
	      test, start, new_length,
	      shift);
  return true;
}

bool TestOverwrite(int length, int start, int insert_bits) {
  printf("Testing overwrite startlen %d start %d insert_bits %d\n",
	 length, start, insert_bits);
  if (start + insert_bits > length)
    throw("Bad specification of TestSplice");
  int bytes = (length + 7)/8;
  int insert_bytes = (insert_bits + 7)/8;
  bitstream original(bytes);
  bitstream insertion(insert_bytes);
  FillRand(&original);
  FillRand(&insertion);
  bitstream test(bytes);
  test.assign(original.begin(), original.end());

  const int new_length = length;
  bool rv = BitShifts::Overwrite(&test, length, start, insertion, insert_bits);

  // Check the start still matches.
  printf("Check start\n");
  VerifyRange(original, 0, length,
	      test, 0, new_length,
	      start);
  // Check the end matches.
  printf("Check end\n");
  VerifyRange(original, start + insert_bits, length,
	      test, start + insert_bits, length,
	      length - start - insert_bits);
  // Check the insertion matches.
  printf("Check overwrite\n");
  VerifyRange(insertion, 0, insert_bits,
	      test, start, length,
	      insert_bits);
  return true;
}

bool TestBitFromStream(int len, unsigned char b) {
  printf("TestBitFromStream len %d byte %x\n", len, b);
  bitstream ones(len);
  FillStream(&ones, b);
  for (int i = 0; i < len; ++i) {
    if (BitFromStream(ones, i) != (1 & (b >> (7-(i % 8)))) ) {
      fprintf(stderr, "Bad bit %d from stream. Tested with len %d byte 0x%x.",
	      i, len, b);
      throw("Fail in TestBitFromStream");
    }
  }
  return true;
}
  
int main(int argc, char **argv) {
  try {
    TestBitFromStream(21, 0xff);
    TestBitFromStream(53, 0x00);
    TestBitFromStream(19, 0xca);

    TestNoShift(100);
    // Whole byte shifts.
    TestShift(57, 0, 8);
    TestShift(195, 32, 16);
    
    // Shift by fractions of a byte.
    TestShift(100, 50, 25);
    //    TestShift(299, 80, 5);
    //  TestShift(299, 3, 5);

    TestOverwrite(227, 16, 16);
    TestOverwrite(227, 13, 16);
    TestOverwrite(227, 16, 19);
    TestOverwrite(227, 13, 25);
    TestOverwrite(7, 3, 3);

    TestInsert(27, 8, 199);
  } catch (const char *message) {
    fprintf(stderr, "Caught message: %s in bit_shifts_test\n", message);
    exit(1);
  }
  return 0;
}
