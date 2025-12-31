#include "test_pattern.h"

#include <Arduino.h>
#include <unity.h>

#include "pattern.h"

void test_pattern_needle_index_to_right() {
  Pattern pattern;
  pattern.set_needle_range(50, 100);

  // When going TO_RIGHT, needle_index = start_offset + needle_in_pattern
  TEST_ASSERT_EQUAL(50, pattern.needle_index(0, TO_RIGHT));
  TEST_ASSERT_EQUAL(51, pattern.needle_index(1, TO_RIGHT));
  TEST_ASSERT_EQUAL(75, pattern.needle_index(25, TO_RIGHT));
  TEST_ASSERT_EQUAL(100, pattern.needle_index(50, TO_RIGHT));
}

void test_pattern_needle_index_to_left() {
  Pattern pattern;
  pattern.set_needle_range(50, 100);

  // When going TO_LEFT, needle_index = end_offset - needle_in_pattern
  TEST_ASSERT_EQUAL(100, pattern.needle_index(0, TO_LEFT));
  TEST_ASSERT_EQUAL(99, pattern.needle_index(1, TO_LEFT));
  TEST_ASSERT_EQUAL(75, pattern.needle_index(25, TO_LEFT));
  TEST_ASSERT_EQUAL(50, pattern.needle_index(50, TO_LEFT));
}

void test_pattern_read_bit_little_endian() {
  Pattern pattern;

  // Create a test buffer: 0b10101010 = 0xAA (bits: 0,1,0,1,0,1,0,1)
  uint8_t buffer[] = {0xAA, 0x55, 0xFF, 0x00};
  pattern.set_buffer(buffer);

  // First byte (0xAA = 0b10101010): bit 0=0, bit 1=1, bit 2=0, bit 3=1, etc.
  TEST_ASSERT_FALSE(pattern.read_bit_little_endian(0));  // bit 0 of byte 0
  TEST_ASSERT_TRUE(pattern.read_bit_little_endian(1));   // bit 1 of byte 0
  TEST_ASSERT_FALSE(pattern.read_bit_little_endian(2));  // bit 2 of byte 0
  TEST_ASSERT_TRUE(pattern.read_bit_little_endian(3));   // bit 3 of byte 0

  // Second byte (0x55 = 0b01010101): bit 0=1, bit 1=0, bit 2=1, bit 3=0, etc.
  TEST_ASSERT_TRUE(pattern.read_bit_little_endian(8));    // bit 0 of byte 1
  TEST_ASSERT_FALSE(pattern.read_bit_little_endian(9));   // bit 1 of byte 1
  TEST_ASSERT_TRUE(pattern.read_bit_little_endian(10));   // bit 2 of byte 1
  TEST_ASSERT_FALSE(pattern.read_bit_little_endian(11));  // bit 3 of byte 1

  // Third byte (0xFF = 0b11111111): all bits are 1
  TEST_ASSERT_TRUE(pattern.read_bit_little_endian(16));  // bit 0 of byte 2
  TEST_ASSERT_TRUE(pattern.read_bit_little_endian(23));  // bit 7 of byte 2

  // Fourth byte (0x00 = 0b00000000): all bits are 0
  TEST_ASSERT_FALSE(pattern.read_bit_little_endian(24));  // bit 0 of byte 3
  TEST_ASSERT_FALSE(pattern.read_bit_little_endian(31));  // bit 7 of byte 3
}

void test_pattern_get_needle_state_to_right() {
  Pattern pattern;
  pattern.set_needle_range(0, 15);  // 16 needles (0-15)

  // Create buffer with pattern: first byte = 0b11110000 (0xF0)
  // Bits 0-3 are 0, bits 4-7 are 1
  uint8_t buffer[] = {0xF0, 0x00};
  pattern.set_buffer(buffer);

  // Reading TO_RIGHT: needle_in_pattern 0 -> offset 0
  TEST_ASSERT_FALSE(pattern.get_needle_state(0, TO_RIGHT));  // bit 0
  TEST_ASSERT_FALSE(pattern.get_needle_state(1, TO_RIGHT));  // bit 1
  TEST_ASSERT_FALSE(pattern.get_needle_state(2, TO_RIGHT));  // bit 2
  TEST_ASSERT_FALSE(pattern.get_needle_state(3, TO_RIGHT));  // bit 3
  TEST_ASSERT_TRUE(pattern.get_needle_state(4, TO_RIGHT));   // bit 4
  TEST_ASSERT_TRUE(pattern.get_needle_state(5, TO_RIGHT));   // bit 5
  TEST_ASSERT_TRUE(pattern.get_needle_state(6, TO_RIGHT));   // bit 6
  TEST_ASSERT_TRUE(pattern.get_needle_state(7, TO_RIGHT));   // bit 7
}

