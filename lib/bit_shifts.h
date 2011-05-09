// Class to apply bit operations to a (JPEG) bit stream.
#ifndef INCLUDE_JPEG_REDACTION_LIBRARY_BIT_SHIFTS
#define INCLUDE_JPEG_REDACTION_LIBRARY_BIT_SHIFTS
class BitShifts {
public:
  // Shift the block after the  start-th bit up by shift,
  // increasing the size of data.
  static bool ShiftTail(std::vector<unsigned char> *data,
		   int *data_bits, 
		   int start, 
		   int shift) {
    if (shift < 0)
      throw("ShiftTail: shift is negative");
    // Enough space for everything.
    *data_bits += shift;
    data->resize((*data_bits + 7) /8);
    
    // Shift up the end bits (check the ending?)
    // This is Extremely approximate
    //    (*data)[data->size()-1] = 0;
    int bit_shift = shift % 8;
    const unsigned char low_mask = (1 << bit_shift) -1;
    const unsigned char high_mask = 0xff - low_mask;
    // The byte where the very last bit has to come from.
    int src_byte = (*data_bits - shift - 1) / 8;
    // Go through all the destination bytes i and pull their bits
    // from the two source bytes.
    for (int i = (*data_bits - 1) / 8; // The last byte
	 i >= (start + shift)/8; --i, src_byte--) {
      unsigned char byte;
      byte  = ((*data)[src_byte] & high_mask) >> bit_shift;
      byte |= ((*data)[src_byte - 1] & low_mask) << bit_shift;
      (*data)[i] = byte;
    }
    return true;
  }
  // Take the first insert_length bits from insertion
  // copy them to data at position start, overwriting old bits.
  static bool Overwrite(std::vector<unsigned char> *data,
		      int length, 
		      int start,
		      const std::vector<unsigned char> &overwrite,
		      int overwrite_length) {
    if (overwrite_length + start > length)
      throw("Length mismatch in Overwrite");
    if (length > 8 * data->size())
      throw("Data length error in Overwrite");
    if (overwrite_length > 8 * overwrite.size())
      throw("Overwrite length error in Overwrite");
    int start_byte = start / 8;
    // Byte with the last bit in it.
    int end_byte = (start + overwrite_length -1) / 8;
    // How much the overwrite has to be shifted up to match the start.
    int bit_shift = start / 8;
    // The high bit_shift bits.
    const unsigned char high_mask = ((1 << bit_shift) -1)<< (8 - bit_shift);
    int ov = 0;
    for (int i = start_byte; i <= end_byte; ++i, ++ov) {
      unsigned char byte;
      if (i == start_byte)
	byte = (*data)[i] & high_mask;
      else
	byte = (overwrite[ov-1] << (8 - bit_shift)) & high_mask;
      byte += (overwrite[ov] >> bit_shift);
      (*data)[i] = byte;
    }
    return true;
  }
  // Take the first insert_length bits from insertion
  // copy them to data at position start, shifting up old bits.
  static bool Insert(std::vector<unsigned char> *data,
		     int *length, 
		     int start,
		     const std::vector<unsigned char> &insertion,
		     int insert_length) {
    ShiftTail(data, length, start, insert_length);
    return Overwrite(data, *length, start, insertion, insert_length);
  }
}; 

#endif  // INCLUDE_JPEG_REDACTION_LIBRARY_BIT_SHIFTS
