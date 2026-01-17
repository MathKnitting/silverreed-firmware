#include <Arduino.h>
#include <unity.h>

#include "communication/ayab.h"
#include "config.h"
#include "knitting.h"
#include "machine/carriage.h"

void new_needle() {
  digitalWrite(PinsCorrespondance::CCP, LOW);
  KnittingProcess.knitting_loop();
  digitalWrite(PinsCorrespondance::CCP, HIGH);
}

void read_buffer(int number_of_bytes, bool* buffer) {
  for (int i = 0; i < number_of_bytes; i++) {
    new_needle();
    KnittingProcess.knitting_loop();
    TEST_ASSERT_EQUAL(KnittingProcess.get_current_needle_index(), i);
    buffer[i] = digitalRead(PinsCorrespondance::DOB);
  }
}

void test_pattern_reading() {
  KnittingProcess.reset();

  // Initialize the knitting process
  uint8_t init_buffer[] = {0x05, 0x01, 0xa1};  // reqInit packet
  Ayab.receive(init_buffer, sizeof(init_buffer));

  // Set the carriage to knit to the right
  digitalWrite(PinsCorrespondance::HOK, LOW);
  KnittingProcess.knitting_loop();

  // Start the knitting process
  // req start b'\xc0\x01Tt\x02[\xc0'
  uint8_t start_buffer[] = {0x01, 0x54, 0x74, 0x02, 0x5b};  // reqStart packet
  Ayab.receive(start_buffer, sizeof(start_buffer));
  TEST_ASSERT_EQUAL(84, KnittingProcess.get_start_needle());
  TEST_ASSERT_EQUAL(116, KnittingProcess.get_end_needle());
  TEST_ASSERT_EQUAL(Knitting, KnittingProcess.get_knitting_state());

  // cnf line from AYAB desktop (without SLIP delimiters 0xc0)
  // b'\xc0B\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xe0\xc7\x0f\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00q\xc0'
  uint8_t confline_buffer[] = {
      0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0xe0, 0xc7, 0x0f, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x71};  // cnfLine packet
  Ayab.receive(confline_buffer, sizeof(confline_buffer));

  KnittingProcess.knitting_loop();
  TEST_ASSERT_EQUAL(CarriageState().DOB, LOW);

  // test not being in pattern
  for (int i = 0; i < 20; i++) {
    new_needle();
    KnittingProcess.knitting_loop();
    TEST_ASSERT_EQUAL(CarriageState().DOB, LOW);
  }

  // set in pattern flag
  digitalWrite(PinsCorrespondance::KSL, HIGH);

  bool pattern_buffer[33];
  bool expected_buffer[] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
                            1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  read_buffer(33, pattern_buffer);

  for (int i = 0; i < 33; i++) {
    TEST_ASSERT_EQUAL_MESSAGE(
        expected_buffer[i], pattern_buffer[i],
        "Pattern mismatch - check debug output for index");
  }
}

void run_module_integration_tests() { RUN_TEST(test_pattern_reading); }