void test_pattern_get_needle_state_to_left() {
  Pattern pattern;
  pattern.set_needle_range(0, 15);  // 16 needles (0-15)

  // Create buffer with pattern
  // 0xF0 = 0b11110000 means bits 0-3=0, bits 4-7=1
  // 0x0F = 0b00001111 means bits 0-3=1, bits 4-7=0
  uint8_t buffer[] = {0xF0, 0x0F};
  pattern.set_buffer(buffer);

  // Reading TO_LEFT: needle_in_pattern 0 -> offset 15
  // offset 15 = byte 1, bit 7: in 0x0F, bit 7 = 0
  TEST_ASSERT_FALSE(pattern.get_needle_state(0, TO_LEFT));

  // needle_in_pattern 1 -> offset 14 = byte 1, bit 6 = 0
  TEST_ASSERT_FALSE(pattern.get_needle_state(1, TO_LEFT));

  // needle_in_pattern 4 -> offset 11 = byte 1, bit 3 = 1
  TEST_ASSERT_TRUE(pattern.get_needle_state(4, TO_LEFT));

  // needle_in_pattern 8 -> offset 7 = byte 0, bit 7 = 1 (in 0xF0)
  TEST_ASSERT_TRUE(pattern.get_needle_state(8, TO_LEFT));

  // needle_in_pattern 12 -> offset 3 = byte 0, bit 3 = 0
  TEST_ASSERT_FALSE(pattern.get_needle_state(12, TO_LEFT));
}

void test_pattern_integration_with_real_data() {
  Pattern pattern;
  pattern.set_needle_range(0, 32);  // 33 needles, using simpler range

  // Use the same pattern from test_integration.cpp
  // Expected pattern when reading DOB outputs:
  // {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
  //  0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  //
  // Working backwards from the expected pattern:
  // Bits 0-8 = 0, bits 9-14 = 1, bits 15-17 = 0, bits 18-23 = 1, bits 24-32 = 0
  // Byte 0: bits 0-7 = 00000000 = 0x00
  // Byte 1: bits 8-15 = 01111110 = 0x7E
  // Byte 2: bits 16-23 = 11111100 = 0xFC
  // Byte 3: bits 24-31 = 00000000 = 0x00
  // Byte 4: bit 32 = 0
  uint8_t buffer[] = {0x00, 0x7E, 0xFC, 0x00, 0x00};
  pattern.set_buffer(buffer);

  // Test going TO_RIGHT
  bool expected_pattern[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
                             0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  for (int i = 0; i < 33; i++) {
    bool state = pattern.get_needle_state(i, TO_RIGHT);
    TEST_ASSERT_EQUAL_MESSAGE(expected_pattern[i], state,
                              "Mismatch in TO_RIGHT pattern");
  }
}

void test_pattern_edge_cases() {
  Pattern pattern;

  // Test with single needle
  pattern.set_needle_range(100, 100);
  uint8_t buffer[] = {0xFF};  // All bits set
  pattern.set_buffer(buffer);

  // Needle 0 should map to offset 100
  // offset 100 = byte 12, bit 4 (100 >> 3 = 12, 100 & 0x07 = 4)
  // But our buffer is only 1 byte, so this tests boundary behavior

  // Test with maximum range
  pattern.set_needle_range(0, 199);
  TEST_ASSERT_EQUAL(0, pattern.get_start_offset());
  TEST_ASSERT_EQUAL(199, pattern.get_end_offset());

  // Test NULL buffer doesn't crash (undefined behavior, but shouldn't crash)
  pattern.set_buffer(nullptr);
  TEST_ASSERT_NULL(pattern.get_buffer());
}

void run_module_pattern_tests() {
  RUN_TEST(test_pattern_needle_index_to_right);
  RUN_TEST(test_pattern_needle_index_to_left);
  RUN_TEST(test_pattern_read_bit_little_endian);
  RUN_TEST(test_pattern_get_needle_state_to_right);
  RUN_TEST(test_pattern_get_needle_state_to_left);
  RUN_TEST(test_pattern_integration_with_real_data);
  RUN_TEST(test_pattern_edge_cases);
}
