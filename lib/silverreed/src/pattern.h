/**
 * @file pattern.h
 * @brief Pattern class for handling the pattern buffer and conversions to
 * needle.
 */
#ifndef PATTERN_H_
#define PATTERN_H_

#include "Arduino.h"
#include "machine/carriage.h"

class Pattern {
 private:
  int start_offset;
  int end_offset;
  uint8_t* buffer;

 public:
  Pattern();
  bool get_needle_state(int needle_in_pattern, CarriageDirection direction);
  bool read_bit_little_endian(int offset);
  int needle_index(int needle_in_pattern, CarriageDirection direction);
  void set_needle_range(uint8_t start_needle, uint8_t end_needle);
  void set_buffer(uint8_t* buffer);
};

#endif
