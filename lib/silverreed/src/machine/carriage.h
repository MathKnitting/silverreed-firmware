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
  CarriageState();

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

 public:
  Carriage();

  void set_DOB_state(int state);

  bool is_solenoid_powered() const;
  bool is_end_of_pattern_section();
  void power_solenoid(int state);
};

#endif
