/**
 * Entry point of the Arduino.
 */
#include <Arduino.h>

#include "communication/ayab.h"
#include "config.h"
#include "debug.h"
#include "knitting.h"
#include "machine/carriage.h"

void setup() {
  /**
   * Setup run at the start of the Arduino.
   */
  // Set the pin modes for input pins.
  pinMode(PinsCorrespondance::ND1, INPUT_PULLUP);  // not used for now
  pinMode(PinsCorrespondance::KSL, INPUT_PULLUP);
  pinMode(PinsCorrespondance::DOB, OUTPUT);
  pinMode(PinsCorrespondance::CCP, INPUT_PULLUP);
  pinMode(PinsCorrespondance::HOK, INPUT_PULLUP);
  pinMode(PinsCorrespondance::SOLENOID_POWER, OUTPUT);

  // Initialize carriage output pins (DOB and SOLENOID_POWER) to safe state.
  // This handles DOB signal inversion automatically for platforms that need it.
  Carriage::secure_carriage();

  // Initialize the program singletons.
  Ayab.init();
  KnittingProcess.reset();

  // DEBUG
  DEBUG_START();
  DEBUG_PRINTLN("Init done");
}

void loop() {
  /**
   * Loop run continuously after the setup.
   */
  Ayab.update();
  KnittingProcess.knitting_loop();
}
