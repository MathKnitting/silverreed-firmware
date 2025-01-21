#include "knitting.h"
#include "pattern.h"
#include "unity.h"

void test_read_little_endian() {
  // Test the Pattern class.

  uint8_t buffer[] = {0x01};
  Pattern pattern = Pattern();
  pattern.set_needle_range(0, 7);
  pattern.set_buffer(buffer);
  TEST_ASSERT_EQUAL(true, pattern.read_bit_little_endian(0));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(1));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(2));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(3));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(4));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(5));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(6));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(7));

  buffer[0] = 0xFF;
  pattern = Pattern();
  pattern.set_needle_range(0, 7);
  pattern.set_buffer(buffer);
  TEST_ASSERT_EQUAL(true, pattern.read_bit_little_endian(0));
  TEST_ASSERT_EQUAL(true, pattern.read_bit_little_endian(1));
  TEST_ASSERT_EQUAL(true, pattern.read_bit_little_endian(2));
  TEST_ASSERT_EQUAL(true, pattern.read_bit_little_endian(3));
  TEST_ASSERT_EQUAL(true, pattern.read_bit_little_endian(4));
  TEST_ASSERT_EQUAL(true, pattern.read_bit_little_endian(5));
  TEST_ASSERT_EQUAL(true, pattern.read_bit_little_endian(6));
  TEST_ASSERT_EQUAL(true, pattern.read_bit_little_endian(7));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(8));

  uint8_t buffer2[] = {0x01, 0x00};
  pattern = Pattern();
  pattern.set_needle_range(0, 7);
  pattern.set_buffer(buffer2);
  TEST_ASSERT_EQUAL(true, pattern.read_bit_little_endian(0));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(1));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(2));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(3));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(4));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(5));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(6));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(7));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(8));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(9));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(10));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(11));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(12));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(13));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(14));
  TEST_ASSERT_EQUAL(false, pattern.read_bit_little_endian(15));
}

void test_needle_index() {
  uint8_t buffer[] = {0x01};
  Pattern pattern = Pattern();
  pattern.set_needle_range(0, 7);
  pattern.set_buffer(buffer);
  TEST_ASSERT_EQUAL(7, pattern.needle_index(0, TO_LEFT));
  TEST_ASSERT_EQUAL(0, pattern.needle_index(0, TO_RIGHT));
  TEST_ASSERT_EQUAL(6, pattern.needle_index(1, TO_LEFT));
  TEST_ASSERT_EQUAL(1, pattern.needle_index(1, TO_RIGHT));
  TEST_ASSERT_EQUAL(0, pattern.needle_index(7, TO_LEFT));
  TEST_ASSERT_EQUAL(7, pattern.needle_index(7, TO_RIGHT));
}

void complex_test() {
  uint8_t buffer[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0xe0, 0xc7, 0x0f, 0x00, 0x00, 0x00, 0x00,
                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  Pattern pattern = Pattern();
  pattern.set_needle_range(84, 116);
  pattern.set_buffer(buffer);
  TEST_ASSERT_EQUAL(84, pattern.needle_index(0, TO_RIGHT));
  TEST_ASSERT_EQUAL(false, pattern.get_needle_state(0, TO_RIGHT));
  TEST_ASSERT_EQUAL(false, pattern.get_needle_state(1, TO_RIGHT));
  TEST_ASSERT_EQUAL(false, pattern.get_needle_state(2, TO_RIGHT));
  TEST_ASSERT_EQUAL(false, pattern.get_needle_state(3, TO_RIGHT));
  TEST_ASSERT_EQUAL(false, pattern.get_needle_state(4, TO_RIGHT));
  TEST_ASSERT_EQUAL(false, pattern.get_needle_state(5, TO_RIGHT));
  TEST_ASSERT_EQUAL(false, pattern.get_needle_state(6, TO_RIGHT));
  TEST_ASSERT_EQUAL(false, pattern.get_needle_state(7, TO_RIGHT));
  TEST_ASSERT_EQUAL(false, pattern.get_needle_state(8, TO_RIGHT));
  TEST_ASSERT_EQUAL(true, pattern.get_needle_state(9, TO_RIGHT));

  TEST_ASSERT_EQUAL(false, pattern.get_needle_state(0, TO_LEFT));
  TEST_ASSERT_EQUAL(false, pattern.get_needle_state(1, TO_LEFT));
  TEST_ASSERT_EQUAL(false, pattern.get_needle_state(2, TO_LEFT));
  TEST_ASSERT_EQUAL(false, pattern.get_needle_state(3, TO_LEFT));
  TEST_ASSERT_EQUAL(false, pattern.get_needle_state(4, TO_LEFT));
  TEST_ASSERT_EQUAL(false, pattern.get_needle_state(5, TO_LEFT));
  TEST_ASSERT_EQUAL(false, pattern.get_needle_state(6, TO_LEFT));
  TEST_ASSERT_EQUAL(false, pattern.get_needle_state(7, TO_LEFT));
  TEST_ASSERT_EQUAL(false, pattern.get_needle_state(8, TO_LEFT));
  TEST_ASSERT_EQUAL(true, pattern.get_needle_state(9, TO_LEFT));
  TEST_ASSERT_EQUAL(true, pattern.get_needle_state(10, TO_LEFT));
  TEST_ASSERT_EQUAL(true, pattern.get_needle_state(11, TO_LEFT));
  TEST_ASSERT_EQUAL(true, pattern.get_needle_state(12, TO_LEFT));
  TEST_ASSERT_EQUAL(true, pattern.get_needle_state(13, TO_LEFT));
  TEST_ASSERT_EQUAL(true, pattern.get_needle_state(14, TO_LEFT));
  TEST_ASSERT_EQUAL(false, pattern.get_needle_state(15, TO_LEFT));
}

void run_module_pattern_tests() {
  RUN_TEST(test_read_little_endian);
  RUN_TEST(test_needle_index);
  RUN_TEST(complex_test);
}
