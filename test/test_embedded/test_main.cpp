/*
 * This is a generated example file that is supposed to be modified.
 */

#define RUN_MODULE(run_function) \
  extern void run_function();    \
  run_function();

#include <Arduino.h>
#include <unity.h>

#include "config.h"
#include "test_ayab.h"
#include "test_carriage.h"
#include "test_integration.h"
#include "test_knitting.h"
#include "test_pattern.h"
#include "test_version.h"

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN();

  pinMode(PinsCorrespondance::ND1, OUTPUT);
  pinMode(PinsCorrespondance::DOB, OUTPUT);
  pinMode(PinsCorrespondance::CCP, OUTPUT);
  pinMode(PinsCorrespondance::HOK, OUTPUT);
  pinMode(PinsCorrespondance::KSL, OUTPUT);
  pinMode(PinsCorrespondance::SOLENOID_POWER, OUTPUT);

  // Initialize all output pins to LOW to ensure clean test state
  digitalWrite(PinsCorrespondance::ND1, LOW);
  digitalWrite(PinsCorrespondance::DOB, LOW);
  digitalWrite(PinsCorrespondance::CCP, LOW);
  digitalWrite(PinsCorrespondance::HOK, LOW);
  digitalWrite(PinsCorrespondance::KSL, LOW);
  digitalWrite(PinsCorrespondance::SOLENOID_POWER, LOW);
}

void loop() {
  RUN_MODULE(run_module_carriage_tests);
  RUN_MODULE(run_module_pattern_tests);
  RUN_MODULE(run_module_knitting_tests);
  RUN_MODULE(run_module_version_tests);
  RUN_MODULE(run_module_ayab_tests);
  RUN_MODULE(run_module_integration_tests);

  UNITY_END();
}
