/**
 * Entry point of the Arduino.
 */
#include <Arduino.h>

#include "communication/ayab.h"
#include "config.h"
#include "debug.h"
#include "knitting.h"

void setup() {
  /**
   * Setup run at the start of the Arduino.
   */
  // Set the pin modes.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PinsCorrespondance::ND1, INPUT);  // not used for now
  pinMode(PinsCorrespondance::KSL, INPUT_PULLUP);
  pinMode(PinsCorrespondance::DOB, OUTPUT);
  pinMode(PinsCorrespondance::CCP, INPUT_PULLUP);
  pinMode(PinsCorrespondance::HOK, INPUT_PULLUP);

  // Set the DOB initial state to low.
  digitalWrite(PinsCorrespondance::DOB, LOW);

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
