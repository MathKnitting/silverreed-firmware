#include <Arduino.h>
#include <unity.h>

#include "config.h"
#include "machine/carriage.h"

void test_carriage_direction() {
  digitalWrite(PinsCorrespondance::HOK, HIGH);
  TEST_ASSERT_EQUAL(CarriageState().get_direction(), TO_LEFT);
  digitalWrite(PinsCorrespondance::HOK, LOW);
  TEST_ASSERT_EQUAL(CarriageState().get_direction(), TO_RIGHT);
}

void test_is_in_pattern_section() {
  digitalWrite(PinsCorrespondance::KSL, HIGH);
  TEST_ASSERT_EQUAL(CarriageState().is_in_pattern_section(), true);
  digitalWrite(PinsCorrespondance::KSL, LOW);
  TEST_ASSERT_EQUAL(CarriageState().is_in_pattern_section(), false);
}

void run_module_carriage_tests() {
  RUN_TEST(test_carriage_direction);
  RUN_TEST(test_is_in_pattern_section);
}
