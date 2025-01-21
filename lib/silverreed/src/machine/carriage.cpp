#include "carriage.h"

#include "Arduino.h"
#include "config.h"

CarriageState::CarriageState() {
  /*
  Init the carriage state by reading the pins values
  */
  this->DOB = digitalRead(PinsCorrespondance::DOB);
  this->CCP = digitalRead(PinsCorrespondance::CCP);
  this->HOK = digitalRead(PinsCorrespondance::HOK);
  this->KSL = digitalRead(PinsCorrespondance::KSL);
}

CarriageDirection CarriageState::get_direction() {
  /*
  Carriage direction is given by the HOK pin.
  It is HIGH when the carriage is moving to the left and LOW when the carriage
  is moving to the right.

  @return The direction of the carriage.
  */
  return this->HOK == HIGH ? TO_LEFT : TO_RIGHT;
}

bool CarriageState::is_in_pattern_section() {
  /*
  * The pattern section is defined by the point cams (magnets placed on the main
  bed).
  * If the carriage is between the two points (in the pattern section), the KSL
  pin is HIGH.

  * @return True if the carriage is in the pattern section.
  */
  return this->KSL == HIGH;
}

bool CarriageState::is_carriage_moving(CarriageState previous_state) {
  /*
   * The carriage is moving if the CCP pin changed values from the previous
   * state.
   *
   * @param True if the carriage is moving.
   */
  return previous_state.CCP != this->CCP;
}

bool CarriageState::is_start_out_of_pattern(CarriageState previous_state) {
  /*
   * The carriage is out of the pattern section if the KSL pin changed values
   * from the previous state (from High to Low).
   *
   * @param True if the carriage is out of the pattern section.
   */
  return this->KSL == LOW && previous_state.KSL == HIGH;
}

bool CarriageState::is_start_of_needle(CarriageState previous_state) {
  /*
   * The carriage is at the start of the needle if the CCP pin changed values
   * from the previous state (from Low to High).
   *
   * @param True if the carriage is at the start of the needle.
   */
  return this->CCP == HIGH && previous_state.CCP == LOW;
}

Carriage::Carriage() { this->power_solenoid(LOW); }

void Carriage::set_DOB_state(int state) {
  /*
   * Set the state of the DOB pin.
   */
  if (this->DOB_state == state) return;

  this->DOB_state = state;
  digitalWrite(PinsCorrespondance::DOB, state);
}

bool Carriage::is_solenoid_powered() const {
  return this->power_solenoid_state == HIGH;
}

void Carriage::power_solenoid(int state) {
  if (this->power_solenoid_state == state) {
    return;
  }

  this->power_solenoid_state = state;
  digitalWrite(PinsCorrespondance::SOLENOID_POWER, state);
  this->solenoid_change_time = millis();
}
