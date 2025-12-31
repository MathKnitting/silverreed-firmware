#include "test_knitting.h"

#include "Arduino.h"
#include "communication/ayab.h"
#include "config.h"
#include "knitting.h"
#include "unity.h"

void test_knitting_state_transitions() {
  KnittingProcess.reset();

  // Initial state should be Idle
  TEST_ASSERT_EQUAL(Idle, KnittingProcess.get_knitting_state());

  // After init, should be WaitingStart
  bool init_result = KnittingProcess.init();
  TEST_ASSERT_TRUE(init_result);
  TEST_ASSERT_EQUAL(WaitingStart, KnittingProcess.get_knitting_state());

  // Calling init again should fail
  init_result = KnittingProcess.init();
  TEST_ASSERT_FALSE(init_result);

  // Start knitting should transition to Knitting state
  bool start_result = KnittingProcess.start_knitting(50, 100, false, false);
  TEST_ASSERT_TRUE(start_result);
  TEST_ASSERT_EQUAL(Knitting, KnittingProcess.get_knitting_state());
  TEST_ASSERT_EQUAL(50, KnittingProcess.get_start_needle());
  TEST_ASSERT_EQUAL(100, KnittingProcess.get_end_needle());
}

void test_knitting_needle_index_tracking() {
  KnittingProcess.reset();

  // Initialize and start knitting
  uint8_t init_buffer[] = {0x05, 0x01, 0xa1};
  Ayab.receive(init_buffer, sizeof(init_buffer));

  uint8_t start_buffer[] = {0x01, 0x54, 0x74, 0x02, 0x5b};
  Ayab.receive(start_buffer, sizeof(start_buffer));

  // Set carriage direction to right
  digitalWrite(PinsCorrespondance::HOK, LOW);

  // Provide pattern data
  uint8_t confline_buffer[] = {0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
                               0xc7, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x71};
  Ayab.receive(confline_buffer, sizeof(confline_buffer));

  // Enter pattern section
  digitalWrite(PinsCorrespondance::KSL, HIGH);

  // Needle index should start at -1
  TEST_ASSERT_EQUAL(-1, KnittingProcess.get_current_needle_index());

  // Simulate needle transitions
  digitalWrite(PinsCorrespondance::CCP, LOW);
  KnittingProcess.knitting_loop();
  digitalWrite(PinsCorrespondance::CCP, HIGH);
  KnittingProcess.knitting_loop();

  // Should increment to 0
  TEST_ASSERT_EQUAL(0, KnittingProcess.get_current_needle_index());

  // Another needle
  digitalWrite(PinsCorrespondance::CCP, LOW);
  KnittingProcess.knitting_loop();
  digitalWrite(PinsCorrespondance::CCP, HIGH);
  KnittingProcess.knitting_loop();

  TEST_ASSERT_EQUAL(1, KnittingProcess.get_current_needle_index());

  // Exit pattern section - should reset to -1
  digitalWrite(PinsCorrespondance::KSL, LOW);
  KnittingProcess.knitting_loop();

  TEST_ASSERT_EQUAL(-1, KnittingProcess.get_current_needle_index());
}

void test_knitting_edge_cases() {
  KnittingProcess.reset();

  // Test start_needle == end_needle (single needle)
  KnittingProcess.init();
  bool result = KnittingProcess.start_knitting(100, 100, false, false);
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL(100, KnittingProcess.get_start_needle());
  TEST_ASSERT_EQUAL(100, KnittingProcess.get_end_needle());

  // Reset and test maximum range
  KnittingProcess.reset();
  KnittingProcess.init();
  result = KnittingProcess.start_knitting(0, 199, false, false);
  TEST_ASSERT_TRUE(result);
  TEST_ASSERT_EQUAL(0, KnittingProcess.get_start_needle());
  TEST_ASSERT_EQUAL(199, KnittingProcess.get_end_needle());
}

void test_knitting_waiting_start_carriage_detection() {
  KnittingProcess.reset();
  KnittingProcess.init();

  TEST_ASSERT_EQUAL(WaitingStart, KnittingProcess.get_knitting_state());

  // Initialize carriage position
  digitalWrite(PinsCorrespondance::CCP, LOW);
  digitalWrite(PinsCorrespondance::HOK, LOW);
  KnittingProcess.knitting_loop();

  // Simulate carriage movement (CCP transition)
  digitalWrite(PinsCorrespondance::CCP, HIGH);
  KnittingProcess.knitting_loop();

  // Should still be in WaitingStart but carriage movement detected
  TEST_ASSERT_EQUAL(WaitingStart, KnittingProcess.get_knitting_state());

  // Now start knitting properly
  uint8_t start_buffer[] = {0x01, 0x54, 0x74, 0x02, 0x5b};
  Ayab.receive(start_buffer, sizeof(start_buffer));

  TEST_ASSERT_EQUAL(Knitting, KnittingProcess.get_knitting_state());
}

void run_module_knitting_tests() {
  RUN_TEST(test_knitting_state_transitions);
  RUN_TEST(test_knitting_needle_index_tracking);
  RUN_TEST(test_knitting_edge_cases);
  RUN_TEST(test_knitting_waiting_start_carriage_detection);
}
