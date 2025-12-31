#ifndef CARRIAGE_H_
#define CARRIAGE_H_

#include <Arduino.h>

enum CarriageDirection { TO_LEFT, TO_RIGHT };

class CarriageState {
 public:
  bool CCP;
  bool KSL;
  bool DOB;
  bool HOK;

  // Default constructor - initializes all pins to LOW
  // Useful for testing and creating previous state snapshots
  CarriageState();

  // Explicit constructor with pin values
  // Use this when you have already read the pin values
  CarriageState(bool ccp, bool ksl, bool dob, bool hok);

  // Static factory method to read current state from hardware pins
  // This is the preferred method for production code
  static CarriageState read_from_pins();

  bool is_in_pattern_section();
  bool is_start_out_of_pattern(CarriageState previous_state);
  bool is_carriage_moving(CarriageState previous_state);
  CarriageDirection get_direction();
  bool is_start_of_needle(CarriageState previous_state);
};

class Carriage {
 private:
  int DOB_state = LOW;
  int power_solenoid_state = LOW;
  unsigned long solenoid_change_time;
  unsigned long last_carriage_movement_time;

 public:
  Carriage();

  void set_DOB_state(int state);

  bool is_solenoid_powered() const;
  bool is_end_of_pattern_section();
  void power_solenoid(int state);
  void update_last_movement();
  void check_and_shutoff_if_inactive();
};

#endif
