#include "pattern.h"

#include "Arduino.h"
#include "config.h"
#include "debug.h"

Pattern::Pattern() {
  /**
   * Init the pattern with default values.
   * By default, the needle range is set to the full machine (0 to
   * DEFAULT_MAX_NEEDLES needles). The buffer pointer is initialized to nullptr
   * and must be set via set_buffer() before calling get_needle_state().
   */
  this->buffer = nullptr;
  this->start_offset = 0;
  this->end_offset = DEFAULT_MAX_NEEDLES;
}

void Pattern::set_needle_range(uint8_t start_needle, uint8_t end_needle) {
  /**
   * Set the range of needles for the pattern.
   *
   * @param start_needle The first needle of the pattern.
   * @param end_needle The last needle of the pattern.
   */
  this->start_offset = start_needle;
  this->end_offset = end_needle;
}

void Pattern::set_buffer(uint8_t* buffer) {
  /**
   * Set the buffer of the pattern.
   *
   * @param buffer The buffer of the pattern.
   */
  this->buffer = buffer;
}

bool Pattern::get_needle_state(int needle_in_pattern,
                               CarriageDirection direction) {
  /**
   * Get the state of a needle in the pattern.
   * The needle is given by its index in the pattern.
   *
   * @param needle_in_pattern The index of the needle in the pattern. (From 0 to
   * the len of the pattern)
   * @param direction The direction of the carriage to choose the correct side
   * of the pattern.
   * @return The state of the needle.
   */
  int needle_offset = needle_index(needle_in_pattern, direction);
  return read_bit_little_endian(needle_offset);
}

int Pattern::needle_index(int needle_in_pattern, CarriageDirection direction) {
  /**
   * Get the index of the needle in the buffer.
   * The needle is given by its index in the pattern.
   * The index is the offset in the buffer.
   * The index is calculated from the start of the buffer.
   *
   * @param needle_in_pattern The index of the needle in the pattern. (From 0 to
   * the len of the pattern)
   * @param direction The direction of the carriage to choose the correct side
   * of the pattern.
   * @return The index of the needle in the buffer.
   */
  if (direction == TO_LEFT) {
    return this->end_offset - needle_in_pattern;
  } else {
    return this->start_offset + needle_in_pattern;
  }
}

bool Pattern::read_bit_little_endian(int offset) {
  /**
   * Read a bit in the buffer.
   * The bit is given by its offset in the buffer.
   * The offset is the number of bits from the start of the buffer.
   *
   * @param offset The offset of the bit in the buffer.
   * @return The value of the bit.
   *
   * @warning This function does not validate that the buffer is set.
   * The caller must ensure set_buffer() was called before using this function.
   */
  uint8_t current_byte = offset >> 3;  // Divide by BITS_PER_BYTE
  bool pixelValue =
      bitRead(this->buffer[current_byte], offset & BIT_INDEX_MASK);
  return pixelValue;
}
