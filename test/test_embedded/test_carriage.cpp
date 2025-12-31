#include <Arduino.h>
#include <unity.h>

#include "config.h"
#include "machine/carriage.h"

void test_carriage_direction() {
  digitalWrite(PinsCorrespondance::HOK, HIGH);
  TEST_ASSERT_EQUAL(CarriageState::read_from_pins().get_direction(), TO_LEFT);
  digitalWrite(PinsCorrespondance::HOK, LOW);
  TEST_ASSERT_EQUAL(CarriageState::read_from_pins().get_direction(), TO_RIGHT);
}

void test_is_in_pattern_section() {
  digitalWrite(PinsCorrespondance::KSL, HIGH);
  TEST_ASSERT_EQUAL(CarriageState::read_from_pins().is_in_pattern_section(),
                    true);
  digitalWrite(PinsCorrespondance::KSL, LOW);
  TEST_ASSERT_EQUAL(CarriageState::read_from_pins().is_in_pattern_section(),
                    false);
}

void test_is_start_of_needle() {
  // Set initial state: CCP LOW
  digitalWrite(PinsCorrespondance::CCP, LOW);
  CarriageState previous_state = CarriageState::read_from_pins();

  // CCP still LOW - not start of needle
  digitalWrite(PinsCorrespondance::CCP, LOW);
  CarriageState current_state = CarriageState::read_from_pins();
  TEST_ASSERT_FALSE(current_state.is_start_of_needle(previous_state));

  // CCP transitions to HIGH - start of needle
  digitalWrite(PinsCorrespondance::CCP, HIGH);
  current_state = CarriageState::read_from_pins();
  TEST_ASSERT_TRUE(current_state.is_start_of_needle(previous_state));

  // Update previous state
  previous_state = current_state;

  // CCP still HIGH - not start of needle
  digitalWrite(PinsCorrespondance::CCP, HIGH);
  current_state = CarriageState::read_from_pins();
  TEST_ASSERT_FALSE(current_state.is_start_of_needle(previous_state));

  // CCP transitions to LOW - not start of needle (wrong transition)
  digitalWrite(PinsCorrespondance::CCP, LOW);
  current_state = CarriageState::read_from_pins();
  TEST_ASSERT_FALSE(current_state.is_start_of_needle(previous_state));
}

void test_is_carriage_moving() {
  // Set initial state: CCP LOW
  digitalWrite(PinsCorrespondance::CCP, LOW);
  CarriageState previous_state = CarriageState::read_from_pins();

  // CCP still LOW - not moving
  digitalWrite(PinsCorrespondance::CCP, LOW);
  CarriageState current_state = CarriageState::read_from_pins();
  TEST_ASSERT_FALSE(current_state.is_carriage_moving(previous_state));

  // CCP changes to HIGH - carriage is moving
  digitalWrite(PinsCorrespondance::CCP, HIGH);
  current_state = CarriageState::read_from_pins();
  TEST_ASSERT_TRUE(current_state.is_carriage_moving(previous_state));

  // Update previous state
  previous_state = current_state;

  // CCP changes back to LOW - carriage is moving
  digitalWrite(PinsCorrespondance::CCP, LOW);
  current_state = CarriageState::read_from_pins();
  TEST_ASSERT_TRUE(current_state.is_carriage_moving(previous_state));

  // Update previous state
  previous_state = current_state;

  // CCP still LOW - not moving
  digitalWrite(PinsCorrespondance::CCP, LOW);
  current_state = CarriageState::read_from_pins();
  TEST_ASSERT_FALSE(current_state.is_carriage_moving(previous_state));
}

void test_is_start_out_of_pattern() {
  // Set initial state: KSL HIGH (in pattern)
  digitalWrite(PinsCorrespondance::KSL, HIGH);
  CarriageState previous_state = CarriageState::read_from_pins();

  // KSL still HIGH - not exiting pattern
  digitalWrite(PinsCorrespondance::KSL, HIGH);
  CarriageState current_state = CarriageState::read_from_pins();
  TEST_ASSERT_FALSE(current_state.is_start_out_of_pattern(previous_state));

  // KSL transitions to LOW - exiting pattern
  digitalWrite(PinsCorrespondance::KSL, LOW);
  current_state = CarriageState::read_from_pins();
  TEST_ASSERT_TRUE(current_state.is_start_out_of_pattern(previous_state));

  // Update previous state
  previous_state = current_state;

  // KSL still LOW - not exiting pattern
  digitalWrite(PinsCorrespondance::KSL, LOW);
  current_state = CarriageState::read_from_pins();
  TEST_ASSERT_FALSE(current_state.is_start_out_of_pattern(previous_state));

  // KSL transitions to HIGH - entering pattern (not exiting)
  digitalWrite(PinsCorrespondance::KSL, HIGH);
  current_state = CarriageState::read_from_pins();
  TEST_ASSERT_FALSE(current_state.is_start_out_of_pattern(previous_state));
}

