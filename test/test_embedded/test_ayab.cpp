#include "test_ayab.h"

#include <Arduino.h>
#include <unity.h>

#include "communication/ayab.h"
#include "config.h"
#include "knitting.h"

void test_CRC8_calculation() {
  Ayab_& ayab_instance = Ayab_::getInstance();

  // Test known CRC8 values from the protocol
  // Example from reqStart: {0x01, 0x54, 0x74, 0x02} -> CRC = 0x5b
  uint8_t buffer1[] = {0x01, 0x54, 0x74, 0x02};
  uint8_t expected_crc1 = 0x5b;
  uint8_t calculated_crc1 = ayab_instance.CRC8(buffer1, sizeof(buffer1));
  TEST_ASSERT_EQUAL_HEX8(expected_crc1, calculated_crc1);

  // Test from reqInit: {0x05, 0x01} -> CRC = 0xa1
  uint8_t buffer2[] = {0x05, 0x01};
  uint8_t expected_crc2 = 0xa1;
  uint8_t calculated_crc2 = ayab_instance.CRC8(buffer2, sizeof(buffer2));
  TEST_ASSERT_EQUAL_HEX8(expected_crc2, calculated_crc2);

  // Test from cnfLine: first 29 bytes -> CRC = 0x71
  uint8_t buffer3[] = {0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
                       0xc7, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t expected_crc3 = 0x71;
  uint8_t calculated_crc3 = ayab_instance.CRC8(buffer3, sizeof(buffer3));
  TEST_ASSERT_EQUAL_HEX8(expected_crc3, calculated_crc3);

  // Test empty buffer
  uint8_t buffer4[] = {};
  uint8_t expected_crc4 = 0x00;
  uint8_t calculated_crc4 = ayab_instance.CRC8(buffer4, 0);
  TEST_ASSERT_EQUAL_HEX8(expected_crc4, calculated_crc4);

  // Test single byte
  uint8_t buffer5[] = {0xFF};
  uint8_t calculated_crc5 = ayab_instance.CRC8(buffer5, sizeof(buffer5));
  TEST_ASSERT_NOT_EQUAL(0x00, calculated_crc5);
}

void test_reqStart_valid_checksum() {
  KnittingProcess.reset();
  KnittingProcess.init();

  // Valid reqStart packet: start=84, end=116, flags=0x02, CRC=0x5b
  uint8_t buffer[] = {0x01, 0x54, 0x74, 0x02, 0x5b};
  Ayab.receive(buffer, sizeof(buffer));

  // Should transition to Knitting state
  TEST_ASSERT_EQUAL(Knitting, KnittingProcess.get_knitting_state());
  TEST_ASSERT_EQUAL(84, KnittingProcess.get_start_needle());
  TEST_ASSERT_EQUAL(116, KnittingProcess.get_end_needle());
}

void test_reqStart_invalid_checksum() {
  KnittingProcess.reset();
  KnittingProcess.init();

  // Invalid reqStart packet: correct data but wrong CRC (0xFF instead of 0x5b)
  uint8_t buffer[] = {0x01, 0x54, 0x74, 0x02, 0xFF};
  Ayab.receive(buffer, sizeof(buffer));

  // Should NOT transition to Knitting state due to CRC error
  TEST_ASSERT_EQUAL(WaitingStart, KnittingProcess.get_knitting_state());
}

void test_reqStart_insufficient_buffer() {
  KnittingProcess.reset();
  KnittingProcess.init();

  // Too short buffer (only 4 bytes, need 5)
  uint8_t buffer[] = {0x01, 0x54, 0x74, 0x02};
  Ayab.receive(buffer, sizeof(buffer));

  // Should NOT transition to Knitting state due to buffer size
  TEST_ASSERT_EQUAL(WaitingStart, KnittingProcess.get_knitting_state());
}

void test_cnfLine_valid_checksum() {
  KnittingProcess.reset();
  KnittingProcess.init();

  // Start knitting first
  uint8_t start_buffer[] = {0x01, 0x54, 0x74, 0x02, 0x5b};
  Ayab.receive(start_buffer, sizeof(start_buffer));

  // Valid cnfLine packet with correct CRC
  uint8_t confline_buffer[] = {0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
                               0xc7, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x71};
  Ayab.receive(confline_buffer, sizeof(confline_buffer));

  // Pattern buffer should be set (non-NULL)
  TEST_ASSERT_NOT_NULL(KnittingProcess.get_pattern().get_buffer());
}

void test_cnfLine_invalid_checksum() {
  KnittingProcess.reset();
  KnittingProcess.init();

  // Start knitting first
  uint8_t start_buffer[] = {0x01, 0x54, 0x74, 0x02, 0x5b};
  Ayab.receive(start_buffer, sizeof(start_buffer));

  // Get current pattern buffer
  uint8_t* buffer_before = KnittingProcess.get_pattern().get_buffer();

  // Invalid cnfLine packet with wrong CRC (0xFF instead of 0x71)
  uint8_t confline_buffer[] = {0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
                               0xc7, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};
  Ayab.receive(confline_buffer, sizeof(confline_buffer));

  // Pattern buffer should remain unchanged due to CRC error
  uint8_t* buffer_after = KnittingProcess.get_pattern().get_buffer();
  TEST_ASSERT_EQUAL_PTR(buffer_before, buffer_after);
}

void test_reqInit_valid() {
  KnittingProcess.reset();

  // Valid reqInit packet
  uint8_t buffer[] = {0x05, 0x01, 0xa1};
  Ayab.receive(buffer, sizeof(buffer));

  // Should transition to WaitingStart
  TEST_ASSERT_EQUAL(WaitingStart, KnittingProcess.get_knitting_state());
}

void test_reqInit_when_not_idle() {
  KnittingProcess.reset();
  KnittingProcess.init();

  // Already in WaitingStart state
  TEST_ASSERT_EQUAL(WaitingStart, KnittingProcess.get_knitting_state());

  // Try to init again
  uint8_t buffer[] = {0x05, 0x01, 0xa1};
  Ayab.receive(buffer, sizeof(buffer));

  // Should remain in WaitingStart (init fails when not Idle)
  TEST_ASSERT_EQUAL(WaitingStart, KnittingProcess.get_knitting_state());
}

void test_empty_packet_ignored() {
  KnittingProcess.reset();

  // Empty packet should be ignored (as per receive() function line 46-48)
  uint8_t buffer[] = {};
  Ayab.receive(buffer, 0);

  // State should remain Idle
  TEST_ASSERT_EQUAL(Idle, KnittingProcess.get_knitting_state());
}

void run_module_ayab_tests() {
  RUN_TEST(test_CRC8_calculation);
  RUN_TEST(test_reqStart_valid_checksum);
  RUN_TEST(test_reqStart_invalid_checksum);
  RUN_TEST(test_reqStart_insufficient_buffer);
  RUN_TEST(test_cnfLine_valid_checksum);
  RUN_TEST(test_cnfLine_invalid_checksum);
  RUN_TEST(test_reqInit_valid);
  RUN_TEST(test_reqInit_when_not_idle);
  RUN_TEST(test_empty_packet_ignored);
}
