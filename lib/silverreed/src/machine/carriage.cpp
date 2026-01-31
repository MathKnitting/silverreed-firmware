#include "carriage.h"

#include "Arduino.h"
#include "config.h"

CarriageState::CarriageState()
    : CCP(false), KSL(false), DOB(false), HOK(false) {
  /*
   * Default constructor - initializes all pins to LOW/false.
   * This is useful for testing and creating initial previous state snapshots
   * without hardware access.
   */
}

CarriageState::CarriageState(bool ccp, bool ksl, bool dob, bool hok)
    : CCP(ccp), KSL(ksl), DOB(dob), HOK(hok) {
  /*
   * Explicit constructor with pin values.
   * Use this when you have already read the pin values and want to
   * construct a state object without re-reading from hardware.
   */
}

CarriageState CarriageState::read_from_pins() {
  /*
   * Static factory method to read current state from hardware pins.
   * This is the preferred method for production code as it clearly
   * indicates that hardware I/O is occurring.
   *
   * @return CarriageState snapshot of current hardware pin states
   */
  bool ccp = digitalRead(PinsCorrespondance::CCP);
  bool ksl = digitalRead(PinsCorrespondance::KSL);
  bool dob = digitalRead(PinsCorrespondance::DOB);
  bool hok = digitalRead(PinsCorrespondance::HOK);

  return CarriageState(ccp, ksl, dob, hok);
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

Carriage::Carriage() {
  this->power_solenoid(LOW);
  this->last_carriage_movement_time = millis();
}

void Carriage::set_DOB_state(int state) {
  /*
   * Set the state of the DOB pin.
   */
  if (this->DOB_state == state) return;

  this->DOB_state = state;

  // Apply inversion if needed
  int physical_state = PinsCorrespondance::DOB_INVERTED ? !state : state;
  digitalWrite(PinsCorrespondance::DOB, physical_state);
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

void Carriage::update_last_movement() {
  /*
   * Update the timestamp of the last carriage movement.
   * This should be called whenever the carriage is detected to be moving.
   */
  this->last_carriage_movement_time = millis();
}

void Carriage::check_and_shutoff_if_inactive() {
  /*
   * Check if the carriage has been inactive for too long and turn off the
   * solenoid if necessary. This prevents unnecessary power consumption and
   * overheating when the carriage is not moving.
   */
  if (!this->is_solenoid_powered()) {
    return;  // Already off, nothing to do
  }

  unsigned long current_time = millis();
  unsigned long elapsed = current_time - this->last_carriage_movement_time;

  if (elapsed >= SOLENOID_INACTIVITY_TIMEOUT_MS) {
    this->power_solenoid(LOW);
  }
}

void Carriage::secure_carriage() {
  /*
   * Static method to initialize carriage hardware pins and set them to a safe
   * state. This should be called during setup() to configure the DOB and
   * SOLENOID_POWER pins before any other operations.
   *
   * The DOB signal inversion (if needed on platforms like ESP32 with
   * level-shifting transistors) is handled automatically.
   */

  // Set both to LOW (safe state)
  // For DOB, apply inversion if needed (e.g., ESP32 with 2N2222A transistor)
  int dob_physical_state = PinsCorrespondance::DOB_INVERTED ? HIGH : LOW;
  digitalWrite(PinsCorrespondance::DOB, dob_physical_state);
  digitalWrite(PinsCorrespondance::SOLENOID_POWER, LOW);
}