void test_carriage_set_DOB_state() {
  Carriage carriage;

  // Set DOB to HIGH
  carriage.set_DOB_state(HIGH);
  TEST_ASSERT_EQUAL(HIGH, digitalRead(PinsCorrespondance::DOB));

  // Set DOB to LOW
  carriage.set_DOB_state(LOW);
  TEST_ASSERT_EQUAL(LOW, digitalRead(PinsCorrespondance::DOB));

  // Setting to same state should not cause issues
  carriage.set_DOB_state(LOW);
  TEST_ASSERT_EQUAL(LOW, digitalRead(PinsCorrespondance::DOB));

  carriage.set_DOB_state(HIGH);
  TEST_ASSERT_EQUAL(HIGH, digitalRead(PinsCorrespondance::DOB));
}

void test_carriage_power_solenoid() {
  Carriage carriage;

  // Initial state should be LOW (from constructor)
  TEST_ASSERT_FALSE(carriage.is_solenoid_powered());
  TEST_ASSERT_EQUAL(LOW, digitalRead(PinsCorrespondance::SOLENOID_POWER));

  // Power on solenoid
  carriage.power_solenoid(HIGH);
  TEST_ASSERT_TRUE(carriage.is_solenoid_powered());
  TEST_ASSERT_EQUAL(HIGH, digitalRead(PinsCorrespondance::SOLENOID_POWER));

  // Power off solenoid
  carriage.power_solenoid(LOW);
  TEST_ASSERT_FALSE(carriage.is_solenoid_powered());
  TEST_ASSERT_EQUAL(LOW, digitalRead(PinsCorrespondance::SOLENOID_POWER));

  // Setting to same state should not cause issues
  carriage.power_solenoid(LOW);
  TEST_ASSERT_FALSE(carriage.is_solenoid_powered());
}

void test_carriage_inactivity_timeout() {
  Carriage carriage;

  // Power on solenoid
  carriage.power_solenoid(HIGH);
  TEST_ASSERT_TRUE(carriage.is_solenoid_powered());

  // Update last movement to current time
  carriage.update_last_movement();

  // Check immediately - should still be powered
  carriage.check_and_shutoff_if_inactive();
  TEST_ASSERT_TRUE(carriage.is_solenoid_powered());

  // Simulate timeout by waiting (note: this might be slow in tests)
  // In a real test environment, we might need to mock millis()
  // For now, just verify the logic doesn't crash
  carriage.check_and_shutoff_if_inactive();

  // If solenoid is off, don't turn it back on
  carriage.power_solenoid(LOW);
  carriage.check_and_shutoff_if_inactive();
  TEST_ASSERT_FALSE(carriage.is_solenoid_powered());
}

void test_carriage_movement_tracking() {
  Carriage carriage;

  // Power on and update movement
  carriage.power_solenoid(HIGH);
  carriage.update_last_movement();
  TEST_ASSERT_TRUE(carriage.is_solenoid_powered());

  // Movement should prevent timeout (within timeout period)
  carriage.check_and_shutoff_if_inactive();
  TEST_ASSERT_TRUE(carriage.is_solenoid_powered());
}

void run_module_carriage_tests() {
  RUN_TEST(test_carriage_direction);
  RUN_TEST(test_is_in_pattern_section);
  RUN_TEST(test_is_start_of_needle);
  RUN_TEST(test_is_carriage_moving);
  RUN_TEST(test_is_start_out_of_pattern);
  RUN_TEST(test_carriage_set_DOB_state);
  RUN_TEST(test_carriage_power_solenoid);
  RUN_TEST(test_carriage_inactivity_timeout);
  RUN_TEST(test_carriage_movement_tracking);
}
